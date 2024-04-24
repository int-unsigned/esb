using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

// Катострофически много дебагов в GetEsbProjectInfoDecls. сыровато очень.. парсить с++ вручную.. хотя и макро-язык.. сплошное TODO
// НО дебаг отключаю, т.к. оно обернуто в try и если что ош. будет перехвачена и вернется нулл
//TODO	Доработать!
#if DEBUG || DEBUG_IN_RELEASE
	using System.Diagnostics;
#endif
using System.IO;
//
using Microsoft.VisualStudio;
//using Microsoft.VisualStudio.Package;
using Microsoft.VisualStudio.Shell;


//TODO	VCProjectEngine это специализация Солюшн-Проджект для нужд с++.
//		т.е. если нужно получить/рулить опциями компилятора (линкера, т.п.), самим линкером (компилятором и т.п.), строить, настраивать и т.д.
//		в общем все опции интерфейса Студии для проекта с++, кроме собственно с++ языка и конструкций
//		(т.е. то что для "обобщенного" ProjectItem видится внутри .Properties для VCProjectItem будет в самом интерфейсе)
//		навскидку из полезного VCFilter отдельный объект и в него можно добавлять/удалять
//		возможно еще что есть
//		т.е. всякие Свойства с++ проекта и его элементов увидеть можно и через .Properties, но специфические методы не вызовешь - тут ее надо...
//		+ нр. проверить собрано или нет и собрать https://stackoverflow.com/questions/1683642/vb-net-envdte-up-to-date-check-before-building-project
// https://learn.microsoft.com/en-us/dotnet/api/microsoft.visualstudio.vcprojectengine.vcproject?view=visualstudiosdk-2022
// там еще проблемы какие-то...
//	https://stackoverflow.com/questions/29951353/can-one-cast-an-envdte-project-into-a-vcproject
// ссылку на нее проставлять надо..
//using Microsoft.VisualStudio.VCProjectEngine;

// есть еще Microsoft.VisualStudio.VCProject namespace и .длл, но в ней собственно VCProject нет
// она маленькая и видимо предназначена для получения минимальной функциональности руления с++ проектом.
// Но нам надо не минимальную а Абсолютноую!

//Microsoft.VisualStudio.VCCodeModel Namespace
//https://learn.microsoft.com/en-us/dotnet/api/microsoft.visualstudio.vccodemodel?view=visualstudiosdk-2022
//Модель кода Visual C++
//https://learn.microsoft.com/en-us/previous-versions/t41260xs(v=vs.140)
//Практическое руководство. Манипулирование кодом с помощью модели кода Visual C++ (Visual Basic)
//https://learn.microsoft.com/en-us/previous-versions/ms228757(v=vs.140)
//How to: Manipulate Code by Using the Visual C++ Code Model (Visual C#)
//https://learn.microsoft.com/en-us/previous-versions/ms228770(v=vs.140)
using Microsoft.VisualStudio.VCCodeModel;


//TODO	Абсолютно не используем событийную модель Студии, а всегда все парсим заново. В какой-то момент это станет проблемой. Нужно разбираться...
//https://github.com/madskristensen/SolutionLoadSample


// Наша EsbStudio преимущественно синхронна и асинхронную функциональность НЕ ИСПОЛЬЗУЕТ
// Принципиально. Принципиально считаю, что помогать кодеру нужно синхронно, а ждут пусть другие
// Возможно когда-то эта модель будет изменена - тогда нужно будет разобираться в местах помеченных этим макросом
// (VSTHRD010 - это ругань по поводу возможного обращения к EnvDTE-элементам из НЕ основного потока
//	с тревованием Microsoft.VisualStudio.Shell.ThreadHelper.ThrowIfNotOnUIThread(); или еще как)
#if ESB_STUDIO_SYNC_MODE
#pragma warning disable VSTHRD010
#endif


namespace EsbStudio 
{
	namespace Vs
	{
		public static class Dte
		{
			//TODO	EnvDTE.ProjectItem.Kind возвращает такое. хз.. где-то наверное константой прописано
			static readonly Guid VC_PROJECT_SELF_KIND_GUID_ = new Guid("{8BC9CEB8-8B4A-11D0-8D11-00A0C91BC942}");
			static readonly Guid VC_PROJECT_FILE_CPP_KIND_GUID_ = new Guid("{6BB5F8EE-4483-11D3-8BCF-00C04F8EC28C}");

