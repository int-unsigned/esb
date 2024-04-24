using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.IO;
using Microsoft.Win32;  // Registery
using System.Diagnostics;
using Microsoft.VisualStudio.Setup.Configuration;
using System.Runtime.InteropServices;
using System.IO.Compression;



namespace EsbStudio
{
    public partial class WizardForm : Form
    {
        const string m_esb_registery_key = "Software\\IntUnsigned\\Esb";
        const string m_esb_registery_location = "Location";

        const string ESBHLP_LIB_NAME            = "esbhlp.lib";
        const string ESBHLP_LIB_DIR_WIN32       = "w32"; //TODO Win32
        const string ESBHLP_LIB_DIR_WIN64       = "w64"; //Win64
        const string ESBHLP_LIB_DIR_DEBUG       = "Debug";
        const string ESBHLP_LIB_DIR_RELEASE     = "Release";
        const int    ESBHLP_LIB_DIR_NAME_LEN     = 5;    //v83**
        
        static string EsbLibPathBuildTypeDirName(Util.ImageBuildType lib_build_type_)
        {
            return lib_build_type_.ToString();
        }
        static string GetEsbLibPathArchitectuleDirName(Util.ImageArchitecture lib_arch_)
        {
            switch (lib_arch_)
            {
                case Util.ImageArchitecture.x86:     return ESBHLP_LIB_DIR_WIN32;
                case Util.ImageArchitecture.x64:     return ESBHLP_LIB_DIR_WIN64;
                default:                             return lib_arch_.ToString();   // если нету такой директории - пусть проверяет (или будет ош) у того кто нас позвал
            };
        }
        static string GetEsbLibNameFromDir(string esb_dir_) {
            return esb_dir_.Substring(0, ESBHLP_LIB_DIR_NAME_LEN);
        }
        static Util.ImageBuildType? GetEsbLibBuildTypeFromDir(string esb_dir_) {
            //expected v8309Debug/v8309Release
            string s_tail = esb_dir_.Substring(ESBHLP_LIB_DIR_NAME_LEN);  //суффикс сразу после имени
            if (s_tail == ESBHLP_LIB_DIR_DEBUG)
                return Util.ImageBuildType.Debug;
            else if(s_tail == ESBHLP_LIB_DIR_RELEASE)
                return Util.ImageBuildType.Release;

            return null;
        }
        static Version MakeItElemEsbVersion(string esb_config_name_)
        {
            //v8300Debug == 10, v8300Release == 12
            if (esb_config_name_.Length < 10)
                return null;
            if (esb_config_name_[0] != 'v')
                return null;
            string s_ver_maj = esb_config_name_.Substring(1, 1);
            string s_ver_min = esb_config_name_.Substring(2, 1);
            string s_ver_bld = esb_config_name_.Substring(3, 2);
            if (s_ver_bld[0] == '0')
                s_ver_bld = s_ver_bld.Substring(1, 1);
            Version ver;
            if (Version.TryParse(s_ver_maj + "." + s_ver_min + "." + s_ver_bld, out ver))
                return ver;
            else
                return null;
        }
        static Version MakeItElem1EsVersion(string oes_dir_name_)
        {
            //"c:\Program Files\1cv8\8.3.9.1818" 
            Version ver;
            if (Version.TryParse(oes_dir_name_, out ver))
                return ver;
            else
                return null;
        }
        static string GetEsbLibPathName(ItElem esb_, Util.ImageBuildType lib_build_type_, Util.ImageArchitecture lib_arch_)
        {
            return Path.Combine(esb_.Location_ + EsbLibPathBuildTypeDirName(lib_build_type_), GetEsbLibPathArchitectuleDirName(lib_arch_), ESBHLP_LIB_NAME);
        }
        

        //
        EsbBuilderSetting m_setting_;
        EsbBuilderSetting Setting => m_setting_;
        //
        List<ItElem>    m_1es_items = null;
        public ListView.CheckedListViewItemCollection SelOneEsItems { get => lvOneEs.CheckedItems; }
        //
        bool            m_result_ok = false;
        public bool     IsResultOk { get => m_result_ok; }

        bool SelResolveImportsVerbose_ { get => Setting.ResolveImportsVerbose_; }   //TODO опцию в диалог

        public string Sel1CPath     { get => Sel1CPathBox.Text; }
        public string SelEsbPath{
            get => tbEsbPath.Text;
        }
        public bool SelEsbOk {
            get => (lbEsbInfoList.Items.Count > 0);
        }
        private List<ItElem> SelEsbItems { get => (List<ItElem>)lbEsbInfoList.DataSource; }

        private string m_lbEsbInfoText_Default_;
        private Color m_InfoBox_ForeColor_Default_;

        public bool DoLoadSetting()
        {
            try {
                m_setting_ = EsbBuilderSetting.Load();
                return true;
            }
            catch (Exception ex)  {
                SetError(ex.ToString());
                return false;
            }
        }
        public Exception SaveSetting()
        {
            try
            {
                Setting.Save(SelEsbPath, Sel1CPath, SelResolveImportsVerbose_);
                return null;
            }
            catch (Exception ex)
            {
                return ex;
            }
        }
        void DoUpdateSaveSetting(bool b_need_save_)
        {
            swEsbPathSave.Checked = b_need_save_;
            swEsbPathSave.Enabled = b_need_save_;
        }
        void DoUpdateSaveSettingAuto()
        {
            bool b_need_save = false;
            if (SelEsbPath != Setting.EsbPath_ && SelEsbOk)
                b_need_save = true;

            DoUpdateSaveSetting(b_need_save);
        }


        static int ItElemCompare(ItElem a, ItElem b) {
            return a.Ver_.CompareTo(b.Ver_);
        }
        // он просто возвращает последний элемент пути
        static string GetFolderNameFromDirPath(string dir_path_) {
            return Path.GetFileName(dir_path_);
        }
        bool SetError(string error_text_) {
            AddInfoLine(error_text_);
            AddInfoLine("Продолжение работы невозможно.");
            InfoBox.ForeColor = Color.Red;
            return false;
        }
        bool DoLoadEsbInfoItems(string esb_location_) 
        {
            lbEsbInfoList.DataSource = null;
            lbEsbInfoList.Items.Clear();

            if (!Directory.Exists(esb_location_))
                return SetError($"Папка '{esb_location_}' не существует.");

            string esb_libs_dir_path = Path.Combine(esb_location_, "Lib");
            if (!Directory.Exists(esb_libs_dir_path))
                return SetError($"В указанной папке '{esb_location_}' отсутствует папка 'Lib'. Это не папка библиотеки Есб или ее структура повреждена.");

            // expect v8300Debug; v8300Release; ...
            string[] esb_lib_dir_content = Directory.GetDirectories(esb_libs_dir_path, "v*");
            if (esb_lib_dir_content.Length == 0)
                return SetError($"В папке '{esb_libs_dir_path}' отсутствуют подпапки для конфигурирования проекта.");
            
            // мы ожидаем, что для каждой версии будет 2 папки - Debug и Release
            List<ItElem> esb_configs = new List<ItElem>( esb_lib_dir_content.Length / 2);
            
            foreach (string esb_lib_subdir in esb_lib_dir_content)
            {
                // expect Win32 + Win64
                string[] esb_lib_subdir_content = Directory.GetDirectories(esb_lib_subdir);

                if (esb_lib_subdir_content.Length != 2)
                    return SetError($"Папка '{esb_lib_subdir}' имеет неверную структуру. Ожидается две поддиректории.");
                if( !( (esb_lib_subdir_content[0].EndsWith(ESBHLP_LIB_DIR_WIN32) && esb_lib_subdir_content[1].EndsWith(ESBHLP_LIB_DIR_WIN64)) ||
                       (esb_lib_subdir_content[0].EndsWith(ESBHLP_LIB_DIR_WIN64) && esb_lib_subdir_content[1].EndsWith(ESBHLP_LIB_DIR_WIN32)) ) )
                    return SetError($"Папка '{esb_lib_subdir}' имеет неверную структуру. Ожидается две поддиректории {ESBHLP_LIB_DIR_WIN32} и {ESBHLP_LIB_DIR_WIN64}.");

                if( !(File.Exists(Path.Combine(esb_lib_subdir_content[0], ESBHLP_LIB_NAME) ) ) )
                    return SetError($"В папке '{esb_lib_subdir_content[0]}' отсутствует необходимый '{ESBHLP_LIB_NAME}'.");
                if (!(File.Exists(Path.Combine(esb_lib_subdir_content[1], ESBHLP_LIB_NAME))))
                    return SetError($"В папке '{esb_lib_subdir_content[1]}' отсутствует необходимый '{ESBHLP_LIB_NAME}'.");

                string                  esb_lib_subdir_name  = GetFolderNameFromDirPath(esb_lib_subdir);
                
                Util.ImageBuildType?    esb_lib_build        = GetEsbLibBuildTypeFromDir(esb_lib_subdir_name);
                if (esb_lib_build == null)
                    return SetError($"Имя папки конфигурации '{esb_lib_subdir_name}' неверного формата. Ожидается v83??Debug или v83??Release.");
                Version                 esb_lib_ver         = MakeItElemEsbVersion(esb_lib_subdir_name);
                if (esb_lib_ver == null)
                    return SetError($"Имя папки конфигурации '{esb_lib_subdir_name}' неверного формата. Ожидается v83**Debug или v83**Release.");

                string                  esb_lib_name        = GetEsbLibNameFromDir(esb_lib_subdir_name);
                string                  esb_lib_location    = Path.Combine(esb_libs_dir_path, esb_lib_name); //будет только ПУТЬ/v8300 и нужно будет добавлять +Debug/x86 тп..
                //
                ItElem it = new ItElem(esb_lib_name, esb_lib_location, esb_lib_ver, (Util.ImageBuildType)esb_lib_build);
                // esb_lib_subdir здесь что попало первое - или ..Debug или ..Release
                // при добавлении мы просто проверяем чтобы с такой версией еще не было
                int insert_at = esb_configs.BinarySearch(it);
                if (insert_at < 0)
                    esb_configs.Insert(~insert_at, it);
            }

#if ESBVSIX_TEST
            string test_esb_libs_dir_path = Path.Combine(AppTestPath(), "Esb", "Lib");
            if (Directory.Exists(test_esb_libs_dir_path)) {
                string[] test_esb_libs_dir_content = Directory.GetDirectories(test_esb_libs_dir_path);
                foreach (string test_esb_dir in test_esb_libs_dir_content) {
                    string              test_esb_dir_name   = GetFolderNameFromDirPath(test_esb_dir);
                    Util.ImageBuildType test_esb_build      = (Util.ImageBuildType)GetEsbLibBuildTypeFromDir(test_esb_dir_name);
                    Version             test_esb_ver        = MakeItElemEsbVersion(test_esb_dir_name);
                    string              test_esb_name       = GetEsbLibNameFromDir(test_esb_dir_name);
                    string              test_esb_location   = Path.Combine(test_esb_libs_dir_path, test_esb_name);
                    ItElem              test_esb_item       = new ItElem(test_esb_name, test_esb_location, test_esb_ver, test_esb_build);
                    int test_esb_item_insert_at = esb_configs.BinarySearch(test_esb_item);
                    if (test_esb_item_insert_at < 0)
                        esb_configs.Insert(~test_esb_item_insert_at, test_esb_item);
                }
            }
#endif
            lbEsbInfoList.DataSource = esb_configs;
            return true;
        }

#if ESBVSIX_TEST
        static string AppTestPath() {
            string path_studio_proj_form_bin_debug = Application.StartupPath;
            string path_studio_proj_form_bin = Path.GetDirectoryName(path_studio_proj_form_bin_debug);
            string path_studio_proj_form = Path.GetDirectoryName(path_studio_proj_form_bin);
            string path_studio_proj = Path.GetDirectoryName(path_studio_proj_form);
            string path_studio = Path.GetDirectoryName(path_studio_proj);
            return Path.Combine(path_studio + "_dev", @"EsbProjectTemplateWizard\WizardForm");
        }
#endif

