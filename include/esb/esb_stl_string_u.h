/*
	"ESB" is designed to create external native components for the 1C(tm) program.
	Copyright © 2023 IntUnsigned	(v8classes@gmail.com)
	"ESB" is free software under LGPLv2.1 license with essential restriction about binary part (view esb-license.txt)
	"ESB" IS DISTRIBUTED "AS-IS" WITHOUT ANY, EVEN THE IMPLIED, WARRANTY. IN NO EVENT SHALL THE AUTHORS OR THE COPYRIGHT OWNER BE LIABLE FOR ANY CLAIM, 
	DAMAGES OR OTHER LIABILITY ARISING OUT OF THE USE OF THIS SOFTWARE.
	THE AUTHOR DOES NOT GIVE ANY GUARANTEES AND DOES NOT BEAR ANY RESPONSIBILITY REGARDING THE USE OF 1C(tm) COMPONENTS PRODUCED USING THE ESB LIBRARY IN THE 1C(tm) ENVIRONMENT.
*/


#pragma once
#ifndef ESB_STL_STRING_U_H
#define ESB_STL_STRING_U_H

#include "esb_stl_string.h"

namespace esb 
{

ESB_WRAPPER_FOR_STL_STRING((const esb::string_t&),
	CppStringUnicodeCRef, СппСтрокаЮникодКонстСсылка, "51434C1E-FDAB-4C55-BD1A-71040EC85BF7", "Объект СппСтрокаЮникодКонстСсылка");
ESB_WRAPPER_FOR_STL_STRING((esb::string_t&),
	CppStringUnicodeRef, СппСтрокаЮникодСсылка, "1F3694DB-729D-46D5-9DA4-F6D79B9794E2", "Объект СппСтрокаЮникодСсылка");
ESB_WRAPPER_FOR_STL_STRING((esb::string_t),
	CppStringUnicode, СппСтрокаЮникод, "931F0A7F-2947-4E3B-BF50-7CC87DE40A40", "Объект СппСтрокаЮникод");
ESB_WRAPPER_FOR_STL_STRING((const esb::string_t),
	CppStringUnicodeConst, СппСтрокаЮникодКонст, "989B1AF5-7B7B-497E-B4AD-E11C6F1D16D0", "Объект СппСтрокаЮникодКонст");


//NOTE	Регистрация в платформе 1С на усмотрение пользователя. Образец для вставки ниже.
//ESB_REGISTER_WRAPPER_FOR(esb::string_t);
//

} //namespace esb 

#endif //ESB_STL_STRING_U_H