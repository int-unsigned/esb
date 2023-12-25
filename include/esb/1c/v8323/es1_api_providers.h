/*
	"ESB" is designed to create external native components for the 1C(tm) program.
	Copyright © 2023 IntUnsigned	(v8classes@gmail.com)
	"ESB" is free software under LGPLv2.1 license with essential restriction about binary part (view esb-license.txt)
	"ESB" IS DISTRIBUTED "AS-IS" WITHOUT ANY, EVEN THE IMPLIED, WARRANTY. IN NO EVENT SHALL THE AUTHORS OR THE COPYRIGHT OWNER BE LIABLE FOR ANY CLAIM,
	DAMAGES OR OTHER LIABILITY ARISING OUT OF THE USE OF THIS SOFTWARE.
	THE AUTHOR DOES NOT GIVE ANY GUARANTEES AND DOES NOT BEAR ANY RESPONSIBILITY REGARDING THE USE OF 1C(tm) COMPONENTS PRODUCED USING THE ESB LIBRARY IN THE 1C(tm) ENVIRONMENT.
*/

#pragma once
#ifndef ESB_API_PROVIDERS_H_
#define ESB_API_PROVIDERS_H_

// здесь "базовые" настройки, и, в том числе подключение пользовательской конфигурации (ESB_USE_API_ALL)
#include "esb/esb_config_base.h"

namespace esb_api 
{

	//TOBE: сделать обработку синглетонов
#define ESBLIB_API_PROVIDER(TAG_, ...)	struct TAG_ {};
	struct ApiProviderTag 
	{
#		include "es1_api_providers.def.h"
	};
	//
#define ESB_API_PROVIDER_TAG_T(TAG_)		esb_api::ApiProviderTag::TAG_
}


#endif //ESB_API_PROVIDERS_H_