// dllmain.cpp : Определяет точку входа для приложения DLL.
// Не используем
//BOOL APIENTRY DllMain( HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
//{
//    switch (ul_reason_for_call)
//    {
//    case DLL_PROCESS_ATTACH:
//    case DLL_THREAD_ATTACH:
//    case DLL_THREAD_DETACH:
//    case DLL_PROCESS_DETACH:
//        break;
//    }
//    return TRUE;
//}


#include "esb_app_config.h"

#include "..\esb.h"
#include "..\esb_ext.h"
#include "..\esb_es_data.h"
#include "..\esb_es_file.h"


#include <vector>
#include <algorithm>
#include <format>

using namespace esb;


#ifdef NDEBUG
#	ifdef ESB_RELEASE_BUILD
#		pragma comment(lib, "../lib/ReleaseBuild/esbhlp.lib")
#	else
#		pragma comment( lib , "../lib/release/esbhlp.lib")
		//Поскольку мы и в релизе используем MessageBox, то нам нужна еще user32.lib
#		pragma comment(lib,"user32.lib")
#	endif
#else
#	pragma comment( lib , "../lib/debug/esbhlp.lib")
#endif // NDEBUG


#ifndef ESB_RELEASE_BUILD
// esbhlp/release/esbhlp.lib - это "assertion build" - то есть ESB_ASSERT оттуда не убран и необходим. Фактически отличается только runtime библиотекой
// Это временное решение на период отладки и тестирования. Впоследствии release-build будет чист от ассертов. (или будем делать ESB_RELEASE_BUILD)
ESB_NORETURN void esb_assert_handler(uint32_t line_) {
	//												 4294967295
	wchar_t mbuf[] = L"Unexpected in esbhlp, line: ""          ";
	wchar_t* p = mbuf + std::size(mbuf) - 1;
	do {
		*--p = L'0' + line_ % 10;
		line_ /= 10;
	} while (line_ != 0);

	MessageBox(NULL, mbuf, L"Abort execution!", MB_TASKMODAL | MB_ICONSTOP | MB_OK | MB_SETFOREGROUND);
	// Тут как-бы надо обрушить все т.к. дальнейшее бессмысленно (std::terminate и аналоги), но корректно обрушить 1С из длл не получается
	// Что-то у нее внутрях сопротивляется - установлен terminate_handler, который пытается сделать "дамп памяти для анализа" и рушится сам.
	// Поэтому просто валимся в отладчик. Ремить его бессмысленно, т.к. при компиляции esbhlp плюсы знали что функция NORETURN и дополнительно
	// от греха насовали int3 после каждого ее вызова.
	__debugbreak();
}
#endif	//#ifndef ESB_RELEASE_BUILD



// вспомогательное. примитивная сортировка с++ вектора чисел для сравнений
// представленая в двух вариантах - по-английски и по-русски. 
// На мой взгляд с++ по-русски смотрится очень даже неплохо. Хотя, конечно, ... будем посмотреть..
void SortVectorOfNumeric(std::vector<Numeric>& vector_of_n, size_t count_) {
	size_t max_index = count_ - 1;
	for (size_t index = 0; index < max_index; ++index) {
		size_t index_of_less = index;
		for (size_t index_2 = index; index_2 < count_; ++index_2) {
			if (vector_of_n[index_2] < vector_of_n[index_of_less])
				index_of_less = index_2;
		}
		if (index_of_less != index)
			std::swap(vector_of_n[index], vector_of_n[index_of_less]);
	}
}

void СортироватьВекторЧисел(std::vector<Число>& вектор_чисел_, size_t количество_) {
	size_t макс_индекс = количество_ - 1;
	for (size_t индекс = 0; индекс < макс_индекс; ++индекс) {
		size_t индекс_меньшего = индекс;
		for (size_t индекс_2 = индекс; индекс_2 < количество_; ++индекс_2) {
			if (вектор_чисел_[индекс_2] < вектор_чисел_[индекс_меньшего])
				индекс_меньшего = индекс_2;
		}
		if (индекс_меньшего != индекс)
			std::swap(вектор_чисел_[индекс], вектор_чисел_[индекс_меньшего]);
	}
}
// вспомогательное. move-перемещение значений из с++ вектора в 1С массив. используется в алгоритмах сортировки "как вектор"
void MoveToArray(Array& array_, std::vector<Numeric>& vec_) {
	auto vec_it = begin(vec_);
	for (auto array_ref : array_) {
		array_ref = std::move(*vec_it);
		++vec_it;
	}
}



//SortArrayOfValueAsObject
//СортироватьМассивЗначенийКакОбъект
МассивЗначений СортироватьМассивЧиселПоРусски(МассивЗначений Массив_) {
	Число колво = Массив_.Количество();
	Число макс_индекс = колво - 1;
	Перем<Число> врем = 0;

	for (Число индекс = 0; индекс < макс_индекс; ++индекс) {
		Число индекс_меньшего = индекс;
		for (Число индекс2 = индекс + 1; индекс2 < колво; ++индекс2) {
			if (Массив_.Получить(индекс2).КакЗначение<Число>() < Массив_.Получить(индекс_меньшего).КакЗначение<Число>())
				индекс_меньшего = индекс2;
		}
		// swap
		if (индекс_меньшего != индекс) {
			врем = Массив_.Получить(индекс).КакЗначение<Число>();
			Массив_.Установить(индекс, Массив_.Получить(индекс_меньшего));
			Массив_.Установить(индекс_меньшего, врем);
		}
	}

	return std::move(Массив_);
}