			const string ESB_MAIN_CPP_ = "esbmain.cpp";

			//ESB_ADDIN_INTERFACE("46EFE873-8489-4C02-8FC5-F95FFB941C70", ("EsbObject", "ЕсбОбъект"), "Объект есб компоненты")
			const string ESB_ADDIN_INTERFACE_			= "ESB_ADDIN_INTERFACE";
			//ESB_ADDIN_INTERFACE_DONE();
			const string ESB_ADDIN_INTERFACE_DONE_		= "ESB_ADDIN_INTERFACE_DONE";
			//ESB_META_INTERFACE_FREE_METH(GetHellow, "ДайПривет"),
			const string ESB_META_INTERFACE_FREE_METH_	= "ESB_META_INTERFACE_FREE_METH";
			const string ESB_META_INTERFACE_STAT_METH_	= "ESB_META_INTERFACE_STAT_METH";
			const string ESB_ESBLDR_					= "esbldr";

			static void DoThrowUnexpected(string message_, [System.Runtime.CompilerServices.CallerMemberName] string caller_name_ = "") {
				Debug.WriteLine($"{caller_name_}: '{message_}'");
				throw new Exception(message_);
			}


			static public EnvDTE.ProjectItem GetEsbProjectMainItem() 
			{
				// Synchronously requesting a service on the UI thread
				//var dte = GetService(typeof(EnvDTE.DTE)) as EnvDTE.DTE;
				Microsoft.VisualStudio.Shell.ThreadHelper.ThrowIfNotOnUIThread();
				EnvDTE.DTE dte = (EnvDTE.DTE)Package.GetGlobalService(typeof(EnvDTE.DTE));
				if(dte == null)
					DoThrowUnexpected("dte == null");

				//Gets the Solution object that represents all open projects in the current instance of the environment and allows access to the build objects.
				EnvDTE.Solution solution = dte.Solution;
				if (!solution.IsOpen)   //может быть выгружено.. хз как мы тогда здесь..
					DoThrowUnexpected("!solution.IsOpen");

				EnvDTE.Projects solution_projects = solution.Projects;
				if (solution_projects == null)
					DoThrowUnexpected("solution_projects == null");
				if (solution_projects.Count == 0)
					DoThrowUnexpected("solution_projects.Count == 0");

				// В решении может быть открыто несколько проектов, а может быть ни одного - просто файл открыли
				// Для нас Есб-Проект этот с++ проект в котором есть esbmain.cpp
				//TODO	пока так по-простому, нужно хотя-бы учесть что может быть два и более есб-проекта в решении
				EnvDTE.ProjectItem esb_main = null;
				//
				foreach (EnvDTE.Project project in solution_projects)
				{
					// Какая-то редкая дурость. У VCProject есть метод .FindFile, НО VCProjectItem НЕТ .FileCodeModel
					// Она есть только у EnvDTE.ProjectItem, но EnvDTE.Project НЕТ .FindFile - все дерево итемов рыть надо итерационно..
/*					VCProject project_cpp = project as VCProject;
					if (project_cpp != null) {
						VCProjectItem esb_main_cpp = project_cpp.FindFile(ESB_MAIN_CPP_) as VCProjectItem;  // VCFile Implements VCProjectItem
						if (esb_main_cpp != null)
							return esb_main_cpp;
					}
*/
					//TODO	возможно проще (лучше) проверять что это с++ проект проверкой на VCCodeModel
					//		VCCodeModel project_model_cpp = project.CodeModel as VCCodeModel;
					Guid project_guid;
					if(Guid.TryParse(project.Kind, out project_guid) && project_guid == VC_PROJECT_SELF_KIND_GUID_) {
						esb_main = GetEsbProjectMainItemFromItems(project.ProjectItems);
						if (esb_main != null)
							return esb_main;
					}
				}

				return esb_main;
			} // GetEsbProjectMain