        bool DoUpdateEsbInit(string esb_location_)
        {
            tbEsbPath.Text = esb_location_;

            bool b_esb_ok = DoLoadEsbInfoItems(esb_location_);
            if (b_esb_ok)
                lbEsbInfoText.Text = "Выбранная библиотека Есб поддерживает следующие конфигурации платформы:";
            else
                lbEsbInfoText.Text = m_lbEsbInfoText_Default_;

            return b_esb_ok;
        }

        bool DoUpdateEsbChange(string new_esb_location_) 
        {
            if (SelEsbPath == new_esb_location_ && SelEsbOk)
                return false;

            tbEsbPath.Text = new_esb_location_;

            bool b_esb_ok = DoUpdateEsbInit(new_esb_location_);

            bool b_some_config_ready = false;
            // мы здесь всегда, когда текущее представление Есб поменялось
            if (b_esb_ok)
            {
                // ... поэтому, если все ок, то надо предложить сохранить текущее
                DoUpdateSaveSetting((b_esb_ok && new_esb_location_ != Setting.EsbPath_));
                //сбрасываем check с 1С списка при любом изменении esb_location_
                //(и связки с esb все тоже, т.к. есб поменялось)
                DoUpdateConfigurationsReset();
                // перевычисляем доступные конфигурации
                DoUpdateConfigurationsAvailable();
                b_some_config_ready = DoUpdate1EsUi();
                lvOneEs.Enabled = true;
            }
            else {
                // ... а если не ок, то наоборот предложение сохранить текущие настройки надо сбросить
                DoUpdateSaveSetting(false);
                DoUpdateConfigurationsReset();
                DoUpdate1EsUi();
                lvOneEs.Enabled = false;
            }

            DoUpdateReady(b_some_config_ready);
            return b_some_config_ready;
        }

        void DoUpdateConfigurationsReset() {
            if (m_1es_items != null) {
                foreach (ItElem v8_item in m_1es_items) {
                    v8_item.ResetEsb();
                }
            }
        }
        ItElem GetEsbCompaitable(List<ItElem> esb_items_, ItElem v8_item_) 
        {
            // не найдено — значение по умолчанию для типа T. а нужен null, лять...
            //ItElem esb_item = esb_items_.Find(x => x.IsCompatableWithOneEsItElem(v8_item_))

            int esb_item_index = esb_items_.FindIndex(v8_item_.IsCompatableWithOneEsItElem);
            return (esb_item_index == -1)? null : esb_items_[esb_item_index];

            //TODO ItElem реализует IComparable но по строкам - по имени (пути для 1С), а искать надо по версии
            //      оно конечно фактически совпадает, но.... Нужно делать отдельный компаратор для .BinarySearch(v8_item_, IComparer<T>)
            //int esb_item_index2 = esb_items_.BinarySearch(v8_item_);
            //return (esb_item_index == -1) ? null : esb_items_[esb_item_index];
        }
        private bool DoUpdateConfigurationsAvailable()
        {
            List<ItElem> esb_items = SelEsbItems;
            bool b_some_compatable = false;
            //
            foreach (ItElem v8_item in m_1es_items)
            {
                v8_item.EsbCompatable_ = GetEsbCompaitable(esb_items, v8_item);
                if (v8_item.EsbCompatable_ != null)
                    b_some_compatable = true;
            }
            return b_some_compatable;
        }

        public WizardForm()
        {
            InitializeComponent();
            //
            ToolStripItem item_pattern = ChooseEsbMenu.Items[0];

            ToolStripLabel lb = new ToolStripLabel( "Внимание! Экспериментальный режим!\n" +
                                                    "Выбранная Вами версия платформы не поддерживается Есб.\n" +
                                                    "Вы можете попробовать либо предыдущую, либо следующую.\n" +
                                                    "(построитель пытается проверить только ABI-совместимость)");
            lb.TextAlign = ContentAlignment.MiddleLeft;
            lb.Font = item_pattern.Font;
            lb.ForeColor = item_pattern.ForeColor;
            lb.BackColor = item_pattern.BackColor;
            //ChooseEsbMenu.AutoSize = false;
            ChooseEsbMenu.Items.Remove(item_pattern);
            ChooseEsbMenu.Items.Insert(0, lb);
            ChooseEsbMenu.AutoSize = true;
            ChooseEsbMenu.Width = ChooseEsbMenu.Width * 2;
            //
            DoLoadSetting();
            m_lbEsbInfoText_Default_ = lbEsbInfoText.Text;
            m_InfoBox_ForeColor_Default_ = InfoBox.ForeColor;
        }
        //
        private void WizardForm_Load(object sender, EventArgs e)
        {
            // Всю отрисовку данных делаем когда форма (и контролы) уже конкретно созданы
            // Если делать в конструкторе, то ListView накапливает ItemCheck(ed) евенты и неожиданно отдает их нам, когда мы первый раз их разрешаем слать
            // Хз.. его логику.. если здесь ListView заполнять данными, то он ненужные евенты не бросает
            bool b_esb_ok = DoUpdateEsbInit(Setting.EsbPath_);

            string sv8_installation_path = Setting.SV8Path_;
            if (sv8_installation_path == null) {
                sv8_installation_path = Path.Combine(Environment.GetFolderPath(Environment.SpecialFolder.ProgramFiles), EsbBuilderSetting.DEF_V8_DIR);
                if (!Directory.Exists(sv8_installation_path)) {
                    var env_ProgramFilesW6432 = Environment.GetEnvironmentVariable("ProgramW6432");
                    if (env_ProgramFilesW6432 != null) {
                        sv8_installation_path = Path.Combine(env_ProgramFilesW6432, EsbBuilderSetting.DEF_V8_DIR);
                        if (!Directory.Exists(sv8_installation_path))
                            sv8_installation_path = "";
                    }
                }
            }
            DoUpdate1Es(sv8_installation_path);    // также вызывает DoUpdateReady()

            if (!b_esb_ok)
                lvOneEs.Enabled = false;
            //
            ResizeOneEsListColumns(lvOneEs);

#if ESBVSIX_TEST
            bnDevTest.Visible = true;
#endif
        }

        private void ResizeOneEsListColumns(ListView lv)
        {
            // Ресайзим первую колонку
            //TODO  Иногда все-таки горизонтальный скролл выскакивает (почему-то?)
            //      и мы не учитываем возможный вертикальный скролл
            int c_cols = lv.Columns.Count;
            if (c_cols == 0)
                return;

            var lv_width = lv.Width - 4;
            for (int i_col = c_cols - 1; i_col > 0; --i_col) {
                lv_width -= lv.Columns[i_col].Width;
            }
            lv.Columns[0].Width = lv_width;
        }

        private void lvOneEs_Resize(object sender, EventArgs e)
        {
            ResizeOneEsListColumns((ListView)sender);
        }

        private void bnEsbPathSelect_Click(object sender, EventArgs e)
        {
            FolderBrowserDialog.Description = "Укажите расположение библиотеки Есб";
            FolderBrowserDialog.SelectedPath = SelEsbPath;
            if (FolderBrowserDialog.ShowDialog() == DialogResult.OK)
                DoUpdateEsbChange(FolderBrowserDialog.SelectedPath);                       
        }
        private void bnSelPath1CChange_Click(object sender, EventArgs e)
        {
            FolderBrowserDialog.Description = "Укажите инсталляционную директорию платформ 1С";
            if (FolderBrowserDialog.ShowDialog() == DialogResult.OK)
                DoUpdate1Es(FolderBrowserDialog.SelectedPath);
        }

        ItElem AddInfoLineElemFail(string text_) {
            AddInfoLine(text_);
            return null; ;
        }
        private ItElem DoMakeOneEsElem(string v8_dir_) 
        {
            string  v8_dir_name         = GetFolderNameFromDirPath(v8_dir_);

            Version v8_ver_from_dir     = MakeItElem1EsVersion(v8_dir_name);
            if (v8_ver_from_dir == null)
                return AddInfoLineElemFail($"Папка 1С '{v8_dir_}' имеет неожиданное имя для папки 1С (ожидается 8.3.хх.хххх). Будет пропущена.");
            
            string v8_dir_bin = Path.Combine(v8_dir_, "bin");
            if (!Directory.Exists(v8_dir_bin))
                return AddInfoLineElemFail($"В папке 1С '{v8_dir_}' отсутствует поддиректория 'Bin'. Будет пропущена.");

#if ESBVSIX_TEST
            string v8_bin_file = "core83.dll";
#else
            string v8_bin_file = ItElem.OENES_EXE_FILE_;
#endif
            string v8_exe_path = Path.Combine(v8_dir_bin, v8_bin_file);
            if (!File.Exists(v8_exe_path))
                return AddInfoLineElemFail($"В папке 1С '{v8_dir_bin}' отсутствует запускаемый 1С файл '{v8_bin_file}'. Будет пропущена.");

            Version v8_ver_from_exe = null;
            try {
                FileVersionInfo v8_ver_from_exe_info = FileVersionInfo.GetVersionInfo(v8_exe_path);
                var v8_ver_from_exe_info_ver = v8_ver_from_exe_info.ProductVersion;
                if (v8_ver_from_exe_info_ver == null)
                { // ну забыли в бинарь инфу о версии положить... с кем не бывает.. (v8.3.19.1467)
                    AddInfoLine($"У бинарника '{v8_exe_path}' отсутствует FileVersionInfo. Используем версию из имени папки: '{v8_ver_from_dir.ToString()}'.");
                    v8_ver_from_exe = v8_ver_from_dir;
                }
                else
                    v8_ver_from_exe = Version.Parse(v8_ver_from_exe_info_ver);
            }
            catch (Exception ex) { 
                return AddInfoLineElemFail($"Не удалось получить версию файла '{v8_exe_path}' по причине '{ex.ToString()}'. Будет пропущена.");
            }
            if( !ItElem.IsCompatableVersion(v8_ver_from_dir, v8_ver_from_exe) )
                return AddInfoLineElemFail($"Версия из имени папки '{v8_dir_name}'({v8_ver_from_dir}) не совпадает с версией файла '{v8_exe_path}'({v8_ver_from_exe}). Будет пропущена.");

            Util.ImageArchitecture v8_exe_architecture = Util.ImageArchitecture._Unknown;
            try {
                v8_exe_architecture = Util.GetProductArchitecture(v8_exe_path);
            }
            catch (Exception ex) {
                return AddInfoLineElemFail($"Не удалось получить архитектуру (x86,x64) для файла '{v8_exe_path}' по причине '{ex.ToString()}'. Будет пропущена.");
            }
            if (!(v8_exe_architecture == Util.ImageArchitecture.x86 || v8_exe_architecture == Util.ImageArchitecture.x64))
                return AddInfoLineElemFail($"Не распознана архитектура (x86,x64) для файла '{v8_exe_path}'. значение = '{v8_exe_architecture}'. Будет пропущена.");

            return new ItElem(v8_dir_, v8_dir_, v8_ver_from_exe, v8_exe_architecture);
        }        
        List<ItElem> DoMake1EsItemsAll(List<string[]> v8_dirs_array_list_)
        {
            List<ItElem> v8_items = new List<ItElem>();
            List<ItElem> esb_items = SelEsbItems;

            foreach (string[] v8_dirs_array in v8_dirs_array_list_) {
                foreach (string v8_install_dir in v8_dirs_array)
                {
                    ItElem v8_elem = DoMakeOneEsElem(v8_install_dir);
                    if (v8_elem != null)
                    {
                        v8_elem.EsbCompatable_ = (esb_items == null)? null : GetEsbCompaitable(esb_items, v8_elem);
                        v8_items.Add(v8_elem);
                    }
                }
            }

            return v8_items;
        }

