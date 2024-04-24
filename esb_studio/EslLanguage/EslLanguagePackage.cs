using Microsoft.VisualStudio.Shell;
using System;
using System.Runtime.InteropServices;
using System.ComponentModel.Design;     //IServiceContainer
using System.Threading;
using Task = System.Threading.Tasks.Task;
using System.Diagnostics;



namespace EsbStudio
{
	internal static class PackageDefs
	{
		public const string PackageGuidString_ = "2e283b48-4d47-4f10-9a33-500c68741940";
	}
}



namespace EsbStudio
{

	namespace Esl
	{
		// Общее описание процедуры взаимодействия Студии с Парсером и Сканером (на примере для 2015)
		//https://learn.microsoft.com/en-us/previous-versions/visualstudio/visual-studio-2015/extensibility/internals/legacy-language-service-parser-and-scanner?view=vs-2015
		// Студия запрашивает LanguageService::GetColorizer(IVsTextLines)
		// и реализация LanguageService::GetColorizer(IVsTextLines) по-умолчание возвращает Microsoft.VisualStudio.Package.Colorizer(LanguageService::GetScanner(IVsTextLines))
		// Хотя параметр декларирован типа IVsTextLines везде идут ссылки на то, что это объект  Microsoft.VisualStudio.Package.Source
		// https://learn.microsoft.com/en-us/dotnet/api/microsoft.visualstudio.package.source?view=visualstudiosdk-2022
		// конструктор Source(LanguageService, IVsTextLines, Colorizer), а создается он путем вызова нашей LanguageService::CreateSource(IVsTextLines)
		// реализация которой по-умолчанию создает new Microsoft.VisualStudio.Package.Source(...)
		// (+	This class is instantiated by a call to the CreateSource(IVsTextLines) method. This is done when the CodeWindowManager object is instantiated
		// (	(the Source object is passed to the CodeWindowManager constructor)
		// https://learn.microsoft.com/en-us/dotnet/api/microsoft.visualstudio.package.languageservice.createsource?view=visualstudiosdk-2022#microsoft-visualstudio-package-languageservice-createsource(microsoft-visualstudio-textmanager-interop-ivstextlines)
		//TODO
		//*	Обратите внимание, что объект Source , возвращаемый этим методом, не будет анализироваться автоматически (с помощью OnIdle ) после загрузки.
		//* Чтобы выполнить автоматический анализ, переопределите этот метод и установите для LastParseTime значение 0.
		// МОЩНЫЙ КЛАСС!!
		//
		//ПС.	Где-то там у них написано, что параметр IVsTextLines нужно использовать ооочень акуратно в фоновых потоках т.к.... проблемы
		//		лучше его не использовать без необходимости...

		internal class DefConstants
		{
			// в описании для LanguageService::GetFileExtensions написано, что можно через ; перечислять, а в описании ProvideLanguageExtensionAttribute такого нет,
			// только строгуо указано что с . должно начинаться.
			// Вот тут как пример для двух расширений прописано два ProvideLanguageExtensionAttribute https://blog.280z28.org/archives/2013/04/165/
			public const string		FileExtension1_ = ".bsl";
			public const string		FileExtension1_Name_ = "1C files (*.bsl)";
			public const string		FileExtension2_ = ".esl";
			public const string		FileExtension2_Name_ = "1C++ files (*.esl)";
			public const string		FileExtensions_ = ".bsl;.esl";
			/* LanguageService::GetFormatFilterList supplies a list of file extension filters formatted for a Save As dialog box.
			 * The extensions are formatted in pairs of descriptions and extensions, with each element separated by newlines or vertical bars ("|").
			 * For example, a set of filters for C++ and C# files might be formatted as
			 * "C++ files (*.cpp)\n*.cpp\nC# files (*.cs)\n*.cs\n".
			 * Visual Studio always prepends the filter list "All Files (*.*)\n*.*\nText Files (*.txt)\n*.txt\n" to whatever is returned from this method.
			 */
			public const string		FileFiltersForSaveAs_ = FileExtension1_Name_ + "\n*" + FileExtension1_ + "\n*" + FileExtension2_Name_ + "\n*" + FileExtension2_ + "\n";

