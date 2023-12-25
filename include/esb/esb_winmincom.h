#pragma once
#ifndef ESB_WINMINCOM_H_
#define ESB_WINMINCOM_H_


// ��� ������ ����� IUnkown � IClassFactory (� HRESULTs)
// �� �������� ��� �� ��������� �������� windows.h ����������...
// ��������� �������
#define COM_NO_WINDOWS_H
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
// ��� ��������� � <windows.h> ��� ������ ����������� ��������
#define NOATOM
#define NOGDI
#define NOGDICAPMASKS
#define NOMETAFILE
#define NOMINMAX
#define NOMSG
#define NOOPENFILE
#define NORASTEROPS
#define NOSCROLL
#define NOSOUND
#define NOSYSMETRICS
#define NOTEXTMETRIC
#define NOWH
#define NOCOMM
#define NOKANJI
#define NOCRYPT
#define NOMCX

ESB_WARNING_SUPRESS(ESB_WARN_DEFINED_IN_MACRO)
// combaseapi ���������� "Unknwnbase.h"
#include <combaseapi.h>
//#include "Unknwnbase.h"
ESB_WARNING_RESTORE()	//ESB_WARN_DEFINED_IN_MACRO


#endif // !ESB_WINMINCOM_H_