			static EnvDTE.ProjectItem GetEsbProjectMainItemFromItems(EnvDTE.ProjectItems project_items_) 
			{
				if (project_items_ == null)
					return null;

				foreach (EnvDTE.ProjectItem project_item in project_items_)
				{
					//proj_item_.Name = esbmain.cpp
					//proj_item_.Kind = {6BB5F8EE-4483-11D3-8BCF-00C04F8EC28C}
					//.Property: VCProjectEngine = 'System.__ComObject'
					//.Property: Kind = 'VCFile'
					//.Property: ItemType = 'ClCompile'
					//.Property: ContentType = 'CppCode'
					//.Property: FileType = '0'
					//.Property: Extension = '.cpp'
					System.Guid project_item_kind_guid;
					if (Guid.TryParse(project_item.Kind, out project_item_kind_guid) && project_item_kind_guid == VC_PROJECT_FILE_CPP_KIND_GUID_)
					{
						//TODO	Этот их дурацкий .Properties бросает исключение если найти свойство не может. поэтому отложим щепетильное нахождение нашего esbmain.cpp на потом
						//		возможно лучше сразу брать VCProjectEngine и VCProject
						if (project_item.Name == ESB_MAIN_CPP_)
							return project_item;
					}

					EnvDTE.ProjectItem esb_main = GetEsbProjectMainItemFromItems(project_item.ProjectItems);
					if (esb_main != null)
						return esb_main;
				}
				return null;
			}


			static internal List<Esl.EslDecl> GetEsbProjectInfo() 
			{
				EnvDTE.ProjectItem esb_main = GetEsbProjectMainItem();
				if (esb_main == null)
					DoThrowUnexpected("esb_main == null");

				var esb_main_code_model = esb_main.FileCodeModel;
				if (esb_main_code_model == null)
					DoThrowUnexpected("esb_main_code_model == null");

				VCFileCodeModel esb_main_code_model_cpp = esb_main_code_model as VCFileCodeModel;
				if (esb_main_code_model_cpp == null)	
					DoThrowUnexpected("esb_main_code_model_cpp == null");

				//TODO								
				//VCFileCodeModel.ParseStatus
				//VCFileCodeModel.Synchronize
				//В редких случаях может потребоваться синхронизация объекта модели кода с файловыми буферами. Иногда это необходимо, когда вы напрямую изменяете файловый буфер.
				return GetEsbProjectInfoDecls(esb_main_code_model_cpp.CodeElements as VCCodeElements);								
			}

			static List<Esl.EslDecl> GetEsbProjectInfoDecls(VCCodeElements elems_) 
			{
				if (elems_ == null)
					return null;

				List<Esl.EslDecl> decls = new List<Esl.EslDecl>();

				EnvDTE.TextPoint esb_interface_declare_point_begin = null;
				EnvDTE.TextPoint esb_interface_declare_point_final = null;

				foreach (VCCodeElement code_elem in elems_)	{
					if (code_elem.Kind == EnvDTE.vsCMElement.vsCMElementFunction)
						esb_interface_declare_point_begin = code_elem.EndPoint;
					else if(code_elem.Kind == EnvDTE.vsCMElement.vsCMElementIncludeStmt && 
							esb_interface_declare_point_begin != null && code_elem.Name.Contains(ESB_ESBLDR_)) {
						//code_elem.Name == "esb/esbldr_component_cpp.h"
						esb_interface_declare_point_final = code_elem.StartPoint;
						break;
					}
				}
				Debug.Assert(esb_interface_declare_point_begin != null);    //TODO 
				Debug.Assert(esb_interface_declare_point_final != null);    //TODO 

				var esb_interface_declare_point = esb_interface_declare_point_begin.CreateEditPoint();
				string esb_interface_declare_text = esb_interface_declare_point.GetText(esb_interface_declare_point_final);
				Debug.Assert(esb_interface_declare_text != null && esb_interface_declare_text.Length > 0);

				System.IO.StringReader esb_interface_reader = new System.IO.StringReader(esb_interface_declare_text);
				string esb_interface_line = esb_interface_reader.ReadLine();
				while (esb_interface_line != null) 
				{
					if (esb_interface_line.Length == 0 || esb_interface_line[0] == '/')
						goto CONTINUE_READ_NEXT_LINE;

					if (esb_interface_line.Contains(ESB_ADDIN_INTERFACE_DONE_))
						break;

					int ipos_1st = esb_interface_line.IndexOf(ESB_META_INTERFACE_FREE_METH_);
					if (ipos_1st < 0) {
						ipos_1st = esb_interface_line.IndexOf(ESB_META_INTERFACE_STAT_METH_);
						if(ipos_1st < 0)
							goto CONTINUE_READ_NEXT_LINE;
					}

					Debug.Assert(ESB_META_INTERFACE_FREE_METH_.Length == ESB_META_INTERFACE_STAT_METH_.Length);
					ipos_1st = esb_interface_line.IndexOf('(', ipos_1st + ESB_META_INTERFACE_FREE_METH_.Length);
					Debug.Assert(ipos_1st > 0);
					++ipos_1st;

					int ipos_end = esb_interface_line.IndexOf(',', ipos_1st);
					Debug.Assert(ipos_end > 0);
					//(a,
					//012
					string name1 = esb_interface_line.Substring(ipos_1st, ipos_end - ipos_1st).Trim();

					ipos_1st = ipos_end + 1;
					ipos_end = esb_interface_line.IndexOf(')', ipos_1st);
					Debug.Assert(ipos_end > 0);
				
					string name2 = esb_interface_line.Substring(ipos_1st, ipos_end - ipos_1st).Trim().Trim('"');

					var func_ref = elems_.Find(name1);
					VCCodeFunction func = func_ref as VCCodeFunction;
					Debug.Assert(func != null);

					Esl.EslMeth m1 = from_vccode_func(name1, func);
					Esl.EslMeth m2 = new Esl.EslMeth(name2, m1.GetReturnType(), m1.GetParams(), null, m1.GetDescription());
					decls.Add(new Esl.EslDecl(new Esl.EslMeth[]{m1}));
					decls.Add(new Esl.EslDecl(new Esl.EslMeth[]{m2}));

				CONTINUE_READ_NEXT_LINE:
					esb_interface_line = esb_interface_reader.ReadLine();
				}

				return decls;
			}

