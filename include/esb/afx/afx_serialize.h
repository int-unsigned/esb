#pragma once
#ifndef AFX_SERIALIZE_H_
#define AFX_SERIALIZE_H_

#include <tuple>
#include <fstream>
#include <filesystem>

#include "afx_base.h"
#include "afx_meta_fields.h"




template<typename T>
inline constexpr bool is_sized_container = requires(T & v) {
	v.size();
	requires is_some_size_t<decltype(v.size())>;
	v.data();
	requires std::is_pointer_v<decltype(v.data())>;
	requires is_constructable_from<T, decltype(v.data()), decltype(v.size())>;
};

//валидно только для is_sized_container
template<typename ContainerType>
using sized_container_data_t = decltype(std::declval<ContainerType>().data());

//валидно только для is_sized_container
template<typename ContainerType>
using sized_container_size_t = decltype(std::declval<ContainerType>().size());
//static_assert(std::is_same_v< sized_container_size_t<DispInfoArray>, DispSizeType >);
static_assert(std::is_same_v< sized_container_size_t<std::string_view>, size_t >);


//созданием/восстановлением контейнера может заниматься другои тип
//(по умолчанию сам контейнер)
template<typename ContainerType>
struct sized_container_creator_type {
	using type = ContainerType;
};
template<typename ContainerType>
using sized_container_creator_t = sized_container_creator_type<ContainerType>::type;




template<typename T>
inline constexpr bool is_depends_from_allocator = requires(T & v) {
	v.get_allocator();
};


template<typename T>
inline constexpr bool is_sized_container_use_alloc = (is_sized_container<T> && is_depends_from_allocator<T>);
template<typename T>
inline constexpr bool is_sized_container_non_alloc = (is_sized_container<T> && !is_depends_from_allocator<T>);
//
static_assert(is_sized_container_use_alloc<std::string>);
static_assert(is_sized_container_non_alloc<std::string_view>);


template<typename T>
inline constexpr bool is_container_c_ctr = requires(T & v) {
	v.c_str();
	requires std::is_pointer_v<decltype(v.c_str())>;
	requires is_some_char_t< std::remove_pointer_t<decltype(v.c_str())> >;
};
static_assert(is_container_c_ctr<std::string> == true);
static_assert(is_container_c_ctr<std::string_view> == false);	//у string_view нету c_str()



template<typename T>
inline constexpr bool is_serializable_binary_intrisic = std::is_arithmetic_v<T> || std::is_enum_v<T>;		//is_integral_v<_Ty> || is_floating_point_v<_Ty>
//
template<typename T, bool = has_meta_fields<T>>
struct is_serializable_binary_type;
//
//обычные типы у которых нет мета-инфо
template<typename T>
struct is_serializable_binary_type<T, false> {
	static constexpr bool value = is_serializable_binary_intrisic<T>;
};
//тупл составленный из неизвестных типов
template<typename... Ts>
struct is_serializable_binary_type< std::tuple<Ts...>, false> {
	static constexpr bool value = ((is_serializable_binary_type<Ts>::value && ...));;
};
//тип для которого есть мета-инфо
template<typename T>
struct is_serializable_binary_type<T, true> {
	static constexpr bool value = is_serializable_binary_type<typename meta_fields<T>::data_tuple_t, false>::value;
};
//
//тип может быть объявлен бинарно-сериализуемым "explicit" путем специализации этой переменной
//для случая когда определение этого через описание полей типа невозможно - битовые поля, пакованная структура т.п.)
//ПОЛНОСТЬЮ НА СОВЕСТИ ВЫЗЫВАЮЩЕГО!
//template<typename>
//inline constexpr bool is_serializable_binary_explicit = false;
//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^ определено в afx_base.h
template<typename T>
inline constexpr bool is_serializable_binary = is_serializable_binary_explicit<T> || is_serializable_binary_type<T>::value;



#ifndef NDEBUG
namespace _test_is_serializable_binary 
{
	static_assert(is_serializable_binary<int>);
	static_assert(!is_serializable_binary<int*>);

	struct BStru {
		int		i;
		char	x;
	};
	META_DESCRIBE_TYPE2(BStru, i, x);
	static_assert(is_serializable_binary<BStru>);

	struct AStru {
		bool	b;
		BStru	s;
	};
	META_DESCRIBE_TYPE2(AStru, b, s);
	static_assert(is_serializable_binary<AStru>);

	struct XStru {
		AStru	a;
		BStru* p;
	};
	META_DESCRIBE_TYPE2(XStru, a, p);
	static_assert(!is_serializable_binary<XStru>);
}
#endif // !DEBUG



