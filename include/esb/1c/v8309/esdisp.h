/*
	"ESB" is designed to create external native components for the 1C(tm) program.
	Copyright © 2023 IntUnsigned	(v8classes@gmail.com)
	"ESB" is free software under LGPLv2.1 license with essential restriction about binary part (view esb-license.txt)
	"ESB" IS DISTRIBUTED "AS-IS" WITHOUT ANY, EVEN THE IMPLIED, WARRANTY. IN NO EVENT SHALL THE AUTHORS OR THE COPYRIGHT OWNER BE LIABLE FOR ANY CLAIM, 
	DAMAGES OR OTHER LIABILITY ARISING OUT OF THE USE OF THIS SOFTWARE.
	THE AUTHOR DOES NOT GIVE ANY GUARANTEES AND DOES NOT BEAR ANY RESPONSIBILITY REGARDING THE USE OF 1C(tm) COMPONENTS PRODUCED USING THE ESB LIBRARY IN THE 1C(tm) ENVIRONMENT.
*/
#pragma once
#ifndef ESB_ESDISP_H_
#define ESB_ESDISP_H_


#include "esb/esb_config_base.h"

namespace esb 
{
	// RoundMode "заремлен" в es1_base.def т.к. его реализация везде ручками (модельная). поэтому декларировать его надо explicity
	class RoundMode;

	namespace _internal 
	{
		namespace _dispinfo
		{
			namespace _v8309
			{
				template<class>
				struct MethIdsFor;
				template<class>
				struct PropIdsFor;
				template<class>
				struct EnumIdsFor;// {//static constexpr dispix_t _ids_enum_count = 0; };

				template<typename T>
				inline constexpr bool is_typeinfo_singleton = true;
			}

			template<typename T>
			using MethIdsFor = _internal::_dispinfo::_v8309::MethIdsFor<T>;
			template<typename T>
			using PropIdsFor = _internal::_dispinfo::_v8309::PropIdsFor<T>;
			template<typename T>
			using EnumIdsFor = _internal::_dispinfo::_v8309::EnumIdsFor<T>;

			template<typename T>
			inline constexpr bool is_typeinfo_singleton = _internal::_dispinfo::_v8309::is_typeinfo_singleton<T>;
		} //_dispinfo
	}// _internal
}// esb
#endif	//ESB_ESDISP_H_