// Фактически тоже, что и ..ПоРусски только разумный вынос переменных из цикла. с++ само не справляется
//СортироватьМассивЧиселКакОбъектОпт 
Array SortArrayOfNumericAsObjectOpt(Array array_)
{
	const Numeric	count = array_.Count();
	const Numeric	max_index = count - 1;
	Var<Numeric>	index = Numeric::Value_0_;
	Var<Arbitrary>	value_at_index;
	Var<Numeric>	index_2 = index;
	Var<Arbitrary>	value_2;
	Var<Numeric>	index_of_less = index;
	Var<Arbitrary>	value_of_less;

	for (; index < max_index; ++index) {
		index_of_less = index;
		value_at_index = array_.Get(index);
		value_of_less = value_at_index;
		for (index_2 = index + Numeric::Value_1_; index_2 < count; ++index_2) {
			value_2 = array_.Get(index_2);
			if (value_2.as_value<Numeric>() < value_of_less.as_value<Numeric>()) {
				index_of_less = index_2;
				value_of_less = std::move(value_2);
			}
		}
		// swap
		if (index_of_less != index) {
			array_.Set(index, value_of_less);
			array_.Set(index_of_less, value_at_index);
		}
	}

	return std::move(array_);
}


//СортироватьМассивЗначенийКакМассив
Array SortArrayOfNumericAsArray(Array array_) {
	size_t count = array_.Size();
	size_t max_index = count - 1;
	Var<Arbitrary> temp{ Undef::Value_ };

	for (size_t index = 0; index < max_index; ++index) {
		size_t index_of_less = index;
		for (size_t index_2 = index; index_2 < count; ++index_2) {
			if (array_.At[index_2].as_value<Numeric>() < array_.At[index_of_less].as_value<Numeric>())
				index_of_less = index_2;
		}
		if (index_of_less != index) {
			temp = static_cast<Arbitrary>(array_[index]);
			array_[index] = array_[index_of_less];
			array_[index_of_less] = temp;
		}
	}

	return std::move(array_);
}


// Тоже выносим переменные из цикла
//СортироватьМассивЧиселКакМассивОпт
Array SortArrayOfNumericAsArrayOpt(Array array_)
{
	IxCollectionRW<Arbitrary>& ix_array = array_.AsIndexedCollection();

	size_t			count = ix_array.Size();
	size_t			max_index = count - 1;
	Var<Arbitrary>	value_at_index;
	Var<Arbitrary>	value_2;
	Var<Arbitrary>	value_at_less;

	for (size_t index = 0; index < max_index; ++index) {
		value_at_index = ix_array.GetAt(index);
		size_t index_of_less = index;
		value_at_less = value_at_index;
		for (size_t index_2 = index + 1; index_2 < count; ++index_2) {
			value_2 = ix_array.GetAt(index_2);
			if (value_2.as_value<Numeric>() < value_at_less.as_value<Numeric>()) {
				index_of_less = index_2;
				value_at_less = std::move(value_2);
			}			
		}
		// swap
		if (index_of_less != index) {
			ix_array.SetAt(index, value_at_less);
			ix_array.SetAt(index_of_less, value_at_index);
		}
	}

	return std::move(array_);
}


// Для сравнения тоже самое, что и ..AsArray, непосредственно на 1С интерфейсе индексированной коллекции и 1С значениях. Также Оптимизированное.
// (Код не распространяется. Работает только на машине разработчика)
extern bool DevModeEnabled();
extern void TestArraySortUseNativeIxCollectionAndValues(esb::IIxCollectionRW& esb_ix_array_);
//
Array TestArraySortUseNative(Array array_) {
	if (DevModeEnabled()) {
		TestArraySortUseNativeIxCollectionAndValues(array_.AsIndexedCollection().get_interface());
		return std::move(array_);
	}
	else
		ESL_THROW_UNEXPECTED();
}



//SortArrayOfNumericAsVector
Array СортироватьМассивЧиселКакСппВектор(Array array_) {
	std::vector<Numeric> vector_of_n;
	size_t count = array_.Size();
	vector_of_n.reserve(count);
	for (auto&& v : array_.EnumItems()) {
		vector_of_n.emplace_back(std::move(v).as_value<Numeric>());
	}

	SortVectorOfNumeric(vector_of_n, count);
	MoveToArray(array_, vector_of_n);

	return std::move(array_);
}


//СортироватьМассивЧиселИспользуяСТД
МассивЗначений СортироватьМассивЧиселИспользуяСТД(МассивЗначений Массив_) {
	using тип_значения_в_массиве = МассивЗначений::iterator::value_type;
	auto компаратор_на_меньше = [](const тип_значения_в_массиве& a, const тип_значения_в_массиве& b) -> bool {
		return (a.КакЗначение<Число>() < b.КакЗначение<Число>());
	};
	std::sort(begin(Массив_), end(Массив_), компаратор_на_меньше);

	return std::move(Массив_);
}


//SortArrayOfNumericAsVectorUseSTD
Array СортироватьМассивЧиселКакСппВекторИспользуяСТД(Array array_) {
	std::vector<Numeric> vector_of_n;
	size_t cnt = array_.Size();
	vector_of_n.reserve(cnt);
	for (auto&& v : array_.EnumItems()) {
		vector_of_n.emplace_back(std::move(v).as_value<Numeric>());
	}

	auto compare_less = [](const Numeric& a, const Numeric& b) -> bool {
		return (a < b);
	};
	std::sort(begin(vector_of_n), end(vector_of_n), compare_less);

	MoveToArray(array_, vector_of_n);
	return std::move(array_);
}



