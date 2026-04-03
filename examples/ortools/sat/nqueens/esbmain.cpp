//
//NOTE  Это "esb.h" в папке проекта, который подключает 
//      - локальные настройки ЕСБ для проекта ("esb_app_config.h")
//      - и собственно ЕСБ ("esb/esb.h")
//      файл локальных настроек проекта ("esb_app_config.h") должен быть подключен ДО включения ЕСБ ("esb/esb.h")
#include "esb.h"


//NOTE  В свойствах проекта в 'Дополнительные каталоги включаемых файлов' нужно добавить 
//      1. $(ESB_PATH)\modules
//      2. <Ваш-Путь-К-ОрТулс-Инклюде>     (нр. D:\Dev\.Pkg\or-tools\9.11\include)
//          (нр. $(ESB_PATH)\modules;D:\Dev\.Pkg\or-tools\9.11\include;%(AdditionalIncludeDirectories) )
//      3. Компоновщик -> Ввод -> Дополнительные зависимости :  ortools_full.lib;utf8_validity.lib;$(CoreLibraryDependencies);%(AdditionalDependencies)
//          ( $(CoreLibraryDependencies);%(AdditionalDependencies) - это стандартная настройка, нужно перед ней вставить ortools_full.lib;utf8_validity.lib; )
//      Также для конфигурации Debug
//          - Компоновщик -> Дополнительные катклоги библиотек :   <путь-к-ортулс-дебаг-либ> (нр. D:\Dev\.Pkg\or-tools\9.11\Lib\Debug)
//      Также для конфигурации Release
//          - Компоновщик -> Дополнительные катклоги библиотек :   <путь-к-ортулс-релиз-либ> (нр. D:\Dev\.Pkg\or-tools\9.11\Lib\Release)
//      4. Также и в конфигурации Дебаг и в конфигурации Релиз должны быть настроены свойства
//          Отладка -> Аргументы команды :                          $(IntDir) $(ESB_1C_PATH)
//          Настройка ЕСБ -> Путь к 1С для запуска и отладки :      <путь-к-1С-1cv8.exe>    (нр. D:\Program Files\1cv8\8.3.23.1688\bin )
//
//NOTE  Даже если не подключать интерфейс модуля в ESB_ADDIN_INTERFACE (esb::modules::ortools::sat::module_interface),
//      НО по самому факту включения заголовка модуля он зарегистрирует в системе типов 1С то,
//      что в нем прописано через ESB_REGISTER_WRAPPER_FOR (и варианты)
//      нр. модуль "esb\modules\ortools\ortools_sat.hpp" зарегистрирует
//          ESB_REGISTER_WRAPPER_FOR(operations_research::sat::SatParameters);
//          ESB_REGISTER_WRAPPER_FOR(operations_research::sat::Model);
//      (пока такая особенность)
//
//NOTE  Собирается достаточно долго. У меня порядка 30-40 секунд.
//      При сборке сыплет Warning C4267/C4244 (неаккуратные преобразования) на код Google.
//      (мой код корректен)
//      Остается надеятся что Google знает что делает
//
#include "esb\modules\ortools\ortools_sat.hpp"
//////////////////////////////


namespace esb
{
    //////////////////////////////
    // Функциональность компоненты

    String GetHellow() {
        return String(ESB_T("Привет мир 1C из мира C++ !!!"));
    }

    /// <summary>
    /// Выводит сообщение в окно сообщений
    /// </summary>
    /// <param name="сообщение_"></param>
    void СкажиПривет(Строка сообщение_) {
        Сообщить(сообщение_);
    }
}


//NOTE  Интерфейс модуля (модулей) из #include "esb\modules\ortools\ortools_sat.hpp" подключается последним параметром макроса
//      НО! Для того чтобы он "проявился" в интерфейсе компоненты необходимо объявлять ESB_ADDIN_INTERFACE_METH/ESB_ADDIN_INTERFACE_PROP
//      ДАЖЕ ЕСЛИ ОНИ ПУСТЫЕ!
//      (пока такая особенность)
//
//NOTE  Даже если не подключать интерфейс модуля в ESB_ADDIN_INTERFACE (esb::modules::ortools::sat::module_interface),
//      НО по самому факту включения заголовка модуля он зарегистрирует в системе типов 1С то,
//      что в нем прописано через ESB_REGISTER_WRAPPER_FOR (и варианты)
//      нр. модуль "esb\modules\ortools\ortools_sat.hpp" зарегистрирует
//          ESB_REGISTER_WRAPPER_FOR(operations_research::sat::SatParameters);
//          ESB_REGISTER_WRAPPER_FOR(operations_research::sat::Model);
//      (пока такая особенность)
//
ESB_ADDIN_INTERFACE("461FF2A1-E9AD-41A9-ABC6-AFB923CC7403", ("EsbOrToolsSat", "ЕсбРешательПроблем"), "Есб компонента OrToolsSat", esb::modules::ortools::sat::module_interface)
ESB_ADDIN_INTERFACE_METH
(
    ESB_META_INTERFACE_FREE_METH(GetHellow,     "ДайПривет"),
    ESB_META_INTERFACE_FREE_METH(СкажиПривет,   "SayHellow")
);
ESB_ADDIN_INTERFACE_PROP
();
ESB_ADDIN_INTERFACE_DONE();



// Здесь собственно создание примитивной 1С-компоненты с тремя методами Register, RegisterAsSCOM, Unregister
// которые нужно вызвать из 1С для регистрации в среде 1С Вашего <ЕсбОбъект> 
// и последующего его создания как основного интерфейса есб-компоненты: __EsbComponentObject__ = Новый(Типа("EsbObject"));
// у которого мы и будем вызывать __EsbComponentObject__.ДайПривет()
// Имя компоненты всегда EsbComponent, т.к. в 1С оно не используется
// (esbldr_component_cpp.h должен быть включен в проект только один раз!, это не заголовок, а кусочек .cpp)
#include "esb/esbldr_component_cpp.h"




