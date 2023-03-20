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


#define ESB_USE_DETOUR				1
#define	ESB_USE_RUSSIAN				1
//#define ESB_USE_EXPLICIT_STRING		0

#include "..\esb.h"
#include "..\esb_ext.h"
#include "..\esb_es_data.h"
#include "..\esb_es_file.h"


#include <vector>
#include <algorithm>
#include <format>

using namespace esb;


#ifdef NDEBUG
#pragma comment( lib , "../lib/release/esbhlp.lib")
//Поскольку мы и в релизе используем MessageBox, то нам нужна еще user32.lib
#pragma comment(lib,"user32.lib")
#else
#pragma comment( lib , "../lib/debug/esbhlp.lib")
#endif // NDEBUG

// esbhlp/release/esbhlp.lib - это "assertion build" - то есть ESB_ASSERT оттуда не убран и необходим. Фактически отличается только runtime библиотекой
// Это временное решение на период отладки и тестирования. Впоследствии release-build будет чист от ассертов.
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




void СортироватьМассивЧиселПоРусски(МассивЗначений Массив_) {
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
}


void СортироватьМассивЗначенийЧислоИспользуяСТД(МассивЗначений Массив_) {
	using тип_значения_в_массиве = МассивЗначений::iterator::value_type;
	auto компаратор_на_меньше = [](const тип_значения_в_массиве& a, const тип_значения_в_массиве& b) -> bool {
		return (a.КакЗначение<Число>() < b.КакЗначение<Число>());
	};
	std::sort(begin(Массив_), end(Массив_), компаратор_на_меньше);
}





void SortArrayOfValueNumericAsArr(Array arr_) {
	size_t cnt = arr_.Size();
	size_t max_i = cnt - 1;
	Var<Arbitrary> tmp{ Undef::Value_ };

	for (size_t i = 0; i < max_i; ++i) {
		size_t i_of_less = i;
		for (size_t j = i; j < cnt; ++j) {
			if (arr_.At[j].as_value<Numeric>() < arr_.At[i_of_less].as_value<Numeric>())
				i_of_less = j;
		}
		if (i_of_less != i) {
			tmp = static_cast<Arbitrary>(arr_[i]);
			arr_[i] = arr_[i_of_less];
			arr_[i_of_less] = tmp;
		}
	}
}




void SortArrayOfValueNumericAsVector(Array arr_) {
	std::vector<Numeric> vector_of_n;
	size_t cnt = arr_.Size();
	vector_of_n.reserve(cnt);
	for (const auto& v : arr_.EnumItems()) {
		vector_of_n.emplace_back(v.as_value<Numeric>());
	}

	auto compare_less = [](const Numeric& a, const Numeric& b) -> bool {
		return (a < b);
	};
	std::sort(begin(vector_of_n), end(vector_of_n), compare_less);

	auto it_n = begin(vector_of_n);
	for (const auto& ref : arr_) {
		ref.SetValue(*it_n++);
	}
}




class ArrayOfNumericObject {
public:
	using DataType = std::vector<Numeric>;
	DataType m_vector_of_n;
public:
	ArrayOfNumericObject() : m_vector_of_n{} {}
	//ТУДУ: маппинг есб-типов на спп-типы поддерфивается только в конструкторах
	ArrayOfNumericObject(size_t capacity_) : m_vector_of_n{} {
		m_vector_of_n.reserve(capacity_);
	}
	