			static Esl.EslType from_vccode_type_not_expext_void(EnvDTE.CodeTypeRef type_ref_) {
				//TODO	type_ref это такой объект по свойству которого .Kind можно узнать что за тип - vsCMTypeRefString, vsCMTypeRefVoid, vsCMTypeRefOther
				//		и если vsCMTypeRefCodeType то можно преобразовать свойство .CodeType к VCCodeClass или скажем VCCodeEnum (как-то, смотреть надоть...)
				// type 'String', kind = vsCMTypeRefOther
				// type 'const Строка &', kind = 21			- хз..
				// пока просто через строку
				string s_type_text = type_ref_.AsString;				
				//TODO	Умнее надо. и возможно более полно..
				string s_type = s_type_text.Replace("const", "").Replace('&', ' ').Trim();
				return Esl.EslTypes.GetOrCreateType(s_type);
			}
			static Esl.EslType from_vccode_type(VCCodeParameter elem_) 
			{
				//TODO	c++ допускает конструкцию foo(void). и что в этом случае получится?
				return from_vccode_type_not_expext_void(elem_.Type);
			}
			static Esl.EslType from_vccode_type(VCCodeFunction elem_) {
				EnvDTE.CodeTypeRef type_ref = elem_.Type;
				if (type_ref.TypeKind == EnvDTE.vsCMTypeRef.vsCMTypeRefVoid)
					return Esl.EslTypes.Void_;
				else
					return from_vccode_type_not_expext_void(type_ref);
			}
			static Esl.EslMeth from_vccode_func(string name_, VCCodeFunction func_) 
			{				
				Esl.EslPara[] a_para = null;
				int c_para = func_.Parameters.Count;
				if (c_para > 0)
				{
					a_para = new Esl.EslPara[c_para];
					//TODO	По какой-то причине попытка вызова func_.Parameters.Item(i_para) приводит к исключению
					//		параметр метода .Item определен как object - ОНИ ЧТО только по Имени параметр отдают, а по числу НЕТ?
					//		как-то разобраться надо что они хотят (пс. возможно int16)
					//		пока так сделал
					int i_para = 0;
					foreach (VCCodeParameter para in func_.Parameters) {
						a_para[i_para] = new Esl.EslPara(para.Name, from_vccode_type(para), null, null);
						++i_para;
					}
/*					for(int i_para = 0; i_para < c_para; ++i_para)
					{
						VCCodeParameter para = func_.Parameters.Item(i_para) as VCCodeParameter;
						a_para[i_para] = new Esl.EslPara(para.Name, from_vccode_type(para), null, null);
					}
*/				}
				else
					a_para = Esl.EslParaSet.Empty_;

				string s_descr = func_.DocComment;
				if(s_descr != null && s_descr.Length > 0)
					s_descr = extract_description_doc_text(s_descr);
				if (s_descr != null && s_descr.Length == 0)
					s_descr = null;

				return new Esl.EslMeth(name_, from_vccode_type(func_), a_para, null, s_descr);
			}

