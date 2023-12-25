/*
	"ESB" is designed to create external native components for the 1C(tm) program.
	Copyright © 2023 IntUnsigned	(v8classes@gmail.com)
	"ESB" is free software under LGPLv2.1 license with essential restriction about binary part (view esb-license.txt)
	"ESB" IS DISTRIBUTED "AS-IS" WITHOUT ANY, EVEN THE IMPLIED, WARRANTY. IN NO EVENT SHALL THE AUTHORS OR THE COPYRIGHT OWNER BE LIABLE FOR ANY CLAIM, 
	DAMAGES OR OTHER LIABILITY ARISING OUT OF THE USE OF THIS SOFTWARE.
	THE AUTHOR DOES NOT GIVE ANY GUARANTEES AND DOES NOT BEAR ANY RESPONSIBILITY REGARDING THE USE OF 1C(tm) COMPONENTS PRODUCED USING THE ESB LIBRARY IN THE 1C(tm) ENVIRONMENT.
*/


#pragma once
#ifndef ESB_META_H
#define ESB_META_H
#include <cassert>
//
#include "esb_core.h"
#include "esb/afx/afx_ustring.h"
using afx_ustring::to_nocase;
using afx_ustring::cstr_compare;
//
#include <tuple>

// В данном файле собраны методы построения диспатч интерфейса наших объектов для 1С
// NOTE		Много consteval/constexpr методов и практически все выписаны ручками без использования алгоритмов std
//			Сделано так потому что неоднократно сталкивался, что std-consteval не работает. Ручками и попороще работает. 
//			(Microsoft Visual Studio Enterprise 2019 Версия 16.11.19)
// Вкратце - Вы определяете свой интерфейс в виде одного (двух) tuple, а все остальное делается здесь.


// Invoker
// Здесь мы описываем наш интерфейс в терминах 1С параметров - in\out\opt. (как наш интерфейс видит 1С)
// Параметр - это формальное описание имени\типа\признаков элемента для вызывающего. Вызывающие туда присваивает значение, и, для нас это становится аргументом.
// 
//	1.	not-in параметров у нас не бывает - все параметры мы можем читать. Вообще малопонятная штука not-in параметр. Таковым можно считать разве-что возвращаемое
//		значение, если считать его одним из параметров. Кстати для некоторых языков\интерфейсов это может быть не одно, а несколько значение - [a, b, c] = InvokeFunc(...)
//		при этом и процедура записывается однотипно - [] = InvokeProc(...). Но это о чем-то гипотетическом...
// 
//	2.	out (для 1С!) параметров у нас пока тоже не сделано. 1С передает нам интерфейс своей переменной - IVariable, и, out-параметр означает, что мы можем 
//		этой переменной сделать IVariable_SetValue(). У нас пока такого не реализовано. _SetValue мы делаем только для переменной представляющей возвращаемое значение.
// 
//	TOBE: в принципе такую функциональность можно реализовать сделав спец-класс с интерфейсом указателя - operator*() возвращает что-то типа ref-класса с опреатором
//		  преобразование к типу EsbClassT для чтения значения из переменной и operator=() для установки значения переменной. Или вообще без ptr-like - сразу ref-like).
//		  Но использование out-параметров само по себе сомнительная техника программирования...
//		
//		Еще раз - out-параметров у нас нет. мы не умеем менять значение в переменной интерфейс которой 1С передает нам как аргумент.
//		Не нужно путать это с возможностью менять значение. То есть если 1С передает нам интерфейс переменной в которой находится, к примеру, массив,
//		то мы спокойно можем этот массив читать, писать, чистить, т.п. Удалить мы его не можем - он все равно останется в переменной у 1С.
//		Также мы ничего не можем сделать с примитивными значениеми (Строка, Число, тп.) - они константные. Мы можем переменной, которая представляет у нас аргумент
//		присвоить новое значение, но оно тут-же у нас останется и с окончанием вызова помрет, а в переменной 1С останется старое. Мы можем менять значение переданного
//		объекта только через его не-константный интерфейс. Для себя мы можем (и нужно) объявлять аргументы с const. Это правильно для с++, но 1С об этом не узнает -
//		у 1С понятие константности отсутствует.
//
// 3.	opt-параметры мы можем объявлять. В принципе сам интерфейс 1С позволяет объявлять opt-параметры с любыми значениями по-умолчанию, и даже (в зависимости от
//		имплементации) меняющимися от вызова к вызову. Естесственно разговор идет о ран-тайм вычислении и значений. Компиле-тайм описания интерфейса у 1С просто нет.
//		Но текущая реализация авто-построителя нашего диспатч-лайк интерфейса считает opt-параметром декларированный как UndefOr<...>. Также реализация интерфейса
//		ExtTypeDescriptorValueObject не инициализирует метод GetParamDefault, а реализация интерфейса объекта в этом случае отдает Undef
//		Если реализовывать ExtTypeDescriptorValueObject вручную, то можно сделать мудренее. А через Invoker пока так.
//
//	Итак. Аргументы метода могут быть в виде (для нас - это аргументы, это для 1С они параметры):
//	Array&			- запрещено правилами с++. Наш метод вызывает Инвокер на-лету. Он не создает перемнных к которым можно привязать lvalue.
//	const Array&	- классичиский немодифицируемый аргумент.
//	Array			- объект можно модифицировать в рамках его не-конст интерфейса.
//	Array&&			- фактически то-же самое что и просто Array, но немного оптимизируется дальнейшая move передача значения аргумента куда-либо. (см. про Var<>)
//	(на месте Array подразумевается любой esb-класс, esb-value-based класс, в том числе и Arbitrary и ClassMix<> со списком классов)
// 
//	Также возможно использование с++ bool для Bollean - имплисит преобразование, const strview_t& для String - (const!), сами данные строки менять мы не имеем права
//  signed\unsigned int\long\64 для Numeric - строгое преобразование с проверкой дробности и знака и переполнения - если что не так, то ошибка! 
//	Если что не так, то Вы всегда можете принять сам Numeric и решить что делать.
// 
//	Var<...>		- в качестве аргумента декларирование не предусмотрено. Конструкция Var<> предназначена для использования в ситуации, когда значение очень активно
//	используется для вызова методов платформы. Это не самый тривиальный объект. В случае если Вам действительно нужен Var<>, то декларируйте интерфейс в виде Array&&
//  и далее инициализируйте Var<> через std::move. НЕ ЗАБЫВАТЬ! что после move оригинальный аргумент ПУСТ! и попытка использовать его приведет к КРАХУ!
//  Опимально это делать используя дополнительный ..Ex метод с аргументом типа Var<>, который инициализируется через move. А сам метод интерфейса оставить пустым - 
//  типа прокси. См. демку.
//	TOBE:	^^^^^^^^^^^^^^^^^^^ Может и стоит, как минимум в дебаге, не обнулять InterfacePtr, а переустанавливать его на интерфейс-заглушку, которая будет давать
//			ассерт на любую попытку его использования (кроме релиз). Можно один такой интерфейс с сотней асерт-методов (на всех хватит) сделать и навязывать
//			через reinerpret_cast
// 
//


namespace esb // esb-meta
{

// Для описания есб-интерфейса какого либо объекта нужно объявить специализацию этой структуры для этого объекта
	template<typename>
	struct interface_info_t;
	// в этой структуре должны быть (все или частично) следующие поля
	// using object_t = <ваш-класс>
	// Минамально достаточно только полей ESB_INTERFACE_PROP_META_FIELD и ESB_INTERFACE_METH_META_FIELD, или одного из них
	// все остальные поля (в случае их отсутствия) есб-мета делает сам в случае их отсутствия
	// (Вы их можете построить сами, если по какой-то причине Вас автогенерация не устраивает. При этом поля ESB_INTERFACE_PROP_META_FIELD и ESB_INTERFACE_METH_META_FIELD
	// могут быть даже не обязательными, поскольку собственно диспатч-лайк интерфейс напрямую с мета-информацией не работает. Ему нужны только INFO и FIND)
#define ESB_INTERFACE_PROP_META_FIELD	InterfacePropMeta_
#define ESB_INTERFACE_METH_META_FIELD	InterfaceMethMeta_
#define ESB_INTERFACE_CTOR_META_FIELD	InterfaceCtorMeta_
#define ESB_INTERFACE_PROP_INFO_FIELD	InterfacePropInfo_
#define ESB_INTERFACE_METH_INFO_FIELD	InterfaceMethInfo_
#define ESB_INTERFACE_CTOR_INFO_FIELD	InterfaceCtorInfo_
#define ESB_INTERFACE_PROP_FIND_FIELD	InterfacePropFind
#define ESB_INTERFACE_METH_FIND_FIELD	InterfaceMethFind
#define ESB_INTERFACE_CTOR_FIND_FIELD	InterfaceCtorFind
#define ESB_INTERFACE_OBJECT_FIELD		object_t
#define ESB_INTERFACE_DESCRIPTOR_FIELD	descriptor_t


#define ESB_INTERFACEDEF_TERM(NAME_EN_, NAME_RU_)	ESB_T(NAME_EN_), ESB_T(NAME_RU_)

#define ESB_INTERFACEDEF(CLASS_T_, DESCRIPTOR_T_, GUID_STR_, NAME_PAIR_, TEXT_)				\
	template<>																				\
	struct interface_info_t<CLASS_T_> {														\
		using object_t = CLASS_T_;															\
		using descriptor_t = DESCRIPTOR_T_< CLASS_T_ >;										\
		static constexpr CLSID				TypeId_ = guid_from_hex(GUID_STR_);				\
		static constexpr FixedTerm			TypeTerm_{ ESB_INTERFACEDEF_TERM NAME_PAIR_ };	\
		static constexpr strview_t			TypeDescriptionInit_{ ESB_T(TEXT_) };


#define ESB_INTERFACEDEF_DESCRIPTOR_T(CLASS_T_)		esb::interface_info_t<CLASS_T_>::descriptor_t