        private bool DoUpdate1Es(string one_es_path_1cv8_) 
        {
            Sel1CPathBox.Text = one_es_path_1cv8_;
            m_1es_items = null;

            DoUpdate1EsUiList_Clear();
            DoUpdateReady(false);

            if (!Directory.Exists(one_es_path_1cv8_))
                return AddInfoError($"Папка '{one_es_path_1cv8_}' не существует.");

            List<string[]> v8_dirs_array_list = new List<string[]> { Directory.GetDirectories(one_es_path_1cv8_, "8*") };            
            if (v8_dirs_array_list[0].Count() == 0)
                return AddInfoError($"В папке '{one_es_path_1cv8_}' нет подпапок с инсталляциями 1С (нр. 8.3.хх.хххх).");
#if ESBVSIX_TEST
            string one_es_path_1cv8_test_x32 = Path.Combine(AppTestPath(), "1cv8x32");
            if (Directory.Exists(one_es_path_1cv8_test_x32))
                v8_dirs_array_list.Add(Directory.GetDirectories(one_es_path_1cv8_test_x32, "8*"));
            string one_es_path_1cv8_test_x64 = Path.Combine(AppTestPath(), "1cv8x64");
            if (Directory.Exists(one_es_path_1cv8_test_x64))
                v8_dirs_array_list.Add(Directory.GetDirectories(one_es_path_1cv8_test_x64, "8*"));

#endif
            m_1es_items = DoMake1EsItemsAll(v8_dirs_array_list);

            bool b_has_check = DoUpdate1EsUiList_Fill();
            DoUpdateReady(b_has_check);
            return b_has_check;
        }
        private bool DoUpdate1Es()
        {
            bool b_has_check = DoUpdate1EsUi();
            DoUpdateReady(b_has_check);
            return b_has_check;
        }
        private void DoUpdate1EsUiList_Clear() {
            lvOneEs.Items.Clear();
            lvOneEs.ListViewItemSorter = null;
        }
        private bool DoUpdate1EsUi() {
            DoUpdate1EsUiList_Clear();
            return DoUpdate1EsUiList_Fill();
        }
        
        void SetOneEsEsbUser(ListViewItem lv_item_, ItElem esb_) 
        {
            ItElem v8_item = lv_item_.Tag as ItElem; 
            if (v8_item != null)
                v8_item.EsbUser_ = esb_;

            SetOneEsEsbUi(lv_item_, esb_);
        }
        void SetOneEsEsbUi(ListViewItem lv_item_, ItElem esb_)
        {
            if (esb_ == null)
                lv_item_.SubItems[colEsb.Index].Text = "";
            else
                lv_item_.SubItems[colEsb.Index].Text = esb_.Name_;
        }

        private bool DoUpdate1EsUiList_Fill() 
        {
            if (m_1es_items == null)
                return false;

            bool b_only_supported   = (swShowUnsupported.Checked == false);
            bool b_has_check        = false;

            
            lvOneEs.ItemCheck -= lvOneEs_ItemCheck;
            lvOneEs.ItemChecked -= lvOneEs_ItemChecked;
            lvOneEs.BeginUpdate();
            
            foreach (ItElem v8_item in m_1es_items) {
                ItElem v8_item_esb_compaitable = v8_item.EsbCompatable_;
                if (b_only_supported && v8_item_esb_compaitable == null) {
                    //bypass
                }
                else
                {
                    ListViewItem lit = new ListViewItem(v8_item.Location_);
                    lit.Tag = v8_item;
                    lit.SubItems.Add(v8_item.Ver_.ToString());
                    lit.SubItems.Add(v8_item.Arch_.ToString());

                    ListViewItem.ListViewSubItem col_esb = new ListViewItem.ListViewSubItem();
                    if (v8_item_esb_compaitable != null)
                    {
                        col_esb.Text = v8_item_esb_compaitable.Name_;
                        lit.Checked = true;
                        b_has_check = true;
                    }
                    lit.SubItems.Add(col_esb);
                    ListViewItem lvIt = lvOneEs.Items.Add(lit);
                }
            }

            lvOneEs.ListViewItemSorter = new LvOneEsComparerByVer();

            lvOneEs.EndUpdate();
            lvOneEs.ItemCheck += lvOneEs_ItemCheck;
            lvOneEs.ItemChecked += lvOneEs_ItemChecked;

            return b_has_check;
        }
        private void bnDone_Click(object sender, EventArgs e)
        {
            if (swEsbPathSave.Visible && swEsbPathSave.Checked) {
                Exception ex = SaveSetting();
                if (ex != null)
                    MessageBox.Show(ex.Message, "Ошибка при сохранении настроек: " + ex.Message);
            }
            m_result_ok = true;
            this.Close();
        }

        private void bnCancel_Click(object sender, EventArgs e)
        {
            m_result_ok = false;
            this.Close();
        }

        private void tbEsbPath_TextChanged(object sender, EventArgs e)
        {

        }


        void DoUpdateReady(bool b_ready_) {
            bnDone.Enabled = b_ready_;
            if (b_ready_)
                InfoBox.ForeColor = m_InfoBox_ForeColor_Default_;
        }
        void DoUpdateReadyAuto() {
            bool b_ready = (lvOneEs.CheckedItems.Count > 0);
            DoUpdateReady(b_ready);
        }

        // при любом клике делаем строку выделенной (и перерисовываемся т.к. юзер сразу должен все видеть)
        private void lvOneEs_MouseDown(object sender, MouseEventArgs e)
        {
            var ht = lvOneEs.HitTest(e.Location);
            if (ht.Item != null && !ht.Item.Selected) {
                ht.Item.Selected = true;
                lvOneEs.Refresh();
            }
        }

        private void lvOneEs_ItemCheck(object sender, ItemCheckEventArgs e)
        {
            int             lv_item_index = e.Index;
            ListViewItem    lv_item = lvOneEs.Items[lv_item_index];
            ItElem          v8_item = (ItElem)lv_item.Tag;

            // если чек уже был как-то проставлен, то снять его всегда можно. проваливаемся дальше.
            if (e.CurrentValue == CheckState.Checked) {
                if (e.NewValue == CheckState.Unchecked && v8_item.EsbCompatable_ == null) { 
                    if(v8_item.EsbUser_ != null)
                        SetOneEsEsbUser(lv_item, null);     // если чек был сопоставлен с выбором пользователя, то сбрасываем вместе с выбором пользователя
                    else
                        SetOneEsEsbUi(lv_item, null);       // иначе просто сбрасываем текст
                }
                return;
            }
            
            // если 1с уже был сопоставлен есб, то установить чек можно без проверок. проваливаемся дальше.
            if (v8_item.EsbCompatable_ != null)
                return;

            // операция выбора НЕ БЛОКИРУЮЩАЯСЯ, поэтому мы отменяем изменение Check и вывалимся из события
            e.NewValue = e.CurrentValue;
            DoChooseEsbForSelected(lv_item);
        }

        static private ListViewItem GetListViewItemFromPoint(ListView lv_, Point mouse_position_)
        {
            Point lv_point = lv_.PointToClient(mouse_position_);
            return lv_.GetItemAt(lv_point.X, lv_point.Y);
        }

        // Событие lvOneEs_ItemChecked возникает при добавлении каждого элемента при обновлении окна БЕЗ СООТВЕТСТВУЮЩЕГО lvOneEs_ItemCheck
        // ПРОСТО ПО ФАКТУ ДОБАВЛЕНИЯ СТРОЧКИ (e.Item.Checked при этом false)
        // Причем что это событие возникает даже если ListViewItem создать отдельно, проставить ему чек, и добавлять уже чекнутым
        // Если же ListViewItem (после добавления или отдельно созданному) проставить .Checked=true, то вслед за этим "голым" lvOneEs_ItemChecked
        // возникает уже lvOneEs_ItemCheck и за ним следом опять lvOneEs_ItemChecked с e.Item.Checked==true
        //
        // ЕСЛИ ListView заполняется вызовом из конструктора формы, а НЕ из Form_Load, то он евенты НАКАПЛИВАЕТ! и выдает их когда присваевается хэндлер!
        private void lvOneEs_ItemChecked(object sender, ItemCheckedEventArgs e)
        {

        }

        private void AddInfoLine(string text_) {
            InfoBox.AppendText(text_);
            InfoBox.AppendText(Environment.NewLine);
        }
        private bool AddInfoLineFalse(string text_)
        {
            AddInfoLine(text_);
            return false;
        }
        private bool AddInfoError(string text_)
        {
            AddInfoLine(text_);
            return false;
        }
        private bool AddInfoAssert(string text_)
        {
#if DEBUG
            AddInfoLine("Упс... Что то не так: " + text_);
#else
//TODO
#endif
            return false;
        }