			// понятия не имею, что за LanguageResourceId_ им нужен... в примере 106, в реестре по пути
			// HKEY_CURRENT_USER\Software\Microsoft\VisualStudio\14.0_Config\Languages\Language Services
			// у каждого языкового сервиса что-то свое.. часто встречается просто 0 (с++), 101 (CSharp), 1003 (Xml).. хз
			// что-то вроде "если есть локализованная sattelitedll, то это идентификатор локализованного ресурса в ней)
			public const int		LanguageResId_		= 106;
			//

			//https://learn.microsoft.com/en-us/dotnet/api/microsoft.visualstudio.package.languageservice.name?view=visualstudiosdk-2022#microsoft-visualstudio-package-languageservice-name
			//- Return the name of the language, such as "HTML" or "C++", and so on.
			//- Returns a string containing the name of the language. This must return the same name the language service was registered with in Visual Studio
			//из всех прочих источников (https://blog.280z28.org/archives/2013/04/165/, https://www.vsixcookbook.com/recipes/Walkthrough-Create-Language-Editor.html)
			//следует что это одно и то же, а точнее что понятия ИмяСлужбы как такового нет
			public const string		LanguageName_ = "ESL";


			public const string		LanguageServiceGuidString_	= "F0E14231-54CA-480D-84C0-FD132715F8C8";
			// Это имя используется для собственно регистрации службы, а не службы языка (аттрибут ProvideServiceAttribute)
			// оно у аттрибута опционально, но видимо в целях отладки и т.п. лучше дать читаемое
			public const string		LanguageServiceName_		= "EslLanguageService";


			//public static readonly Guid RazorLanguageServiceGuid = new(RazorLanguageServiceString);

			//public const string VSProjectItemsIdentifier = "CF_VSSTGPROJECTITEMS";
		}
	}
}



