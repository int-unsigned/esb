#pragma once

#include <assert.h>
#include <string>


//#define _SYS_GUID_OPERATORS_ 0
#include "guiddef.h"


inline void to_raw_hex_byte(wchar_t(&buf_)[2], uint8_t value_) {
	static const wchar_t digits[] = { L'0', L'1', L'2', L'3', L'4', L'5', L'6', L'7', L'8', L'9', L'A', L'B', L'C', L'D', L'E', L'F' };
	static_assert(std::size(digits) == 16);
	buf_[1] = digits[value_ & 0b1111];
	value_ >>= 4;
	buf_[0] = digits[value_ & 0b1111];
}
inline void to_hex_u8(wchar_t* buf_1st_, wchar_t* buf_last_, uint8_t value_) {
	static constexpr wchar_t digits[] = { L'0', L'1', L'2', L'3', L'4', L'5', L'6', L'7', L'8', L'9', L'A', L'B', L'C', L'D', L'E', L'F' };
	static_assert(std::size(digits) == 16);
	assert(buf_last_ - buf_1st_ == 1);

	*buf_last_ = digits[value_ & 0b1111];	value_ >>= 4;	--buf_last_;
	*buf_last_ = digits[value_ & 0b1111];	value_ >>= 4;	--buf_last_;
}
inline void to_hex_raw(wchar_t buf_[32], const CLSID& clsid_) {
	static_assert(sizeof(CLSID) == sizeof(uint32_t) * 4);
	const uint8_t* p = (uint8_t*)&clsid_;
	for (size_t iByte = 0, iWch = 0; iByte < 16; ++iByte, iWch += 2) {
		to_hex_u8(&buf_[iWch], &buf_[iWch + 1], p[iByte]);
	}
}

//inline void to_hex

//записывает GUID в буфер "по-простому", несколько отлича€сь от формата DEFINE_GUID. ѕример:
//DEFINE_GUID(IID_IContext_, 0xFD7B6CC2, 0xDC8E, 0x11D2, 0xB8, 0xD0, 0x00, 0x80, 0x48, 0xDA, 0x03, 0x35);
//Buffer:					   FD7B6CC2	   11D2    DC8E    80    00    D0    B8    35    03    DA    48
//							   0		   0 11	   1111	   11    11    22    22    22    22    22    33
//							   01234567	   8901	   2345	   67	 89	   01    23    45    67    89    01
inline void to_hex(wchar_t* buf_1st_, wchar_t* buf_last_, uint32_t value_) {
	static constexpr wchar_t digits[] = { L'0', L'1', L'2', L'3', L'4', L'5', L'6', L'7', L'8', L'9', L'A', L'B', L'C', L'D', L'E', L'F' };
	static_assert(std::size(digits) == 16);
	assert(buf_last_ - buf_1st_ == 7);

	*buf_last_ = digits[value_ & 0b1111];	value_ >>= 4;	--buf_last_;
	*buf_last_ = digits[value_ & 0b1111];	value_ >>= 4;	--buf_last_;
	*buf_last_ = digits[value_ & 0b1111];	value_ >>= 4;	--buf_last_;
	*buf_last_ = digits[value_ & 0b1111];	value_ >>= 4;	--buf_last_;
	*buf_last_ = digits[value_ & 0b1111];	value_ >>= 4;	--buf_last_;
	*buf_last_ = digits[value_ & 0b1111];	value_ >>= 4;	--buf_last_;
	*buf_last_ = digits[value_ & 0b1111];	value_ >>= 4;	--buf_last_;
	*buf_last_ = digits[value_ & 0b1111];	value_ >>= 4;	--buf_last_;
}

inline void to_hex(wchar_t buf_[32], const CLSID& clsid_) {
	static_assert(sizeof(CLSID) == sizeof(uint32_t) * 4);
	const uint32_t* p = (uint32_t*)&clsid_;
	to_hex(&buf_[0], &buf_[7], p[0]);
	to_hex(&buf_[8], &buf_[15], p[1]);
	to_hex(&buf_[16], &buf_[23], p[2]);
	to_hex(&buf_[24], &buf_[31], p[3]);
}

static_assert(sizeof(wchar_t) == sizeof(char16_t));
inline void to_hex(char16_t* buf_1st_, char16_t* buf_last_, uint32_t value_) {
	return to_hex((wchar_t*)buf_1st_, (wchar_t*)buf_last_, value_);
}
inline void to_hex(char16_t buf_[32], const CLSID& clsid_) {
	static_assert(sizeof(CLSID) == sizeof(uint32_t) * 4);
	const uint32_t* p = (uint32_t*)&clsid_;
	to_hex(&buf_[0], &buf_[7], p[0]);
	to_hex(&buf_[8], &buf_[15], p[1]);
	to_hex(&buf_[16], &buf_[23], p[2]);
	to_hex(&buf_[24], &buf_[31], p[3]);
}


inline std::wstring to_DEFINE_GUID(const wchar_t sclsid_[32]) {
	std::wstring def;
	def.reserve(75);
	def.append(L"0x");	def.append(&sclsid_[0], 8);	def.append(L", ");
	def.append(L"0x");	def.append(&sclsid_[12], 4);	def.append(L", ");
	def.append(L"0x");	def.append(&sclsid_[8], 4);	def.append(L", ");

	def.append(L"0x");	def.append(&sclsid_[22], 2);	def.append(L", ");
	def.append(L"0x");	def.append(&sclsid_[20], 2);	def.append(L", ");
	def.append(L"0x");	def.append(&sclsid_[18], 2);	def.append(L", ");
	def.append(L"0x");	def.append(&sclsid_[16], 2);	def.append(L", ");

	def.append(L"0x");	def.append(&sclsid_[30], 2);	def.append(L", ");
	def.append(L"0x");	def.append(&sclsid_[28], 2);	def.append(L", ");
	def.append(L"0x");	def.append(&sclsid_[26], 2);	def.append(L", ");
	def.append(L"0x");	def.append(&sclsid_[24], 2);	//def.append(L", ");

	return std::move(def);
}


inline std::u16string to_DEFINE_GUID(const char16_t sclsid_[32]) {
	static_assert(sizeof(wchar_t) == sizeof(char16_t));
	std::wstring s = to_DEFINE_GUID((const wchar_t*)sclsid_);
	return std::u16string((const char16_t*)s.data(), s.size());
}
inline void to_hex_raw(char16_t buf_[32], const CLSID& clsid_) {
	static_assert(sizeof(wchar_t) == sizeof(char16_t));
	return to_hex_raw((wchar_t*)buf_, clsid_);
}


//TODO	Ќужно более "пр€мо" сделать мину€ преобразование to_hex
inline std::wstring to_DEFINE_GUID(const GUID& guid_) {
	wchar_t buf[32];
	to_hex(buf, guid_);
	return to_DEFINE_GUID(buf);
}

inline bool is_guid_null(const GUID& guid_) {
	static const GUID g0{};
	return (memcmp(&guid_, &g0, sizeof(GUID)) == 0);
}