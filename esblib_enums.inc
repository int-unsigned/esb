#ifndef ESBLIB_ENUM
#error Need define ESBLIB_ENUM(NAME_, N_, ...ITEMS...) !!!
#endif
//
ESBLIB_ENUM(AddInType, 2, COM, Native)
ESBLIB_ENUM(AllowedLength, 2, Fixed, Variable)
ESBLIB_ENUM(AllowedSign, 2, Any, Nonnegative)
ESBLIB_ENUM(BorderType, 2, Absolute, StyleItem)
ESBLIB_ENUM(ByteOrder, 2, LittleEndian, BigEndian)
ESBLIB_ENUM(ColorType, 5, Absolute, StyleItem, WebColor, WindowsColor, AutoColor)
ESBLIB_ENUM(ControlBorderType, 9, WithoutBorder, Double, Single, Embossed, Indented, Underline, DoubleUnderline, Rounded, Overline)
ESBLIB_ENUM(DateFractions, 3, Date, Time, DateTime)
ESBLIB_ENUM(FileAccess, 3, Read, Write, ReadAndWrite)
ESBLIB_ENUM(FileOpenMode, 6, Create, CreateNew, Open, OpenOrCreate, Truncate, Append)
ESBLIB_ENUM(FontType, 4, Absolute, StyleItem, WindowsFont, AutoFont)
ESBLIB_ENUM(MessageStatus, 6, WithoutStatus, Ordinary, Information, Attention, Important, VeryImportant)
ESBLIB_ENUM(PictureFormat, 10, UnknownFormat, BMP, GIF, Icon, JPEG, PNG, TIFF, EMF, WMF, SVG)
ESBLIB_ENUM(PictureType, 3, Empty, FromLib, Absolute)
ESBLIB_ENUM(QueryRecordType, 4, DetailRecord, GroupTotal, TotalByHierarchy, Overall)
ESBLIB_ENUM(QueryResultIteration, 3, Linear, ByGroups, ByGroupsWithHierarchy)
ESBLIB_ENUM(QuestionDialogMode, 6, OK, OKCancel, AbortRetryIgnore, YesNoCancel, YesNo, RetryCancel)
//ESBLIB_ENUM(RoundMode, 2, Round15as10, Round15as20)
ESBLIB_ENUM(SearchDirection, 2, FromBegin, FromEnd)
//ESBLIB_ENUM(SortDirection, 2, Asc, Desc)
ESBLIB_ENUM(StringEncodingMethod, 2, URLEncoding, URLInURLEncoding)
ESBLIB_ENUM(PositionInStream, 3, Begin, Current, End)
//
#undef ESBLIB_ENUM