	DataType& Data() &										{ return m_vector_of_n; }
	const DataType& Data() const &							{ return m_vector_of_n; }
	DataType&& Data()&&										{ return std::move(m_vector_of_n); }
public:
	Numeric		Count()										{ return m_vector_of_n.size(); }
	Numeric		Get(const Numeric& index_)					{ return m_vector_of_n[AsUnsignedOrThrow(index_)]; }
	void		Set(const Numeric& index_, Numeric value_)	{ m_vector_of_n[AsUnsignedOrThrow(index_)] = std::move(value_); }
public:
	Numeric		GetCapacity() const							{ return m_vector_of_n.capacity(); }
	void		SetCapacity(Numeric capacity_)				{ m_vector_of_n.reserve(AsUnsignedOrThrow(capacity_)); }
};
//
// 1C интерфейс для объекта EsbArrayOfValueNumeric
//template<>
//struct interface_info_t<ArrayOfNumericObject> {
//	using object_t = ArrayOfNumericObject;
//	// {F153A9E0-9C62-4A21-A42C-86A8D958337B}
//	ESB_DEFINE_GUID(static constexpr CLSID	TypeId_, 0xf153a9e0, 0x9c62, 0x4a21,  0xa4, 0x2c, 0x86, 0xa8, 0xd9, 0x58, 0x33, 0x7b );
//	static constexpr FixedTerm				TypeTerm_{ L"ArrayOfNumeric", L"МассивЧисел" };
//	static constexpr std::wstring_view		TypeDescriptionInit_{ L"Массив чисел" };
ESB_INTERFACEDEF_OBJECT_STABLE(ArrayOfNumericObject, "F153A9E0-9C62-4A21-A42C-86A8D958337B", ("ArrayOfNumeric", "МассивЧисел"), "Массив чисел")
	ESB_META_INTERFACE_METH(
		ESB_META_INTERFACE_MEMB_METH(Count,			"Количество"),
		ESB_META_INTERFACE_MEMB_METH(Get,			"Получить"),
		ESB_META_INTERFACE_MEMB_METH(Set,			"Установить")
	);
	ESB_META_INTERFACE_PROP(
		ESB_META_INTERFACE_MEMB_PROP_RW(Capacity,	"Емкость")
	);
	ESB_META_CTOR_INTERFACE(
		ESB_META_CTOR(),
		ESB_META_CTOR(size_t)
	);
ESB_INTERFACEDEF_DONE(ArrayOfNumericObject);
using ArrayOfNumeric = ESB_INTERFACEDEF_VALUE_T(ArrayOfNumericObject);
using ArrayOfNumericTypeFactory = ESB_INTERFACEDEF_FACTORY_T(ArrayOfNumericObject);
//inline ArrayOfNumericTypeFactory ArrayOfNumericTypeFactory::Instance_{};

ESB_INTERFACEDEF_FACTORY_INSTANCE(ArrayOfNumericObject);
bool RegisterArrayOfNumeric() {
	return ArrayOfNumericTypeFactory::RegisterType();
}
bool RevokeArrayOfNumeric() {
	return ArrayOfNumericTypeFactory::RevokeType();
}

ArrayOfNumeric CreateArrayOfNumeric(const UndefOr<const Array>& array_of_value_or_undef_when_omited_) {
	auto array_of_value_opt = array_of_value_or_undef_when_omited_.make_if<const Array>();
	if (!array_of_value_opt)
		return ArrayOfNumeric::Create();
	else {
		const Array& array_of_value = *array_of_value_opt;
		size_t cnt = array_of_value.Size();
		ArrayOfNumeric array_of_numeric = ArrayOfNumeric::Create(cnt);
		ArrayOfNumericObject::DataType& vector_of_numeric = array_of_numeric.GetContained().Data();
		for (const auto& value_may_be_numeric : array_of_value.EnumItems()) {
			vector_of_numeric.emplace_back(value_may_be_numeric.make<Numeric>());
		}
		return std::move(array_of_numeric);
	}
}





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
auto comparator_user(const DelegatToMeth& user_compare_meth_ ) {
	auto comparator = [&user_compare_meth_](const Numeric& a, const Numeric& b) -> bool {
		return user_compare_meth_.Invoke<Boolean>(a, b);
	};
	return comparator;
}

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
	DelegatToMeth sort_meth = mode_.make<DelegatToMeth>();
	return std::sort(it_begin_, it_end_, comparator_user(sort_meth));
}


