using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Microsoft.VisualStudio.TemplateWizard;
using System.Windows.Forms;
using System.Diagnostics;
using EnvDTE;
using Microsoft.VisualStudio.Shell;
using Microsoft.VisualStudio.Shell.Interop;
using System.IO;


//** Как пошаговое руководство использовал это. Все этапы в одном месте
//** Use wizards with project templates
//https://learn.microsoft.com/en-us/visualstudio/extensibility/how-to-use-wizards-with-project-templates?view=vs-2022
//Токен открытого ключа: 955a189734fe190c



//https://habr.com/ru/articles/448216/
//https://learn.microsoft.com/en-us/cpp/build/reference/vcxproj-file-structure?view=msvc-170
//От МС Последовательное по пунктам создание .vcxproj вручную
//https://github.com/MicrosoftDocs/cpp-docs/blob/main/docs/build/walkthrough-using-msbuild-to-create-a-visual-cpp-project.md
// ZIP Task
//https://stackoverflow.com/questions/8397449/msbuild-and-creating-zip-files?rq=4


namespace EsbStudio
{
    public class EsbProjectTemplateWizardImplementation : IWizard
    {
        //Microsoft.VisualStudio.TemplateWizard.

        //private UserInputForm inputForm;
        private WizardForm MyWizardForm;
        //private DTE m_dte = null;

        private string customMessage;

        // This method is called before opening any item that
        // has the OpenInEditor attribute.
        public void BeforeOpeningFile(ProjectItem projectItem)
        {
            //ThreadHelper.ThrowIfNotOnUIThread();
            //Debug.WriteLine("BeforeOpeningFile " + projectItem.Name);
            Debug.WriteLine("BeforeOpeningFile ");
        }

        public void ProjectFinishedGenerating(Project project)
        {
            /*            var xsol = m_dte.Solution;
                        var cprj = xsol.Projects;
            */
            //Debug.WriteLine("ProjectFinishedGenerating " + project.Name);
            Debug.WriteLine("ProjectFinishedGenerating ");
        }

        // This method is only called for item templates,
        // not for project templates.
        public void ProjectItemFinishedGenerating(ProjectItem        projectItem)
        {
            Debug.WriteLine("ProjectItemFinishedGenerating ");


            /* https://stackoverflow.com/questions/52180635/how-to-have-static-folders-within-a-visual-studio-projectgroup-vstemplate
            try
            {
                project.ProjectItems.Item("file1.xml").Remove();
                project.ProjectItems.Item("file2.xml").Remove();
                project.ProjectItems.Item("image.png").Remove();
            }
            catch
            {
               ....
            }
            */
        }


        // This method is called after the project is created.
        public void RunFinished()
        {
            Debug.WriteLine("RunFinished ");

            /* https://stackoverflow.com/questions/52180635/how-to-have-static-folders-within-a-visual-studio-projectgroup-vstemplate
             try
            {
                var src = Path.Combine(_destinationDirectory, "packages");
                var dest = Path.Combine(_solutionDirectory, "admin");
                System.IO.Directory.CreateDirectory(dest);
                dest = Path.Combine(dest, "packages");
                Directory.Move(src, dest);
            }
            catch
            {
               ...
            }
             */
        }
        const string S_WIZARD_CAPTION = "Мастер создания проекта Есб";


        //тут о студии-2005, но книга и разжевано
        //https://flylib.com/books/en/3.50.1.31/1/


