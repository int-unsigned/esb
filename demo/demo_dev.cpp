// �����! ���� � ��� ���� ���� esb_app_config, �� ��� ����� �������� ����� ������ �� ��������� esb. ����� ��������� ����� ���� �������������.
#include "esb_app_config.h"
#include "..\esb_core.h"
#if __has_include("..\..\esb_demo_native.hpp")
#	include "..\..\esb_demo_native.hpp"
	bool DevModeEnabled() { return true; }
#else
	void TestArraySortUseNativeIxCollectionAndValues(esb::IIxCollectionRW& esb_ix_array_) {}
	bool DevModeEnabled() { return false; }
#endif