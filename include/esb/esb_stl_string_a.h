/*
	"ESB" is designed to create external native components for the 1C(tm) program.
	Copyright © 2023 IntUnsigned	(v8classes@gmail.com)
	"ESB" is free software under LGPLv2.1 license with essential restriction about binary part (view esb-license.txt)
	"ESB" IS DISTRIBUTED "AS-IS" WITHOUT ANY, EVEN THE IMPLIED, WARRANTY. IN NO EVENT SHALL THE AUTHORS OR THE COPYRIGHT OWNER BE LIABLE FOR ANY CLAIM, 
	DAMAGES OR OTHER LIABILITY ARISING OUT OF THE USE OF THIS SOFTWARE.
	THE AUTHOR DOES NOT GIVE ANY GUARANTEES AND DOES NOT BEAR ANY RESPONSIBILITY REGARDING THE USE OF 1C(tm) COMPONENTS PRODUCED USING THE ESB LIBRARY IN THE 1C(tm) ENVIRONMENT.
*/


#pragma once
#ifndef ESB_STL_STRING_A_H
#define ESB_STL_STRING_A_H

#include "esb_stl_string.h"

namespace esb 
{

ESB_WRAPPER_FOR_STL_STRING((const std::string&),
	CppStringAnsiCRef, СппСтрокаАнсиКонстСсылка, "BBDFEF53-4A90-48CB-B830-124078C9A338", "Объект СппСтрокаАнсиКонстСсылка");
ESB_WRAPPER_FOR_STL_STRING((std::string&),
	CppStringAnsiRef, СппСтрокаАнсиСсылка, "788CBED5-9985-4ED2-86A2-6C62AA9CCECA", "Объект СппСтрокаАнсиСсылка");
ESB_WRAPPER_FOR_STL_STRING((std::string),
	CppStringAnsi, СппСтрокаАнси, "B633E087-C656-400A-9EEE-D0B31D9C13E1", "Объект СппСтрокаАнси");
ESB_WRAPPER_FOR_STL_STRING((const std::string),
	CppStringAnsiConst, СппСтрокаАнсиКонст, "34D46138-65F1-4406-89A2-A8808EDFC3B5", "Объект СппСтрокаАнсиКонст");

//NOTE	Для регистрация в платформе 1С нужно отдельно указывать в .cpp файле компоненты
//		ESB_REGISTER_WRAPPER_FOR(std::string), ESB_REGISTER_WRAPPER_FOR(const std::string), т.п. по необходимости

} //namespace esb 

#endif //ESB_STL_STRING_A_H