			static string extract_description_doc_text(string doc_deciption_text_) 
			{
				string s_description = null;

				//TODO	Наипримитивнейше просто вырезаем все пустые строки и начинающиеся с '<'. А нужно как-то этот типа-хмл парсить
				System.IO.StringReader description_reader = new System.IO.StringReader(doc_deciption_text_);			
				string description_line = description_reader.ReadLine();								
				while (description_line != null)
				{
					if (description_line.Length > 0 && description_line[0] != '<') {
						if (s_description != null && s_description.Length > 0)
							s_description += Environment.NewLine;
						s_description += description_line;
					}
					description_line = description_reader.ReadLine();
				}

				return s_description;
			}

		} // class EsbStudio.Vs.Dte
	} // namespace EsbStudio.Vs
} // namespace EsbStudio



namespace EsbStudio.Esl
{
	namespace EsbProject 
	{
		using static EslContextApi2;


		class EsbComponent
		{
			// Интерфейс ЕсбКомпоненты тоже может меняться. Кроме предопределенных методов.. да и их тоже можно переобозвать и по-русски добавить..
			internal static EslName EsbComponentNameEn_ = new EslName("EsbComponent", "ESBCOMPONENT");
			internal static EslName EsbComponentNameRu_ = new EslName("ЕсбКомпонент", "ЕСБКОМПОНЕНТ");

			internal static EslDecl[] Declarations_ = new EslDecl[]
			{
				FUNC("Register", EslTypes.Boolean_),
				FUNC("RegisterAsSCOM", EslTypes.Boolean_),
				FUNC("UnRegister", EslTypes.Boolean_),
			};

			internal static EslDecl[] GetEsbInterface() {
				return Declarations_;
			}
		} //class EsbComponent

		class EsbObject
		{
			//TODO	Имена Есб-объектов ТОЖЕ МОГУТ МЕНЯТЬСЯ. Их надо тоже парсить! и потом как-то имя типа EslType EsbObject_ менять да еще и список
			//		и в индексе менять! и список типов пересортировывать! мдяя.. на будущее. когда евенты будут
			//		ну хэш положим весь можно не перестраивать, а для него любимого (их - из с++) отдельный хэш или просто проверку сделать
			//		а в список типов сортированным нужен только для Новый и для этого тоже можно спец.структурку придумать - в конце концов просто же на экран показать надо..
			private static EslName m_name_en = new EslName("EsbObject", "ESBOBJECT");
			private static EslName m_name_ru = new EslName("ЕсбОбъект", "ЕСБОБЪЕКТ");
			//
			internal static EslName GetNameEn() { return m_name_en; }
			internal static EslName GetNameRu() { return m_name_ru; }
			//
			internal static EslDecl[] GetEsbInterface() {
				List<EslDecl> esb_decls = EsbStudio.Vs.Dte.GetEsbProjectInfo();

				// все мемберс должны быть сортированными - это условие имплементации Списка Дополнений Студии.
				//TODO	Также нужно как-то закэшировать этот массив где-то и только обновлять его по событиям откуда-то
				esb_decls.Sort(EslDeclComparier.Instance_);
				return esb_decls.ToArray();
			}
		} //class EsbObject
	} //namespace EsbProject
} //namespace EsbStudio.Esl






//proj_item_.Name = esbmain.cpp
//proj_item_.Kind = {6BB5F8EE-4483-11D3-8BCF-00C04F8EC28C}
//.Property: VCProjectEngine = 'System.__ComObject'
//.Property: Kind = 'VCFile'
//.Property: ItemType = 'ClCompile'
//.Property: ContentType = 'CppCode'
//.Property: FileType = '0'
//.Property: Extension = '.cpp'

//Project Item esb.h, .Kind='{6BB5F8EE-4483-11D3-8BCF-00C04F8EC28C}'
//.Property: FileType = '2'
//.Property: Extension = '.h'
//.Property: VCProjectEngine = 'System.__ComObject'
//.Property: Kind = 'VCFile'
//.Property: ItemType = 'ClInclude'
//.Property: ContentType = 'CppHeader'

