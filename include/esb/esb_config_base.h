#pragma once
/*
	"ESB" is designed to create external native components for the 1C(tm) program.
	Copyright © 2023 IntUnsigned	(v8classes@gmail.com)
	"ESB" is free software under LGPLv2.1 license with essential restriction about binary part (view esb-license.txt)
	"ESB" IS DISTRIBUTED "AS-IS" WITHOUT ANY, EVEN THE IMPLIED, WARRANTY. IN NO EVENT SHALL THE AUTHORS OR THE COPYRIGHT OWNER BE LIABLE FOR ANY CLAIM,
	DAMAGES OR OTHER LIABILITY ARISING OUT OF THE USE OF THIS SOFTWARE.
	THE AUTHOR DOES NOT GIVE ANY GUARANTEES AND DOES NOT BEAR ANY RESPONSIBILITY REGARDING THE USE OF 1C(tm) COMPONENTS PRODUCED USING THE ESB LIBRARY IN THE 1C(tm) ENVIRONMENT.
*/

// "минимальное" конфигурирование для esb. используется в том числе и в _dispinfo.
#pragma once
#ifndef ESB_CONFIG_BASE_H_
#define ESB_CONFIG_BASE_H_


// Первым делом пробуем загрузить пользовательскую конфигурацию. Там возможны указания для нас.
// Файл esb_app_config.h не часть папки esb. Вы можете создать его в папке своего проекта и подключать до подключения esb.h 
// или в свойствах проекта указать как дополнтельную папку проекта $(MSBuildProjectDirectory) тогда этот файл будет найден и подключен автоматически
#if !defined(ESB_APP_CONFIG_H_) && __has_include("esb_app_config.h")
#	include "esb_app_config.h"
#endif


// по-умолчанию все возможное апи платформы не используется
// 1C АПИ это весьма и весьма неконсервативная часть платформы. Часто и непредсказуемо может меняться. 
// Фактически для каждой очередной версии нужно проверять не сбились ли идентификаторы методов. 
// Сейчас в есб идентификаторы от v8.3.9.1818. У 8.3.23.1688 они наполовину другие. 
// Когда произошли изменения нужно разбираться фактически с каждой промежуточной версией.
// При необходимости можно включать по-отдельности определяя индивидуально ESB_API_INCLUDE_core83, ESB_API_INCLUDE_backend0x0, ... и т.п.
// (см. 1c_api_providers.inc)
#ifndef ESB_USE_API_ALL
#	define ESB_USE_API_ALL	0
#endif // !ESB_USE_API_ALL

//"native" api (которое здесь, в есб, переписано) мы подключаем по-умолчанию всегда. оно не требует "провайдеров".
// (можно не включать определив ESB_API_INCLUDE_NATIVE в 0)
#ifndef ESB_API_INCLUDE_NATIVE
#	define ESB_API_INCLUDE_NATIVE		1	
#endif


// если не включать все апи (определив ESB_USE_API_ALL 0, или просто не определяя), то можно подключать апи-провайдеров по отдельности 
// определяя в Вашем esb_app_config.h макросы вида
//#define ESB_USE_API_PROVIDER_core83	1

//NOTE
// **** Using defined as an expansion in conditional directive is considered as Undefined Behavior. ****
//https://stackoverflow.com/questions/41773080/can-a-define-be-assigned-the-result-of-defined

#define ESB_API_PROVIDER_INCLUDE_(P_)	(ESB_USE_API_PROVIDER_ ## P_)
#define ESB_API_PROVIDER_INCLUDE(P_)	(ESB_USE_API_ALL || ESB_API_PROVIDER_INCLUDE_(P_))


// _dispinfo нужны определения наших dispix_t. остальное в esb_config.h
namespace esb
{
	using dispix_t = unsigned int;
}

#endif