// Объект ПсевдоМассивЧисел, который мы зарегистрируем в 1С. Полный интерфейс 1С-массива мы не реализуем - незачем. Но добавим свойство Емкость (шоб було).
// Сначала сделаем собственно с++ класс, который будет работать. Наш тип будет называться PseudoArrayOfNumeric, а к классу мы добавим суффикс ..Object
// (работать мы всегда дальше будем с PseudoArrayOfNumeric, который по-сути является PseudoArrayOfNumericValue, упрощаем свою жизнь и нигде ..Value в
//	именовниях не применяем (за редким исключением где это к месту). По сути у нас все ..Value - и StringValue и NumericValue и т.п.)
// (DataType и Data() удобные "магические" имена при дальнейшей работе с данными в с++. см.ниже)
struct PseudoArrayOfNumericObject {
	using DataType = std::vector<Numeric>;
private:
	DataType m_vector_of_n;
public:
	PseudoArrayOfNumericObject() : m_vector_of_n{}					{ }
	PseudoArrayOfNumericObject(size_t capacity_) : m_vector_of_n{}	{ m_vector_of_n.reserve(capacity_); }
	
	DataType& Data() &										{ return m_vector_of_n; }
	const DataType& Data() const &							{ return m_vector_of_n; }
	DataType&& Data()&&										{ return std::move(m_vector_of_n); }
public:
	Numeric		Count() const								{ return m_vector_of_n.size(); }
	Numeric		Get(const Numeric& index_) const			{ return m_vector_of_n[AsUnsignedOrThrow(index_)]; }
	void		Set(const Numeric& index_, Numeric value_)	{ m_vector_of_n[AsUnsignedOrThrow(index_)] = std::move(value_); }
public:
	Numeric		GetCapacity() const							{ return m_vector_of_n.capacity(); }
	void		SetCapacity(Numeric capacity_)				{ m_vector_of_n.reserve(AsUnsignedOrThrow(capacity_)); }
};
// Объявляем интерфейс, который будет видеть 1С
// (объект интерфейс отделен от самого класса с целью возможности объявлять интерфейсы к классам из не-наших библиотек (если это будет возможно по данным)
ESB_INTERFACEDEF_OBJECT_STABLE(PseudoArrayOfNumericObject, "F153A9E0-9C62-4A21-A42C-86A8D958337B", ("PseudoArrayOfNumeric", "ПсевдоМассивЧисел"), "Объект псевдо-массив чисел")
	ESB_META_INTERFACE_METH(
		ESB_META_INTERFACE_MEMB_METH(	Count,			"Количество"),
		ESB_META_INTERFACE_MEMB_METH(	Get,			"Получить"),
		ESB_META_INTERFACE_MEMB_METH(	Set,			"Установить")
	);
	ESB_META_INTERFACE_PROP(
		ESB_META_INTERFACE_MEMB_PROP_RW(Capacity,		"Емкость")
	);
	ESB_META_CTOR_INTERFACE(
		ESB_META_CTOR(),
		ESB_META_CTOR(size_t)
	);
ESB_INTERFACEDEF_DONE_WITH_VALUE_AND_FACTORY_REG(PseudoArrayOfNumericObject, PseudoArrayOfNumeric);
// ^^^^ Этот макрос делает для нас все необходимое - финализирует интерфейс, проверяет его на корректность, декларирует объект для есб, создает класс-фактори для 1С
//		и прописывает класс в списках стартап-регистрации/дерегистрации для использования в платформе через Новый(...)
//		(если что-то из этого не нужно, то можно по-частям ручками сделать. см.как там устроено)



//
PseudoArrayOfNumeric CreatePseudoArrayOfNumeric(const UndefOr<const Array>& array_of_value_or_undef_when_omited_) {
	auto array_of_value_opt = array_of_value_or_undef_when_omited_.make_if<const Array>();
	if (!array_of_value_opt)
		return PseudoArrayOfNumeric::Create();
	else {
		const Array& array_of_value = *array_of_value_opt;
		size_t cnt = array_of_value.Size();
		PseudoArrayOfNumeric array_of_numeric = PseudoArrayOfNumeric::Create(cnt);
		PseudoArrayOfNumericObject::DataType& vector_of_numeric = array_of_numeric.GetContained().Data();
		for (auto&& value_may_be_numeric : array_of_value.EnumItems()) {
			vector_of_numeric.emplace_back(std::move(value_may_be_numeric).as_value<Numeric>());
		}
		return std::move(array_of_numeric);
	}
}


PseudoArrayOfNumeric SortPseudoArrayOfNumeric(PseudoArrayOfNumeric array_of_numeric_) {
	PseudoArrayOfNumeric::ContainedDataType& vector_of_n = array_of_numeric_.GetContained().Data();
	SortVectorOfNumeric(vector_of_n, vector_of_n.size());

	return std::move(array_of_numeric_);
}


PseudoArrayOfNumeric SortPseudoArrayOfNumericAsInt(PseudoArrayOfNumeric array_of_numeric_) {
	PseudoArrayOfNumeric::ContainedDataType& vector_of_n = array_of_numeric_.GetContained().Data();
	//Sort
	size_t count = vector_of_n.size();
	size_t max_i = count - 1;
	for (size_t i = 0; i < max_i; ++i) {
		size_t i_of_less = i;
		for (size_t j = i; j < count; ++j) {
			if (ToIntegerOrThrow<int64_t>(vector_of_n[j]) < ToIntegerOrThrow<int64_t>(vector_of_n[i_of_less]))
				i_of_less = j;
		}
		if (i_of_less != i)
			std::swap(vector_of_n[i], vector_of_n[i_of_less]);
	}

	return std::move(array_of_numeric_);
}
PseudoArrayOfNumeric SortPseudoArrayOfNumericUseSTD(PseudoArrayOfNumeric array_of_numeric_) {
	PseudoArrayOfNumeric::ContainedDataType& vector_of_numeric = array_of_numeric_.GetContained().Data();
	auto compare_less = [](const Numeric& a, const Numeric& b) -> bool {
		return (a < b);
	};
	std::sort(begin(vector_of_numeric), end(vector_of_numeric), compare_less);

	return std::move(array_of_numeric_);
}