        /*  RunStarted запускается, когда IDE уже стартовало, отработало окно Создать-Проект, ИзШаблона и т.п. и для Dte.Solution вызвано .Create
         *  *Этот метод вызывается сразу после загрузки расширения мастера и перед Раздел TemplateData файла .vstemplate начинает обрабатываться*
         *  https://flylib.com/books/en/3.50.1.31/1/
         *  ! Гхм.. Вероятно имелось в виду *Раздел TemplateContent файла .vstemplate начинает обрабатываться* !
         *  ! т.к. раздел TemplateData содержит общую информацию для Создать->Проект->ИзШаблона !
         *  Короче.. папка для решения уже создана, но полностью пуста - даже .sln нет.
         */
        public void RunStarted(object dte_object_, Dictionary<string, string> replacements_dictionary_, WizardRunKind runKind, object[] custom_params_)
        {            
            /* Accessing DTE(Development tool environment) object should only be done on main thread, otherwise throw exception
             * 
             * Ожидается, что исключение, выброшенное этим методом, не будет кем-либо перехвачено и обработано. 
             * Он предназначен для того, чтобы вызвать сбой вызывающего объекта и привлечь его внимание к тому, 
             * что ему следует вызывать код выдачи в потоке пользовательского интерфейса. 
             * Это аналогично выдаче ArgumentException , когда вызывающий объект просто не понимает контракт метода, который он вызывает.
             * https://learn.microsoft.com/ru-ru/dotnet/api/microsoft.visualstudio.shell.threadhelper.throwifnotonuithread?view=visualstudiosdk-2022
             */
            ThreadHelper.ThrowIfNotOnUIThread();
            // Здесь много с DTE работают ручками. фактически отменяют первоначальное создание решения и пересоздают его в другом мест и по другому
            // Само IDE то у нас открыто и его DTE мы получить можем и можем чудить с его АПИ че попало...
            // var dte2 = (DTE) System.Runtime.InteropServices.Marshal.GetActiveObject("VisualStudio.DTE.10.0");
            // https://stackoverflow.com/questions/3882764/issue-with-visual-studio-template-directory-creation
            // (пока DTE не используем)
            DTE dev_enviroment = (DTE)dte_object_;
            Solution my_solution = dev_enviroment.Solution;


            // NOEXCEPT
            // параметр шаблона $destinationdirectory$ у МС не описан, но все его используют
            // https://learn.microsoft.com/ru-ru/visualstudio/ide/template-parameters?view=vs-2022
            string wizard_destanation_directory_path = "";
            if (!replacements_dictionary_.TryGetValue("$destinationdirectory$", out wizard_destanation_directory_path))
            {
                //https://stackoverflow.com/questions/774104/what-exceptions-should-be-thrown-for-invalid-or-unexpected-parameters-in-net
                throw new ArgumentException("Требуемый аргумен шаблона '$destinationdirectory$' не найден.");
            }
            // это то же самое что и $destinationdirectory$, если галочка "проект и решение в одном каталоге" проставлена,
            // иначе может быть что-то другое и при этом параметр ($specifiedsolutionname$) должен быть не пуст
            // пс. https://stackoverflow.com/questions/47217538/specificsolutionname-is-always-empty-how-can-i-tell-if-the-user-is-making-a-n/47222951#47222951
            string wizard_solution_directory_path = "";
            if (!replacements_dictionary_.TryGetValue("$solutiondirectory$", out wizard_solution_directory_path))
            { /*TODO*/ }


            try
            {
                MyWizardForm = new WizardForm();
                MyWizardForm.ShowDialog();

                if (!MyWizardForm.IsResultOk) {
                    /*
                     * По сути, любое исключение, вызванное IWizard.RunStarted, приведет к отмене процесса мастера. Однако, чтобы быть добросовестным гражданином VSIX, 
                     * вам следует генерировать исключение WizardCancelledException или WizardBackoutException . 
                     * Единственная разница между этими исключениями заключается в том, что WizardBackoutException возвращает пользователя к диалоговому окну «Добавить новый проект»,
                     * (типа кнопка "Назад")
                     * тогда как WizardCancelledException показывает IDE такой, какой она была до отображения диалогового окна «Добавить новый проект».
                     * https://www.neovolve.com/2011/07/19/pitfalls-of-cancelling-a-vsix-project-template-in-an-iwizard/
                     */
                    //throw new WizardBackoutException();      //The exception that is thrown when the template wizard is backed out before it has completed.
                    throw new WizardCancelledException();    //The exception that is thrown when the template wizard is cancelled before it has completed.
                }


                // Если все Ок, то
                //1. Копируем доп.файлы, которые Студия не видит
                //  (мы это делаем здесь, в RunStarted, а не в RunFinished т.к. могут быть ошибки и не стоит оставлять проект сделанным наполовину)
                //
                //  На заметку - customParams[0] это полный путь к .vstemplate файлу, и мы можем оттуда скопировать прочие нужные файлы,
                //  которые template не копирует т.к. на них нет ссылок в .vcxproj
                // (опять не описано, но все используют)
                string s_vstemplate_file = (string)custom_params_[0];
                string s_vstemplate_path = Path.GetDirectoryName(s_vstemplate_file);

                /* The contents of the WizardData element is passed, unchanged, as a parameter inside the string dictionary of parameters in the IWizard.RunStarted method. 
                 * The dictionary key is named $wizarddata$
                 * !! Передается ВСЕ тупо текстом вплоть до комментариев !!
                 * !! Никаких ошибок мы здесь не проверяем - конструктор шаблона должен отработать правильно !!
                 * https://learn.microsoft.com/en-us/visualstudio/extensibility/wizarddata-element-visual-studio-templates?view=vs-2022
                 */
                string wizard_data = "";
                replacements_dictionary_.TryGetValue("$wizarddata$", out wizard_data);
                string[] wizard_ex_files = wizard_data.Split(';');

                Action<string> copy_it = (string file_name_) => {
                    File.Copy(Path.Combine(s_vstemplate_path, file_name_), Path.Combine(wizard_destanation_directory_path, file_name_));
                };
                foreach (string ex_file in wizard_ex_files) {
                    if(ex_file.StartsWith(@"\"))
                        Directory.CreateDirectory(Path.Combine(wizard_destanation_directory_path, ex_file.Substring(1)));
                    else
                        copy_it(ex_file);
                }


                //2. Добавляем все настройки для выбранных конфигураций
                if(MyWizardForm.SelOneEsItems.Count == 0)
                    throw new ArgumentException("Конфигурации для создания проекта не выбраны.");   //Не должно быть! Ошибка в MyWizardForm.IsResultOk

                // сначала добавим все пустыми, а потом перепишем выбранными (ТУДУ)
                replacements_add_all_empty(replacements_dictionary_, app.config_ver_param);
                replacements_add_all_empty(replacements_dictionary_, app.config_run_param);
                replacements_add_all_empty(replacements_dictionary_, app.config_cfg_param);
                replacements_add_all_empty(replacements_dictionary_, app.config_sht_param);
                //
                string one_es_executable_path = "";
                foreach (ListViewItem lv_item in MyWizardForm.SelOneEsItems) {
                    ItElem v8_item = (ItElem)lv_item.Tag;
                    // Нас интересует .Location_, где находится сама 1С,
                    // .Arch_ - архитектура платформы (сама платформа тас интересует только в смысле запуска теста. соответственно на х32-платформе нельзя запускать тест х64 и наоборот)
                    // .EsbCompatable_ или .EsbUser_ - выбранные версии Есб
                    ItElem esb_item = (v8_item.EsbCompatable_ != null)? v8_item.EsbCompatable_ : v8_item.EsbUser_;
                    string esb_item_v8VerTag = esb_item.GetEsbVersionTag();
                    replacements_set_ver_param(replacements_dictionary_, esb_item_v8VerTag);
                    one_es_executable_path = v8_item.GetOneEsLocationBin(); //используем последнюю попавшуюся.  TODO возможно нужно как-то умнее.. Вопрос задать??
                    replacements_set_run_param(replacements_dictionary_, esb_item_v8VerTag, v8_item.Arch_, one_es_executable_path);
                    replacements_set_cfg_param(replacements_dictionary_, esb_item_v8VerTag);
                    replacements_set_sht_param(replacements_dictionary_, esb_item_v8VerTag);
                }

                //3. Создаем тестовую базу 1С
                string one_es_executable_file = Path.Combine(one_es_executable_path, ItElem.OENES_EXE_FILE_);
                string one_es_files_path = Path.Combine(wizard_destanation_directory_path, ItElem.OENES_FILES_DIR_);
                string one_es_ibase_path = Path.Combine(wizard_destanation_directory_path, ItElem.OENES_IBASE_DIR_);
                string one_es_log_file = Path.Combine(wizard_destanation_directory_path, "_1s_create.log");
                if (!Util.DoCreateEsInfobase(one_es_executable_file, one_es_ibase_path, one_es_files_path, one_es_log_file)) {
                    if (MessageBox.Show($"Создание тестовой базы 1С не завершилось успешно. Отменить создание шаблона проекта ???{Environment.NewLine}" +
                        $"(ПС. Тестовую базу можно создать позже из файлов используя конфигуратор 1С)", S_WIZARD_CAPTION, MessageBoxButtons.YesNo) == DialogResult.Yes) {
                        throw new WizardCancelledException();
                    }
                }
            }
            catch (Exception ex)
            {
                if (!(ex is WizardCancelledException))
                {
                    MessageBox.Show("Работа мастера прервана в результате неустранимой ошибки: " + ex.ToString(), S_WIZARD_CAPTION);
                    DoCleanup(wizard_destanation_directory_path, wizard_solution_directory_path, "");
                }
                else
                    DoCleanup(wizard_destanation_directory_path, wizard_solution_directory_path, "Создение проекта отменено. ");
                //
                throw;
            }
        } // RunStarted


        static void replacements_add_all_empty(Dictionary<string, string> replacements_dictionary_, Func<int, string> param_key_constructor_) {
            for (int i_ver = app.V8_VER_MIN_; i_ver <= app.V8_VER_MAX_; ++i_ver)
            {
                replacements_dictionary_.Add(param_key_constructor_(i_ver), "");
            }
        }

        static void replacements_set(Dictionary<string, string> replacements_dictionary_, string s_param_key_, string s_param_val_, string part_) {
            string s_param_old = "";
            if (replacements_dictionary_.TryGetValue(s_param_key_, out s_param_old))
            {
                if (s_param_old.Length > 0)
                    throw new ArgumentException($"{part_}. Параметр '{s_param_key_}' уже установлен в '{s_param_old}'.");
            }
            else
                throw new ArgumentException($"{part_}. Некорректный параметр '{s_param_key_}'.");

            replacements_dictionary_[s_param_key_] = s_param_val_;
        }
        //
        static void replacements_set_ver_param(Dictionary<string, string> replacements_dictionary_, string v8VerTag_) {
            replacements_set(replacements_dictionary_, app.config_ver_param(v8VerTag_), make_config_ver_part(v8VerTag_), "Настройка версий");
        }
        static void replacements_set_run_param(Dictionary<string, string> replacements_dictionary_, string v8VerTag_, Util.ImageArchitecture v8Arch_, string v8Path_) {
            replacements_set(replacements_dictionary_, app.config_run_param(v8VerTag_), make_config_run_part(v8VerTag_, v8Arch_, v8Path_), "Настройка запуска");
        }
        static void replacements_set_cfg_param(Dictionary<string, string> replacements_dictionary_, string v8VerTag_) {
            replacements_set(replacements_dictionary_, app.config_cfg_param(v8VerTag_), make_config_cfg_part(v8VerTag_), "Настройка конфигурации");
        }
        static void replacements_set_sht_param(Dictionary<string, string> replacements_dictionary_, string v8VerTag_) {
            replacements_set(replacements_dictionary_, app.config_sht_param(v8VerTag_), make_config_sht_part(v8VerTag_), "Настройка LocalAppDataPlatform");
        }

        // Для каждой выбранной версии 1С мы делаем 4 конфигурации - Debug/Release + Win32/x64
        static string make_config_ver_part(string v8VER_) {
            return
$@"<ProjectConfiguration Include=""{v8VER_}Debug|Win32"">
      <Configuration>{v8VER_}Debug</Configuration>
      <Platform>Win32</Platform>
</ProjectConfiguration>
<ProjectConfiguration Include=""{v8VER_}Debug|x64"">
    <Configuration>{v8VER_}Debug</Configuration>
    <Platform>x64</Platform>
</ProjectConfiguration>
<ProjectConfiguration Include=""{v8VER_}Release|Win32"">
    <Configuration>{v8VER_}Release</Configuration>
    <Platform>Win32</Platform>
</ProjectConfiguration>
<ProjectConfiguration Include=""{v8VER_}Release|x64"">
    <Configuration>{v8VER_}Release</Configuration>
    <Platform>x64</Platform>
</ProjectConfiguration>
";
        }

        static string make_config_run_part(string v8VerTag_, Util.ImageArchitecture v8Arch_, string v8Path_) {
            if (v8Arch_ == Util.ImageArchitecture.x86)
                return make_config_run_part_32(v8VerTag_, v8Path_);
            else if (v8Arch_ == Util.ImageArchitecture.x64)
                return make_config_run_part_64(v8VerTag_, v8Path_);
            else
                throw new ArgumentException($"Конфигурирование запуска. Неожидаемое значение для архитектуры проекта: {v8Arch_}.");
        }
        static string make_config_run_part_64(string v8VER_, string v8PATH_)
        {
            return
$@"
    <PropertyGroup Label=""EsbSettingsRun"" Condition=""'$(Configuration)|$(Platform)'=='{v8VER_}Debug|x64'"">
        <ESB_1C_PATH>{v8PATH_}</ESB_1C_PATH>
    </PropertyGroup>
    <PropertyGroup Label=""EsbSettingsRun"" Condition=""'$(Configuration)|$(Platform)'=='{v8VER_}Release|x64'"">
        <ESB_1C_PATH>{v8PATH_}</ESB_1C_PATH>
    </PropertyGroup>
";
        }
        static string make_config_run_part_32(string v8VER_, string v8PATH_)
        {
            return
$@"
    <PropertyGroup Label=""EsbSettingsRun"" Condition=""'$(Configuration)|$(Platform)'=='{v8VER_}Debug|Win32'"">
        <ESB_1C_PATH>{v8PATH_}</ESB_1C_PATH>
    </PropertyGroup>
    <PropertyGroup Label=""EsbSettingsRun"" Condition=""'$(Configuration)|$(Platform)'=='{v8VER_}Release|Win32'"">
        <ESB_1C_PATH>{v8PATH_}</ESB_1C_PATH>
    </PropertyGroup>
";
        }

        // Для каждой выбранной версии 1С мы делаем 4 конфигурации - Debug/Release + Win32/x64
        static string make_config_cfg_part(string v8VerTag_) {
            return
$@"
  <PropertyGroup Condition=""'$(Configuration)|$(Platform)'=='{v8VerTag_}Debug|Win32'"" Label=""Configuration"">
    <UseDebugLibraries>true</UseDebugLibraries>
  </PropertyGroup>
  <PropertyGroup Condition=""'$(Configuration)|$(Platform)'=='{v8VerTag_}Release|Win32'"" Label=""Configuration"">
    <UseDebugLibraries>false</UseDebugLibraries>
    <WholeProgramOptimization>true</WholeProgramOptimization>
  </PropertyGroup>
  <PropertyGroup Condition=""'$(Configuration)|$(Platform)'=='{v8VerTag_}Debug|x64'"" Label=""Configuration"">
    <UseDebugLibraries>true</UseDebugLibraries>
  </PropertyGroup>
  <PropertyGroup Condition=""'$(Configuration)|$(Platform)'=='{v8VerTag_}Release|x64'"" Label=""Configuration"">
    <UseDebugLibraries>false</UseDebugLibraries>
    <WholeProgramOptimization>true</WholeProgramOptimization>
  </PropertyGroup>
";
        }
        static string make_config_sht_part(string v8VerTag_) {
            return
$@"
  <ImportGroup Condition=""'$(Configuration)|$(Platform)'=='{v8VerTag_}Debug|Win32'"" Label=""PropertySheets"">
    <Import Project=""$(UserRootDir)\Microsoft.Cpp.$(Platform).user.props"" Condition=""exists('$(UserRootDir)\Microsoft.Cpp.$(Platform).user.props')"" Label=""LocalAppDataPlatform"" />
  </ImportGroup>
  <ImportGroup Condition=""'$(Configuration)|$(Platform)'=='{v8VerTag_}Release|Win32'"" Label=""PropertySheets"">
    <Import Project=""$(UserRootDir)\Microsoft.Cpp.$(Platform).user.props"" Condition=""exists('$(UserRootDir)\Microsoft.Cpp.$(Platform).user.props')"" Label=""LocalAppDataPlatform"" />
  </ImportGroup>
  <ImportGroup Condition=""'$(Configuration)|$(Platform)'=='{v8VerTag_}Debug|x64'"" Label=""PropertySheets"">
    <Import Project=""$(UserRootDir)\Microsoft.Cpp.$(Platform).user.props"" Condition=""exists('$(UserRootDir)\Microsoft.Cpp.$(Platform).user.props')"" Label=""LocalAppDataPlatform"" />
  </ImportGroup>
  <ImportGroup Condition=""'$(Configuration)|$(Platform)'=='{v8VerTag_}Release|x64'"" Label=""PropertySheets"">
    <Import Project=""$(UserRootDir)\Microsoft.Cpp.$(Platform).user.props"" Condition=""exists('$(UserRootDir)\Microsoft.Cpp.$(Platform).user.props')"" Label=""LocalAppDataPlatform"" />
  </ImportGroup>
";
        }

        void DoCleanup(string destanation_directory_path_, string solution_directory_path_, string s_message_prefix_) 
        {
            if (Directory.Exists(destanation_directory_path_))
            {
                if (MessageBox.Show($"{s_message_prefix_}Удалить созданную папку '{destanation_directory_path_}' ?", S_WIZARD_CAPTION, MessageBoxButtons.YesNo) == DialogResult.Yes)
                {
                    Directory.Delete(destanation_directory_path_, true);
                }
                if (solution_directory_path_ != destanation_directory_path_ && Directory.Exists(solution_directory_path_)  )
                {
                    // это проверка на пустую папку
                    /*&& !Directory.EnumerateFileSystemEntries(solution_directory_path_).Any()*/
                    if (MessageBox.Show($"{s_message_prefix_}Удалить созданную папку решения '{solution_directory_path_}' ?", S_WIZARD_CAPTION, MessageBoxButtons.YesNo) == DialogResult.Yes)
                    {
                        Directory.Delete(solution_directory_path_, true);
                    }
                }
            }
        }

        // This method is only called for item templates, not for project templates.
        public bool ShouldAddProjectItem(string filePath)
        {
            Debug.WriteLine("ShouldAddProjectItem " + filePath);
            return true;
        }
    } //WizardImplementation



} //EsbStudio
