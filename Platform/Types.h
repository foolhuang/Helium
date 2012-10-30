#pragma once

#include <stdint.h>

#include "Platform/Platform.h"

//
// Register types
//

#if HELIUM_CC_CL

/// @defgroup intprintf Integer "printf" Formatting Macros
/// These allow portable usage of fixed-sized integers in formatting strings for printf() and similar statements.
/// Macros defined in inttypes.h are used when available on supported platforms.
//@{

/// "char" string format macro for signed 8-bit integers.
#define PRId8 "hhd"
/// "char" string format macro for signed 16-bit integers.
#define PRId16 "hd"
/// "char" string format macro for signed 32-bit integers.
#define PRId32 "I32d"
/// "char" string format macro for signed 64-bit integers.
#define PRId64 "I64d"

/// "char" string format macro for unsigned 8-bit integers.
#define PRIu8 "hhu"
/// "char" string format macro for unsigned 16-bit integers.
#define PRIu16 "hu"
/// "char" string format macro for unsigned 32-bit integers.
#define PRIu32 "I32u"
/// "char" string format macro for unsigned 64-bit integers.
#define PRIu64 "I64u"

/// "char" string format macro for int_fast8_t.
#define PRIdFAST8 PRId8
/// "char" string format macro for int_fast16_t.
#define PRIdFAST16 PRId32
/// "char" string format macro for int_fast32_t.
#define PRIdFAST32 PRId32
/// "char" string format macro for int_fast64_t.
#define PRIdFAST64 PRId64

/// "char" string format macro for uint_fast8_t.
#define PRIuFAST8 PRIu8
/// "char" string format macro for uint_fast16_t.
#define PRIuFAST16 PRIu32
/// "char" string format macro for uint_fast32_t.
#define PRIuFAST32 PRIu32
/// "char" string format macro for uint_fast64_t.
#define PRIuFAST64 PRIu64

/// "char" string format macro for ptrdiff_t.
#define PRIdPD "Id"
/// "char" string format macro for size_t.
#define PRIuSZ "Iu"

/// "wchar_t" string format macro for signed 8-bit integers.
#define WPRId8 L"hhd"
/// "wchar_t" string format macro for signed 16-bit integers.
#define WPRId16 L"hd"
/// "wchar_t" string format macro for signed 32-bit integers.
#define WPRId32 L"I32d"
/// "wchar_t" string format macro for signed 64-bit integers.
#define WPRId64 L"I64d"

/// "wchar_t" string format macro for unsigned 8-bit integers.
#define WPRIu8 L"hhu"
/// "wchar_t" string format macro for unsigned 16-bit integers.
#define WPRIu16 L"hu"
/// "wchar_t" string format macro for unsigned 32-bit integers.
#define WPRIu32 L"I32u"
/// "wchar_t" string format macro for unsigned 64-bit integers.
#define WPRIu64 L"I64u"

/// "wchar_t" string format macro for int_fast8_t.
#define WPRIdFAST8 WPRId8
/// "wchar_t" string format macro for int_fast16_t.
#define WPRIdFAST16 WPRId32
/// "wchar_t" string format macro for int_fast32_t.
#define WPRIdFAST32 WPRId32
/// "wchar_t" string format macro for int_fast64_t.
#define WPRIdFAST64 WPRId64

/// "wchar_t" string format macro for uint_fast8_t.
#define WPRIuFAST8 WPRIu8
/// "wchar_t" string format macro for uint_fast16_t.
#define WPRIuFAST16 WPRIu32
/// "wchar_t" string format macro for uint_fast32_t.
#define WPRIuFAST32 WPRIu32
/// "wchar_t" string format macro for uint_fast64_t.
#define WPRIuFAST64 WPRIu64

/// "wchar_t" string format macro for ptrdiff_t.
#define WPRIdPD L"Id"
/// "wchar_t" string format macro for size_t.
#define WPRIuSZ L"Iu"

/// "tchar_t" string format macro for signed 8-bit integers.
#define TPRId8 TXT( "hhd" )
/// "tchar_t" string format macro for signed 16-bit integers.
#define TPRId16 TXT( "hd" )
/// "tchar_t" string format macro for signed 32-bit integers.
#define TPRId32 TXT( "I32d" )
/// "tchar_t" string format macro for signed 64-bit integers.
#define TPRId64 TXT( "I64d" )

/// "tchar_t" string format macro for unsigned 8-bit integers.
#define TPRIu8 TXT( "hhu" )
/// "tchar_t" string format macro for unsigned 16-bit integers.
#define TPRIu16 TXT( "hu" )
/// "tchar_t" string format macro for unsigned 32-bit integers.
#define TPRIu32 TXT( "I32u" )
/// "tchar_t" string format macro for unsigned 64-bit integers.
#define TPRIu64 TXT( "I64u" )