struct PseudoArrayOfIntObject {
	using DataType = std::vector<int>;
private:
	DataType m_vector_of_n;
public:
	PseudoArrayOfIntObject() : m_vector_of_n{}					{ }
	PseudoArrayOfIntObject(size_t capacity_) : m_vector_of_n{}	{ m_vector_of_n.reserve(capacity_); }

	DataType& Data() &										{ return m_vector_of_n; }
	const DataType& Data() const &							{ return m_vector_of_n; }
	DataType&& Data()&&										{ return std::move(m_vector_of_n); }
public:
	Numeric		Count() const								{ return m_vector_of_n.size(); }
	Numeric		Get(const Numeric& index_) const			{ return Numeric(m_vector_of_n[AsUnsignedOrThrow(index_)]); }
	void		Set(const Numeric& index_, Numeric value_)	{ m_vector_of_n[AsUnsignedOrThrow(index_)] = static_cast<int>(value_); }
};
ESB_INTERFACEDEF_OBJECT_STABLE(PseudoArrayOfIntObject, "2EB431BF-F5F9-48F4-8F79-3C7970504D19", ("PseudoArrayOfInt", "ПсевдоМассивЦелых"), "Объект псевдо-массив целых чисел")
	ESB_META_INTERFACE_METH(
		ESB_META_INTERFACE_MEMB_METH(	Count,			"Количество"),
		ESB_META_INTERFACE_MEMB_METH(	Get,			"Получить"),
		ESB_META_INTERFACE_MEMB_METH(	Set,			"Установить")
	);
	ESB_META_CTOR_INTERFACE(
		ESB_META_CTOR(),
		ESB_META_CTOR(size_t)
	);
ESB_INTERFACEDEF_DONE_WITH_VALUE_AND_FACTORY_REG(PseudoArrayOfIntObject, PseudoArrayOfInt);


PseudoArrayOfInt CreatePseudoArrayOfInt(const UndefOr<const Array>& array_of_value_or_undef_when_omited_) {
	auto input_array = array_of_value_or_undef_when_omited_.make_if<const Array>();
	if (!input_array)
		return PseudoArrayOfInt::Create();
	else {
		const Array& array_of_numeric = *input_array;
		size_t count = array_of_numeric.Size();
		PseudoArrayOfInt array_of_int = PseudoArrayOfInt::Create(count);
		PseudoArrayOfInt::ContainedDataType& vector_of_int = array_of_int.GetContained().Data();
		for (size_t i = 0; i < count; ++i) {
			vector_of_int.push_back( static_cast<int>(array_of_numeric.At[i].as_value<Numeric>()) );
		}
		return std::move(array_of_int);
	}
}

// Интересно что только в таком виде - "на-указателях" компилятор делает код хотя-бы изредка сравнимый с VB6.
// Кол=50 000: 4 555, 4 539... против 3 806, 3 775...
// (обычно все-таки чуть медленнее)
// Если то же самое сделать на индексах и операторах [], то однозначно мы VB6 проигрываем. VB6 очень силен.. Очень!
PseudoArrayOfInt SortPseudoArrayOfInt(PseudoArrayOfInt array_) {
	PseudoArrayOfInt::ContainedDataType& vector_of_int = array_.GetContained().Data();
	int* vector_of_int_data = vector_of_int.data();
	//Sort
	size_t count = vector_of_int.size();
	if (count > 1) {
		int* pi = vector_of_int_data;
		int* p_end = vector_of_int_data + count;
		int* p_last = vector_of_int_data + (count - 1);
		int* pj;
		int* pless;

		for (; pi != p_last; ++pi) {
			pless = pi;
			for (pj = pi; pj != p_end; ++pj) {
				if (*pj < *pless)
					pless = pj;
			}
			if (pless != pi) {
				int tmp = *pi;
				*pi = *pless;
				*pless = tmp;
			}
		}
	}
	return std::move(array_);
}




// Имплементация "универсальной" функции сортировки. сортирует все и опциями режима.
using ArraySortMode = ClassMix<Undef, SortDirectionValue, DelegatToMeth>;

auto comparator_less() {
	auto comparator = [](const Numeric& a, const Numeric& b) -> bool {
		return (a < b);
	};
	return comparator;
}
auto comparator_gret() {
	auto comparator = [](const Numeric& a, const Numeric& b) -> bool {
		return (a > b);
	};
	return comparator;
}
// Обратите внимание что метод который сравнивает значения должен быть корректным (т.е. если а>b то ошибка если b>a). c++ это проверяет в дебаге.
struct comparator_user : public DelegatToMeth {
	comparator_user(DelegatToMeth&& meth_) : DelegatToMeth(std::move(meth_)) {}
	bool operator()(const Numeric& a, const Numeric& b) const {
		return DelegatToMeth::Invoke<Boolean>(a, b);
	}
};

template <class IteratorT>
void DoSortIt(const IteratorT it_begin_, const IteratorT it_end_, const ArraySortMode& mode_) {
	if (mode_.is<Undef>())
		return std::sort(it_begin_, it_end_, comparator_less());

	if (mode_.is<SortDirectionValue>()) {
		SortDirectionValue sort_mode = mode_.make<SortDirectionValue>();
		if (sort_mode == SortDirectionEnum::Asc)
			return std::sort(it_begin_, it_end_, comparator_less());

		assert(sort_mode == SortDirectionEnum::Desc);
		return std::sort(it_begin_, it_end_, comparator_gret());
	}

	assert(mode_.is<DelegatToMeth>());
	return std::sort(it_begin_, it_end_, comparator_user(mode_.make<DelegatToMeth>()));
}