std::vector<Var<Numeric>> ArrayOfValueNumericToVector(const Array& arr_) {
	std::vector<Var<Numeric>> vector_of_n;
	size_t cnt = arr_.Size();
	vector_of_n.reserve(cnt);
	for (auto v : arr_.EnumItems()) {
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


void SortArrayOfNumeric(ArrayOfNumeric array_of_numeric_, const ArraySortMode& mode_) {
	ArrayOfNumericObject::DataType& vector_of_numeric = array_of_numeric_.GetContained().Data();
	DoSortIt(begin(vector_of_numeric), end(vector_of_numeric), mode_);
}



void SortArray(const ClassMix<Array, ArrayOfNumeric>& array_mix_, const ArraySortMode& mode_) {
	if (auto opt_array = array_mix_.make_if<Array>())
		return SortArrayOfValueNumericAsVectorEx(*opt_array, mode_);
	else
		return SortArrayOfNumeric(array_mix_.make<ArrayOfNumeric>(), mode_);
}

Boolean RegisterDelegatToMeth() {
	return ExtRegisterDelegatToMeth();
}
Boolean RevokeDelegatToMeth() {
	return ExtRevokeDelegatToMeth();
}
DelegatToMeth CreateDelegatToMeth(const CommonModule& module_, const String meth_) {
	return DelegatToMeth::Create(module_, meth_);
}




//ТУДУ: Наш авто-преобразователь типов на входе не преобразовывает их IVariable в нашу IVariable. Только вынимает из их IVariable значение, проверяет тип и отдает нам.
//		то есть параметры методов нужно задавать в виде Numeric, а не Var<Numeric>) даже если они дальше сразу отдаются назад платформе через какое-либо 
//		имплисит или експлисит преобразование в IVariable.
//		Сделано так потому-что все-таки создание Var<Numeric это немного затратнее чем просто отдать Numeric, а авто-преобразователь не знает - нужно оно или нет.
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
	constexpr size_t line_len_ = std::wstring_view(L"сообщить(* ------------------------------------------------------------------------------------------------------);").size();
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
				size_t nStar = 1 + (m_nStarMax * val) / (m_ValMax+1);	//m_nStarMax * (val / m_ValMax)
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

//2. Методы (конструкторы) с массой опт-параметров в конце развернуть на разные версии методов
//	 в плюсах опт-параметры могут быть опущены только в конце. тогда их лучше не передавать вообще - пусть платформа сама разбирается. думаю разберется
// 
//5. New сделать.
// 
ValueTable CreateResultTable(const Array& results_array_) {

	NumberQualifiers qn = NumberQualifiers::Create(Numeric(12), Numeric(2));
	StringQualifiers qs = StringQualifiers::Create(Numeric(20));
	Array arr_of_types = Array::Create();
	//auto t = Type(L"Строка");
	//auto t = TypeOf<String>()
	auto t = TypeDefOf<String>.Value();
	arr_of_types.Add(t);
	TypeDescription td_str = TypeDescription::Create(arr_of_types, DefUndef<NumberQualifiers>, qs);
	
	Numeric x(0);
	auto nt = TypeOf(x);

	arr_of_types.Clear();
	arr_of_types.Add(Numeric::Type_.Value());
	TypeDescription td_num = TypeDescription::Create(arr_of_types, qn);

	ValueTable vt = ValueTable::Create();
	vt.Columns.Add(String(L"Текст"), td_str, String(L"Column-1"));
	vt.Columns.Add(String(L"Значение"), td_num, String(L"Column-2"), Numeric(20));

	size_t crows = results_array_.Size();
	for (size_t irow = 0; irow < crows; ++irow) {
		ResultStruct s = results_array_.At[irow].make<FixedStructure>();
		auto r = vt.Add();
		r.SetAt(0, s.GetTAG());
		r.SetAt(1, s.GetVAL());
	}

	return vt;
}

// Просто в порядке тестирования классов и АПИ.
void SaveResultsToFile(const Array& results_array_) {			
	static_assert(sizeof(strview_t::value_type) == 2 && sizeof(unsigned) == 4, "alg requirement!");

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
	auto write_to_bb = [&bb, &i_byte](const strview_t& str_) {
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
		std::wstring val = std::to_wstring(AsUnsignedOrThrow(s.GetVAL()));
		write_to_bb(val);
		write_to_bb(L"\n");
	}

	// ЕСБ позволяет писать код "ну почти как в 1С", но для с++ это чудовищно неэффективный код. просто мясорубка и мешанина вызовов платформы.
	String		s_db = Upper(InfoBaseConnectionString());
	Numeric		pos_1 = StrFind(s_db, String(L"FILE="));
	Numeric		pos_e = StrFind(s_db, String(L"\\"), SearchDirectionEnum::FromEnd);
	string_t	s_dir = Mid(s_db, pos_1 + Numeric(6), pos_e - pos_1 - Numeric(6)).string();
	String		s_file = String(s_dir + L"\\test_info.txt");

	FileStream fs = FileStream::Create(s_file, FileOpenModeEnum::OpenOrCreate);
	fs.Write(bb, Numeric(0), Numeric(i_byte));
	fs.Flush();
	fs.Close();

	Message(String(L"Файл результатов сохранен как:"));
	Message(s_file);
}



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





#if 0
extern void SortArrayOfNumericNativeProcStd(const esb::Object& arr_object_);
void SortArrayOfNumericNative(const Array& arr_) {
	return SortArrayOfNumericNativeProcStd(arr_);
}

extern void SortArrayOfNumericNativeProcOpt(const esb::Object& arr_object_);
void SortArrayOfNumericNativeOpt(const Array& arr_) {
	return SortArrayOfNumericNativeProcOpt(arr_);
}


extern int Addin_TestApiBslGeneralContexts();
extern esb::IObjectPtr find_provider_context(esb::ProviderId pid_);
Numeric TestApiBslGeneralContexts() {
	//IObjectPtr x = find_provider_context(esb::ProviderId::core);
	//__debugbreak();
	IObjectPtr x = esb_api::get_provider_context<esb_api::ProviderId::core>();
	return (x != nullptr);
	//return Addin_TestApiBslGeneralContexts();
}
#endif





esb::Numeric GetTicks() {
	return esb::Numeric(GetTickCount());
}


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

	static constexpr auto							InterfaceMethMeta1_ = make_meta_interface_meth(
		ESB_META_INTERFACE_STAT_METH(GetHellow,										"ДайПривет"),
		ESB_META_INTERFACE_STAT_METH(Mul,											"Умножить"),
		ESB_META_INTERFACE_STAT_METH(Div,											"Делить"),
		ESB_META_INTERFACE_FREE_METH(GetTicks,										"ДайТики"),
		ESB_META_INTERFACE_FREE_METH(СортироватьМассивЧиселПоРусски,				"SortArrayOfValueNumericAsObj"),
//		ESB_META_INTERFACE_FREE_METH(SortArrayOfNumericNative,						"СортироватьМассивЧиселНапрямую"),
//		ESB_META_INTERFACE_FREE_METH(SortArrayOfNumericNativeOpt,					"СортироватьМассивЧиселНапрямуюОпт"),
		ESB_META_INTERFACE_FREE_METH(SortArrayOfValueNumericAsArr,					"СортироватьМассивЧиселКакМассив"),
		ESB_META_INTERFACE_FREE_METH(СортироватьМассивЗначенийЧислоИспользуяСТД,	"SortArrayOfValueNumericUseSTD"),
		ESB_META_INTERFACE_FREE_METH(SortArrayOfValueNumericAsVector,				"СортироватьМассивЧиселКакСппВектор"),
		ESB_META_INTERFACE_FREE_METH(CreateArrayOfNumeric,							"СоздатьМассивЧисел")
	);
	static constexpr auto							InterfaceMethMeta2_ = make_meta_interface_meth(
		ESB_META_INTERFACE_FREE_METH(SortArrayOfNumeric,					"СортироватьМассивЧисел"),
		ESB_META_INTERFACE_FREE_METH(SortArray,								"СортироватьМассив"),
		ESB_META_INTERFACE_FREE_METH(RegisterDelegatToMeth,					"ЗарегистрироватьДелегатМетода"),
		ESB_META_INTERFACE_FREE_METH(RevokeDelegatToMeth,					"ОтозватьДелегатМетода"),
		ESB_META_INTERFACE_FREE_METH(CreateDelegatToMeth,					"СоздатьДелегатМетода"),
		ESB_META_INTERFACE_FREE_METH(ResultArrayAdd,						"ДобавитьВМассивРезультатов"),
		ESB_META_INTERFACE_FREE_METH(ResultArraySort,						"СортироватьМассивРезультатов"),
		ESB_META_INTERFACE_FREE_METH(ResultArrayShow,						"ПоказатьМассивРезультатов"),
		ESB_META_INTERFACE_FREE_METH(CreateResultTable,						"СоздатьТаблицуРезультатов"),
		ESB_META_INTERFACE_FREE_METH(SelectTestArrayOfNumeric,				"ВыбратьТестовыйНаборЧисел"),
		ESB_META_INTERFACE_FREE_METH(SaveResultsToFile,						"СохранитьРезультатВФайл")
	);
	//static constexpr auto							InterfaceMethMeta3_ = make_meta_interface_meth(
	//	ESB_META_INTERFACE_FREE_METH(TestApiBslGeneralContexts,				"TestApiBslGeneralContexts")
	//);
	// workaround: fatal error C1054: ограничение компилятора: недопустимая степень вложения инициализаторов. Максимум 10-15. Реально 11
	static constexpr auto							InterfaceMethMeta_ = std::tuple_cat(InterfaceMethMeta1_, InterfaceMethMeta2_);// , InterfaceMethMeta3_);

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