/// "tchar_t" string format macro for int_fast8_t.
#define TPRIdFAST8 TPRId8
/// "tchar_t" string format macro for int_fast16_t.
#define TPRIdFAST16 TPRId32
/// "tchar_t" string format macro for int_fast32_t.
#define TPRIdFAST32 TPRId32
/// "tchar_t" string format macro for int_fast64_t.
#define TPRIdFAST64 TPRId64

/// "tchar_t" string format macro for uint_fast8_t.
#define TPRIuFAST8 TPRIu8
/// "tchar_t" string format macro for uint_fast16_t.
#define TPRIuFAST16 TPRIu32
/// "tchar_t" string format macro for uint_fast32_t.
#define TPRIuFAST32 TPRIu32
/// "tchar_t" string format macro for uint_fast64_t.
#define TPRIuFAST64 TPRIu64

/// "tchar_t" string format macro for ptrdiff_t.
#define TPRIdPD TXT( "Id" )
/// "tchar_t" string format macro for size_t.
#define TPRIuSZ TXT( "Iu" )

//@}

/// @defgroup intscanf Integer "scanf" Formatting Macros
/// These allow portable usage of fixed-sized integers in formatting strings for scanf() and similar statements.  Macros
/// defined in inttypes.h are used instead on supported platforms.
//@{

/// "char" string format macro for signed 8-bit integers.
#define SCNd8 "hhd"
/// "char" string format macro for signed 16-bit integers.
#define SCNd16 "hd"
/// "char" string format macro for signed 32-bit integers.
#define SCNd32 "I32d"
/// "char" string format macro for signed 64-bit integers.
#define SCNd64 "I64d"

/// "char" string format macro for unsigned 8-bit integers.
#define SCNu8 "hhu"
/// "char" string format macro for unsigned 16-bit integers.
#define SCNu16 "hu"
/// "char" string format macro for unsigned 32-bit integers.
#define SCNu32 "I32u"
/// "char" string format macro for unsigned 64-bit integers.
#define SCNu64 "I64u"

/// "char" string format macro for int_fast8_t.
#define SCNdFAST8 SCNd8
/// "char" string format macro for int_fast16_t.
#define SCNdFAST16 SCNd32
/// "char" string format macro for int_fast32_t.
#define SCNdFAST32 SCNd32
/// "char" string format macro for int_fast64_t.
#define SCNdFAST64 SCNd64

/// "char" string format macro for uint_fast8_t.
#define SCNuFAST8 SCNu8
/// "char" string format macro for uint_fast16_t.
#define SCNuFAST16 SCNu32
/// "char" string format macro for uint_fast32_t.
#define SCNuFAST32 SCNu32
/// "char" string format macro for uint_fast64_t.
#define SCNuFAST64 SCNu64

/// "char" string format macro for ptrdiff_t.
#define SCNdPD "Id"
/// "char" string format macro for size_t.
#define SCNuSZ "Iu"

/// "wchar_t" string format macro for signed 8-bit integers.
#define WSCNd8 L"hhd"
/// "wchar_t" string format macro for signed 16-bit integers.
#define WSCNd16 L"hd"
/// "wchar_t" string format macro for signed 32-bit integers.
#define WSCNd32 L"I32d"
/// "wchar_t" string format macro for signed 64-bit integers.
#define WSCNd64 L"I64d"

/// "wchar_t" string format macro for unsigned 8-bit integers.
#define WSCNu8 L"hhu"
/// "wchar_t" string format macro for unsigned 16-bit integers.
#define WSCNu16 L"hu"
/// "wchar_t" string format macro for unsigned 32-bit integers.
#define WSCNu32 L"I32u"
/// "wchar_t" string format macro for unsigned 64-bit integers.
#define WSCNu64 L"I64u"

/// "wchar_t" string format macro for int_fast8_t.
#define WSCNdFAST8 WSCNd8
/// "wchar_t" string format macro for int_fast16_t.
#define WSCNdFAST16 WSCNd32
/// "wchar_t" string format macro for int_fast32_t.
#define WSCNdFAST32 WSCNd32
/// "wchar_t" string format macro for int_fast64_t.
#define WSCNdFAST64 WSCNd64

/// "wchar_t" string format macro for uint_fast8_t.
#define WSCNuFAST8 WSCNu8
/// "wchar_t" string format macro for uint_fast16_t.
#define WSCNuFAST16 WSCNu32
/// "wchar_t" string format macro for uint_fast32_t.
#define WSCNuFAST32 WSCNu32
/// "wchar_t" string format macro for uint_fast64_t.
#define WSCNuFAST64 WSCNu64

/// "wchar_t" string format macro for ptrdiff_t.
#define WSCNdPD L"Id"
/// "wchar_t" string format macro for size_t.
#define WSCNuSZ L"Iu"

/// "tchar_t" string format macro for signed 8-bit integers.
#define TSCNd8 TXT( "hhd" )
/// "tchar_t" string format macro for signed 16-bit integers.
#define TSCNd16 TXT( "hd" )
/// "tchar_t" string format macro for signed 32-bit integers.
#define TSCNd32 TXT( "I32d" )
/// "tchar_t" string format macro for signed 64-bit integers.
#define TSCNd64 TXT( "I64d" )