struct XStorageInStream {
	using pointer_flag_t = unsigned int;

	struct Storer
	{
		std::basic_ofstream<std::byte> storage_;
		//
		template<typename T> requires (is_serializable_binary<T>)
			void store_raw(const T& data_, size_t count_) {
			assert(count_ > 0);
			storage_.write(reinterpret_cast<const std::byte*>(&data_), count_ * sizeof(T));
		}
		template<typename T> requires (is_serializable_binary<T>)
			void store_raw(const T& data_) {
			store_raw(data_, 1);
		}

		template<typename T>
		inline void store_pointer_flag(const T* pointer_) {
			if (pointer_)
				store_raw<pointer_flag_t>(sizeof(T));	//flag "pointer present"
			else
				store_raw<pointer_flag_t>(static_cast<pointer_flag_t>(0));
		}
		template<typename T>
		void store_pointer(const T* data_) {
			store_pointer_flag(data_);
			if (data_)
				Store(*data_);
		}
		template<typename ContainerSizeT, typename ContainerElemT>
		void store_container_data(ContainerSizeT size_, const ContainerElemT* data_) {
			assert(data_);
			for (size_t i = 0; i < size_; ++i)
				Store(data_[i]);
		}
		template<typename ContainerSizeT, typename ContainerElemT>
		requires (is_serializable_binary<ContainerElemT>)
			void store_container_data(ContainerSizeT size_, const ContainerElemT* data_) {
			//мы здесь не делаем разницы для контейнера c_str и конечный 0 не сохраняем
			//но при загрузке мы это учитываем и один символ добавляем и 0-терминируем строку
			assert(data_);
			store_raw(*data_, size_);
		}
		template<typename ContainerOfT>
		void store_container(const ContainerOfT& container_) {
			auto size = container_.size();
			if constexpr (is_container_c_ctr<ContainerOfT>) {
				// у с_стр контейнера size это кол-во символов без учета завершающего 0
				using c_str_data_t = sized_container_data_t<ContainerOfT>;
				using c_str_size_t = sized_container_size_t<ContainerOfT>;
				static_assert(std::is_pointer_v<c_str_data_t>);
				using c_str_char_t = std::remove_pointer_t<c_str_data_t>;
				const c_str_char_t* data_ptr = container_.data();
				if (size == 0) {
					if(data_ptr)
						store_raw(static_cast<c_str_size_t>(1));
					else
						store_raw(static_cast<c_str_size_t>(0));
				}
				else {
					assert(data_ptr);
					store_raw(static_cast<c_str_size_t>(size + 1));
					store_raw(*data_ptr, size);
				}
			}
			else {
				store_raw(size);
				if (size)
					store_container_data(size, container_.data());
			}
		}
		template<typename T, typename FieldsMetaT, size_t... FieldIndex>
		void store_user_impl(const T& data_, const FieldsMetaT& meta_, std::index_sequence<FieldIndex...>) {
			(Store(data_.*std::get<FieldIndex>(meta_)), ...);
		}
		template<typename UserType, typename UserTypeFildsMeta>
		void store_user(const UserType& data_, const UserTypeFildsMeta& meta_fields_) {
			store_user_impl(data_, meta_fields_, std::make_index_sequence< std::tuple_size_v<UserTypeFildsMeta> >{});
		}
		//
		//
		template<typename T>
		requires (!std::is_pointer_v<T>)
		void Store(const T& data_) {
			//T может быть интегралом , может быть указателем на*, может быть массивом (sized-sequence of*)
			if constexpr (is_sized_container<T>)
				store_container(data_);
			else if constexpr (has_meta_fields<T>)
				store_user(data_, meta_fields<T>::fields_);
			else {
				static_assert(is_serializable_binary<T>, "must be is_serializable_binary (fundamental or describe user data)!");
				store_raw(data_);
			}
		}
		template<typename T>
		void Store(const T* data_) {
			store_pointer(data_);
		}
	}; //Storer
	//
	//
	template<class AllocatorT>
	struct Restorer {
		AllocatorT&						allocator_;
		std::basic_ifstream<std::byte>	storage_;
		//
		template<typename T>
		void* allocate(size_t count_ = 1) {
			return allocator_.allocate<T>(count_);
		}
		//
		template<typename T> requires (is_serializable_binary<T>)
			void restore_raw(T& data_, size_t count_) {
			assert(count_ > 0);
			storage_.read(reinterpret_cast<std::byte*>(&data_), count_ * sizeof(T));
		}
		template<typename T> requires (is_serializable_binary<T>)
			void restore_raw(T& data_) {
			restore_raw(data_, 1);
		}
		template<typename T> requires (is_serializable_binary<T>)
		T restore_raw_value() {
			T value{};
			restore_raw(value);
			return value;
		}
		template<typename T> requires (is_serializable_binary<T>)
		T* restore_raw_allocate(size_t alloc_count_, size_t restore_count_) {
			assert(alloc_count_ > 0);
			assert(alloc_count_ >= restore_count_);
			void* buffer = allocate<T>(alloc_count_);
			if(restore_count_)
				storage_.read(reinterpret_cast<std::byte*>(buffer), restore_count_ * sizeof(T));
			return reinterpret_cast<T*>(buffer);
		}
		//
		template<typename T>
		bool restore_pointer_flag_for() {
			pointer_flag_t flag = restore_raw_value<pointer_flag_t>();
			//todo maybe check flag for T here
			return (flag) ? true : false;
		}
		template<typename T>
		void restore_pointer(T*& data_) {
			auto has_data = restore_pointer_flag_for<T>();
			if (has_data) {
				void* pv_data = allocate<T>();
				T it{};
				Restore(it);
				data_ = new(pv_data) T(std::move(it));
			}
			else
				data_ = nullptr;
		}