/*
.Project Item '1С Файлы', .Kind='{6BB5F8F0-4483-11D3-8BCF-00C04F8EC28C}', .ExtenderCATID='{610d461d-d0d5-11d2-8599-006097c68e81}'
.FileCodeModel null
.Property: Kind = 'VCFilter'
.Property: Filter = ''
.Property: UniqueIdentifier = '{10e734ec-dc6d-41c5-8cc8-705c17653273}'
.Property: ParseFiles = 'True'

.Project Item 'Общие модули', .Kind='{6BB5F8F0-4483-11D3-8BCF-00C04F8EC28C}', .ExtenderCATID='{610d461d-d0d5-11d2-8599-006097c68e81}'
.FileCodeModel null
.FileCount: 1
.FileName: Общие модули
.Property: CanonicalName = '1С Файлы\Общие модули'
.Property: Kind = 'VCFilter'
.Property: Filter = 'bsl'
.Property: UniqueIdentifier = '{c5ca8243-d55f-462d-bd2e-2d5ce7a240c9}'
.Property: ItemName = 'Общие модули'
.Property: ParseFiles = 'True'

.Project Item OrdinaryApplicationModule.bsl, .Kind='{6BB5F8EE-4483-11D3-8BCF-00C04F8EC28C}', .ExtenderCATID='{610d461f-d0d5-11d2-8599-006097c68e81}'
.FileCodeModel null
.FileCount: 1
.FileName: C:\Users\Garry\source\repos\EsbComponent7\1s_files\Ext\OrdinaryApplicationModule.bsl
.Property: FileType = '25'
.Property: Extension = '.bsl'
.Property: UnexpandedRelativePath = '1s_files\Ext\OrdinaryApplicationModule.bsl'
.Property: Kind = 'VCFile'
.Property: ItemType = 'OneEsModuleItem'
.Property: ItemName = 'OrdinaryApplicationModule.bsl'
.Property: RelativePath = '1s_files\Ext\OrdinaryApplicationModule.bsl'
.Property: ContentType = 'OneEsModuleContent'
.Property: FullPath = 'C:\Users\Garry\source\repos\EsbComponent7\1s_files\Ext\OrdinaryApplicationModule.bsl'
.Property: Name = 'OrdinaryApplicationModule.bsl'

.Project Item Module.bsl, .Kind='{6BB5F8EE-4483-11D3-8BCF-00C04F8EC28C}', .ExtenderCATID='{610d461f-d0d5-11d2-8599-006097c68e81}'
.FileCodeModel null
.FileCount: 1
.FileName: C:\Users\Garry\source\repos\EsbComponent7\1s_files\CommonModules\EsbComponent\Ext\Module.bsl
.Property: FileType = '25'
.Property: Extension = '.bsl'
.Property: UnexpandedRelativePath = '1s_files\CommonModules\EsbComponent\Ext\Module.bsl'
.Property: SubType = ''
.Property: Kind = 'VCFile'
.Property: ItemType = 'OneEsModuleItem'
.Property: CustomTool = ''
.Property: ItemName = 'Module.bsl'
.Property: RelativePath = '1s_files\CommonModules\EsbComponent\Ext\Module.bsl'
.Property: ContentType = 'OneEsModuleContent'
.Property: FullPath = 'C:\Users\Garry\source\repos\EsbComponent7\1s_files\CommonModules\EsbComponent\Ext\Module.bsl'
.Property: Name = 'Module.bsl'


.Project Item EslApplicationModule.esl, .Kind='{66A2671F-8FB5-11D2-AA7E-00C04F688DDE}', .ExtenderCATID='{610d4613-d0d5-11d2-8599-006097c68e81}'
.FileCodeModel null
.FileCount: 1
Поток 0x242c завершился с кодом 0 (0x0).
Поток 0x11e0 завершился с кодом 0 (0x0).

System.ArgumentException: Параметр задан неверно. (Исключение из HRESULT: 0x80070057 (E_INVALIDARG)) в EnvDTE.ProjectItem.get_FileNames(Int16 index) в EsbStudio.Esl.EslContext_Esb.dump_it(ProjectItem proj_item_, Int32 level_) 

 */
