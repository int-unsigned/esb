#ifndef ESBLIB_TYPE
#error Need define ESBLIB_TYPE(ESB_NAME_, ES1_NAME_) !!!
#endif
#ifndef ESBLIB_ENUM
#error Need define ESBLIB_ENUM(ESB_NAME_, ES1_NAME_) !!!
#endif

//
ESBLIB_TYPE(Undef,				Undefined)
ESBLIB_TYPE(NullValue,			Null)
ESBLIB_TYPE(Boolean,			Boolean)
ESBLIB_TYPE(Numeric,			Numeric)
ESBLIB_TYPE(String,				String)
ESBLIB_TYPE(DateTime,			Date)
ESBLIB_TYPE(UUIDValue,			UUIDValue)
ESBLIB_TYPE(TypeValue,			Type)
ESBLIB_TYPE(Array,				ValueArray)
ESBLIB_TYPE(FixedArray,			FixedValueArray)
ESBLIB_TYPE(KeyAndValue,		ValuePair)
ESBLIB_TYPE(Structure,			Structure)			//+Value
ESBLIB_TYPE(FixedStructure,		FixedStructure)		//+Value
ESBLIB_TYPE(Map,				Map)				//+Value
ESBLIB_TYPE(FixedMap,			FixedValueMap)
ESBLIB_ENUM(SortDirection,		SortDirection)		//EnumValue && EnumSelf
ESBLIB_ENUM(RoundMode,			RoundMode)			//EnumValue && EnumSelf
ESBLIB_TYPE(ErrorInfoValue,		BSLException)
//
//ESBLIB_TYPE(CommonModule,		CommonModule)//mngcore
//
#undef ESBLIB_TYPE
#undef ESBLIB_ENUM
