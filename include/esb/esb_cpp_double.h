/*
	"ESB" is designed to create external native components for the 1C(tm) program.
	Copyright © 2023 IntUnsigned	(v8classes@gmail.com)
	"ESB" is free software under LGPLv2.1 license with essential restriction about binary part (view esb-license.txt)
	"ESB" IS DISTRIBUTED "AS-IS" WITHOUT ANY, EVEN THE IMPLIED, WARRANTY. IN NO EVENT SHALL THE AUTHORS OR THE COPYRIGHT OWNER BE LIABLE FOR ANY CLAIM, 
	DAMAGES OR OTHER LIABILITY ARISING OUT OF THE USE OF THIS SOFTWARE.
	THE AUTHOR DOES NOT GIVE ANY GUARANTEES AND DOES NOT BEAR ANY RESPONSIBILITY REGARDING THE USE OF 1C(tm) COMPONENTS PRODUCED USING THE ESB LIBRARY IN THE 1C(tm) ENVIRONMENT.
*/


#pragma once
#ifndef ESB_CPP_DOUBLE_H
#define ESB_CPP_DOUBLE_H

#include "esb_base.h"
#include "esb_wrapper.h"

namespace esb 
{

ESB_WRAPPER(double, Double, Действительное)
    Numeric ESB_WRAPPER_STD_METH_GetValue() const
    {
        return convert_to<Numeric>(_);
    }
    void ESB_WRAPPER_STD_METH_SetValue(Numeric value_)
    {
        _ = convert_to<double>(value_);
    }
};
ESB_WRAPPER_INTERFACE(double)
    ESB_WRAPPER_INTERFACE_METH_MUTABLE
    (
       ESB_META_INTERFACE_MEMB_METH(SetValue, ESB_WRAPPER_STD_METH_SetValue_Ru)
    )
    //TODO
    //NOTE  ESB_WRAPPER_INTERFACE_METH нужно обязательно, даже если пустой 
    //      - он ESB_WRAPPER_INTERFACE_METH_MUTABLE вызывает 
    //      и если ESB_WRAPPER_INTERFACE_METH() не сделать, 
    //      то и ESB_WRAPPER_INTERFACE_METH_MUTABLE методов в интерфейсе не будет
    ESB_WRAPPER_INTERFACE_METH      
    ()
    ESB_WRAPPER_INTERFACE_PROP
    (
        ESB_META_INTERFACE_MEMB_PROP_RO(Value, ESB_WRAPPER_STD_PROP_Value_Ru)
    )
    ESB_WRAPPER_INTERFACE_CTOR
    (
        ESB_META_CTOR(),
        //TODO  Совсем забыл как "это" работает !!!
        //      Как "как-то-автоматом" делается конструктор для double-внутри от Numeric, если я его не объявлял ???? !!!!
        //      И почему если это работает, то не работает от Double (т.е. от себя родного) ???? !!!!
        //      Разобраться надо !
        ESB_META_CTOR(Numeric)
    )
};
ESB_INTERFACEDEF_FOR_WRAPPER(const double&, "31409726-F53D-48E3-B1C9-1CFFB2E4EFB6", DoubleCRef,     ДействительноеКонстСсылка,      "Объект компоненты ДействительноеКонстСсылка");
ESB_INTERFACEDEF_FOR_WRAPPER(double&,       "AE386BD2-121E-4AC5-A49E-E7D5E00FE3E8", DoubleRef,      ДействительноеСсылка,           "Объект компоненты ДействительноеСсылка");
ESB_INTERFACEDEF_FOR_WRAPPER(double,        "C92CD91C-4861-4ED3-A6BE-941AAF24CC55", Double,         Действительное,                 "Объект Действительное"); 
ESB_INTERFACEDEF_FOR_WRAPPER(const double,  "F0BC04D3-D731-4D18-9E45-F527721E9108", DoubleConst,    ДействительноеКонст,            "Объект компоненты ДействительноеКонст");


//NOTE	Для регистрация в платформе 1С нужно отдельно указывать в .cpp файле компоненты
//		ESB_REGISTER_WRAPPER_FOR(double), ESB_REGISTER_WRAPPER_FOR(const double), т.п. по необходимости

} //namespace esb 

#endif //ESB_CPP_DOUBLE_H