std::vector<Var<Numeric>> ArrayOfValueNumericToVector(const Array& arr_) {
	std::vector<Var<Numeric>> vector_of_n;
	size_t cnt = arr_.Size();
	vector_of_n.reserve(cnt);
	for (auto&& v : arr_.EnumItems()) {
		vector_of_n.emplace_back(std::move(v).as_value<Numeric>());
	}
	return std::move(vector_of_n);
}
void ArrayOfValueFill(Array& arr_, std::vector<Var<Numeric>>& vector_) {
	auto it_source = begin(vector_);
	for (size_t i = 0; i < vector_.size(); ++i, ++it_source) {
		arr_.SetAt(i, *it_source);
	}
}
void SortArrayOfValueNumericAsVectorEx(Array arr_, const ArraySortMode& mode_) {
	std::vector<Var<Numeric>> vector_of_n = ArrayOfValueNumericToVector(arr_);
	DoSortIt(begin(vector_of_n), end(vector_of_n), mode_);
	ArrayOfValueFill(arr_, vector_of_n);
}


//TOBE	Можно сделать шаблон
void SortPseudoArrayOfNumericWithMode(PseudoArrayOfNumeric array_of_, const ArraySortMode& mode_) {
	PseudoArrayOfNumeric::ContainedDataType& vector_of_ = array_of_.GetContained().Data();
	DoSortIt(begin(vector_of_), end(vector_of_), mode_);
}
void SortPseudoArrayOfIntWithMode(PseudoArrayOfInt array_of_, const ArraySortMode& mode_) {
	PseudoArrayOfInt::ContainedDataType& vector_of_ = array_of_.GetContained().Data();
	DoSortIt(begin(vector_of_), end(vector_of_), mode_);
}


// Собственно "универсальная" сортировка - разные объекты и разные режимы
void SortArray(const ClassMix<Array, PseudoArrayOfNumeric, PseudoArrayOfInt>& array_mix_, const ArraySortMode& mode_) {
	if (auto opt_array = array_mix_.make_if<Array>())
		return SortArrayOfValueNumericAsVectorEx(*opt_array, mode_);
	else if(auto opt_array_of_int = array_mix_.make_if<PseudoArrayOfInt>())
		return SortPseudoArrayOfIntWithMode(*opt_array_of_int, mode_);
	else
		return SortPseudoArrayOfNumericWithMode(array_mix_.make<PseudoArrayOfNumeric>(), mode_);
}



// Делегата на метод перед созданием в среде 1С через Новый(...) нужно зарегистирировать как тип для 1С
// Это можно сделать автоматически при загрузке или вручную добавив соответствующие методы в интерфейс Есб.
// (одно другому не мешает, но после принудительного вызова RevokeDelegatToMeth из 1С делегат будет отозван - подсчет количества вызовов init/term не ведется)
ESB_REGISTER_AUTO_DELEGAT_TO_METH();
//
Boolean RegisterDelegatToMeth() {
	return ExtRegisterDelegatToMeth();
}
Boolean RevokeDelegatToMeth() {
	return ExtRevokeDelegatToMeth();
}
//NOTE	Для создания делегата методом Create регеситрировать его не обязательно, как и все определяемы в Есб новые типы
//		Все ExtRegister/ExtRevoke нужны только для создания в среде 1С через Новый (а это в общем и целом неудобно)
DelegatToMeth CreateDelegatToMeth(const CommonModule& module_, const String meth_) {
	return DelegatToMeth::Create(module_, meth_);
}




// Все работы с "результатом" исключительно для тестирования и демонстрации. В 1С можно сделать тоже самое и не менее эффективно - значений немного.
// 
//NOTE: Наш авто-преобразователь типов на входе не преобразовывает их IVariable в нашу IVariable. Только вынимает из их IVariable значение, проверяет тип и отдает нам.
//		то есть параметры методов нужно задавать в виде Numeric, а не Var<Numeric>) даже если они дальше сразу отдаются назад платформе через какое-либо 
//		имплисит или експлисит преобразование в IVariable.
//		Сделано так потому-что все-таки создание Var<Numeric> это немного затратнее чем просто отдать Numeric, а авто-преобразователь не знает - нужно оно или нет.
//		В целом, если аргумент используется в спп-контексте, то это вообще не нужно и черевато ошибками. Если аргумент далее используется для отправки в платформу один раз,
//		то без разницы кто его преобразует - авто-преобразователь на входе в метод или он в теле метода тихо преобразуется.
//		И только если аргумент передается методам платформы более одного раза, то тогда имеет смысл привести его к виду Var<Numeric> только один раз и далее только отдавать.
void ResultArrayAddEx(Array& arr_, const Var<String>& tag_, const Var<Numeric>& val_) {
	const Var<String> keys(L"TAG,VAL");
	//Здесь плюсам надо подсказать, чтобы попробовала преобразование к Arbitrary. Само не понимает.. 
	Var<FixedStructure> s = FixedStructure::Create<Arbitrary, Arbitrary>(keys, tag_, val_);
	arr_.Add(s);
}
void ResultArrayAdd(Array arr_, const String& tag_, Numeric&& val_) {
	// Если как-бы мы val_ хотели использовать несколько раз. Не забываем, что после мув val_ уже пуст!! и использовать его нельзя.
	Var<Numeric> vval = std::move(val_);
	// поэтому, чтобы не ошибиться лучше сделать перегрузку метода с, к примеру, ...Ex именем и отправить туда const Var<>&
	// ПС. Перегрузку с тем же именем делать тоже нельзя, т.к. построитель интерфейса тогда путается какой метод из перегруженных ему выбрать
	//Здесь плюсам надо подсказать, чтобы попробовала преобразование к Arbitrary. Само не понимает.. 
	return ResultArrayAddEx(arr_, Var<String>(std::move(tag_)), vval);
}
void ResultArraySort(Array arr_) {
	// мы полагаем, что нам дали массив структур TAG,VAL. Мы его будем сортировать по полю VAL.
	auto comparator = [](const Arbitrary& a, const Arbitrary& b) -> bool {
		// в обычном 1С массиве лежат просто значения и нам нужно воленс-неволенс преобразовать их в вид FixedStructure
		FixedStructure sa = a.make<FixedStructure>();
		FixedStructure sb = b.make<FixedStructure>();
		// самое простое и эффективное - обращение к полю структуры по его индексу (от 0). При этом, естесственно, нужно быть железобетонно уверенным что это "наша" структура.
		Arbitrary va = sa.Field[1];
		// также можно получить этот индекс из строки. Если такого поля нет, то -1.
		int ixVAL = sb.GetFieldId(String(L"VAL"));
		assert(ixVAL == 1);
		Arbitrary vb = sb.Field[(unsigned)ixVAL];
		// также можно получить ссылку-ацессор на поле структуры как отдельный объект
		auto b_field_val = sb[L"VAL"];
		//проверить что он привязался к полю
		assert(b_field_val);	// b_field_val.isOk(); 
		// Если ОК, то можно использовать GetId (или св-во Id) - вернут индекс поля. Если не ОК, то методы GetId возбудят исключние!
		// также можно напрямую получить значение поля
		return (va.as_value<Numeric>() < b_field_val.Value.as_value<Numeric>());
	};

	std::sort(begin(arr_), end(arr_), comparator);
}


