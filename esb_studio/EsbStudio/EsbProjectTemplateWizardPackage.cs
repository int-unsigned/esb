using Microsoft.VisualStudio.Shell;
using System;
using System.Runtime.InteropServices;
using System.Threading;
using Task = System.Threading.Tasks.Task;

//** Как пошаговое руководство использовал это. Все этапы в одном месте
//** Use wizards with project templates
//https://learn.microsoft.com/en-us/visualstudio/extensibility/how-to-use-wizards-with-project-templates?view=vs-2022


//https://learn.microsoft.com/en-us/dotnet/api/microsoft.visualstudio.shell.asyncpackage?view=visualstudiosdk-2022
// Хотя МС настаивает, что сейчас нужно делать ТОЛЬКО Асинх
// но в документации для просто Package информации больше. Можно и что-то в своей ветке реестра создавать..
//https://learn.microsoft.com/en-us/dotnet/api/microsoft.visualstudio.shell.package?view=visualstudiosdk-2022

//Расширение Visual Studio не загружается в экспериментальном экземпляре
//https://stackoverflow.com/questions/26961669/visual-studio-extension-does-not-load-in-experimental-instance
//* Хрен пойми что написано, но вся инфа о "эксперементальной студии" в
//* c:\Users\ИМЯ\AppData\Local\Microsoft\VisualStudio\16.0_87a5a19cExp 
//* где 16.0_87a5a19 собственно студия, а Exp суффикс указанный на вкладке ОТЛАДКА /rootsuffix Exp
//* вроде говорят что эту папку можно даже полностью удалять - она пересоздастся - хз..
//* но полностью очищать подпапку Extensions и удалять всякое ProjectTemplatesCache_{00000000-0000-0000-0000-000000000000}
//* ПОМОГАЕТ
//
// Также можно открыть c:\Users\ИМЯ\AppData\Local\Microsoft\VisualStudio\16.0_87a5a19cExp\privateregistry.bin 
// в редакторе реестра (подключить ветвь и отключить не забыть) и что-то пытаться настраивать... хз - там 12 мегабайт всякого разного...
//



namespace EsbStudio
{
    /// <summary>
    /// This is the class that implements the package exposed by this assembly.
    /// </summary>
    /// <remarks>
    /// <para>
    /// The minimum requirement for a class to be considered a valid package for Visual Studio
    /// is to implement the IVsPackage interface and register itself with the shell.
    /// This package uses the helper classes defined inside the Managed Package Framework (MPF)
    /// to do it: it derives from the Package class that provides the implementation of the
    /// IVsPackage interface and uses the registration attributes defined in the framework to
    /// register itself and its components with the shell. These attributes tell the pkgdef creation
    /// utility what data to put into .pkgdef file.
    /// </para>
    /// <para>
    /// To get loaded into VS, the package must be referred by &lt;Asset Type="Microsoft.VisualStudio.VsPackage" ...&gt; in .vsixmanifest file.
    /// </para>
    /// </remarks>
    [PackageRegistration(UseManagedResourcesOnly = true, AllowsBackgroundLoading = true)]
    [Guid(EsbProjectTemplateWizardPackage.PackageGuidString)]
    public sealed class EsbProjectTemplateWizardPackage : AsyncPackage
    {
        /// <summary>
        /// EsbProjectTemplateWizardPackage GUID string.
        /// </summary>
        public const string PackageGuidString = "905087df-0172-4e78-96ff-249209ba9374";

        #region Package Members

        /// <summary>
        /// Initialization of the package; this method is called right after the package is sited, so this is the place
        /// where you can put all the initialization code that rely on services provided by VisualStudio.
        /// </summary>
        /// <param name="cancellationToken">A cancellation token to monitor for initialization cancellation, which can occur when VS is shutting down.</param>
        /// <param name="progress">A provider for progress updates.</param>
        /// <returns>A task representing the async work of package initialization, or an already completed task if there is none. Do not return null from this method.</returns>
        protected override async Task InitializeAsync(CancellationToken cancellationToken, IProgress<ServiceProgressData> progress)
        {
            // When initialized asynchronously, the current thread may be a background thread at this point.
            // Do any initialization that requires the UI thread after switching to the UI thread.
            await this.JoinableTaskFactory.SwitchToMainThreadAsync(cancellationToken);

            //VS.Events.SolutionEvents.OnAfterOpenProject += OnAfterOpenProject;
        }
        
        #endregion
    }
}