/*	ВСЯКИЕ УСТАНОВКИ ДЛЯ СЕРВИСА
[<ProvideLanguageService(languageService = typeof<FSharpLanguageService>,
                         strLanguageName = FSharpConstants.FSharpLanguageName,
                         languageResourceID = 100,
                         MatchBraces = true,
                         MatchBracesAtCaret = true,
                         ShowCompletion = true,
                         ShowMatchingBrace = true,
                         ShowSmartIndent = true,
                         EnableAsyncCompletion = true,
                         QuickInfo = true,
                         DefaultToInsertSpaces = true,
                         CodeSense = true,
                         DefaultToNonHotURLs = true,
                         RequestStockColors = true,
                         EnableCommenting = true,
                         CodeSenseDelay = 100,
                         ShowDropDownOptions = true)>]
	 
*/
namespace EsbStudio
{
	// The minimum requirement for a class to be considered a valid package for Visual Studio
	// is to implement the IVsPackage interface and register itself with the shell.
	// This package uses the helper classes defined inside the Managed Package Framework (MPF)
	// to do it: it derives from the Package class that provides the implementation of the
	// IVsPackage interface and uses the registration attributes defined in the framework to
	// register itself and its components with the shell. These attributes tell the pkgdef creation
	// utility what data to put into .pkgdef file.
	// 
	// To get loaded into VS, the package must be referred by &lt;Asset Type="Microsoft.VisualStudio.VsPackage" ...&gt; in .vsixmanifest file.
	
	
	//TODO	[AboutDialogInfo(PackageGuidString, "Razor (ASP.NET Core)", "#110", "#112", IconResourceID = "#400")]
	[ProvideServiceAttribute(typeof(Esl.EslLanguageService), ServiceName = Esl.DefConstants.LanguageServiceName_)]
	/*	Register the language service with Visual Studio.  "106" is the resource ID in the satellite DLL to the localized language name.
	 *	TODO Это минимальный набор Аттрибутов. Их там намооого блольше
	 *	https://learn.microsoft.com/ru-ru/dotnet/api/microsoft.visualstudio.shell.providelanguageserviceattribute?view=visualstudiosdk-2022
	 */
	[ProvideLanguageServiceAttribute(
		typeof(Esl.EslLanguageService), 
		Esl.DefConstants.LanguageName_, 
		Esl.DefConstants.LanguageResId_,
#if ESB_STUDIO_SYNC_MODE
		EnableAsyncCompletion = false,
#else
#error EsbLanguageService must be full tested and complaint for Async Parsing and Completition. If so remove this line.
		EnableAsyncCompletion = true,
#endif
		EnableLineNumbers = true,
		CodeSense = true,
		CodeSenseDelay = 100,
		QuickInfo = true,
		ShowCompletion = true
	)]
	// Associate a file extension with our language service.
	[ProvideLanguageExtensionAttribute(typeof(Esl.EslLanguageService), Esl.DefConstants.FileExtension1_)]
	[ProvideLanguageExtensionAttribute(typeof(Esl.EslLanguageService), Esl.DefConstants.FileExtension2_)]
	// TODO	еще надо (можно)
	// ProvideLanguageEditorOptionPageAttribute  https://learn.microsoft.com/ru-ru/dotnet/api/microsoft.visualstudio.shell.providelanguageeditoroptionpageattribute?view=visualstudiosdk-2022
	// ProvideLanguageCodeExpansionAttribute https://learn.microsoft.com/ru-ru/dotnet/api/microsoft.visualstudio.shell.providelanguagecodeexpansionattribute?view=visualstudiosdk-2022
	[Guid(PackageDefs.PackageGuidString_)]
#if ESB_STUDIO_USE_PACKAGE_ASYNC
	[PackageRegistration(UseManagedResourcesOnly = true, AllowsBackgroundLoading = true)]
	public sealed class EslLanguagePackage : AsyncPackage
	{
		// Initialization of the package; this method is called right after the package is sited, so this is the place
		// where you can put all the initialization code that rely on services provided by VisualStudio.
		// <param name="cancellationToken">A cancellation token to monitor for initialization cancellation, which can occur when VS is shutting down.</param>
		// <param name="progress">A provider for progress updates.</param>
		// <returns>A task representing the async work of package initialization, or an already completed task if there is none. Do not return null from this method.</returns>
		protected override async Task InitializeAsync(CancellationToken cancellationToken, IProgress<ServiceProgressData> progress) {
			// When initialized asynchronously, the current thread may be a background thread at this point.
			// Do any initialization that requires the UI thread after switching to the UI thread.
			await this.JoinableTaskFactory.SwitchToMainThreadAsync(cancellationToken);
		}
	}
#else
	[PackageRegistration(UseManagedResourcesOnly = true, AllowsBackgroundLoading = false)]
	class EslLanguagePackage : Package
	{
		protected override void Initialize() 
		{
			Debug.WriteLine("EslLanguagePackage::Initialize");



			base.Initialize();

			IServiceContainer serviceContainer = this as IServiceContainer;
			Esl.EslLanguageService languageService = new Esl.EslLanguageService();

			//https://learn.microsoft.com/en-us/dotnet/api/microsoft.visualstudio.package.languageservice.setsite?view=visualstudiosdk-2022

			//https://github.com/Microsoft/vs-threading/blob/main/doc/analyzers/VSTHRD010.md
			ThreadHelper.ThrowIfNotOnUIThread();
			//await ThreadHelper.JoinableTaskFactory.SwitchToMainThreadAsync();
			languageService.SetSite(this);

			serviceContainer.AddService(typeof(Esl.EslLanguageService), languageService, true);
		}
	}
#endif
} // namespace EsbStudio
