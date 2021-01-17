#include "CodePageToUtf8Conterter.h"

#include <spdlog/spdlog.h>

#include <stdio.h>
#include <ctype.h>            /* for isspace, etc.    */
#include <assert.h>
#include <string.h>

#include "unicode/utypes.h"   /* Basic ICU data types */
#include "unicode/ucnv.h"     /* C   Converter API    */
#include "unicode/ustring.h"  /* some more string fcns*/
#include "unicode/uchar.h"    /* char names           */

static const UChar kNone[] = { 0x0000 };

/* Print a UChar if possible, in seven characters. */
void prettyPrintUChar(UChar c)
{
    if(  (c <= 0x007F) &&
         (isgraph(c))  ) {
        printf(" '%c'   ", (char)(0x00FF&c));
    } else if ( c > 0x007F ) {
        char buf[1000];
        UErrorCode status = U_ZERO_ERROR;
        int32_t o;

        o = u_charName(c, U_EXTENDED_CHAR_NAME, buf, 1000, &status);
        if(U_SUCCESS(status) && (o>0) ) {
            buf[6] = 0;
            printf("%7s", buf);
        } else {
            printf(" ??????");
        }
    } else {
        switch((char)(c & 0x007F)) {
            case ' ':
                printf(" ' '   ");
                break;
            case '\t':
                printf(" \\t    ");
                break;
            case '\n':
                printf(" \\n    ");
                break;
            default:
                printf("  _    ");
                break;
        }
    }
}

void printUChars(const char  *name = "?",
                 const UChar *uch  = kNone,
                 int32_t     len   = -1 )
{
    int32_t i;

    if( (len == -1) && (uch) ) {
        len = u_strlen(uch);
    }

    printf("%5s: ", name);
    for( i = 0; i <len; i++) {
        printf("%-6d ", i);
    }
    printf("\n");

    printf("%5s: ", "uni");
    for( i = 0; i <len; i++) {
        printf("\\u%04X ", (int)uch[i]);
    }
    printf("\n");

    printf("%5s:", "ch");
    for( i = 0; i <len; i++) {
        prettyPrintUChar(uch[i]);
    }
    printf("\n");
}

void printBytes(const char  *name = "?",
                const char *uch  = "",
                int32_t     len   = -1 )
{
    int32_t i;

    if( (len == -1) && (uch) ) {
        len = static_cast<int32_t>(strlen(uch));
    }

    printf("%5s: ", name);
    for( i = 0; i <len; i++) {
        printf("%-4d ", i);
    }
    printf("\n");

    printf("%5s: ", "uni");
    for( i = 0; i <len; i++) {
        printf("\\x%02X ", 0x00FF & (int)uch[i]);
    }
    printf("\n");

    printf("%5s:", "ch");
    for( i = 0; i <len; i++) {
        if(isgraph(0x00FF & (int)uch[i])) {
            printf(" '%c' ", (char)uch[i]);
        } else {
            printf("     ");
        }
    }
    printf("\n");
}

UErrorCode convsample_02()
{


    // ucnv_getMaxCharSize

    // "cat<cat>OK"
    UChar source[] = { 0x041C, 0x043E, 0x0441, 0x043A, 0x0432,
                       0x0430, 0x0021, 0x0000 };
    char target[100];
    UErrorCode status = U_ZERO_ERROR;
    UConverter *conv;
    int32_t     len;

    // set up the converter
    //! [ucnv_open]
    conv = ucnv_open("koi8-r", &status);
    //! [ucnv_open]
    assert(U_SUCCESS(status));

    // convert to koi8-r
    len = ucnv_fromUChars(conv, target, 100, source, -1, &status);
    assert(U_SUCCESS(status));

    // close the converter
    ucnv_close(conv);

    // ***************************** END SAMPLE ********************

    // Print it out
    printUChars("src", source);
    printf("\n");
    printBytes("targ", target, len);

    return U_ZERO_ERROR;
}

// https://github.com/unicode-org/icu/blob/master/icu4c/source/samples/ucnv/convsamp.cpp
std::string CodePageToUtf8Conterter::fromCp1252(std::string const &in)
{
    UErrorCode status = U_ZERO_ERROR;
    u_setDataDirectory("./data");

//    auto const count = ucnv_countAvailable();
//    spdlog::info("{} converters available.", count);

//    for(auto i = 0; i!=count; ++i)
//    {
//        spdlog::info("* {})\t {}", i, ucnv_getAvailableName(i));
//    }

//    UConverter * const conv = ucnv_open("koi8-r", &status); // XXX
    UConverter * const conv = ucnv_open("cp1252", &status);
    if(!U_SUCCESS(status))
    {
        spdlog::error("Unable to open requested ucnv");
        return "";
    }

    std::string result;
    char const *source = in.data();
    size_t const size = in.size()*2;
    char16_t *buffer = new char16_t [size];
    char16_t *target = buffer;
    char16_t *targetLimit = &(target[size]);

// https://unicode-org.github.io/icu-docs/apidoc/released/icu4c/ucnv_8h.html#a9451f05be7b1b75832d5ec55b4e6d67f
//    ucnv_toUnicode(UConverter *converter,
//                   UChar ** target,
//                   const UChar * targetLimit,
//                   const char ** source,
//                   const char * sourceLimit,
//                   int32_t * offsets,
//                   UBool flush,
//                   UErrorCode * err );
    ucnv_toUnicode(conv, &target, targetLimit, &source, (in.data()+in.size()), nullptr, false, &status);

    result = std::string(buffer, target);

    ucnv_close(conv);
    return result;
}