        int DialogMenuChooseTryCompatabable_Selected = -1;
        private void DialogMenuChooseTryCompatabable_OnItemClick(object sender, System.EventArgs e)
        {
            MenuItem mit = (MenuItem)sender;
            DialogMenuChooseTryCompatabable_Selected = mit.Index;
            mit.Tag = mit.Index;
            ContextMenu mm = (ContextMenu) mit.Parent;
            mm.Tag = mit.Index;
            MessageBox.Show("DialogMenuChooseTryCompatabable_OnItemClick:" + mm.Tag);
            //m.GetContextMenu().Tag = m;
            //MessageBox.Show("context_menu_item1_click");
        }
        ItElem DialogMenuChooseTryCompatabable(ItElem v8_item, ItElem esb_item_prev_, ItElem esb_item_next_) 
        {
            var evt_click = new System.EventHandler(this.DialogMenuChooseTryCompatabable_OnItemClick);
            
            MenuItem m1 = new MenuItem("", evt_click);
            if (esb_item_prev_ == null) {
                m1.Text = "<>";
                m1.Enabled = false;
            }
            else
                m1.Text = esb_item_prev_.ToString();

            //MenuItem m2 = new MenuItem("", new System.EventHandler(this.DialogMenuChooseTryCompatabable_OnItemClick));
            MenuItem m2 = new MenuItem("", evt_click);
            if (esb_item_next_ == null) {
                m2.Text = "<>";
                m2.Enabled = false;
            }
            else
                m2.Text = esb_item_next_.ToString();

            
            ContextMenu m = new ContextMenu(new MenuItem[] { m1, m2 });
            DialogMenuChooseTryCompatabable_Selected = -1;
            m.Show(bnDevTest, new System.Drawing.Point(20, 20));

            MessageBox.Show("DialogMenuChooseTryCompatabable:" + m.Tag);

            MenuItem selected_menu = null;
            if (DialogMenuChooseTryCompatabable_Selected >= 0)
                selected_menu = m.MenuItems[DialogMenuChooseTryCompatabable_Selected];
            if (selected_menu != null) {
                if (selected_menu.Index == 0)
                    MessageBox.Show("esb_item_prev_ :" + esb_item_prev_.ToString() + "; :" + selected_menu.Tag);
                else if (selected_menu.Index == 1)
                    MessageBox.Show("esb_item_next_ :" + esb_item_next_.ToString() + "; :" + selected_menu.Tag);
                else
                    MessageBox.Show("??? :");
            }
            return null;
        }

        private void context_menu_item1_click(object sender, System.EventArgs e)
        {
            MessageBox.Show("context_menu_item1_click");
        }



        ListViewItem ChooseEsbMenuArgument(object sender_) { 
            ContextMenuStrip     me = (ContextMenuStrip)sender_;
            return (ListViewItem)me.Tag;
        }
        ItElem OneEsItemInfo(ListViewItem lv_item_) {
            return (ItElem)lv_item_.Tag;
        }
        private void ChooseEsbMenu_ItemClicked(object sender, ToolStripItemClickedEventArgs e)
        {
            object          it_tag = e.ClickedItem.Tag;
            if (it_tag == null)
                return;
            
            ListViewItem    lv_item = ChooseEsbMenuArgument(sender);
            ItElem          esb_item = (ItElem)it_tag;

            lvOneEs.ItemCheck -= this.lvOneEs_ItemCheck;
                SetOneEsEsbUser(lv_item, esb_item);
                lv_item.Checked = true;
            lvOneEs.ItemCheck += this.lvOneEs_ItemCheck;
        }

        static void ChooseEsbMenuItem_Init(ToolStripMenuItem item_, string text_, ItElem esb_item_, bool b_enable_)
        {
            item_.Text = text_;
            item_.Tag = esb_item_;
            item_.Enabled = b_enable_;
        }
        static void ChooseEsbMenuItem_Reset(ToolStripMenuItem item_, string item_text_)
        {
            ChooseEsbMenuItem_Init(item_, item_text_, null, false);
        }
        static string ChooseEsbMenuItem_MakePrevText(string text_)      { return "Предыдущая: " + text_;   }
        static string ChooseEsbMenuItem_MakeNextText(string text_)      { return "Следующая : " + text_;   }    // + 1 пробел для выравнивания с прев

        void ChooseEsbMenuItem_ResetPrev() {
            ChooseEsbMenuItem_Reset(ChooseEsbMenuItemPrev, ChooseEsbMenuItem_MakePrevText("<отсутствует>") );
        }
        void ChooseEsbMenuItem_ResetNext()
        {
            ChooseEsbMenuItem_Reset(ChooseEsbMenuItemNext, ChooseEsbMenuItem_MakeNextText("<отсутствует>"));
        }
        void ChooseEsbMenuItem_InitPrev(string text_, ItElem esb_item_, bool b_enable_)
        {
            ChooseEsbMenuItem_Init(ChooseEsbMenuItemPrev, ChooseEsbMenuItem_MakePrevText(text_), esb_item_, b_enable_);
        }
        void ChooseEsbMenuItem_InitNext(string text_, ItElem esb_item_, bool b_enable_)
        {
            ChooseEsbMenuItem_Init(ChooseEsbMenuItemNext, ChooseEsbMenuItem_MakeNextText(text_), esb_item_, b_enable_);
        }

        private void ChooseEsbMenu_Closed(object sender, ToolStripDropDownClosedEventArgs e)
        {
            ChooseEsbMenuItem_ResetPrev();
            ChooseEsbMenuItem_ResetNext();
            ChooseEsbMenu.Tag = null;
        }
        void AssignEsbPrevNext(ItElem v8_item_, List<ItElem> esb_items_)
        {
            if (esb_items_ == null) {
                v8_item_.EsbNext_ = null;
                v8_item_.EsbNextState_ = CheckState.Checked;
                v8_item_.EsbPrev_ = null;
                v8_item_.EsbPrevState_ = CheckState.Checked;
                return;
            }
            
            foreach (ItElem esb_item in esb_items_)
            {
                int cmp = ItElem.CompareCompatable(esb_item, v8_item_);
                if (cmp < 0)
                    v8_item_.EsbPrev_ = esb_item;
                else if (cmp > 0) {
                    v8_item_.EsbNext_ = esb_item;
                    break;
                }
            }
            v8_item_.EsbPrevState_ = (v8_item_.EsbPrev_ == null)? CheckState.Unchecked : CheckState.Indeterminate;     
            v8_item_.EsbNextState_ = (v8_item_.EsbNext_ == null)? CheckState.Unchecked : CheckState.Indeterminate;
        }
        void EnsureEsbPrevNext(ItElem v8_item_) 
        {
            // если нас уже вызывали, то либо .EsbPrev_/.EsbNext_ не null, либо они null, но .EsbPrevOk_/.EsbNextOk_ true
            if (v8_item_.EsbPrev_ != null || v8_item_.EsbNext_ != null)
                return;
            if ((v8_item_.EsbPrev_ == null && v8_item_.EsbPrevState_ != CheckState.Indeterminate) || (v8_item_.EsbNext_ == null && v8_item_.EsbNextState_ != CheckState.Indeterminate))
                return;

            AssignEsbPrevNext(v8_item_, SelEsbItems);
        }
        bool DoChooseEsbForSelected(ListViewItem lv_item_) 
        {
            if (ChooseEsbMenu.Tag != null)
                AddInfoAssert("ChooseEsbMenu.Tag != null");                
            ItElem  v8_item = (ItElem)lv_item_.Tag;
            if (v8_item == null)
                return AddInfoAssert("v8_item == null");
            if (v8_item.EsbCompatable_ != null)
                return AddInfoAssert("v8_item.EsbCompatable_ != null");

            var coursor_current = Cursor.Current;
            Cursor.Current = Cursors.WaitCursor;

            
            EnsureEsbPrevNext(v8_item);
            //
            if (v8_item.EsbPrev_ == null)
                ChooseEsbMenuItem_ResetPrev();
            else 
            {
                bool b_compatable = (v8_item.EsbPrevState_ == CheckState.Checked);
                if (v8_item.EsbPrevState_ == CheckState.Indeterminate) {
                    b_compatable = TryResolveImport(v8_item, v8_item.EsbPrev_);
                    v8_item.EsbPrevState_ = (b_compatable)? CheckState.Checked : CheckState.Unchecked;
                }
                ChooseEsbMenuItem_InitPrev(v8_item.EsbPrev_.Name_, v8_item.EsbPrev_, b_compatable);
            }
            if (v8_item.EsbNext_ == null)
                ChooseEsbMenuItem_ResetNext();
            else
            {
                bool b_compatable = (v8_item.EsbNextState_ == CheckState.Checked);
                if (v8_item.EsbNextState_ == CheckState.Indeterminate) {
                    b_compatable = TryResolveImport(v8_item, v8_item.EsbNext_);
                    v8_item.EsbNextState_ = (b_compatable)? CheckState.Checked : CheckState.Unchecked;
                }
                ChooseEsbMenuItem_InitNext(v8_item.EsbNext_.Name_, v8_item.EsbNext_, b_compatable);
            }

            Cursor.Current = coursor_current;

            ChooseEsbMenu.Tag = lv_item_;
            ChooseEsbMenu.Show(Cursor.Position);
            // Можно сделать меню-типа-диалог блокирующемся (как Win32::TrackPopUpMenu), но это как-то не по-с#-шарповски...
            /*            while (ChooseEsbMenu.Visible) {
                            Application.DoEvents();
                        }
            */
            return true;
        }
        bool TryResolveImport(ItElem v8_item_, ItElem esb_item_)
        {
            if (SelResolveImportsVerbose_)
                return TryResolveImportVerb(v8_item_, esb_item_);
            else
                return TryResolveImportSilent(v8_item_, esb_item_);
        }
        bool TryResolveImportSilent(ItElem v8_item_, ItElem esb_item_) 
        {
            string  esb_item_lib = GetEsbLibPathName(esb_item_, Util.ImageBuildType.Debug, v8_item_.Arch_);
            var     esb_item_lib_imports = Util.GetProductImports(esb_item_lib);
            foreach (var x_imp_lib in esb_item_lib_imports) {
                string v8_lib_path = Path.Combine(v8_item_.Location_, "bin", x_imp_lib.ImpLibName_ + ".dll");
                if (Util.ResolveImports(x_imp_lib.ImpEntries_, v8_lib_path) > 0)
                    return false;
            }
            return true;
        }
        bool TryResolveImportVerb(ItElem v8_item_, ItElem esb_item_)
        {
            string esb_item_lib         = GetEsbLibPathName(esb_item_, Util.ImageBuildType.Debug, v8_item_.Arch_);
            var esb_item_lib_imports    = Util.GetProductImports(esb_item_lib);
            bool b_resolve_ok           = true;
            AddInfoLine($"Проверяем импорты для '{esb_item_lib}' у платформы '{v8_item_.Location_}'.");
            foreach (var x_imp_lib in esb_item_lib_imports)
            {
                string v8_dll_path_name = Path.Combine(v8_item_.Location_, "bin", x_imp_lib.ImpLibName_ + ".dll");
                AddInfoLine($"Необходима '{v8_dll_path_name}'.");
                if (File.Exists(v8_dll_path_name)) {
                    var not_resolve_set = Util.ResolveImportsEx(x_imp_lib.ImpEntries_, v8_dll_path_name);
                    if (not_resolve_set.Count == 0)
                        AddInfoLine($"Все импорты разрешены.");
                    else
                    { 
                        b_resolve_ok = false;
                        foreach (string imp_symbol in not_resolve_set) {
                            AddInfoLine($"Необходимый симбол '{imp_symbol}' в библиотеке '{v8_dll_path_name}' отсутствует.");
                            string s_name = Util.UndecorateName(imp_symbol);
                            if (s_name == null)
                                AddInfoAssert($"!Undecorate '{imp_symbol}' failed.");
                            else
                                AddInfoLine($"({s_name})");
                        }                            
                    }
                }
                else {
                    AddInfoLine($"Необходимая '{v8_dll_path_name}' не найдена.");
                    b_resolve_ok = false;
                }         
            }
            return b_resolve_ok;
        }
        bool DoTryResolveImportVerbForSelectedOneEs() {
            if (lvOneEs.SelectedItems.Count == 0)
                return AddInfoLineFalse("Не выбрана 1С для проверки импорта.");

            ListViewItem lv_item = lvOneEs.SelectedItems[0];
            ItElem v8_item = lv_item.Tag as ItElem;
            if (v8_item == null)
                return AddInfoAssert("Выбранная 1С не инициализирована");
            
            if (v8_item.EsbCompatable_ != null)
                return TryResolveImportVerb(v8_item, v8_item.EsbCompatable_);
            else if(v8_item.EsbUser_ != null)
                return TryResolveImportVerb(v8_item, v8_item.EsbUser_);

            return false;
        }
        private void button1_Click(object sender, EventArgs e)
        {
#if ESBVSIX_TEST
            ListViewItem lv_item = lvOneEs.SelectedItems[0];
            ItElem v8_item = (ItElem)lv_item.Tag;
            string one_es_executable_path = v8_item.GetOneEsLocationBin();
            
            string wizard_destanation_directory_path = AppTestPath();
            string one_es_executable_file = Path.Combine(one_es_executable_path, ItElem.OENES_EXE_FILE_);
            string one_es_ibase_path = Path.Combine(wizard_destanation_directory_path, ItElem.OENES_IBASE_DIR_);
            string one_es_files_path = Path.Combine(wizard_destanation_directory_path, ItElem.OENES_FILES_DIR_);
            string one_es_log_file = Path.Combine(wizard_destanation_directory_path, "_1s_create.log");
            Util.DoCreateEsInfobase(one_es_executable_file, one_es_ibase_path, one_es_files_path, one_es_log_file);
#endif

            //DoTryResolveImportVerbForSelectedOneEs();
        }
        private void swShowUnsupported_CheckedChanged(object sender, EventArgs e)
        {
            //TODO  Нужно сделать swShowUnsupported Enabled/Disabled в зависимости от собственно наличия этих "неподдерживаемых" платформ
            //      (а то к примеру, когда или Есб или 1С вообще не найдены, эта доступная галочка вводит в заблуждение)
            DoUpdate1Es();
        }
    } // form

