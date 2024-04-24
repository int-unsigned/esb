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
using System.Diagnostics;
using System.IO.Compression;

//esbmain.cpp
//TODO ("EsbComponentObject", "ОбъектЕсбКомпоненты") нужно переопределить как ("$esb_component_en$", "$esb_component_ru$")
//		но для этого нужно возиться с переименованиями 1С-папок, файлов и внутри тоже..
//		vsix-template++: ESB_ADDIN_INTERFACE(EsbComponentObject, "$guid2$", ("$esb_component_en$", "$esb_component_ru$"), "Объект есб '$esb_component_ru$'")



namespace EsbStudio
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }


/*        static string config_ver_v8(int i_ver_)
        {
            Debug.Assert(i_ver_ >= app.V8_VER_MIN_ && i_ver_ <= app.V8_VER_MAX_);
            return "v83" + ((i_ver_ < 10)? "0" + i_ver_.ToString() : i_ver_.ToString());
        }
        static string config_ver_param(string s_ver_v8_)
        {
            return $"$ESB_CONFIG_{s_ver_v8_}$";
        }
        static string config_ver_param(int i_ver_)
        {
            return config_ver_param(config_ver_v8(i_ver_));
        }
        static string config_run_param(int i_ver_)
        {
            return $"$ESB_CONFIG_RUN_{config_ver_v8(i_ver_)}$";
        }
        static string config_cfg_param(int i_ver_) {
            return $"$ESB_CONFIG_CFG_{config_ver_v8(i_ver_)}$";
        }
*/
        static string read_lines_while_not_contains(StreamReader stream_, string contains_) {
            string s_line = stream_.ReadLine();
            while (!s_line.Contains(contains_))
            {
                s_line = stream_.ReadLine();
            }
            return s_line;
        }

        static void copy_file_vcxproj(string target_path_, string vcxproj_file_name_, StreamReader content_, StreamWriter vstemplate_output_)
        {
            string s_target_file = Path.Combine(target_path_, vcxproj_file_name_);
            bool b_config_run_param_done = false;
            bool b_config_cfg_param_done = false;
            bool b_config_sht_param_done = false;

            using (StreamWriter output = new StreamWriter(s_target_file))
            {
                string s_line = "";
                while ((s_line = content_.ReadLine()) != null)
                {
                    if (s_line.Contains("<ProjectGuid>{"))
                    {
                        output.WriteLine("    <ProjectGuid>{$guid1$}</ProjectGuid>");
                        continue;
                    }
                    if (s_line.Contains(@"<ItemGroup Label=""ProjectConfigurations"">"))
                    {
                        output.WriteLine(s_line);
                        write_lines_params(output, app.config_ver_param);
                        s_line = read_lines_while_not_contains(content_, @"</ItemGroup>");
                        //проваливаемся записать s_line == </ItemGroup>
                    }
                    if (s_line.Contains(@"<PropertyGroup") && s_line.Contains(@"Label=""EsbSettingsRun"""))
                    {
                        if (b_config_run_param_done)
                        {   // Блок настроек отладчика пишется для каждой конфигурации отдельно, а мы параметр config_run_param задаем для всей версии скопом
                            // т.к. что отладочную версию запускать, что релизную - все равно отдна и таже 1С
                            // поэтому все последующие после первого блоки EsbSettingsRun мы тихо глотаем
                            read_lines_while_not_contains(content_, @"</PropertyGroup>");
                        }
                        else
                        {
                            write_lines_params(output, app.config_run_param);
                            read_lines_while_not_contains(content_, @"</PropertyGroup>");
                            b_config_run_param_done = true;
                        }
                        continue;
                    }
                    if (s_line.Contains(@"<PropertyGroup Condition=""'$(Configuration)|$(Platform)") && s_line.Contains(@"Label=""Configuration"""))
                    {
                        if (b_config_cfg_param_done)    // то же, что и для config_run_param
                            read_lines_while_not_contains(content_, @"</PropertyGroup>");
                        else
                        {
                            write_lines_params(output, app.config_cfg_param);
                            read_lines_while_not_contains(content_, @"</PropertyGroup>");
                            b_config_cfg_param_done = true;
                        }
                        continue;
                    }
                    if (s_line.Contains(@"<ImportGroup Condition=""'$(Configuration)|$(Platform)") && s_line.Contains(@"Label=""PropertySheets"""))
                    {
                        if (!b_config_sht_param_done)    // то же, что и для config_run_param
                        {
                            write_lines_params(output, app.config_sht_param);
                            b_config_sht_param_done = true;
                        }
                        read_lines_while_not_contains(content_, @"</ImportGroup>");
                        continue;
                    }

                    output.WriteLine(s_line);
                }
            }

            vstemplate_write_item_ex(vstemplate_output_, vcxproj_file_name_, "$safeprojectname$.vcxproj");
        }
        static void write_lines_params(StreamWriter output_, Func<int, string> param_constructor_) 
        {
            for (int i_ver = app.V8_VER_MIN_; i_ver <= app.V8_VER_MAX_; ++i_ver)
            {
                output_.WriteLine(param_constructor_(i_ver));
            }
        }

        static void copy_file(string target_path_, string target_file_name_, StreamReader content_, StreamWriter vstemplate_output_) 
        {
            string s_target_file = Path.Combine(target_path_, target_file_name_);

            using (StreamWriter output = new StreamWriter(s_target_file, false, new UTF8Encoding(true)))
            {
                string s_line = "";
                while ((s_line = content_.ReadLine()) != null)
                {
                    if (s_line.StartsWith("//vsix-template--: "))
                    {
                        string s_line_to_replace = s_line.Substring(18).TrimStart(' ', '\t');

                        s_line = content_.ReadLine();
                        Debug.Assert(s_line != null);
                        Debug.Assert(s_line.StartsWith("//vsix-template++:"));
                        string s_line_replace_with = s_line.Substring(19).TrimStart(' ', '\t');

                        s_line = content_.ReadLine();
                        Debug.Assert(s_line != null);
                        Debug.Assert(s_line.TrimStart(' ', '\t') == s_line_to_replace);

                        output.WriteLine(s_line_replace_with);
                    }
                    else
                        output.WriteLine(s_line);
                }
            }

            if(target_file_name_ == "esb_template.user.props")
                vstemplate_write_item_ex(vstemplate_output_, target_file_name_, "$safeprojectname$.user.props");
            else if (target_file_name_ == "esb_template.vcxproj.filters")
                vstemplate_write_item_ex(vstemplate_output_, target_file_name_, "$safeprojectname$.vcxproj.filters");
            else if (target_file_name_ == "esb_template.vcxproj.user")
                vstemplate_write_item_ex(vstemplate_output_, target_file_name_, "$safeprojectname$.vcxproj.user");
            else
                vstemplate_write_item(vstemplate_output_, target_file_name_);
        }


        static void vstemplate_write_folder_begin(StreamWriter vstemplate_output_, string folder_name_) {
            vstemplate_output_.WriteLine($@"<Folder Name=""{folder_name_}"">");
        }
        static void vstemplate_write_folder_final(StreamWriter vstemplate_output_)
        {
            vstemplate_output_.WriteLine(@"</Folder>");
        }
        static void vstemplate_write_folder_empty(StreamWriter vstemplate_output_, string folder_name_)
        {
            vstemplate_write_folder_begin(vstemplate_output_, folder_name_);
            vstemplate_write_folder_final(vstemplate_output_);
        }
        static void vstemplate_write_item(StreamWriter vstemplate_output_, string file_name_)
        {
            vstemplate_write_item_ex(vstemplate_output_, file_name_, file_name_);
        }
        static void vstemplate_write_item_ex(StreamWriter vstemplate_output_, string src_file_name_, string tag_file_name_)
        {
            vstemplate_output_.WriteLine($@"<ProjectItem ReplaceParameters=""true"" TargetFileName=""{tag_file_name_}"">{src_file_name_}</ProjectItem>");
        }


        static void copy_folder(string s_source_folder_, string s_target_path_, string s_target_folder_name_, StreamWriter vstemplate_output_) 
        {
            string[] source_files = Directory.GetFiles(s_source_folder_);
            string s_target_folder = Path.Combine(s_target_path_, s_target_folder_name_);

            Directory.CreateDirectory(s_target_folder);
            vstemplate_write_folder_begin(vstemplate_output_, s_target_folder_name_);
            
            foreach (string s_src_file in source_files)
            {
                string s_file_name = Path.GetFileName(s_src_file);
                using (StreamReader content_stream = File.OpenText(s_src_file))
                {
                    copy_file(s_target_folder, s_file_name, content_stream, vstemplate_output_);
                }
            }
            
            string[] source_folders = Directory.GetDirectories(s_source_folder_);
            foreach (string s_dir in source_folders)
            {
                string s_dir_name = Path.GetFileName(s_dir);
                copy_folder(s_dir, s_target_folder, s_dir_name, vstemplate_output_);
            }

            vstemplate_write_folder_final(vstemplate_output_);
        }


        static string AppStudioPath()
        {
            string path_studio_maker_bin_debug = Application.StartupPath;                        //...\esb-dev\esb_studio\EsbStudio\ProjectTemplateMaker\bin\Debug\
            string path_studio_maker_bin = Path.GetDirectoryName(path_studio_maker_bin_debug);   //...\esb-dev\esb_studio\EsbStudio\ProjectTemplateMaker\bin\
            string path_studio_maker = Path.GetDirectoryName(path_studio_maker_bin);             //...\esb-dev\esb_studio\EsbStudio\ProjectTemplateMaker\
            string path_studio = Path.GetDirectoryName(path_studio_maker);                       //...\esb-dev\esb_studio\EsbStudio\
            return path_studio;
        }
        static string AppPathSource()
        {
            return Path.Combine(AppStudioPath(), "ProjectTemplateVCProject");
        }
        static string AppPathTargetFiles() {
            return Path.Combine(AppStudioPath(), "ProjectTemplateFiles");
        }
        static string AppPathTargetZip()
        {
            return Path.Combine(AppStudioPath(), @"ProjectTemplates");
        }

        const string S_TEMPLATE_NAME = "EsbProjectTemplate";

        public static bool IsDirectoryEmpty(string directory_path_)
        {
            return !Directory.EnumerateFileSystemEntries(directory_path_).Any();
        }
        public static void ClearDirectory(string directory_path_)
        {
            var dir = new DirectoryInfo(directory_path_);
            foreach (FileInfo dir_file in dir.EnumerateFiles()) {
                dir_file.Delete();
            }
            foreach (DirectoryInfo dir_subdir in dir.EnumerateDirectories()) {
                dir_subdir.Delete(true);
            }
        }

		private void DoMake() 
        {
            string s_path_source = AppPathSource();
            string s_path_target = AppPathTargetFiles();

            if (!IsDirectoryEmpty(s_path_target))
                ClearDirectory(s_path_target);


            StreamReader vstemplate_stream = File.OpenText(Path.Combine(s_path_source, "vstemplate._vstemplate"));
            StreamWriter vstemplate_output = new StreamWriter(Path.Combine(s_path_target, S_TEMPLATE_NAME + ".vstemplate"));
            string s_vstemplate_line = "";
            while ((s_vstemplate_line = vstemplate_stream.ReadLine()) != null)
            {
                vstemplate_output.WriteLine(s_vstemplate_line);
                if (s_vstemplate_line.Contains(@"<Project TargetFileName="))
                    break;
            }

            string[] source_files = Directory.GetFiles(s_path_source);
            foreach (string s_src_file in source_files)
            {
                if (s_src_file.EndsWith(".log"))
                    continue;
                if (s_src_file.EndsWith(".sln"))
                    continue;
                if (s_src_file.EndsWith("._vstemplate"))
                    continue;
                if (s_src_file.EndsWith("._vstemplate_lst"))
                    continue;
                if (s_src_file.EndsWith(".vcxproj.user"))   //мы полностью игнорируем .vcxproj.user т.к. мастер шаблонов студии его не копирует, а создает по-месту
                    continue;

                System.Console.WriteLine(s_src_file);

                string s_file_name = Path.GetFileName(s_src_file);
                string s_tag_file = Path.Combine(s_path_target, s_file_name);

                if (s_file_name.EndsWith(".cmd"))
                {
                    File.Copy(s_src_file, s_tag_file, true);
                    continue;
                }
                if (s_file_name.EndsWith(".jpg"))
                {
                    File.Copy(s_src_file, s_tag_file, true);
                    continue;
                }

                using (StreamReader content_stream = File.OpenText(s_src_file))
                {
                    if (s_file_name.EndsWith(".vcxproj"))
                        copy_file_vcxproj(s_path_target, s_file_name, content_stream, vstemplate_output);
                    else
                        copy_file(s_path_target, s_file_name, content_stream, vstemplate_output);
                }
            }

            string[] source_folders = Directory.GetDirectories(s_path_source);
            foreach (string s_dir in source_folders)
            {
                if (s_dir.EndsWith(".vs"))
                    continue;
                if (s_dir.EndsWith("bin"))
                    continue;

                System.Console.WriteLine(s_dir);
                //
                string s_dir_name = Path.GetFileName(s_dir);
                if (s_dir_name == "1s")
                {
                    Directory.CreateDirectory(Path.Combine(s_path_target, s_dir_name));
                    vstemplate_write_folder_empty(vstemplate_output, s_dir_name);
                }
                else
                    copy_folder(s_dir, s_path_target, s_dir_name, vstemplate_output);
            }

            while ((s_vstemplate_line = vstemplate_stream.ReadLine()) != null)
            {
                vstemplate_output.WriteLine(s_vstemplate_line);
            }


            vstemplate_output.Flush();
            vstemplate_output.Close();
            vstemplate_output.Dispose();
            vstemplate_output = null;
        }

        private void DoZip() 
        {
            string s_file_zip = Path.Combine(AppPathTargetZip(), S_TEMPLATE_NAME + ".zip");
            if (File.Exists(s_file_zip))
                File.Delete(s_file_zip);

            System.IO.Compression.ZipFile.CreateFromDirectory(AppPathTargetFiles(), s_file_zip);
        }

        private void bnMake_Click(object sender, EventArgs e) {
            DoMake();
            MessageBox.Show("Make Done");

        }
        private void bnZip_Click(object sender, EventArgs e)
        {
            DoZip();
            MessageBox.Show("Zip Done");
        }
		private void bnMakeAndZip_Click(object sender, EventArgs e) 
        {
            DoMake();
            DoZip();
            MessageBox.Show("MakeAndZip Done. Will Close.");
            this.Close();
        }

	}
}