	template<class ExtObjectT> consteval bool dispinterface_check();

#define ESB_INTERFACEDEF_DONE(CLASS_T_)		\
	};	static_assert(dispinterface_check< esb::interface_info_t<CLASS_T_>::descriptor_t::interface_t > () == true, "invalid dispinterface for " #CLASS_T_ )


#define ESB_INTERFACEDEF_OBJECT_STABLE(CLASS_T_, GUID_STR_, NAME_PAIR_, TEXT_)					\
	ESB_INTERFACEDEF(CLASS_T_, TypeDescriptorValueObjectStable, GUID_STR_, NAME_PAIR_, TEXT_)


#define ESB_INTERFACEDEF_VALUE(CLASS_T_, GUID_STR_, NAME_PAIR_, TEXT_)							\
	ESB_INTERFACEDEF(CLASS_T_, TypeDescriptorValueSimpleAuto, GUID_STR_, NAME_PAIR_, TEXT_)		\
		ESB_META_CTOR_INTERFACE_VALUE_AUTO(CLASS_T_);											\
	ESB_INTERFACEDEF_DONE(CLASS_T_)


	template<class MetaInterfaceT>
	inline constexpr bool is_interface_has_prop_meta = requires { MetaInterfaceT::ESB_INTERFACE_PROP_META_FIELD; };
	template<class MetaInterfaceT>
	inline constexpr bool is_interface_has_meth_meta = requires { MetaInterfaceT::ESB_INTERFACE_METH_META_FIELD; };
	template<class MetaInterfaceT>
	inline constexpr bool is_interface_has_ctor_meta = requires { MetaInterfaceT::ESB_INTERFACE_CTOR_META_FIELD; };

	template<class ExtObjectT>
	inline constexpr bool is_interface_has_prop_info = requires { ExtObjectT::ESB_INTERFACE_PROP_INFO_FIELD; };
	template<class ExtObjectT>
	inline constexpr bool is_interface_has_meth_info = requires { ExtObjectT::ESB_INTERFACE_METH_INFO_FIELD; };
	template<class ExtObjectT>
	inline constexpr bool is_interface_has_ctor_info = requires { ExtObjectT::ESB_INTERFACE_CTOR_INFO_FIELD; };

	template<class ExtObjectT>
	inline constexpr bool is_interface_has_prop_info_find = requires { ExtObjectT::ESB_INTERFACE_PROP_FIND_FIELD; };
	template<class ExtObjectT>
	inline constexpr bool is_interface_has_meth_info_find = requires { ExtObjectT::ESB_INTERFACE_METH_FIND_FIELD; };
	template<class ExtObjectT>
	inline constexpr bool is_interface_has_ctor_info_find = requires { ExtObjectT::ESB_INTERFACE_CTOR_FIND_FIELD; };


	template<class T>
	inline constexpr bool is_dispinfo_item = false;
	template<>	inline constexpr bool is_dispinfo_item<DispInfoMembMeth> = true;
	template<>	inline constexpr bool is_dispinfo_item<DispInfoMembProp> = true;
	template<>	inline constexpr bool is_dispinfo_item<DispInfoStatMeth> = true;
	template<>	inline constexpr bool is_dispinfo_item<DispInfoStatProp> = true;

	template<class T>
	concept DispInfoItemConcept = is_dispinfo_item<T>;


	// Доопределяем построитель дисп-информации для 1С о наших методах
	template<class... Types>
	struct DispParaInfoMaker;
	template <>
	struct DispParaInfoMaker<> {
		using bitpack_t = DispMethInfo::bitpack_t;
		static consteval bitpack_t make_bitpack(size_t /*para_count_*/, size_t /*n_para_*/) { return 0; }
		static consteval bitpack_t make_bitpack() { return 0; }
	};
	template<class ThisT, class... RestT>
	struct DispParaInfoMaker<ThisT, RestT...> {
		using bitpack_t = DispParaInfoMaker<>::bitpack_t;
		static consteval bitpack_t make_bitpack(size_t para_count_, size_t n_para_) {
			return (DispMethInfo::make_para_bit(para_count_, n_para_, is_param_opt_v<ThisT>) | DispParaInfoMaker<RestT...>::make_bitpack(para_count_, n_para_ + 1));
		}
		static consteval bitpack_t make_bitpack() {
			static_assert((sizeof...(RestT) + 1) <= DispMethInfo::MAX_PARA_COUNT, "param count > MAX_PARA_COUNT");
			return make_bitpack((sizeof...(RestT) + 1), 0);
		}
	};

	template<bool HasRetval_, class... ParaT>
	consteval DispMethInfo make_DispMethInfo() {
		using bitpack_t = DispMethInfo::bitpack_t;
		static_assert(sizeof...(ParaT) <= DispMethInfo::MAX_PARA_COUNT);
		bitpack_t info_bitpack_size = sizeof...(ParaT);
		bitpack_t info_bitpack_retv = ((HasRetval_) << ESB_METHINFO_SIZE_BITS);
		bitpack_t info_bitpack_para = DispParaInfoMaker<ParaT...>::make_bitpack();
		return DispMethInfo{ bitpack_t(info_bitpack_para | info_bitpack_retv | info_bitpack_size) };
	}



	// Структуры Invoker определяют собственно диспатч интерфейс для методов.
	// т.е. метод класса имеет некоторый с++ вид: Boolean DoSomtning(Sntring, Numeric, bool), а вызывается платформой как (IVariable* ret_, const argpack_t& args_)
	// Invoker осуществляет необходимое преобразования аргументов и возвращаемого значения. Плюс вычисляет необходимую информацию о методе.
	template<typename RetTypeT, typename ... ParamTypesT>
	struct InvokerImplBase {
		using ret_t = RetTypeT;
		static constexpr size_t param_count_ = sizeof...(ParamTypesT);
		static constexpr bool	is_proc_ = std::is_same_v<RetTypeT, void>;
		static constexpr bool	is_func_ = !is_proc_;
		static constexpr bool	has_retval_ = is_func_;
		static consteval DispMethInfo make_meth_info() {
			return make_DispMethInfo<has_retval_, ParamTypesT...>();
		}
	};

	template<typename ClassTypeT, typename RetTypeT, typename ... ParamTypesT>
	struct InvokerImpl : public InvokerImplBase<RetTypeT, ParamTypesT...> {
		using base_t = InvokerImplBase<RetTypeT, ParamTypesT...>;
		using class_t = ClassTypeT;
		using func_t = typename RetTypeT(ClassTypeT::*)(ParamTypesT...);
		using const_func_t = typename RetTypeT(ClassTypeT::*)(ParamTypesT...) const;

		template<size_t... _Ix>
		static void do_invoke(class_t& inst_, func_t func_, ESB_UNUSED IVariable* ret_, const argpack_t& args_, std::index_sequence<_Ix...>) {
			if constexpr (base_t::is_proc_) {
				assert(ret_ == nullptr);
				(inst_.*func_)(check_and_make_from_var_ex<std::remove_cvref_t<ParamTypesT>>(args_[_Ix])...);
			}
			else {
				typename base_t::ret_t res = (inst_.*func_)(check_and_make_from_var_ex<std::remove_cvref_t<ParamTypesT>>(args_[_Ix])...);
				_internal::variable_set_value(ret_, std::move(res));
			}
		}
		template<size_t... _Ix>
		static void do_invoke(const class_t& inst_, const_func_t func_, IVariable* ret_, const argpack_t& args_, std::index_sequence<_Ix...>) {
			if constexpr (base_t::is_proc_) {
				assert(ret_ == nullptr);
				(inst_.*func_)(check_and_make_from_var_ex<std::remove_cvref_t<ParamTypesT>>(args_[_Ix])...);
			}
			else {
				typename base_t::ret_t res = (inst_.*func_)(check_and_make_from_var_ex<std::remove_cvref_t<ParamTypesT>>(args_[_Ix])...);
				_internal::variable_set_value(ret_, std::move(res));
			}
		}
	};


	template<typename RetTypeT, typename ... ParamTypesT>
	struct InvokerImpl<void, RetTypeT, ParamTypesT...> : public InvokerImplBase<RetTypeT, ParamTypesT...> {
		using base_t = InvokerImplBase<RetTypeT, ParamTypesT...>;
		using class_t = void;
		using func_t = typename RetTypeT(ParamTypesT...);
		template<size_t... _Ix>
		static void do_invoke(func_t func_, ESB_UNUSED IVariable* ret_, const argpack_t& args_, std::index_sequence<_Ix...>) {
			if constexpr (base_t::is_proc_) {
				assert(ret_ == nullptr);
				func_(check_and_make_from_var_ex<std::remove_cvref_t<ParamTypesT>>(args_[_Ix])...);
			}
			else {
				typename base_t::ret_t res = func_(check_and_make_from_var_ex<std::remove_cvref_t<ParamTypesT>>(args_[_Ix])...);
				_internal::variable_set_value(ret_, std::move(res));
			}
		}
	};




	template<typename RetTypeT, typename ... ParamTypesT>
	struct Invoker<typename RetTypeT(ParamTypesT...)> : public InvokerImpl<void, RetTypeT, ParamTypesT...> {
		using base_t = InvokerImpl<void, RetTypeT, ParamTypesT...>;

		static void invoke_as_meth(base_t::func_t func_, IVariable* ret_, const argpack_t& args_) {
			if (args_.size() != base_t::param_count_)
				ESL_THROW_NOTCTXPARACOUNT();
			return base_t::do_invoke(func_, ret_, args_, std::make_index_sequence<base_t::param_count_>{});
		}
		static void invoke_as_prop_get(base_t::func_t func_, IVariable& retval_) {
			static_assert(base_t::has_retval_ && base_t::param_count_ == 0, "invalid get property value method signature: expected function with no arguments.");
			typename base_t::ret_t res = func_();
			_internal::variable_set_value(retval_, std::move(res));
		}
		static void invoke_as_prop_set(base_t::func_t func_, const IVariable& newval_) {
			static_assert(base_t::is_proc_ && base_t::param_count_ == 1, "invalid set property value method signature: expected procedure with 1 argument.");
			func_(check_and_make< std::remove_cvref_t<ParamTypesT> >(IVariable_GetValue(newval_))...);
		}
	};


	template<typename ClassTypeT, typename RetTypeT, typename ... ParamTypesT>
	struct Invoker<typename RetTypeT(ClassTypeT::*)(ParamTypesT...)> : public InvokerImpl<ClassTypeT, RetTypeT, ParamTypesT...> {
		using base_t = InvokerImpl<ClassTypeT, RetTypeT, ParamTypesT...>;

		static void invoke_as_meth(base_t::class_t& inst_, base_t::func_t func_, IVariable* ret_, const argpack_t& args_) {
			if (args_.size() != base_t::param_count_)
				ESL_THROW_NOTCTXPARACOUNT();
			return base_t::do_invoke(inst_, func_, ret_, args_, std::make_index_sequence<base_t::param_count_>{});
		}
		static void invoke_as_prop_set(base_t::class_t& inst_, base_t::func_t func_, const IVariable& newval_) {
			static_assert(base_t::is_proc_ && base_t::param_count_ == 1, "invalid set property value method signature: expected procedure with 1 argument.");
			(inst_.*func_)(check_and_make< std::remove_cvref_t<ParamTypesT>>(IVariable_GetValue(newval_))...);
		}
	};


	template<typename ClassTypeT, typename RetTypeT, typename ... ParamTypesT>
	struct Invoker<typename RetTypeT(ClassTypeT::*)(ParamTypesT...) const> : public InvokerImpl<ClassTypeT, RetTypeT, ParamTypesT...> {
		using base_t = InvokerImpl<ClassTypeT, RetTypeT, ParamTypesT...>;

		static void invoke_as_meth(const base_t::class_t& inst_, base_t::const_func_t func_, IVariable* ret_, const argpack_t& args_) {
			if (args_.size() != base_t::param_count_)
				ESL_THROW_NOTCTXPARACOUNT();
			return base_t::do_invoke(inst_, func_, ret_, args_, std::make_index_sequence<base_t::param_count_>{});
		}
		static void invoke_as_prop_get(const base_t::class_t& inst_, base_t::const_func_t func_, IVariable& retval_) {
			static_assert(base_t::has_retval_ && base_t::param_count_ == 0, "invalid get property value method signature: expected function with no arguments.");
			//TOBE	после внедрения variable_set_value везде в таком виде - сначала res, а потом мув его. Нужно почиститься.
			//		Также если RetTypeT неподдерживается нужно сделать static_assert с толковым объяснением проблемы (у InvokerImpl разом на всех)
			typename base_t::ret_t res = (inst_.*func_)();
			_internal::variable_set_value(retval_, std::move(res));
		}
	};


	// Invoker для конструкторов. У него специфика т.к. у конструкторов нет адреса и ему надо descriptor_t::CreateValueInstance
	template<typename, typename>
	struct ExtInvoker4CtorFromTypePack;
	template<typename ExtDescriptorT, typename... ParaTypesPackT>
	struct ExtInvoker4CtorFromTypePack<ExtDescriptorT, typepack<ParaTypesPackT...>> {
		using descriptor_t = ExtDescriptorT;
		static constexpr size_t param_count_ = sizeof...(ParaTypesPackT);
		template<size_t... Ix>
		static IValuePtr invoke_as_ctor_impl(const TypeDescriptorValue& td_, const argpack_t& args_, std::index_sequence<Ix...>) {
			return IValuePtr{ descriptor_t::CreateValueInstance(static_cast<const descriptor_t&>(td_),
																		check_and_make_from_var_ex<std::remove_cvref_t<ParaTypesPackT>>(args_[Ix])...) };
		}
		static IValuePtr invoke_as_ctor(const TypeDescriptorValue& td_, const argpack_t& args_) {
			if (args_.size() != param_count_)
				ESL_THROW_NOTCTXPARACOUNT();
			return invoke_as_ctor_impl(td_, args_, std::make_index_sequence<param_count_>{});
		}
	};




	// Вспомогательное. Используется при consteval создании диспатч-массивов методов и свойств.
	// Массивы строятся в порядке возростания имени методов приведенных к регистро-независимому виду.
	// В ран-тайме метод по имени ищется в таком массиве простым бинарным поиском.
	template<std::size_t N>
	consteval void consteval_array_fill_nocase(std::array<strchar_t, N>& arr_, const strchar_t(&str_)[N]) {
		for (size_t i = 0; i < N; ++i) {
			arr_[i] = to_nocase(str_[i]);
		}
	}

	template<std::size_t N>
	consteval std::array<strchar_t, N> consteval_array_make_nocase(const strchar_t(&arr_)[N]) {
		std::array<strchar_t, N> a{};
		consteval_array_fill_nocase(a, arr_);
		return a;
	}


	//TOBE Примитивнейшая сортировка. Возможно в какой-то момент std перестанет глючить на consteval и можно будет убрать этот костыль
	template<class ElemT, size_t NSize, class PredT>
	consteval void consteval_array_sort(std::array<ElemT, NSize>& arr_, PredT better_pred_) {
		for (size_t i = 0; i < NSize; ++i) {
			ElemT* better_elem = &arr_[i];
			size_t	better_elem_index = i;
			for (size_t j = i + 1; j < NSize; j++) {
				if (better_pred_(arr_[j], *better_elem)) {
					better_elem = &arr_[j];
					better_elem_index = j;
				}
			}
			if (better_elem_index != i)
				swap(arr_[i], *better_elem);
		}
	}

	//TODO:	Поскольку все диспатч-АПИ 1С построено на int/unsigned int и в режиме x64 тоже, то целесообразно отказаться от "общности" 
	//		и перенести find в meta привязав к dispid/dispix.
	//		
	//TODO: в esb_meta используется версия от array, а в esbldr просто массив. Нужно доработать esbldr и привязать его к мета.
	//TODO:	Не исключено что нужно сделать "свой" std::array, т.к. реализация от MS (или стандарт вообще) не допускают пустой массив
	//		если у элемента нет конструктора по-умолчанию. Это неудобно и существенно осложняет реализацию dispatch-interface. (см. meta)
	inline constexpr isize_t ARRAY_FIND_BINARY_NOT_FOUND = isize_t(-1);
	//
	template<class ElemT, class ValT, class CompT>
	constexpr isize_t array_find_binary_proc(const ElemT* arr_, isize_t size_, const ValT& val_, CompT comp_, isize_t notfound_ = ARRAY_FIND_BINARY_NOT_FOUND) {
		int i_min = 0;				// int - это очень!!!
		int i_max = size_ - 1;		// ----- !!очень!! важно для алгоритма!!!

		while (i_min <= i_max) {
			unsigned int i_mid = (unsigned int)(i_min + (i_max - i_min) / 2);

			int cmp = comp_(arr_[i_mid], val_);
			if (cmp < 0)
				i_min = (int)i_mid + 1;
			else if (cmp > 0)
				i_max = (int)i_mid - 1;	// i_mid может дойти до 0 и тогда, если i_max unsigned, он становится 4 миллиарда...
			else
				return i_mid; // mid_val == key_. found
		}

		return notfound_;
	}
	template<class ElemT, unsigned int NSize, class ValT, class CompT>
	constexpr dispid_t disparray_find(const ElemT(&arr_)[NSize], const ValT& val_, CompT comp_) {
		isize_t ix = array_find_binary_proc(arr_, NSize, val_, comp_, ARRAY_FIND_BINARY_NOT_FOUND);
		return (ix == ARRAY_FIND_BINARY_NOT_FOUND) ? DISPID_NOTFOUND : as_dispid(ix);

		//return array_find_binary_proc(arr_, NSize, val_, comp_, notfound_);
	}
	template<class ElemT, unsigned int NSize, class ValT, class CompT>
	constexpr dispid_t disparray_find(const std::array<ElemT, NSize>& arr_, const ValT& val_, CompT comp_) {
		isize_t ix = array_find_binary_proc(arr_.data(), NSize, val_, comp_, ARRAY_FIND_BINARY_NOT_FOUND);
		return (ix == ARRAY_FIND_BINARY_NOT_FOUND)? DISPID_NOTFOUND : as_dispid(ix);
	}
	//-------------------------------------------------------------------------------------------------------------------------
	//-------------------------------------------------------------------------------------------------------------------------



	// Структуры MetaInterfaceMeth/MetaInterfaceProp являются собственно структурами собирающими всю мета-информацию о методе 
	// т.е. адрес мтода (указатель на метод (класса)) и имена - русское и английское, и связывают метод с инвокером
	// эти структуры укладываются в tuple, который и представляет мета-интефейс класса
	// Также структура отдает элемент DispInfoMeth(Prop) для построения диспатч-массива.
	//NOTE
	//https://stackoverflow.com/questions/7245404/reference-to-member-function
	//https://stackoverflow.com/questions/21952386/why-doesnt-reference-to-member-exist-in-c
	//в С++ нету понятия ссылки-на-мембер. Исторически.. Поэтому для ExtInterfaceMembMethMeta meth_t всегда pointer и от нуллптр защищаемся ассертом. с++ нам не помогает..


	template<auto MethPtr, size_t NEn, size_t NRu>
	struct MetaInterfaceMeth {
		static_assert(MethPtr != nullptr, "Meth can't be nullptr!!!");

		using						meth_t = decltype(MethPtr);
		using						invoker_t = Invoker< std::remove_pointer_t<meth_t> >;
		static constexpr meth_t		func_ = MethPtr;

		static void invoke(IVariable* retv_, const argpack_t& args_) requires std::is_same_v<invoker_t::class_t, void> {
			return invoker_t::invoke_as_meth(func_, retv_, args_);
		}
		// интерфейс объекта. позволяет привязать к объекту статические (или даже чужие или свободные) методы, "тихо" глотая obj_ 
		static void invoke(ExtValueObjectBase& obj_, IVariable* retv_, const argpack_t& args_) {
			if constexpr (std::is_same_v<invoker_t::class_t, void>)
				return invoker_t::invoke_as_meth(func_, retv_, args_);
			else
				return invoker_t::invoke_as_meth(obj_.GetContainedData<invoker_t::class_t>(), func_, retv_, args_);
		}
		MetaNameData<NEn>			name_;
		MetaNameData<NRu>			alias_;
		constexpr MetaInterfaceMeth(const strchar_t(&sname_)[NEn], const strchar_t(&salias_)[NRu]) : name_{ sname_ }, alias_{ salias_ }
		{}

		// dispinfo DispInfoStatMeth or DispInfoMembMeth 
		// MetaInterfaceMeth представляет как статический (или свободный) метода, так и метод экземпляра объекта 
		// В дисп-интерефейс объекта можно привязать статический\свободный метод - наша реализация при этом тихо глотает obj-instance
		// Но DispInfoStatMeth нельзя создать из member_function_pointer - obj-instance взять просто неоткуда
		// (в принципе и без static_assert ошибка будет, но ее понять тяжело...)
		template<typename DispInfoT> consteval DispInfoT make_dispinfo() const {
			static_assert(std::is_same_v<DispInfoT, DispInfoMembMeth> || std::is_same_v<invoker_t::class_t, void>, "invalid dispinfo request.");
			return /*DispInfoT*/{ {name_, alias_}, invoker_t::make_meth_info(), invoke };
		}
	};

	// При создании объекта MetaInterfaceMeth нам нужно инициализировать поля .code_ у имени и псевдонима.
	// (это нужно для построения эффективного поиска метода по текстовому идентификатору в рантайме)
	// Логично это было бы делать прямо в конструкторе MetaNameData, но компилятор отказывается говоря "попытка доступа к памяти среды исполнения". (MSVC 16.11.19)
	// А в таком виде - создаем внутри consteval функции, заполняем и возвращаем - позволяет
	// (TOBE: Возможно красивее сделать static MetaInterfaceMeth::make())
	template<auto MethPtr, size_t NEn, size_t NRu>
	consteval MetaInterfaceMeth<MethPtr, NEn, NRu> make_MetaInterfaceMeth(const strchar_t(&sen_)[NEn], const strchar_t(&sru_)[NRu]) {
		MetaInterfaceMeth<MethPtr, NEn, NRu> m{ sen_, sru_ };
		consteval_array_fill_nocase(m.name_.code_, sen_);
		consteval_array_fill_nocase(m.alias_.code_, sru_);
		return m;
	}

	// Макросы упрощающие вызов построителя мета-элементов-метод инетрфейса.
	// базовый макрос позволяет иметь разными имя метода и английское (первое) имя для диспатч-интерфейса, но считаю это нежелательным т.к. рисковано нюансами
	// (для простоты реализации макрос ожидает в первом параметре OBJ_ имя вместе с ::	- object_t:: / somewhat_t:: / <пусто>
#define ESB_META_INTERFACE_MAKE_METH(OBJ_, NAME_EN_, NAME_RU_)		make_MetaInterfaceMeth<& OBJ_ NAME_EN_>( ESB_T( #NAME_EN_ ), ESB_T(NAME_RU_) )
// Основные макросы описывающие мета-интерфейс методов объекта строят его для предопределенного в структуре псевданима object_t
#define ESB_META_INTERFACE_MEMB_METH(NAME_EN_, NAME_RU_)		ESB_META_INTERFACE_MAKE_METH(ESB_INTERFACE_OBJECT_FIELD ::, NAME_EN_, NAME_RU_)
#define ESB_META_INTERFACE_STAT_METH(NAME_EN_, NAME_RU_)		ESB_META_INTERFACE_MAKE_METH(ESB_INTERFACE_OBJECT_FIELD ::, NAME_EN_, NAME_RU_)
#define ESB_META_INTERFACE_FREE_METH(NAME_EN_, NAME_RU_)		ESB_META_INTERFACE_MAKE_METH( , NAME_EN_, NAME_RU_)


	template<typename>
	inline constexpr bool is_meta_interface_meth = false;
	template<auto MethPtr, size_t NEn, size_t NRu>
	inline constexpr bool is_meta_interface_meth< MetaInterfaceMeth<MethPtr, NEn, NRu> > = true;

	template<typename T>
	concept MetaInterfaceMethConcept = is_meta_interface_meth<T>;

	//NOTE
	// Если делать этот метод consteval, то утыкаемся в ограничение компилятора (MSVC 16.11.19)
	// fatal error C1054: ограничение компилятора: недопустимая степень вложения инициализаторов. Максимум 10-15. Реально 11.
	// workaround: делать InterfaceMethMeta1_, InterfaceMethMeta2_, ... и
	// InterfaceMethMeta_ = std::tuple_cat(InterfaceMethMeta1_, InterfaceMethMeta2_, ...)
	// Или сделать функцию не consteval, а constexpr
	//TOBE	Также можно подумать над генерацией дисп-интерфейса напрямую минуя стадию мета-интерфейса
	// В конечном итоге нам нужен именно дисп, а мета используется как промежуточное. 
	// Модель мета-интерфейса и дисп-интерфейс из него представляется красивой и очень соблазнительно дотянуть до когда у с++ появится std::meta...
	template<MetaInterfaceMethConcept... MethT>
	constexpr auto make_meta_interface_meth(MethT&&... args_) {
		return std::make_tuple(std::forward<MethT>(args_)...);
	}

	// Макрос упрощающий создание списка методов. Аргументы - макросы ESB_META_INTERFACE_xxx_METH. в конце требует ;
#define ESB_META_INTERFACE_METH(...)	static constexpr auto ESB_INTERFACE_METH_META_FIELD = make_meta_interface_meth (__VA_ARGS__ )


	// Все тоже самое для свойств (с небольшой спецификой). Свойство - это два метода: 
	// чтение - SomewhatType GetSomewhat() const и запись - void SetSomewhat(const SomewhatType& new_value_)
	// Один из методов может отсутствовать что трактуется как только-чтение или только-запись. Оба отсутствовать не могут!
	template<auto MethGetPtr, auto MethSetPtr, size_t NEn, size_t NRu>
	struct MetaInterfaceProp {
		static_assert(MethGetPtr || MethSetPtr, "property must have or get-form, or set-form, or both form. property can't be nullptr for both form!");

		using							meth_get_t = decltype(MethGetPtr);
		static constexpr meth_get_t		func_get_ = MethGetPtr;
		using							invoker_get_t = Invoker< std::remove_pointer_t<meth_get_t> >;
		using							meth_set_t = decltype(MethSetPtr);
		static constexpr meth_set_t		func_set_ = MethSetPtr;
		using							invoker_set_t = Invoker< std::remove_pointer_t<meth_set_t> >;

		// имеем два интерфейса - и для ExtInterfaceStatPropInfo и для ExtInterfaceMembPropInfo
		// интерфейс для статических свойств (не привязаннях к экземпляру объекта). доступен только для инвокеров без объекта
		static void invoke_get(IVariable& retval_)				requires (std::is_same_v<invoker_get_t::class_t, void>) {
			return invoker_get_t::invoke_as_prop_get(func_get_, retval_);
		}
		static void invoke_set(const IVariable& newval_)		requires (std::is_same_v<invoker_set_t::class_t, void>) {
			return invoker_set_t::invoke_as_prop_set(func_set_, newval_);
		}
		// интерфейс для свойств объекта. допускает "тихо" привязать к объекту статические свойства
		static void invoke_get(const ExtValueObjectBase& obj_, IVariable& retval_) {
			if constexpr (std::is_same_v<invoker_get_t::class_t, void>)
				return invoker_get_t::invoke_as_prop_get(func_get_, retval_);
			else
				return invoker_get_t::invoke_as_prop_get(obj_.GetContainedData<invoker_get_t::class_t>(), func_get_, retval_);
		}
		static void invoke_set(ExtValueObjectBase& obj_, const IVariable& newval_) {
			if constexpr (std::is_same_v<invoker_set_t::class_t, void>)
				return invoker_set_t::invoke_as_prop_set(func_set_, newval_);
			else
				return invoker_set_t::invoke_as_prop_set(obj_.GetContainedData<invoker_set_t::class_t>(), func_set_, newval_);
		}

		MetaNameData<NEn>			name_;
		MetaNameData<NRu>			alias_;
		constexpr MetaInterfaceProp(const strchar_t(&sname_)[NEn], const strchar_t(&salias_)[NRu]) noexcept : name_{ sname_ }, alias_{ salias_ }
		{}

		// disp-interface. Объект DispInfoT может быть как DispInfoStatProp так и DispInfoMembProp. Мы обслуживаем оба варианта и нам надо выбрать
		// какая перегрузка invoke_get/invoke_set нужна вызывающему. Для этого эти методы специализируются decltype(DispInfoT::invoke_...) 
		// и компилятор выбирает из двух вариантов требуемый.
		// (пс. с++ не имеет никакого иного механизма выбора конкретного метода из нескольких перегрузок кроме как 
		// попытаться присвоить этот метод переменной (или возвращаемому значению) с типом нужного метода)
		template<typename FnPtrT>	static consteval FnPtrT get_invoke_get() { CONSTEXPR_IIF(func_get_, return &invoke_get, return nullptr); }
		template<typename FnPtrT>	static consteval FnPtrT get_invoke_set() { CONSTEXPR_IIF(func_set_, return &invoke_set, return nullptr); }
		template<typename DispInfoT> consteval DispInfoT make_dispinfo() const {
			return { {name_, alias_}, get_invoke_get<decltype(DispInfoT::invoke_get_)>(), get_invoke_set<decltype(DispInfoT::invoke_set_)>() };
		}
	};
	//
	template<auto MethGet, auto MethSet, size_t NEn, size_t NRu>
	consteval MetaInterfaceProp<MethGet, MethSet, NEn, NRu> make_MetaInterfaceProp(const strchar_t(&sen_)[NEn], const strchar_t(&sru_)[NRu]) {
		MetaInterfaceProp<MethGet, MethSet, NEn, NRu> prop{ sen_, sru_ };
		// не получается делать преобразование в конструкторах объектов :(. Только так - сначала создавать пустые и потом записывать. (MSVC 16.11.19)
		consteval_array_fill_nocase(prop.name_.code_, sen_);
		consteval_array_fill_nocase(prop.alias_.code_, sru_);
		return prop;
	}

	// для задания области видимости OBJ_ нужно передавать в виде OBJ:: (или пусто)
#define ESB_META_INTERFACE_MAKE_PROP_RW(OBJ_, NAME_EN_, NAME_RU_)	make_MetaInterfaceProp< & OBJ_ Get##NAME_EN_, & OBJ_ Set##NAME_EN_>( ESB_T( #NAME_EN_ ), ESB_T(NAME_RU_) )
#define ESB_META_INTERFACE_MAKE_PROP_RO(OBJ_, NAME_EN_, NAME_RU_)	make_MetaInterfaceProp< & OBJ_ Get##NAME_EN_, nullptr>( ESB_T( #NAME_EN_ ), ESB_T(NAME_RU_) )
#define ESB_META_INTERFACE_MAKE_PROP_WO(OBJ_, NAME_EN_, NAME_RU_)	make_MetaInterfaceProp<nullptr, & OBJ_ Set##NAME_EN_>( ESB_T( #NAME_EN_ ), ESB_T(NAME_RU_) )
// для создания элементов интерфейса объекта мы требуем, чтобы в структуре описания интерфейса был определен object_t для которого создается интерфейс
// все элементы интерфейса объекта определеяются для этого object_t
// (TOBE: иначи можно наопределять для разных объектов и что получится неведомо..., неплохо бы сделать защиту получше...)
#define ESB_META_INTERFACE_MEMB_PROP_RW(NAME_EN_, NAME_RU_)		ESB_META_INTERFACE_MAKE_PROP_RW(ESB_INTERFACE_OBJECT_FIELD ::, NAME_EN_, NAME_RU_)
#define ESB_META_INTERFACE_MEMB_PROP_RO(NAME_EN_, NAME_RU_)		ESB_META_INTERFACE_MAKE_PROP_RO(ESB_INTERFACE_OBJECT_FIELD ::, NAME_EN_, NAME_RU_)
#define ESB_META_INTERFACE_MEMB_PROP_WO(NAME_EN_, NAME_RU_)		ESB_META_INTERFACE_MAKE_PROP_WO(ESB_INTERFACE_OBJECT_FIELD ::, NAME_EN_, NAME_RU_)
// для статических элементов тоже относительно object_t, хотя это и не обязательно, но пусть будет для порядка
#define ESB_META_INTERFACE_STAT_PROP_RW(NAME_EN_, NAME_RU_)		ESB_META_INTERFACE_MAKE_PROP_RW(ESB_INTERFACE_OBJECT_FIELD ::, NAME_EN_, NAME_RU_)
#define ESB_META_INTERFACE_STAT_PROP_RO(NAME_EN_, NAME_RU_)		ESB_META_INTERFACE_MAKE_PROP_RO(ESB_INTERFACE_OBJECT_FIELD ::, NAME_EN_, NAME_RU_)
#define ESB_META_INTERFACE_STAT_PROP_WO(NAME_EN_, NAME_RU_)		ESB_META_INTERFACE_MAKE_PROP_WO(ESB_INTERFACE_OBJECT_FIELD ::, NAME_EN_, NAME_RU_)
// для свободных свойств - для любых видимых методов
#define ESB_META_INTERFACE_FREE_PROP_RW(NAME_EN_, NAME_RU_)		ESB_META_INTERFACE_MAKE_PROP_RW(, NAME_EN_, NAME_RU_)
#define ESB_META_INTERFACE_FREE_PROP_RO(NAME_EN_, NAME_RU_)		ESB_META_INTERFACE_MAKE_PROP_RO(, NAME_EN_, NAME_RU_)
#define ESB_META_INTERFACE_FREE_PROP_WO(NAME_EN_, NAME_RU_)		ESB_META_INTERFACE_MAKE_PROP_WO(, NAME_EN_, NAME_RU_)


	template<typename>
	inline constexpr bool is_meta_interface_prop = false;
	template<typename auto MethGet, auto MethSet, size_t NEn, size_t NRu>
	inline constexpr bool is_meta_interface_prop< MetaInterfaceProp<MethGet, MethSet, NEn, NRu> > = true;

	template<typename T>
	concept MetaInterfacePropConcept = is_meta_interface_prop<T>;

	template<MetaInterfacePropConcept... PropT>
	constexpr auto make_meta_interface_prop(PropT&&... args_) {
		return std::make_tuple(std::forward<PropT>(args_)...);
	}

#define ESB_META_INTERFACE_PROP(...)	static constexpr auto ESB_INTERFACE_PROP_META_FIELD = make_meta_interface_prop (__VA_ARGS__ )



	template<typename DescriptorT, typename ParaTypesPackT>
	struct MetaInterfaceCtor {
		using descriptor_t = DescriptorT;
		using para_types_pack_t = ParaTypesPackT;
		using invoker_t = ExtInvoker4CtorFromTypePack< descriptor_t, para_types_pack_t>;
		template<typename DispInfoT> consteval DispInfoCtor make_dispinfo() const {
			static_assert(std::is_same_v<DispInfoT, DispInfoCtor>, "expected DispInfoCtor");
			return /*DispInfoT*/{ invoker_t::param_count_, invoker_t::invoke_as_ctor };
		}
	};

#define ESB_META_CTOR(...)	MetaInterfaceCtor< ESB_INTERFACE_DESCRIPTOR_FIELD, typepack< __VA_ARGS__ > >{}

	template<typename>
	inline constexpr bool is_meta_interface_ctor = false;
	template<typename TD, typename... ParaTypesPackT>
	inline constexpr bool is_meta_interface_ctor<MetaInterfaceCtor<TD, typepack<ParaTypesPackT...>>> = true;
	template<typename T> concept MetaInterfaceCtorConcept = is_meta_interface_ctor<T>;


	template<MetaInterfaceCtorConcept... CtorMetaT>
	constexpr std::tuple<CtorMetaT...> make_meta_interface_ctor(CtorMetaT&&... args_) {
		return std::make_tuple(std::forward<CtorMetaT>(args_)...);
	}
	template<class ExtValueDataT>
	consteval auto make_meta_interface_ctor_value_auto() {
		using descriptor_t = ESB_INTERFACEDEF_DESCRIPTOR_T(ExtValueDataT);
		if constexpr (is_compaitable_cpp_type<ExtValueDataT>)
			//NOTE	Мы здесь декларируем не сам конструктор, а его описание. поэтому что-то типа "const ExtValueDataT&" не нужно
			return make_meta_interface_ctor(MetaInterfaceCtor<descriptor_t, typepack<>>{}, MetaInterfaceCtor<descriptor_t, typepack<ExtValueDataT>>{} );
		else
			return make_meta_interface_ctor(MetaInterfaceCtor<descriptor_t, typepack<>>{});
	}


#define ESB_META_CTOR_INTERFACE(...)					static constexpr auto ESB_INTERFACE_CTOR_META_FIELD = make_meta_interface_ctor (__VA_ARGS__ )
#define ESB_META_CTOR_INTERFACE_VALUE_AUTO(CLASS_T_)	static constexpr auto ESB_INTERFACE_CTOR_META_FIELD = make_meta_interface_ctor_value_auto<CLASS_T_>()



	// это ошибочная реакция студии на { std::get<MetaTupleIx>(meta_).make_dispinfo<DispInfoT>() ... } (см.примечание к ESB_WARN_INIT_WITH_BRACES)
	ESB_WARNING_SUPRESS(ESB_WARN_INIT_WITH_BRACES)
		//
			// DispInfoT - это структура DispInfo(Stat|Memb)(Meth|Prop) и мы делам массив этих структур для диспатч интерфейса из МетаТупл - интерфейса. 
			// Непосредственно с tuple в рантайме работать можно, но неудобно и неэффективно, а массив - очень удобно и очень эффективно
		template<class DispInfoT>
	class disp_interface_array_maker_t {
		template<typename MetaTupleT, size_t... MetaTupleIx>
		static consteval std::array<DispInfoT, std::tuple_size_v<MetaTupleT>> make_impl(const MetaTupleT& meta_, std::index_sequence<MetaTupleIx...>) {
			return { std::get<MetaTupleIx>(meta_).make_dispinfo<DispInfoT>() ... };
		}
	public:
		template<typename MetaTupleT>
		static consteval std::array<DispInfoT, std::tuple_size_v<MetaTupleT>> make(const MetaTupleT& meta_) {
			//TOBE	К сожалениею имплементация std::array<..., 0> у (MSVC 16.11.19) такова, что пустой масив можно сделать только для типов у которых есть
			//		конструктор по-умолчанию. У наших DispInfoT его нет, т.к. нет у DispTerm/DispName. И не уверен, что его нужно туда добавлять т.к. 
			//		у нас станут возможны имена с nullptr, а усложнение конструкторов / дружественных конструкторов боком вылезет на consteval-вычислениях
			//		Можно, конечно, написать и свой типа-аррай который будет допускать такое (достаточно скопипастить мс-аррай и изменить скрытый фейковый элемент типа Т на Т*)
			//NOTE	Сейчас реализация построителя диспатч-массивов просто не допускает попадания сюда. Если нет свойств или методов, то просто нет соответствующего поля
			//		(ESB_INTERFACE_(PROP|METH|CTOR)_META_FIELD) в структуре interface_info_t
			if constexpr (std::tuple_size_v<MetaTupleT> == 0)
				static_assert(always_false_v<DispInfoT>, "DispInfoT array with zero size not supported.");
			//return std::array<DispInfoT, 0>();
			else
				return make_impl(meta_, std::make_index_sequence<std::tuple_size_v<MetaTupleT>>{});
		}
	};
	//
	ESB_WARNING_RESTORE() //ESB_WARN_INIT_WITH_BRACES

		template <typename DispInfoT, typename MetaTupleT>
	consteval std::array<DispInfoT, std::tuple_size_v<MetaTupleT>>  make_disp_interface_array(const MetaTupleT& face_) {
		return disp_interface_array_maker_t<DispInfoT>::make(face_);
	}


	// Массивы DispInfoT создаются точно в таком виде, как они были описаны через ESB_INTERFACE_...
	// Но для эффективного поиска конкретного элемента по его имени нам нужна некоторая вспомогательная структура. 
	// (Некоторый hash-map с consteval конструктором. напомню, что все это делается в компиле-тайм)
	// Возможно есть какое-то хорошее решение у кого-то, а здесь пока сделано относительно просто и относительно эффективно.
	// Просто строится индекс для имен (точнее для их code_) "по-возрастанию" и в рантайме ищется бинарным поиском. Структура простая, сложность логарифм.
	// (ПС. Вы всегда можете сами реализовать идеальный поиск для вашего интерфейса реализовав в нем свой perfect-hash метод ESB_INTERFACE_(PROP|METH)_FIND_FIELD)
	// TOBE	Ввиду того, что затраты на поиск метода по имени являются частью затрат на вызов метода через диспатч интерфейс, то совершенствовать этот алгоритм
	//		все таки целесообразно. (Как минимум разбить на "букеты" по длине строк. Бинарно искать букет с нужной длиной. Внутри букета уже искать по строке и 
	//		тоже бинарн и с учетом того, что при каждом шаге часть строки уже проверена, т.п.)
	//		Но не считаю это первоочередной задачей т.к. общие затраты на диспатч интерфейс и специфические типы данных 1С велики сами по себе и использовать
	//		есб для активного обмена вызовами 1С<->ЕСБ нецелесообразно само по себе. Целесообразно получить данный от 1С, преобразовать их в с++ вид, много/хорошо/быстро
	//		обработать и вернуть в 1С результат. При таком использовании диспатч интерфейса это решение вполне приемлемо.
	struct DispNameIndex {
		const DispName* name_;
		dispix_t		index_;
		// выносим в окружающий namespace метод swap, чтобы работали алгоритмы сортировки опирающиеся на наличие такого метода
		friend consteval void swap(DispNameIndex& a, DispNameIndex& b) noexcept {
			return std::swap(a, b);
		}
	};

	constexpr int cstr_compare_as_is(const strchar_t* ps1_, const strchar_t* ps2_) {
		return afx_ustring::cstr_compare(afx_ustring::CStrIterator(ps1_), afx_ustring::CStrIterator(ps2_));
	}

	// При построении поискового индекса имен учитываем, что у каждого элемента DispTerm::names_count_ имен и каждое из них ссылается на один и тот же метод
	template<class DispInfoT, size_t NItems>
	consteval std::array<DispNameIndex, NItems* DispTerm::names_count_> make_disp_names_index(const std::array<DispInfoT, NItems>& info_) {
		// Считаю целесообразным использовать концепты когда возможен выбор того или иного варианта или специализации структуры или функции
		// а когда вариант в сущности один, но нужно провериться то лучше использовать ассерт т.к. он сразу прерывает компиляцию и можно описать ситуацию
		// (с неудовлетворенным концептом компилятор просто отбрасывает эту перегрузку и начинает "пахать поле непаханное" дальнейших ошибок)
		static_assert(is_dispinfo_item<DispInfoT>, "DispInfoT not a dispatch array element. Check your code.");
		// объявляем
		std::array<DispNameIndex, NItems* DispTerm::names_count_> names{};
		// заполняем массив учитывая DispTerm::names_count_==2 (можно, конечно и цикл сделать)
		static_assert(DispTerm::names_count_ == 2, "expected DispTerm::names_count_==2");
		for (dispix_t i = 0; i < NItems; ++i) {
			names[i * 2 + 0].name_ = &info_[i].term_.names_[0];		names[i * 2 + 0].index_ = i;
			names[i * 2 + 1].name_ = &info_[i].term_.names_[1];		names[i * 2 + 1].index_ = i;
		}

		//NOTE	consteval в конце объявления лямбды - он относится к методу () и без него лямбда в consteval не работает.
		constexpr auto lesser = [](const DispNameIndex& a, const DispNameIndex& b) consteval -> bool {
			return ((cstr_compare(a.name_->code_, b.name_->code_) < 0));		// сравниваем "как есть" т.к. поле .code уже преобразрвано к no-case
		};
		// сортируем
		consteval_array_sort(names, lesser);
		// отдаем
		return names;
	}


	// Этот компаратор работает в рантайме, когда к нам приходит имя в неизвестно-case-виде. С учетом, что у нас для имени уже сформировано nocase .code_
	struct DispNameComparatorNoCaseXxCase {
		constexpr int operator()(const DispNameIndex& a, const strview_t& b_xxcase_) const {
			return afx_ustring::cstr_compare(a.name_->code_, afx_ustring::CStrIteratorNoCase(b_xxcase_.data()));
		}
	};

	//TOBE	Ну абсолютно одинаковы, но только при материализации типа инициализаруют себя от разных полей. Поскольку они используются как пюре-статик-констекспр структуры 
	//		(т.е. инстансы их не создаются), то и передать требуемое поле в конструктор нельзя т.к. конструкторов у них нет. Можно пробовать через auto параметр шаблона...
	//		Их всего две. Пусть так будет.. Появится super-puper-perfect-consteval-hash-map все перепишем.
	template<class ExtObjectT>
	struct DispFinderMeth {
		static constexpr auto elem_index_ = make_disp_names_index(ExtObjectT::ESB_INTERFACE_METH_INFO_FIELD);
		static constexpr dispid_t find(const strview_t& name_) {
			dispid_t did = disparray_find(elem_index_, name_.data(), DispNameComparatorNoCaseXxCase{});
			return (did == DISPID_NOTFOUND)? DISPID_NOTFOUND : as_dispid(elem_index_[as_dispix(did)].index_);
		}
	};
	template<class ExtObjectT>
	struct DispFinderProp {
		static constexpr auto elem_index_ = make_disp_names_index(ExtObjectT::ESB_INTERFACE_PROP_INFO_FIELD);
		static constexpr dispid_t find(const strview_t& name_) {
			dispid_t did = disparray_find(elem_index_, name_.data(), DispNameComparatorNoCaseXxCase{});
			return (did == DISPID_NOTFOUND)? DISPID_NOTFOUND : as_dispid(elem_index_[as_dispix(did)].index_);
		}
	};
	template<class ExtInterfaceT>
	struct DispFinderCtor {
		static constexpr ExtInstanceCreatorFn* find(size_t cargs_) {
			// используем примитивный поиск по порядку т.к. массив гарантировано (check_interface_ctor) сделан по-возрастанию, не содержит дублей и ожидается маленьким
			for (const DispInfoCtor& c : ExtInterfaceT::ESB_INTERFACE_CTOR_INFO_FIELD) {
				if (c.param_count_ == cargs_)
					return c.func_;
			}
			return nullptr;
		}
	};



	//----------------------------------------------------------------------------------------------------------------------------------------
	// Типовая имплементация дисп-интерфейса на основе созданный в структуре DispInterfaceT полей ESB_INTERFACE_(PROP|METH|CTOR)_INFO_FIELD
	// Имплементация каждой функции по-отдельности т.к. мы просто "предоставляем сервисы" тому, кто реализует дисп-интерфейс. 
	// собственно реализация может быть разной. К примеру если не дать функцию count, то с интерфейсом работать можно, но "енумеровать" его будет нельзя.
	// Типовая имплементация учитывает, что какие-то категории интерфейса (скажем свойства) могут отсутствовать.
	// Также учитывает, что пользователь может предоставить свою функцию find.
	// Обращаю внимание, что объект DispInterfaceT относительно которого реализуются методы диспатч-интерфейса не обязательно должен быть специализацией
	// interface_info_t. Он может быть любым классов, к примеру самим классом для которого реалиуется интерфейс. Главное чтобы у этого объекта
	// были необходимые "магические" поля ESB_INTERFACE_(PROP|METH|CTOR)_INFO_FIELD
	// 
	// Проверки корректности dispid-индекса сделаны просто assert, т.к. надеемся что платформа будет нам давать корректные dispid (если мы сами все корректно сделаем)
	// (пс. эти методы не вкомпилируются в esbhlp поэтому просто assert достаточно)
	// Вариантами могут быть выброс исключения или возврат nullptr. Напомню, что собственно реализация 1С-диспатч для 1С интерфейсов находится в esbhlp
	// и там вариант возврата нуллптр корректно обрабатывается.
#define DISPNTERFACE_CHECK_DISPID_INDEX(DISPID_, SIZE_)		assert(  is_valid_dispid_index( (DISPID_), (SIZE_) )  );	

	// Имплементация для свойства дисп-интерфейса
	template<class DispInterfaceT>
	constexpr size_t dispinterface_impl_prop_count() {
		if constexpr (is_interface_has_prop_info<DispInterfaceT>)
			return std::size(DispInterfaceT::ESB_INTERFACE_PROP_INFO_FIELD);
		else
			return 0;
	}
	template<class DispInterfaceT>
	constexpr dispid_t dispinterface_impl_prop_find(const strview_t& name_) {
		if constexpr (is_interface_has_prop_info_find<DispInterfaceT>)
			return DispInterfaceT::ESB_INTERFACE_PROP_FIND_FIELD(name_);
		else if constexpr (is_interface_has_prop_info<DispInterfaceT>)
			return DispFinderProp<DispInterfaceT>::find(name_);
		else
			return DISPID_NOTFOUND;
	}
	template<class DispInterfaceT>
	constexpr const DispInfoMembProp* dispinterface_impl_memb_prop(dispid_t prop_) {
		if constexpr (is_interface_has_prop_info<DispInterfaceT>) {
			DISPNTERFACE_CHECK_DISPID_INDEX(prop_, std::size(DispInterfaceT::ESB_INTERFACE_PROP_INFO_FIELD));
			return  &DispInterfaceT::ESB_INTERFACE_PROP_INFO_FIELD[as_dispix(prop_)];
		}
		else
			return nullptr;
	}
	template<class DispInterfaceT>
	constexpr const DispInfoStatProp* dispinterface_impl_stat_prop(dispid_t prop_) {
		if constexpr (is_interface_has_prop_info<DispInterfaceT>) {
			DISPNTERFACE_CHECK_DISPID_INDEX(prop_, std::size(DispInterfaceT::ESB_INTERFACE_PROP_INFO_FIELD));
			return  &DispInterfaceT::ESB_INTERFACE_PROP_INFO_FIELD[as_dispix(prop_)];
		}
		else
			return nullptr;
	}

	// Имплементация для методов дисп-интерфейса
	template<class DispInterfaceT>
	constexpr size_t dispinterface_impl_meth_count() {
		if constexpr (is_interface_has_meth_info<DispInterfaceT>)
			return std::size(DispInterfaceT::ESB_INTERFACE_METH_INFO_FIELD);
		else
			return 0;
	}
	template<class DispInterfaceT>
	constexpr dispid_t dispinterface_impl_meth_find(const strview_t& name_) {
		if constexpr (is_interface_has_meth_info_find<DispInterfaceT>)
			return DispInterfaceT::ESB_INTERFACE_METH_FIND_FIELD(name_);
		else if constexpr (is_interface_has_meth_info<DispInterfaceT>)
			return DispFinderMeth<DispInterfaceT>::find(name_);
		else
			return DISPID_NOTFOUND;
	}
	template<class DispInterfaceT>
	constexpr const DispInfoMembMeth* dispinterface_impl_memb_meth(dispid_t meth_) {
		if constexpr (is_interface_has_meth_info<DispInterfaceT>) {
			DISPNTERFACE_CHECK_DISPID_INDEX(meth_, std::size(DispInterfaceT::ESB_INTERFACE_METH_INFO_FIELD));
			return  &DispInterfaceT::ESB_INTERFACE_METH_INFO_FIELD[as_dispid(meth_)];
		}
		else
			return nullptr;
	}
	template<class DispInterfaceT>
	constexpr const DispInfoStatMeth* dispinterface_impl_stat_meth(dispid_t meth_) {
		if constexpr (is_interface_has_meth_info<DispInterfaceT>) {
			DISPNTERFACE_CHECK_DISPID_INDEX(meth_, std::size(DispInterfaceT::ESB_INTERFACE_METH_INFO_FIELD));
			return  &DispInterfaceT::ESB_INTERFACE_METH_INFO_FIELD[as_dispid(meth_)];
		}
		else
			return nullptr;
	}

	// Имплементация для вызова конструкторов объекта через дисп-интерфейса
	template<class DispInterfaceT>
	consteval ExtInstanceCreatorFindFn* dispinterface_impl_ctor_find() {
		if constexpr (is_interface_has_ctor_info_find<DispInterfaceT>)
			return DispInterfaceT::ESB_INTERFACE_CTOR_FIND_FIELD;
		else if constexpr (is_interface_has_ctor_info<DispInterfaceT>)
			return DispFinderCtor<DispInterfaceT>::find;
		else
			return nullptr;
	}
#undef DISPNTERFACE_CHECK_DISPID_INDEX




	//---------------------------------------------------------------------------------------------------------
	// Собственно реализация структуры авто-диспатч-интерфейса на основе полей мета-интерфейса
	// NOTE Поскольку у (MSVC 16.11.19) нельзя делать std::array<T,0> нулевой длины если у T нет конструктора по-умолчанию, то
	//		мы вынуждены просто не делать те части дисп-интерфейса которые отсутствуют (см. примечание к make_disp_interface_array)
	//		иначе можно было-бы делать массивы нулевой длины и было-бы проще
	template<typename InterfaceT, class DispInfoT, bool NeedDispInfo>
	struct dispinterface_part_meth;
	template<typename InterfaceT, class DispInfoT>
	struct dispinterface_part_meth<InterfaceT, DispInfoT, /*NeedDispInfo=*/ false> {};
	template<typename InterfaceT, class DispInfoT>
	struct dispinterface_part_meth<InterfaceT, DispInfoT, /*NeedDispInfo=*/ true> {
		static constexpr auto ESB_INTERFACE_METH_INFO_FIELD = make_disp_interface_array<DispInfoT>(InterfaceT::ESB_INTERFACE_METH_META_FIELD);
	};

	template<typename InterfaceT, class DispInfoT, bool NeedDispInfo>
	struct dispinterface_part_prop;
	template<typename InterfaceT, class DispInfoT>
	struct dispinterface_part_prop<InterfaceT, DispInfoT, /*NeedDispInfo=*/ false> {};
	template<typename InterfaceT, class DispInfoT>
	struct dispinterface_part_prop<InterfaceT, DispInfoT, /*NeedDispInfo=*/ true> {
		static constexpr auto ESB_INTERFACE_PROP_INFO_FIELD = make_disp_interface_array<DispInfoT>(InterfaceT::ESB_INTERFACE_PROP_META_FIELD);
	};

	template<typename InterfaceT, bool NeedDispInfo>
	struct dispinterface_part_ctor;
	template<typename InterfaceT>
	struct dispinterface_part_ctor<InterfaceT, /*NeedDispInfo=*/ false> {};
	template<typename InterfaceT>
	struct dispinterface_part_ctor<InterfaceT, /*NeedDispInfo=*/ true> {
		static constexpr auto ESB_INTERFACE_CTOR_INFO_FIELD = make_disp_interface_array<DispInfoCtor>(InterfaceT::ESB_INTERFACE_CTOR_META_FIELD);
	};

	template<typename InterfaceT, class DispInfoMethT, class DispInfoPropT>
	struct dispinterface_base
		: dispinterface_part_meth<InterfaceT, DispInfoMethT, is_interface_has_meth_meta<InterfaceT> && !is_interface_has_meth_info<InterfaceT>>,
		dispinterface_part_prop<InterfaceT, DispInfoPropT, is_interface_has_prop_meta<InterfaceT> && !is_interface_has_prop_info<InterfaceT>>
	{};


	// возможны два варианта дисп-интерфейса - для объекта и статический который к экземпляру объекту не привязан. 
	// (разница в структурах DispInfoMembMeth, DispInfoMembProp. отличаются полями DispInvoke(Memb|Stat)MethFn) invoke - 
	//	в одном случае экземпляр класса нужен, а в другом нет)
	// у дисп-интерфейса для объекта также возможен интерфейс для создания экземпляра. у статик создавать нечего по-определению.
	// Также мы наследуемся от самого MetaInterfaceT т.к. там могут быть определены какие-то части интерфейса другим способом.
	// Мы просто "дополняем" интерфейс недостающими частями.
	template<typename MetaInterfaceT>
	struct dispinterface_object_t : MetaInterfaceT,
		dispinterface_base<MetaInterfaceT, DispInfoMembMeth, DispInfoMembProp>,
		dispinterface_part_ctor<MetaInterfaceT, is_interface_has_ctor_meta<MetaInterfaceT> && !is_interface_has_ctor_info<MetaInterfaceT>>
	{};

	// статический дисп-инетрфейс используется для esb AddinObject т.к. Instance_ у него статическая одна на всю dll.
	template<typename MetaInterfaceT>
	struct dispinterface_static_t : MetaInterfaceT, dispinterface_base<MetaInterfaceT, DispInfoStatMeth, DispInfoStatProp>
	{};
	//---------------------------------------------------------------------------------------------------------



	//---------------------------------------------------------------------------------------------------------
	//
	template<typename InterfaceT>
	constexpr AddinDescriptor::AddinDescriptor(std::type_identity<InterfaceT>) : AddinDescriptor(InterfaceT::TypeId_, InterfaceT::TypeTerm_, InterfaceT::TypeDescriptionInit_,
		dispinterface_impl_prop_count<InterfaceT>, dispinterface_impl_prop_find<InterfaceT>, dispinterface_impl_stat_prop<InterfaceT>,
		dispinterface_impl_meth_count<InterfaceT>, dispinterface_impl_meth_find<InterfaceT>, dispinterface_impl_stat_meth<InterfaceT>)
	{}
	//---------------------------------------------------------------------------------------------------------



	//---------------------------------------------------------------------------------------------------------
	//проверка корректности дисп-интерфейса
	template<class ExtObjectT>
	consteval bool dispinterface_check_prop() {
		size_t elem_count = std::size(ExtObjectT::ESB_INTERFACE_PROP_INFO_FIELD);
		if constexpr (is_interface_has_prop_info_find<ExtObjectT>) {
			for (size_t i_elem = 0; i_elem < elem_count; ++i_elem) {
				for (size_t i_lang = 0; i_lang < ExtObjectT::ESB_INTERFACE_PROP_INFO_FIELD[(unsigned)i_elem].names_count(); ++i_lang) {
					if (ExtObjectT::ESB_INTERFACE_PROP_FIND_FIELD(ExtObjectT::ESB_INTERFACE_PROP_INFO_FIELD[(unsigned)i_elem].name_text(i_lang)) != i_elem) return false;
				}
			}
			if (ExtObjectT::ESB_INTERFACE_PROP_FIND_FIELD(ESB_T("")) != ARRAY_FIND_BINARY_NOT_FOUND) return false;
		}

		using face_elem_map_t = DispFinderProp<ExtObjectT>;
		for (int i_elem = 0; (unsigned)i_elem < elem_count; ++i_elem) {
			for (size_t i_lang = 0; i_lang < ExtObjectT::ESB_INTERFACE_PROP_INFO_FIELD[(unsigned)i_elem].term_.names_count(); ++i_lang) {
				if (face_elem_map_t::find(ExtObjectT::ESB_INTERFACE_PROP_INFO_FIELD[(unsigned)i_elem].term_.name_text(i_lang)) != i_elem) return false;
			}
		}
		if (face_elem_map_t::find(ESB_T("")) != ARRAY_FIND_BINARY_NOT_FOUND) return false;

		return true;
	}

	template<class ExtObjectT>
	consteval bool dispinterface_check_meth() {
		size_t meth_count = std::size(ExtObjectT::ESB_INTERFACE_METH_INFO_FIELD);
		if constexpr (is_interface_has_meth_info_find<ExtObjectT>) {
			for (size_t i_meth = 0; i_meth < meth_count; ++i_meth) {
				for (size_t i_lang = 0; i_lang < ExtObjectT::ESB_INTERFACE_METH_INFO_FIELD[i_meth].term_.names_count(); ++i_lang) {
					if (ExtObjectT::ESB_INTERFACE_METH_FIND_FIELD(ExtObjectT::ESB_INTERFACE_METH_INFO_FIELD[i_meth].term_.name_text(i_lang)) != i_meth) return false;
				}
			}
			if (ExtObjectT::ESB_INTERFACE_METH_FIND_FIELD(ESB_T("")) != ARRAY_FIND_BINARY_NOT_FOUND) return false;
		}

		using face_meth_map_t = DispFinderMeth<ExtObjectT>;
		for (int i_meth = 0; (unsigned)i_meth < meth_count; ++i_meth) {
			for (size_t i_lang = 0; i_lang < ExtObjectT::ESB_INTERFACE_METH_INFO_FIELD[(unsigned)i_meth].term_.names_count(); ++i_lang) {
				if (face_meth_map_t::find(ExtObjectT::ESB_INTERFACE_METH_INFO_FIELD[(unsigned)i_meth].term_.name_text(i_lang)) != i_meth) return false;
			}
		}
		if (face_meth_map_t::find(ESB_T("")) != ARRAY_FIND_BINARY_NOT_FOUND) return false;

		return true;
	}


	// simple. not empty, sorted and unique
	consteval bool check_interface_ctor_meta_impl_is_ok(const std::initializer_list<size_t>& arr_) {
		if (arr_.size() == 0)
			return false;

		const size_t* pn = arr_.begin();
		size_t n_prev = *pn;
		while (++pn != arr_.end()) {
			size_t n_this = *pn;
			if (n_this <= n_prev)
				return false;
			n_prev = n_this;
		}

		return true;
	}
	template<typename MetaTupleT, size_t... MetaTupleIx>
	consteval bool check_interface_ctor_meta_impl(const MetaTupleT& meta_, std::index_sequence<MetaTupleIx...>) {
		return check_interface_ctor_meta_impl_is_ok({ std::tuple_element_t<MetaTupleIx, MetaTupleT>::para_types_pack_t::size() ... });
	}
	template<typename MetaTupleT>
	consteval bool check_interface_ctor_meta(const MetaTupleT& meta_) {
		return check_interface_ctor_meat_impl(meta_, std::make_index_sequence<std::tuple_size_v<MetaTupleT>>{});
	}
	template<class ExtObjectT>
	consteval bool check_interface_ctor_meta() {
		return check_interface_ctor_meta(ExtObjectT::ESB_INTERFACE_CTOR_META_FIELD);
	}

#define CONSTEVAL_STATIC_ASSERT(c, msg) do { if (!(c)) throw msg; } while(false)


	template<class ExtObjectT>
	consteval auto dispinterface_check_ctor() {
		size_t count = std::size(ExtObjectT::ESB_INTERFACE_CTOR_INFO_FIELD);
		if (count == 0)
			return false;

		using finder_t = DispFinderCtor<ExtObjectT>;
		int i_para_count_prev = -1;
		for (size_t i = 0; i < count; ++i) {
			int						c_para = (signed)ExtObjectT::ESB_INTERFACE_CTOR_INFO_FIELD[i].param_count_;

			ExtInstanceCreatorFn*	c_func = ExtObjectT::ESB_INTERFACE_CTOR_INFO_FIELD[i].func_;
			ExtInstanceCreatorFn*	p_func = finder_t::find(c_para);
			//TOBE	Нужно ждать constexpr static_assert или throw "compile-msg" или еще что. пока так. static_message сделать не получается.
			if (c_para < i_para_count_prev)
				return false;				//static_assert(always_false_v<ExtObjectT>, "dispinfo for ctor not sorted by param_count_.");
			if (c_para == i_para_count_prev)
				return false;				//static_assert(always_false_v<ExtObjectT>, "dispinfo for ctor not unique by param_count_.");
			if (c_func == nullptr)
				return false;				//static_assert(always_false_v<ExtObjectT>, "dispinfo for ctor func_ not proper initialized.");
			if (p_func == nullptr)
				return false;				//static_assert(always_false_v<ExtObjectT>, "dispinfo find ctor func_ by param_count_ failed.");
			if (c_func != p_func)
				return false;				//static_assert(always_false_v<ExtObjectT>, "dispinfo find ctor func_ by param_count_ not working properly.");

			i_para_count_prev = c_para;
		}
		return true;
	}


	template<class ExtObjectT>
	consteval bool dispinterface_check() {
		constexpr bool b_meth = is_interface_has_meth_meta<ExtObjectT>;
		if constexpr (b_meth && !dispinterface_check_meth<ExtObjectT>())
			static_assert(always_false_v<ExtObjectT>, "interface for meth invalid.");

		constexpr bool b_prop = is_interface_has_prop_meta<ExtObjectT>;
		if constexpr (b_prop && !dispinterface_check_prop<ExtObjectT>())
			static_assert(always_false_v<ExtObjectT>, "interface for prop invalid.");
		
		if constexpr (is_interface_has_ctor_meta<ExtObjectT>) {
			static_assert(is_interface_has_ctor_info<ExtObjectT>, "meta-info for ctor present, but disp-info absent.");
			static_assert(dispinterface_check_ctor<ExtObjectT>(), "dispinterface for ctor invalid.");
		}
			
		if constexpr(is_type_descriptor_object<ExtObjectT::ESB_INTERFACE_DESCRIPTOR_FIELD>)
			static_assert(b_meth || b_prop, "object interface empty.");
		//
		return true;
	}


} //namespace esb // esb-meta


#endif //ESB_META_H