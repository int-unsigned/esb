#ifndef ESBLIB_TYPE
#	error Need define ESBLIB_TYPE(ESB_NAME_, ...) !!!
#endif
#ifndef ESBLIB_ENUM
#	error Need define ESBLIB_ENUM(ESB_NAME_, ...) !!!
#endif

//
#include "es1_base.def"
#include "es1_main.def"
#include "es1_data.def"
#include "es1_file.def"
//...
//

#undef ESBLIB_TYPE
#undef ESBLIB_ENUM