    class LvOneEsComparerByVer : System.Collections.IComparer
    {
        private int col;
        public LvOneEsComparerByVer()
        {
            col = 0;
        }
        public LvOneEsComparerByVer(int column)
        {
            col = column;
        }
        public int Compare(object a, object b)
        {

            ItElem it_a = (ItElem)((ListViewItem)a).Tag;
            if (it_a == null)
                return -1;
            ItElem it_b = (ItElem)((ListViewItem)b).Tag;
            return it_a.Ver_.CompareTo(it_b.Ver_);
            //return String.Compare(((ListViewItem)x).SubItems[col].Text, ((ListViewItem)y).SubItems[col].Text);
        }
    }

    
    public struct ItElemConfig 
    {
        public Util.ImageArchitecture  Arch_;
        public Util.ImageBuildType     BuildType_;
        public ItElemConfig(Util.ImageArchitecture arch_, Util.ImageBuildType build_type_) {
            Arch_ = arch_;
            BuildType_ = build_type_;
        }
    };
    class ItElem : IComparable<ItElem>
    {
        public string                   Name_;
        public string                   Location_;
        public Version                  Ver_;
        List<ItElemConfig>              Configs_;
        //
        public Util.ImageArchitecture   Arch_ = Util.ImageArchitecture._Unknown;
        // Для v8Items
        public ItElem                   EsbCompatable_ = null;  // авто-определнная совместимая версия Есб (ее может не быть - нулл, но юзер может вбрать себе EsbUser_)
        //
        public ItElem                   EsbPrev_ = null;
        public CheckState               EsbPrevState_ = CheckState.Indeterminate;
        public ItElem                   EsbNext_ = null;        // если не null, но EsbNextOk_ false, значит по ABI не подходит
        public CheckState               EsbNextState_ = CheckState.Indeterminate; // если true, но EsbNext_ null значит проверено и следующего просто нет
        public ItElem                   EsbUser_ = null;
        //
        public void ResetEsb() 
        {
            EsbCompatable_ = null;
            ResetEsbPrevNext();
        }
        public void ResetEsbPrevNext()
        {
            EsbPrev_ = null;
            EsbPrevState_ = CheckState.Indeterminate;
            EsbNext_ = null;
            EsbNextState_ = CheckState.Indeterminate;
            EsbUser_ = null;
        }
        private ItElem(string name_, string location_, Version ver_)
        {
            Name_ = name_;
            Location_ = location_;
            Ver_ = ver_;
        }
        public ItElem(string name_, string location_, Version ver_, Util.ImageBuildType build_type_) : this(name_, location_, ver_)
        {
            Configs_ = new List<ItElemConfig> { new ItElemConfig(Util.ImageArchitecture._Unknown, build_type_) };
        }
        public ItElem(string name_, string location_, Version ver_, Util.ImageArchitecture architecture_): this(name_, location_, ver_)
        {
            Arch_ = architecture_;
        }

        public const string ESB_VERSION_TAG_PREFIX = "v";
        public string GetEsbVersionTag() {
            return ESB_VERSION_TAG_PREFIX + 
                                Ver_.Major +    //8
                                Ver_.Minor +    //3
                                ((Ver_.Build < 10)? ("0" + Ver_.Build.ToString()) : Ver_.Build.ToString())
                                ;     
        }
        public string GetOneEsLocationBin() {
            return Path.Combine(Location_, "Bin");
        }
        public static string OENES_EXE_FILE_ = "1cv8.exe";
        public static string OENES_IBASE_DIR_ = "1s";
        public static string OENES_FILES_DIR_ = "1s_files";
        //
        public override string ToString()
        {
            return Name_;
        }
        //
        public int CompareTo(ItElem other_)
        {
            return Name_.CompareTo(other_.Name_);
        }
        //
        public bool IsCompatableWithOneEsItElem(ItElem other_one_es_item_)
        {
            if (other_one_es_item_.Ver_ == null)
                return false;
            //major.minor[.build[.revision]]
            //при сравнении .revision игнорируем
            //return (Ver_.Major == one_es_item_.Ver_.Major && Ver_.Minor == one_es_item_.Ver_.Minor && Ver_.Build == one_es_item_.Ver_.Build);
            return IsCompatableVersion(this.Ver_, other_one_es_item_.Ver_);
        }
        public static bool IsCompatableVersion(Version a, Version b) {
            return (a.Major == b.Major && a.Minor == b.Minor && a.Build == b.Build);
        }
        public static int CompareCompatable(ItElem a, ItElem b) {
            return CompareCompatableVer(a.Ver_, b.Ver_);
        }
        public static int CompareCompatableVer(Version a, Version b)
        {
            int cmp_maj = a.Major.CompareTo(b.Major);            if (cmp_maj != 0) return cmp_maj;
            int cmp_min = a.Minor.CompareTo(b.Minor);            if (cmp_min != 0) return cmp_min;
            return a.Build.CompareTo(b.Build);
        }
    };
    class ItElemSearchVer
    {
        Version Ver_;
        public bool IsCompatableWithOneEsItElem(ItElem one_es_item_)
        {
            if (one_es_item_.Ver_ == null)
                return false;
            //major.minor[.build[.revision]]
            //при сравнении .revision игнорируем
            return (Ver_.Major == one_es_item_.Ver_.Major && Ver_.Minor == one_es_item_.Ver_.Minor && Ver_.Build == one_es_item_.Ver_.Build);
        }
    }


    class XW32 
    {
        /*
        [DebuggerDisplay("{Value}")]
        public readonly unsafe struct PWSTR : IEquatable<PWSTR>
        {
            public readonly char* Value;
            public PWSTR(char* value) => Value = value;
            public static implicit operator char*(PWSTR value) => value.Value;
            public static implicit operator PWSTR(char* value) => new(value);
            public static bool operator ==(PWSTR left, PWSTR right) => left.Value == right.Value;
            public static bool operator !=(PWSTR left, PWSTR right) => !(left == right);
            public bool Equals(PWSTR other) => Value == other.Value;
            public override bool Equals(object obj) => obj is PWSTR other && Equals(other);
            public override int GetHashCode() => (int)Value;
            public override string ToString() => new PCWSTR(Value).ToString();
            public static implicit operator PCWSTR(PWSTR value) => new(value.Value);
            public int Length => new PCWSTR(Value).Length;
            public Span<char> AsSpan() => Value is null ? default : new Span<char>(Value, Length);
        };

        [DebuggerDisplay("{" + nameof(DebuggerDisplay) + "}")]
        public readonly unsafe struct PCWSTR : IEquatable<PCWSTR>
        {
            public readonly char* Value;
            public PCWSTR(char* value) => Value = value;
            public static explicit operator char*(PCWSTR value) => value.Value;
            public static implicit operator PCWSTR(char* value) => new(value);
            public bool Equals(PCWSTR other) => Value == other.Value;
            public override bool Equals(object obj) => obj is PCWSTR other && Equals(other);
            public override int GetHashCode() => (int)Value;
            public int Length
            {
                get
                {
                    var p = Value;

                    if (p is null)
                        return 0;

                    while (*p != '\0')
                        p++;

                    return checked((int)(p - Value));
                }
            }
            public override string ToString() => Value is null ? string.Empty : new string(Value);
            public ReadOnlySpan<char> AsSpan() => Value is null ? default : new ReadOnlySpan<char>(Value, Length);
            private string DebuggerDisplay => ToString();
        };


        [DebuggerDisplay("{Value}")]
        public readonly struct HANDLE : IEquatable<HANDLE>
        {
            public readonly IntPtr Value;
            HANDLE(IntPtr value_) {
                Value = value_;
            }
            public static HANDLE Null => default;
            public bool IsNull => Value == default;
            public static implicit operator IntPtr(HANDLE value) => value.Value;
            //public static explicit operator HANDLE(IntPtr value) => new(value);
            public static bool operator ==(HANDLE left, HANDLE right) => left.Value == right.Value;
            public static bool operator !=(HANDLE left, HANDLE right) => !(left == right);
            public bool Equals(HANDLE other) => Value == other.Value;
            public override bool Equals(object obj) => obj is HANDLE other && Equals(other);
            public override int GetHashCode() => Value.GetHashCode();
            public override string ToString() => $"0x{Value:x}";
        }

        public struct PROCESS_INFORMATION
        {
            public HANDLE hProcess;
            public HANDLE hThread;
            public uint dwProcessId;
            public uint dwThreadId;
        }


        [DllImport("KERNEL32.dll", ExactSpelling = true, SetLastError = true)]
        [DefaultDllImportSearchPaths(DllImportSearchPath.System32)]
        public static extern unsafe BOOL CreateProcessW(
        PCWSTR lpApplicationName,
        PWSTR lpCommandLine,
        [Optional] SECURITY_ATTRIBUTES* lpProcessAttributes,
        [Optional] SECURITY_ATTRIBUTES* lpThreadAttributes,
        BOOL bInheritHandles,
        PROCESS_CREATION_FLAGS dwCreationFlags,
        [Optional] void* lpEnvironment,
        PCWSTR lpCurrentDirectory,
        STARTUPINFOW* lpStartupInfo,
        PROCESS_INFORMATION* lpProcessInformation);

        */


