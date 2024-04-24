// Для ЕСБ не требуется точка входа DllMain
// Если это нужно Вашему проекту раскомментируйте этот блок и сделайте то что Вам нужно
//
#if 0
#define WIN32_LEAN_AND_MEAN             // Исключите редко используемые компоненты из заголовков Windows
#define NOMINMAX
// Файлы заголовков Windows
#include <windows.h>

// dllmain.cpp : Определяет точку входа для приложения DLL.
BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}
#endif