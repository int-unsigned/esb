///////////
// std::string


//NOTE	—оответствует деклараци€м в esb_stl_string_a.h !!!
//
%esb_wrap_as_val(std::string,  		"B633E087-C656-400A-9EEE-D0B31D9C13E1", "CppStringAnsi", 		"—пп—трокајнси",			"ќбъект —пп—трокајнси");
//NOTE	ƒеклараци€ _info_ это просто добавление аттрибутов к типу std::string и не приводит к добавлению "враппера" дл€ типа 'const std::string&'
//		—амосто€тельные "врапперы" дл€ типов-с-модификаторами мы не делаем, только отслеживаем дл€ самого типа ".used_as_cref_, etc..."
//		¬се "врапперы" дл€ std::string сделаны в самом esb
%esb_wrap_info_cref(std::string,  	"BBDFEF53-4A90-48CB-B830-124078C9A338", "CppStringAnsiCRef", 	"—пп—трокајнси онст—сылка",	"ќбъект —пп—трокајнси онст—сылка");
%esb_wrap_info_ref(	std::string,  	"788CBED5-9985-4ED2-86A2-6C62AA9CCECA", "CppStringAnsiRef", 	"—пп—трокајнси—сылка",	"ќбъект —пп—трокајнси—сылка");
%esb_wrap_info_cval(std::string,  	"34D46138-65F1-4406-89A2-A8808EDFC3B5", "CppStringAnsiConst", 	"—пп—трокајнси онст",	"ќбъект —пп—трокајнси онст");
%esb_well_known_type(std::string);
//
namespace std 
{
  	class string{};
}


//NOTE	»спользование этого typemap переопределит апи врапперов дл€ std::string
//		аргументы будут приниматьс€ как "const String&" (а не как CppStringAnsi)
//		и возвращатьс€ будет String (а не CppStringAnsi).
//		¬ыполн€етс€ конвертирование String <--> std::string. 
//		(помним что 1—-—трока это юникод, char16_t или wchar_t раньше)
//		ѕолезно если апи примитивное типа SetName, GetName и т.п. и используетс€ редко.
//		≈сли же модуль много и сложно работает с анси-строками, 
//		то целесообразно использовать именно CppStringAnsi с подвариантами - меньше преобразований туда-сюда будет
//
//NOTE	’от€ определение typemap по сути не зависит от определени€ самого типа, но макрос определен здесь, 
//		а не непосредственно в основном esb.swg
//		ƒл€ пор€дку..
//		“ем более не исключено, что в дальнейшем ≈—виг будет как-то свер€тьс€ с наличием определени€ самого типа.
//
%define %esb_1C_typemap_for_std_string_a()
	%typemap(ctype, ret="String") 	std::string, std::string&, std::string&&, const std::string, const std::string&, const std::string&&	"const String&"
%enddef