        //UnDecorateSymbolName(szName, szUndName, sizeof(szUndName), UNDNAME_COMPLETE)  
        //UNDNAME_COMPLETE == 0     https://learn.microsoft.com/ru-ru/windows/win32/api/dbghelp/nf-dbghelp-undecoratesymbolnamew
        //(CA1838: не используйте параметры StringBuilder для вызовов P/Invoke)
        //https://learn.microsoft.com/ru-ru/dotnet/fundamentals/code-analysis/quality-rules/ca1838
        //
        //
        //Using the C#/Win32 code generator to enhance your WinUI 3 app
        //https://www.sharpgis.net/post/Using-the-CWin32-code-generator-to-enhance-your-WinUI-3-app
        //
        //Generating PInvoke code for Win32 apis using a Source Generator
        //https://www.meziantou.net/generate-pinvoke-code-for-win32-apis-using-a-source-generator.htm
        //
        //https://github.com/microsoft/CsWin32/
        //https://www.pinvoke.dev/
        //
/*        [DllImport("Dbghelp.dll", SetLastError = true)]
        public static extern uint UnDecorateSymbolName(string decorated, StringBuffer undecoratedBuffer, uint undecoratedBufferLength, uint flags);
*/

        //https://developercommunity.visualstudio.com/t/libexe-does-not-rename-export-names-from-dll-when/1548994
        //https://learn.microsoft.com/ru-ru/windows/win32/debug/pe-format       + IMPORT_OBJECT_ORDINAL
        //https://learn.microsoft.com/en-us/windows/win32/debug/pe-format#the-idata-section
        //https://learn.microsoft.com/en-us/cpp/build/reference/overview-of-lib?view=msvc-170
        //
        //https://processhacker.sourceforge.io/doc/maplib_8c_source.html

    };


    public class Util
    {




        /* How to get Visual Studio installation path
         * Установить
            https://www.nuget.org/packages/Microsoft.VisualStudio.Setup.Configuration.Interop/
            (	нугет пакеты устанавливаются для каждого проекта отдельно в ссылках и в его папку
	            это сделано видимо чтобы при создании нового проекта всегда качать новую версию
	            и не устраивать на своей машине зоопарк версий
	            но есть возможность сослаться на локальный кэш пакетов который находится в c:\Users\Garry\.nuget\packages\
	            см. https://stackoverflow.com/questions/58308275/global-package-installation-using-nuget)
            )
         * https://learn.microsoft.com/ru-ru/visualstudio/extensibility/launch-visual-studio-dte?view=vs-2022           (просто пример для SetupConfiguration)
         * https://github.com/microsoft/vsixbootstrapper/blob/develop/src/VSIXBootstrapper.Shared/VSIXBootstrapper.h    (просто пример для с++)
         * https://github.com/microsoft/vs-setup-samples/issues/6   (здесь народ MSBuild.exe ищет)
         * https://llvm.org/doxygen/MSVCSetupApi_8h_source.html     (здесь как на с++ с этим работать)
         */

        // видимо должно работать только под DevEnv (DTE)
        public static ISetupInstance GetVsSetupInstanceCurrent() {
            ISetupConfiguration setup_сonfig = new SetupConfiguration();
            return setup_сonfig.GetInstanceForCurrentProcess();
        }

        //* https://learn.microsoft.com/ru-ru/visualstudio/extensibility/launch-visual-studio-dte?view=vs-2022           (просто пример для SetupConfiguration)
        public static IEnumerable<ISetupInstance> GetSetupInstances()
        {
            ISetupConfiguration setupConfiguration = new SetupConfiguration();
            IEnumSetupInstances enumerator = setupConfiguration.EnumInstances();

            int count;
            do
            {
                ISetupInstance[] setupInstances = new ISetupInstance[1];
                enumerator.Next(1, setupInstances, out count);
                if (count == 1 && setupInstances[0] != null)
                {
                    yield return setupInstances[0];
                }
            }
            while (count == 1);
        }
        public static ISetupInstance GetVsSetupInstance1st()
        {
            ISetupConfiguration setup_сonfig = new SetupConfiguration();
            IEnumSetupInstances setup_instances_enumerator = setup_сonfig.EnumInstances();

            int setup_instances_retrieved_count;
            ISetupInstance[] setup_instances_retrieve = new ISetupInstance[1];
            setup_instances_enumerator.Next(1, setup_instances_retrieve, out setup_instances_retrieved_count);
            if (setup_instances_retrieved_count == 1 && setup_instances_retrieve[0] != null)
                return setup_instances_retrieve[0];
            else
                return null;
        }
        // В визарде мы опираемся на текущую инсталляцию студии, а в отладке на че попало...
        public static ISetupInstance GetVsSetupInstanceAuto()
        {
#if ESBVSIX_TEST
            return GetVsSetupInstance1st();
#else
            return GetVsSetupInstanceCurrent();
#endif
            /*
            try
            {
                return GetVsSetupInstanceCurrent();
            }
            catch {
                return GetVsSetupInstance1st();
            }
            */
        }
        public static string GetVsPathRoot(ISetupInstance instance_) {
            return instance_.ResolvePath(); //везде так делают - оно '\' в конце добавляет
        }

        //TODO  Вот тут явно видно, что или весь VcTools нужно делать отдельным классом т.к. постоянно GetVsSetupInstance дергаем
        //      нужно один раз получать (или Instance или только ее рут-путь)
        public static string GetVsPathRoot()
        {
            return GetVsPathRoot(GetVsSetupInstanceAuto());
        }

        //TODO  Вот здесь толковей с учетом инсталлированных пакетов
        //      https://gist.github.com/damianh/0215b54dbb4b2570461b6d2866acd284

        public static string GetVcToolsPath(string vs_instance_root_)   //C:\Program Files\Microsoft Visual Studio\2019\Enterprise\
        {
            // Реально тулзы находятся где-то здесь
            // "c:\Program Files\Microsoft Visual Studio\2019\Enterprise\VC\Tools\MSVC\14.29.30133\bin\[Hostx86|Hostx64]\[x86|x64]\";
            //      Никакого более умного способа выйти на \Tools не нашел
            //      Здесь тоже народ ищет MSBuild и ничего более умного кроме жесткого задания пути не знает
            //      * https://github.com/microsoft/vs-setup-samples/issues/6
            //      Зададим кусочек пути, и начиная с него поиск уже относительно быстр:
            const string VC_TOOLS_MSVC = @"VC\Tools\MSVC\";
            string inst_VcToolsMSVC = Path.Combine(vs_instance_root_, VC_TOOLS_MSVC);
            //TODO  Непонятно может ли быть на месте \14.29.30133\ что-то другое или несколько таких папок. И как с ними разбираться?
            //      Или для для конкретной ISetupInstance всегда что-то фиксированное
            string inst_VcToolsMSVC_Bin = Directory.EnumerateDirectories(inst_VcToolsMSVC, "bin", SearchOption.AllDirectories).LastOrDefault();
            // DumpBin вообще-то любой хоть 32 хоть 64 работает, но думаю правильно так
            // *возможно более правильно https://learn.microsoft.com/en-us/dotnet/api/system.runtime.interopservices.runtimeinformation.osarchitecture?view=net-8.0)
            // *вдруг на маке запустят... тьфу, тьфу..
            string s_BIN_TAIL = (Environment.Is64BitOperatingSystem)? @"Hostx64\x64" : @"Hostx86\x86";
            // мы не проверяем валидность. кому надо сам проверит или выбросит исключение
            return Path.Combine(inst_VcToolsMSVC_Bin, s_BIN_TAIL);
        }
        public static string GetVcToolsPath() {
            return GetVcToolsPath(GetVsPathRoot());
        }
        public static string GetVcTool(ISetupInstance instance_, string tool_file_name_ext_)
        {
            /*
            var inst_GetDescription         = instance_.GetDescription();           //Комплексное масштабируемое решение для команд любого размера.
            var inst_GetDisplayName         = instance_.GetDisplayName();           //Visual Studio Enterprise 2019
            var inst_GetInstallationName    = instance_.GetInstallationName();      //VisualStudio/16.11.19+32901.82
            var inst_GetInstallationPath    = instance_.GetInstallationPath();      //C:\Program Files\Microsoft Visual Studio\2019\Enterprise
            var inst_GetInstallationVersion = instance_.GetInstallationVersion();   //16.11.32901.82
            var inst_GetInstanceId          = instance_.GetInstanceId();            //87a5a19c
            var inst_GetInstallDate         = instance_.GetInstallDate();           //??? {System.Runtime.InteropServices.ComTypes.FILETIME}: dwHighDateTime	30919249	int, 		dwLowDateTime	788119296	int
            */
            string inst_Root                = instance_.ResolvePath();              //C:\Program Files\Microsoft Visual Studio\2019\Enterprise\

            /*
            ISetupInstance2 inst2 = (ISetupInstance2) instance_;
            var inst2_GetEnginePath     = inst2.GetEnginePath();
            var inst2_GetProduct        = inst2.GetProduct();
            var inst2_GetProductPath    = inst2.GetProductPath();
            ISetupPackageReference[] inst2_GetPackages       = inst2.GetPackages();     Здесь хренова тонна инсталлированных пакетов (см. https://gist.github.com/damianh/0215b54dbb4b2570461b6d2866acd284)
            ISetupPropertyStore inst2_GetProperties         = inst2.GetProperties();
            string[] inst2_GetProperties_names = inst2_GetProperties.GetNames();
            //++ GetState ++ etc...
            */

            // Реально тулзы находятся где-то здесь
            // @"c:\Program Files\Microsoft Visual Studio\2019\Enterprise\VC\Tools\MSVC\14.29.30133\bin\Hostx86\x86\";
            //TODO  Поиск начиная с inst_Root ДОЛГО.
            //      Никакого более умного способа выйти на \Tools не нашел
            //      Здесь тоже народ ищет MSBuild и ничего более умного кроме жесткого задания пути не знает
            //      * https://github.com/microsoft/vs-setup-samples/issues/6
            //      Зададим кусочек пути, и начиная с него поиск уже относительно быстр:
            const string VC_TOOLS_MSVC = @"VC\Tools\MSVC\";
            string inst_VcToolsMSVC = Path.Combine(inst_Root, VC_TOOLS_MSVC);
            //TODO  Непонятно может ли быть на месте \14.29.30133\ что-то другое или несколько таких папок. И как с ними разбираться?
            //      Или для для конкретной ISetupInstance всегда что-то фиксированное
            //TODO  Кусочек \Hostx86\x86\ может быть разный в зависимости от на какой машине смотрим
            //      На x86 машине папка Hostx64 фактически пуста (там только какой-то llvm-symbolizer.exe ???)
            //      Но на x64 машине не пусты ни \Hostx86 ни \Hostx64
            //      То есть на x64 можно вызывать как х86 тулзы так и х64 тулзы
            //      и каждую из них можно вызывать как для х86 образов так и для х64 образов (? разбираться надо...)
            string inst_VcToolsMSVC_Bin = Directory.EnumerateDirectories(inst_VcToolsMSVC, "bin", SearchOption.AllDirectories).LastOrDefault();

            // DumpBin вообще-то любой хоть 32 хоть 64 работает, но думаю правильно так
            // *возможно более правильно https://learn.microsoft.com/en-us/dotnet/api/system.runtime.interopservices.runtimeinformation.osarchitecture?view=net-8.0)
            // *вдруг на маке запустят... тьфу, тьфу..
            string VC_TOOLS_BIN_X = (Environment.Is64BitOperatingSystem)? @"Hostx64\x64" : @"Hostx86\x86";
            // мы не проверяем валидность. кому надо сам проверит или выбросит исключение
            return Path.Combine(inst_VcToolsMSVC_Bin, VC_TOOLS_BIN_X, tool_file_name_ext_);
        }