		template<typename ContainerOfT>
		void restore_container(ContainerOfT& data_)
		{
			using container_data_t = sized_container_data_t<ContainerOfT>;
			static_assert(std::is_pointer_v<container_data_t>);
			using container_elem_t = std::remove_const_t< std::remove_pointer_t<container_data_t> >;
			using container_size_t = sized_container_size_t<ContainerOfT>;

			container_size_t c_elem = restore_raw_value<container_size_t>();
			if (c_elem) {
				if constexpr (is_serializable_binary<container_elem_t>) {
					if constexpr (is_container_c_ctr<ContainerOfT>) {
						//для с-стр контейнера мы пишем: при дата==нулл - 0, иначе size+1. Но данных пишем ровно size (т.е. без завершающего 0)
						//соответственно если записано 0, то это нулл-стринг. контейнер должен явно поддерживать такой режим 
						//декларируя конструктор от nullptr (наш StringRef поддерживает). для других думать будем...
						if (c_elem) {
							container_size_t c_string_len = c_elem - 1;
							container_elem_t* container_data = restore_raw_allocate<container_elem_t>(c_elem, c_string_len);
							container_data[c_string_len] = 0;
							new ((void*)&data_) ContainerOfT{ container_data , c_string_len };
						}
						else
							new ((void*)&data_) ContainerOfT{ nullptr };
					}
					else {
						container_elem_t* container_data = restore_raw_allocate<container_elem_t>(c_elem, c_elem);
						new ((void*)&data_) ContainerOfT{ container_data , c_elem };
					}
				}
				else {
					ContainerOfT container_for_restore{};
					sized_container_creator_t<ContainerOfT> container_creator{ container_for_restore, allocator_ };
					container_creator.reserve(c_elem);
					for (size_t i = 0; i < c_elem; ++i) {
						container_elem_t elem{};
						Restore<container_elem_t>(elem);
						container_creator.emplace_back(std::move(elem));
					}
					data_ = std::move(container_for_restore);
				}
			}
		}

		template<typename T, typename FieldsMetaT, size_t... FieldIndex>
		void restore_user_impl(T& data_, const FieldsMetaT& meta_, std::index_sequence<FieldIndex...>) {
			(Restore(data_.*std::get<FieldIndex>(meta_)), ...);
		}
		template<typename UserType, typename UserTypeFildsMeta>
		void restore_user(UserType& data_, const UserTypeFildsMeta& meta_fields_) {
			restore_user_impl(data_, meta_fields_, std::make_index_sequence< std::tuple_size_v<UserTypeFildsMeta> >{});
		}
		//
		//
		template<typename T>
		requires (!std::is_pointer_v<T>)
			void Restore(T& data_) {
			//T может быть интегралом , может быть указателем на*, может быть массивом (sized-sequence of*)
			if constexpr (is_sized_container<T>)
				restore_container(data_);
			else if constexpr (has_meta_fields<T>)
				restore_user(data_, meta_fields<T>::fields_);
			else {
				static_assert(is_serializable_binary<T>, "must be is_serializable_binary (fundamental or describe user data)!");
				restore_raw(data_);
			}
		}
		template<typename T>
		void Restore(T*& data_) {
			restore_pointer(data_);
		}
	}; //Restorer
}; //XStorageInStream




