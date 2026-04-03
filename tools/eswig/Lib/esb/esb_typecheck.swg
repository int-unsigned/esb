/* Typecheck typemaps - The purpose of these is merely to issue a warning for overloaded C++ functions 
 * that cannot be overloaded in C# as more than one C++ type maps to a single C# type */
//
%typecheck(SWIG_TYPECHECK_BOOL)
    bool,
    const bool &
    ""

%typecheck(SWIG_TYPECHECK_CHAR)
    char, 
    const char &
    ""

%typecheck(SWIG_TYPECHECK_INT8)
    signed char,
    const signed char &
    ""

%typecheck(SWIG_TYPECHECK_UINT8)
    unsigned char, 
    const unsigned char & 
    ""

%typecheck(SWIG_TYPECHECK_INT16)
    short, 
    const short &
    ""

%typecheck(SWIG_TYPECHECK_UINT16)
    unsigned short, 
    const unsigned short &
    ""

%typecheck(SWIG_TYPECHECK_INT32)
    int, 
    long, 
    const int &, 
    const long &
    ""

%typecheck(SWIG_TYPECHECK_UINT32)
    unsigned int, 
    unsigned long, 
    const unsigned int &, 
    const unsigned long &
    ""

%typecheck(SWIG_TYPECHECK_INT64)
    long long, 
    const long long &
    ""

%typecheck(SWIG_TYPECHECK_UINT64)
    unsigned long long,
    const unsigned long long &
    ""

%typecheck(SWIG_TYPECHECK_FLOAT)
    float,
    const float &
    ""

%typecheck(SWIG_TYPECHECK_DOUBLE)
    double,
    const double &
    ""

%typecheck(SWIG_TYPECHECK_STRING)
    char *,
    char *&,
    char[ANY],
    char[]
    ""

%typecheck(SWIG_TYPECHECK_POINTER)
    SWIGTYPE, 
    SWIGTYPE *, 
    SWIGTYPE &, 
    SWIGTYPE &&, 
    SWIGTYPE *const&, 
    SWIGTYPE [],
    SWIGTYPE (CLASS::*)
    ""

