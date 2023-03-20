#ifndef ESBLIB_API_PROVIDER
#error Need define ESBLIB_API_PROVIDER(NAME_) !!!
#endif
//
// !!! Будешь добавлять провайдеров не забудь что в esbhlp.cpp сигдатуры для provider_in_use в /alternatename:
// !!! ПРОПИСАНЫ ручками !!!
//
ESBLIB_API_PROVIDER(core)
ESBLIB_API_PROVIDER(addin)
ESBLIB_API_PROVIDER(mngui)
ESBLIB_API_PROVIDER(backend)
ESBLIB_API_PROVIDER(entext2)
ESBLIB_API_PROVIDER(extui)
ESBLIB_API_PROVIDER(frntend)
ESBLIB_API_PROVIDER(mngbase)
ESBLIB_API_PROVIDER(mngcore)
ESBLIB_API_PROVIDER(odata)
ESBLIB_API_PROVIDER(xml2)
ESBLIB_API_PROVIDER(ext)
//
#undef ESBLIB_API_PROVIDER