template<class AllocatorT>
struct SerializerInMem {
	//Для простоты (пока не споткнулись) считаем не требующими in-memory сериализации объекты которые бинарно сериализуемы
	template<typename T>
	static constexpr bool bypass_in_mem_store = is_serializable_binary<T>;
	//
	struct Storer {
		const AllocatorT& allocator_;	//сохранение/восстановление указателя константная для аллокатора операция. его внутренние структуры не меняются
		//
		template<typename T, typename FieldsMetaT, size_t... FieldIndex>
		void store_user_impl(T& data_, const FieldsMetaT& meta_, std::index_sequence<FieldIndex...>) const {
			//оператор "," сохраняет последовательность вызовов. поэтому мы сохраняем элементы структуры в порядке перечисления (объявления)
			//!при РЕСТОРЕ нужно делать в ОБРАТНОМ ПОРЯДКЕ!
			(Store(data_.*std::get<FieldIndex>(meta_)), ...);
		}
		template<typename UserType, typename UserTypeFildsMeta>
		void store_user(UserType& data_, const UserTypeFildsMeta& meta_fields_) const {
			store_user_impl(data_, meta_fields_, std::make_index_sequence< std::tuple_size_v<UserTypeFildsMeta> >{});
		}
		template<typename T>
		void StorePointerSelf(T*& pointer_) const {
			static_assert(!std::is_pointer_v<T>, "pointer to pointer not supported!");
			allocator_.pointer_store(pointer_);
		}
		//если нам подают указатель, то мы полагаем, что это просто указатель, а не массив! 
		//у контейнеров элементов должны быть свои .in_mem_store(this_storer_&)/.in_mem_restore(this_restorer_&)!
		template<typename T>
		void Store(T*& data_pointer_) const {
			static_assert(!std::is_pointer_v<T>, "pointer to pointer not supported!");
			if (data_pointer_)
				Store(*data_pointer_);
			StorePointerSelf<T>(data_pointer_);
		}
		template<typename T>
		requires (!std::is_pointer_v<T>)
		void Store(T& data_) const {
			if constexpr (has_meta_fields<T>)
				store_user(data_, meta_fields<T>::fields_);	// если есть мета-инфо - используем ее
			else if constexpr (bypass_in_mem_store<T>) {
				/*bypass*/ }
			else
				data_.in_mem_store(*this);					//иначе у структуры должны быть методы .in_mem_store/.in_mem_restore(this&)
		}
	};
	//
	//
	struct Restorer
	{
		const AllocatorT& allocator_;
		//
		template<typename T, typename FieldsMetaT, size_t... FieldIndex>
		void restore_user_impl(T& data_, const FieldsMetaT& meta_, std::index_sequence<FieldIndex...>) const {
			//оператор "," сохраняет последовательность вызовов. поэтому мы сохраняем элементы структуры в порядке перечисления (объявления)
			//!при РЕСТОРЕ нужно делать в ОБРАТНОМ ПОРЯДКЕ!
			static_assert(sizeof...(FieldIndex) > 0);
			constexpr size_t field_index_max = sizeof...(FieldIndex) - 1;
			(Restore(data_.*std::get<field_index_max - FieldIndex>(meta_)), ...);
		}
		template<typename UserType, typename UserTypeFildsMeta>
		void restore_user(UserType& data_, const UserTypeFildsMeta& meta_fields_) const {
			restore_user_impl(data_, meta_fields_, std::make_index_sequence< std::tuple_size_v<UserTypeFildsMeta> >{});
		}
	public:
		template<typename T>
		void RestorePointerSelf(T*& pointer_) const {
			static_assert(!std::is_pointer_v<T>, "pointer to pointer not supported!");
			allocator_.pointer_restore(pointer_);
		}
		//если нам подают указатель, то мы полагаем, что это просто указатель, а не массив! 
		//у контейнеров элементов должны быть свои .in_mem_store(this_storer_&)/.in_mem_restore(this_restorer_&)!
		template<typename T>
		void Restore(T*& data_pointer_) const {
			static_assert(!std::is_pointer_v<T>, "pointer to pointer not supported!");
			RestorePointerSelf<T>(data_pointer_);
			if (data_pointer_)
				Restore(*data_pointer_);
		}
		template<typename T>
		requires (!std::is_pointer_v<T>)
		void Restore(T& data_) const {
			if constexpr (has_meta_fields<T>)
				restore_user(data_, meta_fields<T>::fields_);	//если у структуры есть мета-инфо - используем ее
			else if constexpr (is_serializable_binary<T>) {
				//bypass
			}
			else
				data_.in_mem_restore(*this);					//иначе у структуры должен быть метод in_mem_restore
		}
	};	//SerializerInMem::Restorer
};


#endif