class ResultStruct {
	FixedStructure m_stru;
public:
	ResultStruct(FixedStructure&& stru_) : m_stru(std::move(stru_)) {}
	ResultStruct(Arbitrary&& x_) : m_stru(std::move(x_).make<FixedStructure>()) {}
	bool isOk() const			{ return (m_stru.GetFieldId(String(L"TAG")) == 0 && m_stru.GetFieldId(String(L"VAL")) == 1); }
	String GetTAG() const		{ return m_stru.GetField(0).make<String>(); }
	Numeric GetVAL() const		{ return m_stru.GetField(1).make<Numeric>(); }
};

void ResultArrayShow(const Array& arr_) {
	// Нам должны были передать массив структур TAG,VAL с текстом (описанием) результата в поле TAG и числовым значение в поле VAL.
	constexpr size_t line_len_ = strview_t(L"сообщить(* --------------------------------------------------------------------------------------------------------------);").size();
	// "без лишего шума и пыли" (выделения памяти) - рекурсивно
	struct result_maker {
		const size_t&	line_len_;
		const Array&	arr_;
		const size_t	arr_size_;
		result_maker(const size_t& l_, const Array& a_) : line_len_(l_), arr_(a_), arr_size_(a_.Size()) {}
		size_t			m_TagMax = 0;
		size_t			m_ValMax = 0;
		std::wstring	m_ValMaxStr;
		size_t			m_nStarMax = 0;
		std::wstring	m_res;
		void make_recursive(size_t irow_) {
			if (irow_ == arr_size_) {
				m_ValMaxStr = std::to_wstring(m_ValMax);
				m_nStarMax = line_len_ - m_TagMax - m_ValMaxStr.length() - 5; // _:_
				m_res.reserve(line_len_ * (arr_size_ - 1));
				return;
			}
			else {
				ResultStruct s = arr_.At[irow_].make<FixedStructure>();
				assert(s.isOk());
				const std::wstring_view& tag = s.GetTAG().view_when_safe();
				size_t tag_len = tag.length();					if (tag_len > m_TagMax) m_TagMax = tag_len;
				size_t val = static_cast<int>(s.GetVAL());		if (val > m_ValMax) m_ValMax = val;
				make_recursive(++irow_);
				//
				m_res.append(tag);
				if(tag_len < m_TagMax)
					m_res.append((unsigned) m_TagMax- tag_len, L' ');
				m_res.append(L" : ");
				if (val == m_ValMax)
					m_res.append(m_ValMaxStr);
				else {
					std::wstring val_str = std::to_wstring(val);
					if(size_t lfmt = (unsigned)m_ValMaxStr.length() - val_str.length()) 
						m_res.append(lfmt, L' ');
					m_res.append(val_str);
				}
				m_res.append(L": ");
				size_t nStar = 1 + (m_nStarMax * val) / (m_ValMax+1);
				m_res.append(nStar, L'*');
				m_res += L"\n";
			}
		}
		std::wstring operator()() {
			make_recursive(0);
			return std::move(m_res);
		}
	};

	std::wstring res = result_maker(line_len_, arr_)();
	Message(String(res));
}


// в порядке тестирования ValueTable и связанных классов
ValueTable CreateResultTable(const Array& results_array_) 
{
	NumberQualifiers qn = NumberQualifiers::Create(Numeric(12), Numeric(2));
	StringQualifiers qs = StringQualifiers::Create();
	Array array_of_types = Array::Create();
	auto ts = TypeOf<String>().Value();
	array_of_types.Add(ts);
	TypeDescription td_string = TypeDescription::Create(array_of_types, DefUndef<NumberQualifiers>, qs);
	
	array_of_types.Clear();
	array_of_types.Add(Numeric::Type_.Value());
	TypeDescription td_numeric = TypeDescription::Create(array_of_types, qn);

	ValueTable vt = ValueTable::Create();
	vt.Columns.Add(String(L"Текст"), td_string, String(L"Column-1"));
	vt.Columns.Add(String(L"Значение"), td_numeric, String(L"Column-2"), Numeric(20));

	size_t crows = results_array_.Size();
	for (size_t irow = 0; irow < crows; ++irow) {
		ResultStruct s = results_array_.At[irow].make<FixedStructure>();
		auto r = vt.Add();
		r.SetAt(0, s.GetTAG());
		r.SetAt(1, s.GetVAL());
	}

	return std::move(vt);
}