        public static string GetVcTool_DumpBin()    { return Path.Combine(GetVcToolsPath(), "dumpbin.exe");        }
        public static string GetVcTool_Lib()        { return Path.Combine(GetVcToolsPath(), "lib.exe"); }
        public static string GetVcTool_UndName()    { return Path.Combine(GetVcToolsPath(), "undname.exe"); }


        public static Process OneEsTool(string tool_file_, string tool_arguments_)
        {
            Process p = new Process();
            p.StartInfo.UseShellExecute = false;
            p.StartInfo.RedirectStandardOutput = true;
            p.StartInfo.FileName = tool_file_;
            p.StartInfo.Arguments = tool_arguments_;
            //https://stackoverflow.com/questions/5377423/hide-console-window-from-process-start-c-sharp
            //processTemp.EnableRaisingEvents = true;   ???
            p.StartInfo.CreateNoWindow = true;
            return p;
        }        
        public static int OneEsTool_CreateInfoBase(string one_es_executable_, string infobase_path_, string log_file_path_name_)
        {
            Process one_es_process_create = OneEsTool(one_es_executable_, $@"CREATEINFOBASE File=""{infobase_path_}""; /DisableStartupMessages /DisableStartupDialogs /Out""{log_file_path_name_}""");
            one_es_process_create.Start();
            one_es_process_create.WaitForExit();
            int one_es_process_result = one_es_process_create.ExitCode;
            one_es_process_create.Close();
            one_es_process_create.Dispose();
            one_es_process_create = null;
            return one_es_process_result;
        }
        public static int OneEsTool_LoadConfigFromFilesAll(string one_es_executable_, string infobase_path_, string load_files_path_, string log_file_path_name_, bool truncate_log_)
        {
            string opt_truncate_log = (truncate_log_)? "" : "-NoTruncate";
            Process one_es_process_load = OneEsTool(one_es_executable_, $@"DESIGNER /F""{infobase_path_}"" /DisableStartupMessages /DisableStartupDialogs /LoadConfigFromFiles""{load_files_path_}"" /UpdateDBCfg /Out""{log_file_path_name_}"" {truncate_log_}");
            one_es_process_load.Start();
            one_es_process_load.WaitForExit();
            int one_es_process_result = one_es_process_load.ExitCode;
            one_es_process_load.Close();
            one_es_process_load.Dispose();
            one_es_process_load = null;
            return one_es_process_result;
        }