/// "tchar_t" string format macro for unsigned 8-bit integers.
#define TSCNu8 TXT( "hhu" )
/// "tchar_t" string format macro for unsigned 16-bit integers.
#define TSCNu16 TXT( "hu" )
/// "tchar_t" string format macro for unsigned 32-bit integers.
#define TSCNu32 TXT( "I32u" )
/// "tchar_t" string format macro for unsigned 64-bit integers.
#define TSCNu64 TXT( "I64u" )

/// "tchar_t" string format macro for int_fast8_t.
#define TSCNdFAST8 TSCNd8
/// "tchar_t" string format macro for int_fast16_t.
#define TSCNdFAST16 TSCNd32
/// "tchar_t" string format macro for int_fast32_t.
#define TSCNdFAST32 TSCNd32
/// "tchar_t" string format macro for int_fast64_t.
#define TSCNdFAST64 TSCNd64

/// "tchar_t" string format macro for uint_fast8_t.
#define TSCNuFAST8 TSCNu8
/// "tchar_t" string format macro for uint_fast16_t.
#define TSCNuFAST16 TSCNu32
/// "tchar_t" string format macro for uint_fast32_t.
#define TSCNuFAST32 TSCNu32
/// "tchar_t" string format macro for uint_fast64_t.
#define TSCNuFAST64 TSCNu64

/// "tchar_t" string format macro for ptrdiff_t.
#define TSCNdPD TXT( "Id" )
/// "tchar_t" string format macro for size_t.
#define TSCNuSZ TXT( "Iu" )

//@}

/// @defgroup stringprintf Char/String "printf" Formatting Macros
/// These allow portable usage of fixed-sized characters in formatting strings for printf() and similar statements.
//@{

#define PRItc "c"
#define PRIts "s"
#define PRIc "hc"
#define PRIs "hs"
#define PRIlc "lc"
#define PRIls "ls"

#define WPRItc L"c"
#define WPRIts L"s"
#define WPRIc L"hc"
#define WPRIs L"hs"
#define WPRIlc L"lc"
#define WPRIls L"ls"

#define TPRItc TXT( "c" )
#define TPRIts TXT( "s" )
#define TPRIc TXT( "hc" )
#define TPRIs TXT( "hs" )
#define TPRIlc TXT( "lc" )
#define TPRIls TXT( "ls" )

//@}

#else  // HELIUM_CC_CL

// Use inttypes.h where available; we simply try to provide relevant type definitions for platforms that don't provide
// it.
#include <inttypes.h>

#endif  // HELIUM_CC_CL

/// @defgroup floattypes Floating-point Types
/// While these may not be particularly necessary, they do provide some level of consistency with the integer types.
//@{

/// Single-precision floating-point.
typedef float float32_t;
/// Double-precision floating-point.
typedef double float64_t;

//@}

//
// String types
//

#include <string>
#include <fstream>
#include <sstream>
#include <strstream>

#if HELIUM_OS_WIN
# include <tchar.h>
#endif

#ifdef _UNICODE
# ifndef UNICODE
#  define UNICODE
# endif
# ifndef HELIUM_UNICODE
#  define HELIUM_UNICODE 1
# endif
#endif

#ifdef UNICODE
# ifndef _UNICODE
#  define _UNICODE
# endif
#endif

#if HELIUM_UNICODE

/// Default character type.
typedef wchar_t tchar_t;

/// Prefix for declaring string and character literals of the default character type.
#define TXT( X ) L##X

#else  // HELIUM_UNICODE

/// Default character type.
typedef char tchar_t;

/// Prefix for declaring string and character literals of the default character type.
#define TXT( X ) X

#endif  // HELIUM_UNICODE

typedef std::basic_string< tchar_t > tstring;

typedef std::basic_istream< tchar_t, std::char_traits< tchar_t > > tistream;
typedef std::basic_ostream< tchar_t, std::char_traits< tchar_t > > tostream;
typedef std::basic_iostream< tchar_t, std::char_traits< tchar_t > > tiostream;

typedef std::basic_ifstream< tchar_t, std::char_traits< tchar_t > > tifstream;
typedef std::basic_ofstream< tchar_t, std::char_traits< tchar_t > > tofstream;
typedef std::basic_fstream< tchar_t, std::char_traits< tchar_t > > tfstream;

typedef std::basic_istringstream< tchar_t, std::char_traits< tchar_t >, std::allocator< tchar_t > > tistringstream;
typedef std::basic_ostringstream< tchar_t, std::char_traits< tchar_t >, std::allocator< tchar_t > > tostringstream;
typedef std::basic_stringstream< tchar_t, std::char_traits< tchar_t >, std::allocator< tchar_t > > tstringstream;