// в порядке тестирования классов и АПИ.
void SaveResultsToFile(const Array& results_array_) {			
	static_assert(sizeof(strview_t::value_type) == 2 && sizeof(unsigned) == 4, "write_to_bb alg requirement!");

	size_t c_wchars = 2; //FF FE
	for (auto x : results_array_) {	
		ResultStruct s = ResultStruct(std::move(x).make<FixedStructure>());
		c_wchars += s.GetTAG().length();
		c_wchars += 1;	//\tab
		c_wchars += 10; //4294967295	- с запасом, закладываемся на максимальное uint32_t
		c_wchars += 1;	//\n
	}

	BinaryDataBuffer	bb = BinaryDataBuffer::Create(Numeric(c_wchars * sizeof(strchar_t)));
	size_t				i_byte = 0;
	auto write_to_bb = [&bb, &i_byte](const strview_t& str_) -> void {
		const uint8_t* pb = (uint8_t*)str_.data();
		const uint8_t* pb_end = pb + str_.length() * sizeof(strview_t::value_type);
		while (pb != pb_end) {
			bb.SetByteBufferByte(i_byte++, *pb++);
			bb.SetByteBufferByte(i_byte++, *pb++);
		}
	};

	bb.SetByteBufferByte(i_byte++, 0xFF);	//UTF-16 (LE)
	bb.SetByteBufferByte(i_byte++, 0xFE);

	for (ResultStruct s : results_array_) {		// здесь мы второй раз проходим по массиву и тип его уже проверен. поэтому сразу создаем нашу структуру
		String tag = s.GetTAG();
		write_to_bb(tag.view());
		write_to_bb(L"\t");
		signed ival = static_cast<signed>(s.GetVAL());
		if (ival < 0)
			ESL_THROW_UNEXPECTED();
		std::wstring val = std::to_wstring( static_cast<unsigned>(ival) );
		write_to_bb(val);
		write_to_bb(L"\n");
	}

	// ЕСБ позволяет писать код "ну почти как в 1С", но для с++ это чудовищно неэффективный код. просто мясорубка и мешанина вызовов платформы.
	String		s_db = Upper(InfoBaseConnectionString());
	Numeric		pos_1 = StrFind(s_db, String(L"FILE="));				//len=6
	Numeric		pos_e = StrFind(s_db, String(L"\\"), SearchDirectionEnum::FromEnd);
	string_t	s_dir = Mid(s_db, pos_1 + Numeric(6), pos_e - pos_1 - Numeric(6)).string();
	String		s_file = String(s_dir + L"\\test_info.txt");

	FileStream fs = FileStream::Create(s_file, FileOpenModeEnum::OpenOrCreate);
	fs.SetSize(Numeric::Value_0_);
	fs.Write(bb, Numeric::Value_0_, Numeric(i_byte));
	fs.Flush();
	fs.Close();

	Message(String(L"Таблица результатов сохранена как:"));
	Message(s_file);
}


// просто тестирование объекта Query и QueryResult
Array SelectTestArrayOfNumeric() {
	Query qry = Query::Create();
	qry.Text = String(L"ВЫБРАТЬ ТестовыйНабор.Значение КАК Значение ИЗ РегистрСведений.ТестовыйНабор КАК ТестовыйНабор");
	auto qry_res = qry.Execute().make<QueryResult>();
	auto rec = qry_res.Select();
	Numeric crec = rec.Count();
	Array res = Array::Create(crec);
	size_t ix = 0;
	while (rec.Next()) {
		res[ix] = rec[0].Value;
		++ix;
	}
	return std::move(res);
}


// Для замера быстродействия
esb::Numeric GetTicks() {
	return esb::Numeric(GetTickCount());
}



// Строим компоненту. Сначала наш объект, который собственно и будет поставлять интерфейс для 1С
// Сам объект DemoAddin какого-то особого смысла не несет. Нам нужен только тип к которому можно привязать интерфейс. 
// Методы все статик, т.к. никакого состояния у объекта нет
// Ниже демонстрируется также привязка "свободных" функций к интерфейсу объекта
struct DemoAddin {
	static String GetHellow() {
		return String(L"Привет мир!");
	}
	static Numeric Mul(const Numeric& a, const Numeric& b) {
		return a * b;
	}
	static Numeric Div(const Numeric& a, const Numeric& b) {
		if (b == 0)
			ESL_THROW_DIV_BY_ZERO();
		else
			return a / b;
	}
};
template<>
struct interface_info_t<DemoAddin> {
	using object_t = DemoAddin;
	//
	// {41436F3F-36DB-46B8-AE15-53D6A5FF3536}
	ESB_DEFINE_GUID(static constexpr UniqueTypeId	TypeId_,			0x41436f3f, 0x36db, 0x46b8, 0xae, 0x15, 0x53, 0xd6, 0xa5, 0xff, 0x35, 0x36);
	static constexpr UniqueTypeTerm					TypeTerm_			{ L"EsbDemo", L"ЕсбДемо" };
	static constexpr std::wstring_view				TypeDescriptionInit_{ L"Объект ЕСБ Демо" };

