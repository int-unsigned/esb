///////////
// google absl

%esb_insert_module_begin 
%{
#include "absl/types/span.h"
#include "absl/strings/string_view.h"
%}

//TODO	Swig читает и создает insertDirective::code буквально "as-is"
//		То есть даже tab туда "для красоты" вставить нельзя - оно будет также и результирующем файле
//		Можно было бы "для красоты" по одному tab в начале строки удалять, если они во всех строках есть
//		Настройку такого поуедения можно сделать опцией
%esb_insert_module_begin 
%{
//NOTE  По какой-то причине (в esb_ext::ExEnumeratorForContainer) не видятся неквалифицированные begin/end для absl::Span
//      для std::span видятся, для std::vector видятся, а для absl::Span НЕТ.
//      Поэтому кастомизируем их (в их же неймспейс, а то все равно не видятся)
namespace absl 
{
    ABSL_NAMESPACE_BEGIN

    template <typename T>
    constexpr auto begin(const absl::Span<T>& container_) noexcept -> decltype(container_.begin())  { return container_.begin(); }
    template <typename T>
    constexpr auto begin(absl::Span<T>& container_) noexcept -> decltype(container_.begin())        { return container_.begin(); }
    template <typename T>
    constexpr auto end(const absl::Span<T>& container_) noexcept -> decltype(container_.end())      { return container_.end(); }
    template <typename T>
    constexpr auto end(absl::Span<T>& container_) noexcept -> decltype(container_.end())            { return container_.end(); }

    ABSL_NAMESPACE_END
}
%}



//TODO	
%include <esb_std_string_view_a.swg>


//NOTE	Если НЕ задавать typemap ниже, а просто подключить %include <esb_std_string_view_a.swg> в которм определен класс std::string_view, 
//		то используются аттрибуты самого класса (то есть, к примеру, аргументы принимаются как CppStringViewAnsi)
//		(а конвертация будет "convert_to<std::string>" т.к. для std::string_view так задано)
//
//NOTE	При применении этого typemap он переопределяет заданный для самого класса typemap
//		(видимо потому что сам класс по коду ВЫШЕ. ЕСвиг сначала видит класс и устанавливает ему typemap из аттрибутов, а уже потом видит директиву %typemap и ПЕРЕУСТАНАВЛИВАТ)
//
//%esb_1C_typemap_for_std_string_view_a();



// ЕЩЕРАЗ - фичи нужно делать ДО декларации тапа !!!
%esb_declare_as_ref_type(absl::Span);
%esb_template_name("AbslSpanWrapper")  absl::Span;
%esb_container(absl::Span);
//
namespace absl 
{
  	template<class _Tp >
  	class Span{};
}



//TODO	В используемой библиотеке absl absl::string_view определен псевдонимом на std::string_view
//		это поведение контролируется опцией ABSL_USES_STD_STRING_VIEW
//		так сконфигурирован и собран используемый ortools.
//		В иной конфигурации это нужно проверять и при необходимости объявлять класс absl::string_view
//		и делать для него враппер.
//
namespace absl 
{
	using string_view = std::string_view;
}
//
//NOTE	Псевдоним выше нужен, чтобы алгоритмы eswig знали с каким типом мы имеем дело
//		А алгоритмы swig знали какие typemap применять
//
//NOTE	Если выше НЕ делать using absl::string_view = std::string_view, то Свиг тип-мап-для-std::string_view не применяет, что логично. То есть Свиг по типдефам ходит. 
//		ESwig-проверка выдает ошибку, что совсем неизвестный тип absl::string_view, а емиттер просто прямолинейно делает convert_to<absl::string_view> (т.е. в оригинальный тип)
//		(см.TODO ниже)
//NOTE	Если выше НЕ делать using absl::string_view = std::string_view, но объявить непосредстванно тип-мап прямо для absl::string_view,
//%typemap(ctype, ret="String", converter="convert_to<std::string>") 	absl::string_view, absl::string_view&, absl::string_view&&, const absl::string_view, const absl::string_view&, const absl::string_view&&	"const String&"
//		то ругается только ESwig-проверка, а емиттер использует этот тип-мап и емитит все Ок. т.е. это "оригинальный" режим Свиг как он был задуман.

