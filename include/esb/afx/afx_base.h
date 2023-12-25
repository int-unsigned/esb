#pragma once
#ifndef AFX_BASE_H_
#define AFX_BASE_H_

// ���� ���� �������� ������ ��������, ������� ������� �������������, �� ���� ��� �������
//
// ��������� "�������" ����� ������������ "������" ���������� � <concepts>, ������� ����� ���� ��������
#include <concepts>


template <class T, class... Ts>
inline constexpr bool is_some_of = std::disjunction_v<std::is_same<T, Ts>...>;


// ���������� remove_cv_t, �� �� remove_cvref_t. �.�. ������ ��� ��� ����� ����..
// (� std ��� is_integral � �.�. ��� ������)
template<typename T>
inline constexpr bool is_some_char_t = is_some_of<std::remove_cv_t<T>, char, unsigned char, signed char, wchar_t, char8_t, char16_t, char32_t>;
//TODO	����������� unsigned char � ���� �������� �������� ������ ������������...
template<typename T>
inline constexpr bool is_some_size_t = is_some_of<std::remove_cv_t<T>, unsigned char, unsigned short, unsigned int, unsigned long, unsigned long long>;


// std::is_constructible_v � �������� ��������� ���������� ������� ������������, � �� �����������
// �������� � ���������� �����������
// (����� ��������� std::construct_at, �� IDE �� �� ��������, ���� ���������� ��������. ������ ��� �����)
// ��. ���� �������� ��������������� unsigned int �� ������ int �� ����������� - �� ��������� �� ������������!
template<class T, class... ArgsTs>
inline constexpr bool is_constructable_from = requires(ArgsTs&&... args_) {
	T{ std::forward<ArgsTs>(args_)... };
	::new (static_cast<void*>(nullptr)) T{ std::forward<ArgsTs>(args_)... };	//��� � �� ���-�� ����� - ��������� � ��� � ���
};
static_assert(is_constructable_from<std::string_view, char*, unsigned int> == true);
static_assert(is_constructable_from<std::string_view, char> == false);


//
#ifdef __cpp_lib_to_underlying
template<typename T>
inline constexpr std::underlying_type_t<T> to_underlying_enum_type_value(const T& value_) {
		return std::to_underlying(value_);
}
#else
template<typename T>
inline constexpr std::underlying_type_t<T> to_underlying_enum_type_value(const T& value_) {
	return static_cast<std::underlying_type_t<T>>(value_);
}
#endif


//��. afx_serialize.h
//����� ����������� �������� ����, ����� ������� �� ������������� � ���������� � afx_serialize.h
// ��� ����� ���� �������� �������-������������� "explicit" ����� ������������� ���� ����������
// ��� ������ ����� ����������� ����� ����� �������� ����� ���� ���������� - ������� ����, ���������� ��������� �.�.)
// ��������� �� ������� �����������!
//��. � �����-������ namespace ���������������� ���������� ������, � ������ ���� �����
template<typename>
struct is_serializable_binary_explicit_type {
	static constexpr bool value = false;
};
template<typename T>
inline constexpr bool is_serializable_binary_explicit = is_serializable_binary_explicit_type<T>::value;
//
#define META_DECLARE_BINARY_SERIALIZEBLE(TYP_)			\
	template<>											\
	struct is_serializable_binary_explicit_type<TYP_> {	\
		static constexpr bool value = true;				\
	}

//��. afx_serialize.h
template<typename ContainerType>
struct sized_container_creator_type;



using compile_stamp_t = uint32_t;
//
template<size_t NChar>
consteval compile_stamp_t comlile_TIME_as_uint32(const char(&ts)[NChar]) {
	static_assert(NChar == 9);
	uint32_t n = 0;
	n += ts[0] - '0';	n *= 10;
	n += ts[1] - '0';	n *= 10;
	//2
	n += ts[3] - '0';	n *= 10;
	n += ts[4] - '0';	n *= 10;
	//5
	n += ts[6] - '0';	n *= 10;
	n += ts[7] - '0';

	return n;
}
consteval compile_stamp_t COMPILE_STAMP() {
	return comlile_TIME_as_uint32(__TIME__);
}


#endif