	static constexpr auto							InterfaceMethMeta_ = make_meta_interface_meth(
		ESB_META_INTERFACE_STAT_METH(GetHellow,											"ДайПривет"),
		ESB_META_INTERFACE_STAT_METH(Mul,												"Умножить"),
		ESB_META_INTERFACE_STAT_METH(Div,												"Делить"),
		ESB_META_INTERFACE_FREE_METH(GetTicks,											"ДайТики"),
		ESB_META_INTERFACE_FREE_METH(СортироватьМассивЧиселПоРусски,					"SortArrayOfValueAsObject"),
		ESB_META_INTERFACE_FREE_METH(SortArrayOfNumericAsArray,							"СортироватьМассивЧиселКакМассив"),
		ESB_META_INTERFACE_FREE_METH(SortArrayOfNumericAsArrayOpt,						"СортироватьМассивЧиселКакМассивОпт"),
		ESB_META_INTERFACE_FREE_METH(СортироватьМассивЧиселКакСппВектор,				"SortArrayOfNumericAsVector"),
		ESB_META_INTERFACE_FREE_METH(СортироватьМассивЧиселИспользуяСТД,				"SortArrayOfNumericUseSTD"),
		ESB_META_INTERFACE_FREE_METH(СортироватьМассивЧиселКакСппВекторИспользуяСТД,	"SortArrayOfNumericAsVectorUseSTD"),
		ESB_META_INTERFACE_FREE_METH(CreatePseudoArrayOfNumeric,						"СоздатьПсевдоМассивЧисел"),
		ESB_META_INTERFACE_FREE_METH(SortPseudoArrayOfNumeric,							"СортироватьПсевдоМассивЧисел"),
		ESB_META_INTERFACE_FREE_METH(SortPseudoArrayOfNumericUseSTD,					"СортироватьПсевдоМассивЧиселИспользуяСТД"),
		ESB_META_INTERFACE_FREE_METH(SortArray,											"СортироватьМассив"),
		ESB_META_INTERFACE_FREE_METH(RegisterDelegatToMeth,								"ЗарегистрироватьДелегатМетода"),
		ESB_META_INTERFACE_FREE_METH(RevokeDelegatToMeth,								"ОтозватьДелегатМетода"),
		ESB_META_INTERFACE_FREE_METH(CreateDelegatToMeth,								"СоздатьДелегатМетода"),
		ESB_META_INTERFACE_FREE_METH(ResultArrayAdd,									"ДобавитьВМассивРезультатов"),
		ESB_META_INTERFACE_FREE_METH(ResultArraySort,									"СортироватьМассивРезультатов"),
		ESB_META_INTERFACE_FREE_METH(ResultArrayShow,									"ПоказатьМассивРезультатов"),
		ESB_META_INTERFACE_FREE_METH(SelectTestArrayOfNumeric,							"ВыбратьТестовыйНаборЧисел"),
		ESB_META_INTERFACE_FREE_METH(CreateResultTable,									"СоздатьТаблицуРезультатов"),
		ESB_META_INTERFACE_FREE_METH(SaveResultsToFile,									"СохранитьРезультатВФайл"),
		ESB_META_INTERFACE_FREE_METH(SortPseudoArrayOfNumericAsInt,						"СортироватьПсевдоМассивЧиселКакЦелые"),
		ESB_META_INTERFACE_FREE_METH(DevModeEnabled,									"РежимРазработчикаДоступен"),
		ESB_META_INTERFACE_FREE_METH(TestArraySortUseNative,							"СортироватьМассивЧиселНаитивно"),
		ESB_META_INTERFACE_FREE_METH(SortArrayOfNumericAsObjectOpt,						"СортироватьМассивЧиселКакОбъектОпт"),
		ESB_META_INTERFACE_FREE_METH(CreatePseudoArrayOfInt,							"СоздатьПсевдоМассивЦелых"),
		ESB_META_INTERFACE_FREE_METH(SortPseudoArrayOfInt,								"СортироватьПсевдоМассивЦелых")
	);
	
	static constexpr auto							InterfacePropMeta_ = make_meta_interface_prop(
		ESB_META_INTERFACE_FREE_PROP_RO(Ticks, "Тики")
	);
};
const AddinDescriptor AddinDescriptor::Instance_{ std::type_identity<dispinterface_static_t<interface_info_t<DemoAddin>>>{} };





//1C Компонента
//---------------------------------------------------------------------------
#include "..\esbldr.h"
const std::wstring_view EsbComponent::ComponentName_	= L"EsbDemoComponent";
const WCHAR_T			EsbComponent::AddinName_[]		= WCHAR_T_STRING("1");		// L"esb";
IAddInDefBase*			EsbComponent::AppConnect_		= nullptr;
IMemoryManager*			EsbComponent::AppMemoryManager_ = nullptr;
AppCapabilities			EsbComponent::AppCapabilities_	= AppCapabilities::eAppCapabilitiesInvalid;
EsbComponent			EsbComponent::Instance_{};

//---------------------------------------------------------------------------//
AppCapabilities SetPlatformCapabilities(const AppCapabilities capabilities_) {
	return EsbComponent::setAppCapabilities(capabilities_);
}
const WCHAR_T* GetClassNames() {
	return EsbComponent::getAddinName();
}
long GetClassObject(const WCHAR_T* name_, IComponentBase** ppComponent) {
	if (ppComponent && *ppComponent==NULL && name_ && name_[0] == L'1' && name_[1] == L'\0') {
		*ppComponent = &EsbComponent::getInstance();
		return (long)*ppComponent;
	}
	return 0;
}
long DestroyObject(IComponentBase** ppComponent) {
	if (ppComponent && *ppComponent && *ppComponent == &EsbComponent::getInstance()) {
		EsbComponent* pComponent = static_cast<EsbComponent*>(*ppComponent);
		pComponent->DestroyInstance();
		*ppComponent = NULL;
		return 0;
	}
	return -1;	
}
//---------------------------------------------------------------------------

