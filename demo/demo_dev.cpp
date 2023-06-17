// Важно! Если у Вас есть свой esb_app_config, то его нужно включать везде первым до включения esb. Иначи настройки могут быть несовместными.
#include "esb_app_config.h"
#include "..\esb_core.h"
#if __has_include("..\..\esb_demo_native_disabled_.hpp")
#	include "..\..\esb_demo_native_.hpp"
	bool DevModeEnabled() { return true; }
#else
	void TestArraySortUseNativeIxCollectionAndValues(esb::IIxCollectionRW& esb_ix_array_) {}
	bool DevModeEnabled() { return false; }
#endif