        public static bool DoCreateEsInfobase(string one_es_executable_, string ibase_path_, string files_path_, string log_file_)
        {
            string one_es_log_text = "";
            int one_es_result = -1;
            try
            {
                one_es_result = Util.OneEsTool_CreateInfoBase(one_es_executable_, ibase_path_, log_file_);
                if (one_es_result != 0)
                {
                    one_es_log_text = File.ReadAllText(log_file_, Encoding.GetEncoding(1251));
                    MessageBox.Show($"Создание тестовой базы 1С не выполнено (результат={one_es_result})." + Environment.NewLine +
                                                    "Ответ 1С:" + Environment.NewLine + $"{one_es_log_text}", "Создание тестовой базы 1C");
                    return false;
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show($"Создание тестовой базы 1С не выполнено в результате ошибки:{Environment.NewLine}{ex.Message}", "Создание тестовой базы 1C");
                return false;
            }

            try
            {
                one_es_result = Util.OneEsTool_LoadConfigFromFilesAll(one_es_executable_, ibase_path_, files_path_, log_file_, false);
                if (one_es_result != 0)
                {
                    one_es_log_text = File.ReadAllText(log_file_, Encoding.GetEncoding(1251));
                    MessageBox.Show($"Тестовая база 1С создана, но загрузка файлов конфигурации не выполнена (результат={one_es_result})." + Environment.NewLine +
                                                    "Ответ 1С:" + Environment.NewLine + $"{one_es_log_text}", "Создание тестовой базы 1C");
                    return false;
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show($"Тестовая база 1С создана, но загрузка файлов конфигурации не выполнена в результате ошибки:{Environment.NewLine}{ex.Message}", "Создание тестовой базы 1C");
                return false;
            }

#if ESBVSIX_TEST
            one_es_log_text = File.ReadAllText(log_file_, Encoding.GetEncoding(1251));
            MessageBox.Show($"Создание тестовой базы 1С завершено успешно (результат={one_es_result})." + Environment.NewLine +
                                                "Ответ 1С:" + Environment.NewLine + $"{one_es_log_text}", "Создание тестовой базы 1C");
#endif
            try
            {
                File.Delete(log_file_);
            }
            catch (Exception ex)
            {
                MessageBox.Show($"Создание тестовой базы 1С завершено успешно (результат={one_es_result})." + Environment.NewLine +
                                                    "Ответ 1С:" + Environment.NewLine + $"{one_es_log_text}" + Environment.NewLine + Environment.NewLine +
                                                    $"Но! удаление файла лога 1С '{log_file_}'не выполнено из-за ошибки:{Environment.NewLine}{ex.Message}", "Создание тестовой базы 1C");
            }
            return true;
        }



        public static Process VcTool(string tool_file_, string tool_arguments_)
        {
            Process p = new Process();
            p.StartInfo.UseShellExecute = false;
            p.StartInfo.RedirectStandardOutput = true;
            p.StartInfo.FileName = tool_file_;
            p.StartInfo.Arguments = tool_arguments_;
            //https://stackoverflow.com/questions/5377423/hide-console-window-from-process-start-c-sharp
            //processTemp.EnableRaisingEvents = true;   ???
            p.StartInfo.CreateNoWindow = true;
            return p;
        }

        public static string EnsureInQuote(string path_name_) {
            if (path_name_.StartsWith(@""""))
                return path_name_;
            else
                return @"""" + path_name_ + @"""";
        }
        public static Process VcTool_DumpBinHeaders(string image_path_name_)
        {
            Process p = VcTool(GetVcTool_DumpBin(), "/NOLOGO /HEADERS " + EnsureInQuote(image_path_name_));
            p.Start();
            return p;
        }
        public static Process VcTool_DumpBinExports(string image_path_name_)
        {
            Process p = VcTool(GetVcTool_DumpBin(), "/NOLOGO /EXPORTS " + EnsureInQuote(image_path_name_));
            p.Start();
            return p;
        }
        public static Process VcTool_UndName(string symbol_)
        {
            // у undname нет флага /NOLOGO. имеющиеся флаги только отключают какую-либо функциональность.
            // по умолчанию показывает все что может, а если не может возвращает ту же строку
            Process p = VcTool(GetVcTool_UndName(), symbol_);
            p.Start();
            return p;
        }

        public static Process VcTool_DumpBinLinkMember1(string image_path_name_)
        {
            /* https://learn.microsoft.com/en-us/cpp/build/reference/linkermember?view=msvc-170
             * **Specify the 1 argument to display symbols in object order, along with their offsets**
             * При указании параметра :1 дамп делается в порядке находящихся в ЛИБ объектов (.obj, .lib) (то что нам нужно)
             * Мы считываем симболы импортов и затем __IMPORT_DESCRIPTOR_<lib_name> откуда их брать
             * (если :2, то как-то 'offsets and index numbers of objects, and then list the symbols in alphabetical order, along with the object index for each'
             *  непонятно, короче. нам не нужно..)
             *  Если : не указано то и так и сяк. вообще каша..
             */
            Process p = VcTool(GetVcTool_DumpBin(), "/NOLOGO /LINKERMEMBER:1 " + EnsureInQuote(image_path_name_));
            p.Start();
            return p;
        }

        //TODO  Как то желательно процесс без исключений убивать...
        //      https://stackoverflow.com/questions/4281425/how-to-avoid-a-system-runtime-interopservices-comexception
        public static void ProcessStopNoError(ref Process ref_proc_) 
        {
            try { ref_proc_.Kill(); } catch { } finally { ref_proc_.Dispose(); };
            ref_proc_ = null;
            return;
        }
        public enum ImageArchitecture
        {
            x86 = System.Runtime.InteropServices.Architecture.X86,
            x64 = System.Runtime.InteropServices.Architecture.X64,
            _Unknown = -1
        };
        public enum ImageBuildType 
        { 
            Debug, 
            Release 
        };    //must be the same as ESBHLP_LIB_DIR_DEBUG/ESBHLP_LIB_DIR_RELEASE

        public static ImageArchitecture GetProductArchitecture(string image_file_)
        {
            Process p = Util.VcTool_DumpBinHeaders(image_file_);
            StreamReader p_out = p.StandardOutput;
            string s_line = p_out.ReadLine();
            bool b_header = false;
            ImageArchitecture n_architecture = ImageArchitecture._Unknown;
            while (s_line != null)
            {
                if (b_header)
                {
                    if (s_line.EndsWith("(x86)"))
                    { //'             14C machine (x86)'
                        n_architecture = ImageArchitecture.x86;
                        break;
                    }
                    if (s_line.EndsWith("(x64)"))
                    { //'             8664 machine (x64)'
                        n_architecture = ImageArchitecture.x64;
                        break;
                    }
                }
                else if (s_line == "FILE HEADER VALUES")
                    b_header = true;

                s_line = p_out.ReadLine();
            }

            ProcessStopNoError(ref p);

            return n_architecture;
        }

        private static string GetDumpExportSymbol(string dump_line_)
        {
            /*
             *'    ordinal hint RVA      name'
             *''
             *'          1    0 00012700 ??0?$Refcountable@VStyleEntryBase@core@@UAtomicCounter@2@@core@@QAE@ABV01@@Z
             *                           ^pos==26
             *'...'
             *''
             */

            string s_symbol = dump_line_.Substring(26);
            Debug.Assert(s_symbol.Length > 0);
            return s_symbol;
        }
        public static string UndecorateName(string symbol_)
        {
            Process         p = Util.VcTool_UndName(symbol_);
            StreamReader    p_out = p.StandardOutput;
            string          s_line = p_out.ReadLine();
            int             n_line = 0;
            string          s_name = null;
            while (s_line != null)
            {
                if (n_line == 4) {
                    // Лого не отключается
                    /* 
                     * Microsoft (R) C++ Name Undecorator
                     * Copyright (C) Microsoft Corporation. All rights reserved.
                     *
                     * Undecoration of :- "foo"
                     * is :- "foo"
                     */
                    if (s_line.StartsWith("is :- \"")) {
                        s_name = s_line.Substring(7, s_line.Length - 8);    // кавычки убираем тоже
                        break;
                    }
                }
                s_line = p_out.ReadLine();
                ++n_line;
            }

            p.WaitForExit();
            p.Dispose();
            //ProcessStopNoError(ref p);

            return s_name;
        }

        public static HashSet<string> GetProductExports(string image_file_, int capacity_hint_)
        {
            Process p = Util.VcTool_DumpBinExports(image_file_);
            StreamReader p_out = p.StandardOutput;
            string  s_line = p_out.ReadLine();
            bool    b_start = false;
            HashSet<string> exports_set = new HashSet<string>(capacity_hint_);
            while (s_line != null)
            {
                if (b_start) {
                    if (s_line.Length == 0) 
                        break;
                    string  s_symbol = GetDumpExportSymbol(s_line);
                    bool    b_added = exports_set.Add(s_symbol);
                    Debug.Assert(b_added);  //expect unique
                }
                else if (s_line == "    ordinal hint RVA      name") {
                    p_out.ReadLine();   // read one empty line after title
                    b_start = true;
                }
                s_line = p_out.ReadLine();
            }

            ProcessStopNoError(ref p);
            
            return exports_set;
        }
        public static int ResolveImports(HashSet<string> imports_, string link_to_image_file_)
        {
            Process         p = Util.VcTool_DumpBinExports(link_to_image_file_);
            StreamReader    p_out = p.StandardOutput;
            string          s_line = p_out.ReadLine();
            bool            b_start = false;
            int             n_imports = imports_.Count;
            while (s_line != null)
            {
                if (b_start)
                {
                    if (s_line.Length == 0)
                        break;
                    string s_symbol = GetDumpExportSymbol(s_line);
                    if (imports_.Contains(s_symbol) && --n_imports == 0)
                        break;
                }
                else if (s_line == "    ordinal hint RVA      name")
                {
                    p_out.ReadLine();   // read one empty line after title
                    b_start = true;
                }
                s_line = p_out.ReadLine();
            }

            ProcessStopNoError(ref p);

            return n_imports;
        }
        public static HashSet<string> ResolveImportsEx(HashSet<string> imports_, string link_to_image_file_)
        {
            Process         p = Util.VcTool_DumpBinExports(link_to_image_file_);
            StreamReader    p_out = p.StandardOutput;
            string          s_line = p_out.ReadLine();
            bool            b_start = false;
            int             n_imports = imports_.Count;
            HashSet<string> x_imports = new HashSet<string>(imports_);
            while (s_line != null)
            {
                if (b_start)
                {
                    if (s_line.Length == 0)
                        break;
                    string s_symbol = GetDumpExportSymbol(s_line);
                    if (x_imports.Remove(s_symbol) && --n_imports == 0)
                        break;
                }
                else if (s_line == "    ordinal hint RVA      name")
                {
                    s_line = p_out.ReadLine();   // read one empty line after title
                    Debug.Assert(s_line.Length == 0);
                    b_start = true;
                }
                s_line = p_out.ReadLine();
            }

            ProcessStopNoError(ref p);

            return x_imports;
        }

        public class ProductImportLib {
            public string           ImpLibName_;
            public HashSet<string>  ImpEntries_;
            public ProductImportLib() {
                ImpEntries_ = new HashSet<string>();
            }
        };
        public static List<ProductImportLib> GetProductImports(string image_file_)
        {
            Process         p = Util.VcTool_DumpBinLinkMember1(image_file_);
            StreamReader    p_out = p.StandardOutput;
            string          s_line;
            bool            b_start = false;
            List<ProductImportLib>  import_libs = new List<ProductImportLib>();
            ProductImportLib        import_lib_current = new ProductImportLib{};

            while (true)
            {
                /*
                 *'    1505 public symbols'
                 *''
                 *'    25AA4 ?deallocate@__new_alloc@stlp_std@@SAXPAXI@Z
                 *'    25AA4 __imp_?deallocate@__new_alloc@stlp_std@@SAXPAXI@Z'
                 *           ^11
                 *'...'
                 *'    2621E stl83_NULL_THUNK_DATA'
                 *'    2636C __NULL_IMPORT_DESCRIPTOR'
                 *'    264A0 __IMPORT_DESCRIPTOR_stl83'
                 *'...'
                 *'    2C5C2 ??$?8VRoundMode@esb@@@_internal@esb@@YA_NABVRoundMode@1@0@Z'
                 *'    2C5C2 ??$check_and_make@VString@esb@@@esb@@YA?AVString@0@$$QAV?$InterfacePtr@VIValue@esb@@@core@@@Z'
                 *... no double line up to end
                 */
                s_line = p_out.ReadLine();
                if (s_line == null)
                    break;

                if (b_start)
                {
                    if (s_line.Length == 0)
                        break;

                    string s_symbol_1 = s_line.Substring(10);
                    if (s_symbol_1.EndsWith("_NULL_THUNK_DATA"))
                        continue;
                    if (s_symbol_1 == "__NULL_IMPORT_DESCRIPTOR")
                        continue;
                    if (s_symbol_1.StartsWith("__IMPORT_DESCRIPTOR_")) {                    Debug.Assert(import_lib_current.ImpLibName_ == null);
                        import_lib_current.ImpLibName_ = s_symbol_1.Substring(20);
                        import_libs.Add(import_lib_current);
                        import_lib_current = new ProductImportLib{};
                        continue;
                    }
                    
                    string s_line_2 = p_out.ReadLine();
                    // в блоке импортируемых символов после символа обязательно должно быть __imp_... его
                    // и вообще - блок символов должен быть непрерывным. если считали пустую строку - конец
                    if (s_line_2.Length == 0)
                        break;

                    string s_symbol_2 = s_line_2.Substring(10);
                    if ( !(s_symbol_2.StartsWith("__imp_")) )
                        break;

                    string s_symbol_2_symb = s_symbol_2.Substring(6);
                    if ( !(s_symbol_2_symb == s_symbol_1) )
                        break;
                    
                    bool b_added = import_lib_current.ImpEntries_.Add(s_symbol_2_symb);      Debug.Assert(b_added);  //TODO  expect unique
                }
                else if (s_line.EndsWith("public symbols"))
                {
                    s_line = p_out.ReadLine();   // read one empty line after title
                    Debug.Assert(s_line.Length == 0);
                    b_start = true;
                }
            }

            if (import_lib_current.ImpLibName_ != null || (import_lib_current.ImpEntries_ != null && import_lib_current.ImpEntries_.Count > 0))
                import_libs.Add(import_lib_current);

            ProcessStopNoError(ref p);

            return import_libs;
        }

        public static Version GetFileProductVersion(string file_path_name_) {
            FileVersionInfo v = FileVersionInfo.GetVersionInfo(file_path_name_);
            return Version.Parse(v.ProductVersion);
        }
    }; // Util

    class EsbBuilderSetting
    {
        public const string REGISTERY_ROOT_             = "Software\\IntUnsigned\\Esb";
        public const string REGISTERY_ESB_PATH_         = "Location";
        public const string REGISTERY_OES_PATH_         = "SV8Path";
        public const string REGISTERY_VERBOSE_          = "ResolveImportsVerbose";
        public const string DEF_V8_DIR                  = "1cv8";
        //
        public const string     DEF_ESB_PATH_           = @"C:\cpp\esb";
        public const bool DEF_RESOLVE_IMPORTS_VERBOSE_  = false;
        //

        public string   EsbPath_ = DEF_ESB_PATH_;
        public string   SV8Path_ = null;
        public bool     ResolveImportsVerbose_ = DEF_RESOLVE_IMPORTS_VERBOSE_;
        //
        //
        public static EsbBuilderSetting Load()
        {
            EsbBuilderSetting setting = new EsbBuilderSetting();
            using (RegistryKey key = Registry.CurrentUser.OpenSubKey(REGISTERY_ROOT_))
            {
                if (key != null)
                {
                    setting.EsbPath_                = (string) key.GetValue(REGISTERY_ESB_PATH_, EsbBuilderSetting.DEF_ESB_PATH_);
                    setting.SV8Path_                = key.GetValue(REGISTERY_OES_PATH_) as string;
                    setting.ResolveImportsVerbose_  = (bool)key.GetValue(REGISTERY_VERBOSE_, EsbBuilderSetting.DEF_RESOLVE_IMPORTS_VERBOSE_);
                }
            }
            return setting;
        }
        public void Save(string esb_path_, string esp_path_, bool resolve_imports_verbose_) {
            using (RegistryKey key = Registry.CurrentUser.CreateSubKey(REGISTERY_ROOT_, true))
            {
                if (this.EsbPath_ != esb_path_)
                {
                    key.SetValue(REGISTERY_ESB_PATH_, esb_path_, RegistryValueKind.String);
                    this.EsbPath_ = esb_path_;
                }
                if (this.SV8Path_ == null || this.SV8Path_ != esp_path_) {
                    key.SetValue(REGISTERY_OES_PATH_, esp_path_, RegistryValueKind.String);
                    this.SV8Path_ = esp_path_;
                }
                if (this.ResolveImportsVerbose_ != resolve_imports_verbose_)
                {
                    key.SetValue(REGISTERY_VERBOSE_, resolve_imports_verbose_, RegistryValueKind.DWord);
                    this.ResolveImportsVerbose_ = resolve_imports_verbose_;
                }

                key.Flush();
            }
        }
    };//EsbBuilderSetting

    class ListViewHlp {
        public static void ResetAllCheck(ListView lv) {
            foreach (ListViewItem it in lv.Items)
                it.Checked = false;
        }
    };
}
