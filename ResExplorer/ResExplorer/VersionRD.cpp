// VersionRD.cpp : implementation file
//

#include "stdafx.h"
#include "resexplorer.h"
#include "ResInfo.h"
#include "ResUtilities.h"
#include "VersionRD.h"
#include <mmsystem.h>
#include <AviFmt.h>
#include <MMREG.H>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CVersionRD

typedef struct {
   WORD wLen;
   WORD wVLen;
   WORD wType;
   CString data;
   PWORD pChild;
} t_VERSHEAD;

static PWORD pWord = NULL, pString = NULL, pEndStr;
static FIXEDFILEINFO* pFFI = NULL;
static t_VERSHEAD vih, sfi, v, st, s;

static PWORD VersHead(PWORD pW, t_VERSHEAD *pVH, BOOL align)
{
   WORD w;

   pVH->wLen  = *pW++;
   pVH->wVLen = *pW++;
   pVH->wType = *pW++;
   pVH->data.Empty();
   while (w = *pW++) pVH->data += TCHAR(w);

   if (align) pW = PWORD(DWORD(pW + 1) & 0xFFFFFFFC);

   pVH->pChild = pW;
   return pW;
}

static PWORD VersString(PWORD pW, CString& name, CString& data)
{
   pW = VersHead(pW, &s, TRUE);
   name = s.data;
   data.Empty();
   s.wVLen = (s.wVLen + 1) & 0xFFFFFFFE;
   for (int n = 0; n < s.wVLen; n++) data += TCHAR(*pW++);

   return pW;
}


UINT VersionInit(HINSTANCE hinst, LPCTSTR resName, CString& name, CString& item)
{
   vih.pChild = sfi.pChild = v.pChild = st.pChild = s.pChild = NULL;
   pString = NULL;

   DWORD rSize = 0;
   pWord = (PWORD)RawData(hinst, resName, RT_VERSION, &rSize);
   if (pWord) {

      // Analyse de VS_VERSIONINFO
      pWord = VersHead(pWord, &vih, TRUE);             // head VS_VERSIONINFO
      if (vih.data.Compare("VS_VERSION_INFO") != 0) return 0;

      pFFI =  (vih.wVLen == sizeof(FIXEDFILEINFO))? (FIXEDFILEINFO*)pWord: NULL;
      pWord = PWORD((vih.wVLen + 3 + DWORD(pWord)) & 0xFFFFFFFC);

      // Analyse de StringFileInfo
      pWord = VersHead(pWord, &sfi, TRUE);
      if ( sfi.data.Compare("StringFileInfo") == 0 ) {
         // Analyse de la stringTable
         VersHead(pWord, &st, TRUE);
         pEndStr = PWORD(st.wLen + 3 + DWORD(pWord) & 0xFFFFFFFC);
         name = "FileInfo";
         DWORD fi = strtoul(st.data, NULL, 16);
         pWord = VersHead(pEndStr, &sfi, TRUE);
         item.Format("%08x   %s", fi, GetLang(WORD(fi >> 16)) );
      }
      if (sfi.data.Compare("VarFileInfo")== 0) pWord = VersHead(pWord, &v, TRUE);
   }
   return rSize;
}

BOOL VersionDecod(PWORD& pString, CString& name, CString& item)
{
   item.Empty();
   if (st.pChild) {

      if (pString == NULL) {
         pString = st.pChild;

      } else if ( pString >= pEndStr-2 ) {
         return FALSE;

      }
      pString = VersString(pString, name, item);
      return TRUE;
   }

   return FALSE;
}

typedef struct {
   DWORD dwFlags;
   LPCTSTR lpName;
} t_FLAGSNAME;

const static t_FLAGSNAME fflags[] = {
   {VS_FF_DEBUG,    "DEBUG"},
   {VS_FF_INFOINFERRED, "INFOINFERRED"},
   {VS_FF_PATCHED,  "PATCHED"   },
   {VS_FF_PRERELEASE,"PRERELEASE"},
   {VS_FF_PRIVATEBUILD,"PRIVATEBUILD"},
   {VS_FF_SPECIALBUILD,"SPECIALBUILD"},
   { 0,              NULL        }
};

const static t_FLAGSNAME flags[] = {
   {VOS_DOS,        "VOS_DOS"       },
   {VOS_NT,         "VOS_NT"        },
   {VOS__WINDOWS16, "VOS_WINDOWS16" },
   {VOS__WINDOWS32, "VOS_WINDOWS32" },
   {VOS_OS216,      "VOS_OS2_16"    },
   {VOS_OS232,      "VOS_OS2_32"    },
   {VOS__PM16,      "VOS_PM_16"     },
   {VOS__PM32,      "VOS_PM_32"     },
   {VOS_UNKNOWN,    "VOS_UNKNOWN"   },
   { 0,              NULL        }
};

const static t_FLAGSNAME type[] = {
   {VFT_UNKNOWN,     "VFP_UNKNOWN"  },
   {VFT_APP,         "VFP_APP"      },
   {VFT_DLL,         "VFP_DLL"      },
   {VFT_VXD,         "VFP_VXD"      },
   {VFT_STATIC_LIB,  "VFP_STATIC_LIB"},
   { 0,              NULL        }
};

const static t_FLAGSNAME dType[] = {
   {VFT2_UNKNOWN,    "VFT2_UNKNOWN"  },
   {VFT2_DRV_COMM,   "VFT2_DRV_COMM"     },
   {VFT2_DRV_PRINTER,"VFT2_DRV_PRINTER"  },
   {VFT2_DRV_KEYBOARD,"VFT2_DRV_KEYBOARD"},
   {VFT2_DRV_LANGUAGE,"VFT2_DRV_LANGUAGE"},
   {VFT2_DRV_DISPLAY,"VFT2_DRV_DISPLAY"  },
   {VFT2_DRV_MOUSE,  "VFT2_DRV_MOUSE"    },
   {VFT2_DRV_NETWORK,"VFT2_DRV_NETWORK"  },
   {VFT2_DRV_SYSTEM, "VFT2_DRV_SYSTEM"   },
   {VFT2_DRV_INSTALLABLE,"VFT2_DRV_INSTALLABLE"},
   {VFT2_DRV_SOUND,  "VFT2_DRV_SOUND"    },
   { 0,              NULL       }
};

const static t_FLAGSNAME fType[] = {
   {VFT2_UNKNOWN,    "VFT2_UNKNOWN"  },
   {VFT2_FONT_RASTER,"VFT2_FONT_RASTER"   },
   {VFT2_FONT_VECTOR,"VFT2_FONT_VECTOR"   },
   {VFT2_FONT_TRUETYPE,"VFT2_FONT_TRUETYPE"},
   { 0,              NULL       }
};


BOOL FixedFileInfo(UINT& n, CString& name, CString& item)
{
   name.Empty();
   item.Empty();
   if (pFFI == NULL || pFFI->dwSignature != 0xFEEF04BD) return FALSE;

   SYSTEMTIME st;
   FILETIME ft;
   const t_FLAGSNAME *pFt;

   switch (n ++) {
   case 0 :
      name = "FileVersion";
      item.Format("%d, %d, %d, %d",
         pFFI->dwFileVersionMS >> 16, pFFI->dwFileVersionMS & 0xFFFF,
         pFFI->dwFileVersionLS >> 16, pFFI->dwFileVersionLS & 0xFFFF);
      break;
   case 1 :
      name = "ProductVersion";
      item.Format("%d, %d, %d, %d",
         pFFI->dwProductVersionMS >> 16, pFFI->dwProductVersionMS & 0xFFFF,
         pFFI->dwProductVersionLS >> 16, pFFI->dwProductVersionLS & 0xFFFF);
      break;
   case 2 :
      name = "FileFlagsMask";
      item = FormatHexa(pFFI->dwFileFlagsMask);
      break;
   case 3 :
      name = "FileFlags";
      for (pFt = fflags; pFt->lpName; pFt++)
         if (pFFI->dwFileFlagsMask & pFt->dwFlags) {
            if (item.IsEmpty()) item = FormatHexa(pFFI->dwFileFlags, " ");
            else item += ", ";
            item += CString(pFt->lpName) + "=";
            item += (pFFI->dwFileFlags & pFt->dwFlags)? "T": "F";
         }
      break;
   case 4 :
      name = "FileOS";
      for (pFt = flags; pFt->lpName; pFt++)
         if (pFFI->dwFileOS & pFt->dwFlags) item += CString(pFt->lpName) + " ";
      break;
   case 5 :
      name = "FileType";
      if (pFFI->dwFileType == VFT_DRV) {
         item = "DRV";
         for (pFt = dType; pFt->lpName; pFt++)
            if (pFFI->dwFileSubtype & pFt->dwFlags) item += "/" + CString(pFt->lpName);
      } else if (pFFI->dwFileType == VFT_FONT) {
         item = "FONT";
         for (pFt = fType; pFt->lpName; pFt++)
            if (pFFI->dwFileSubtype & pFt->dwFlags) item += "/" + CString(pFt->lpName);
      } else {
         for (pFt = type; pFt->lpName; pFt++)
            if (pFFI->dwFileType == pFt->dwFlags) {
               item = CString(pFt->lpName);
               break;
            }
      }
      break;
   case 6 :
      name = "FileDate";
      ft.dwHighDateTime= pFFI->dwFileDateMS;
      ft.dwLowDateTime = pFFI->dwFileDateLS;
      if ((ft.dwHighDateTime | ft.dwLowDateTime) != 0 ) {
         ::FileTimeToSystemTime(&ft, &st);
         item.Format("%d/%d/%d %d:%d:%d",
                        st.wDay, st.wMonth, st.wYear, st.wHour, st.wMinute, st.wSecond);
      }
      break;
   default :
      return FALSE;
   }
   return TRUE;
}

typedef struct {
   WORD code;
   TCHAR* msg;
} t_LANGTBL;

#define LANG(p, s, m) { MAKELANGID(p, s), m }

static t_LANGTBL langTbl[] = {
   LANG(LANG_NEUTRAL, SUBLANG_NEUTRAL, "Language neutral"),
   LANG(LANG_NEUTRAL, SUBLANG_DEFAULT, "User default language"),
   LANG(LANG_NEUTRAL, SUBLANG_SYS_DEFAULT, "System default language"),

   LANG(LANG_AFRIKAANS, SUBLANG_NEUTRAL, "Afrikaans"),

   LANG(LANG_ALBANIAN, SUBLANG_NEUTRAL, "Albanian"),

   LANG(LANG_ARABIC, SUBLANG_NEUTRAL, "Arabic"),
   LANG(LANG_ARABIC, SUBLANG_ARABIC_SAUDI_ARABIA, "Arabic (Saudi Arabia)"),
   LANG(LANG_ARABIC, SUBLANG_ARABIC_IRAQ, "Arabic (Iraq)"),
   LANG(LANG_ARABIC, SUBLANG_ARABIC_EGYPT, "Arabic (Egypt)"),
   LANG(LANG_ARABIC, SUBLANG_ARABIC_LIBYA, "Arabic (Libya)"),
   LANG(LANG_ARABIC, SUBLANG_ARABIC_ALGERIA, "Arabic (Algeria)"),
   LANG(LANG_ARABIC, SUBLANG_ARABIC_MOROCCO, "Arabic (Morocco)"),
   LANG(LANG_ARABIC, SUBLANG_ARABIC_TUNISIA, "Arabic (Tunisia)"),
   LANG(LANG_ARABIC, SUBLANG_ARABIC_OMAN, "Arabic (Oman)"),
   LANG(LANG_ARABIC, SUBLANG_ARABIC_YEMEN, "Arabic (Yemen)"),
   LANG(LANG_ARABIC, SUBLANG_ARABIC_SYRIA, "Arabic (Syria)"),
   LANG(LANG_ARABIC, SUBLANG_ARABIC_JORDAN, "Arabic (Jordan)"),
   LANG(LANG_ARABIC, SUBLANG_ARABIC_LEBANON, "Arabic (Lebanon)"),
   LANG(LANG_ARABIC, SUBLANG_ARABIC_KUWAIT, "Arabic (Kuwait)"),
   LANG(LANG_ARABIC, SUBLANG_ARABIC_UAE, "Arabic (U.A.E)"),
   LANG(LANG_ARABIC, SUBLANG_ARABIC_BAHRAIN, "Arabic (Bahrain)"),
   LANG(LANG_ARABIC, SUBLANG_ARABIC_QATAR, "Arabic (Qatar)"),

   LANG(LANG_ARMENIAN, SUBLANG_NEUTRAL, "Armenian"),
   LANG(LANG_ASSAMESE, SUBLANG_NEUTRAL, "Assamese"),

   LANG(LANG_AZERI, SUBLANG_NEUTRAL, "Azeri"),
   LANG(LANG_AZERI, SUBLANG_AZERI_LATIN, "Azeri (Latin)"),
   LANG(LANG_AZERI, SUBLANG_AZERI_CYRILLIC, "Azeri (Cyrillic)"),

   LANG(LANG_BASQUE, SUBLANG_NEUTRAL, "Basque"),
   LANG(LANG_BELARUSIAN, SUBLANG_NEUTRAL, "Belarusian"),
   LANG(LANG_BENGALI, SUBLANG_NEUTRAL, "Bengali"),
   LANG(LANG_BULGARIAN, SUBLANG_NEUTRAL, "Bulgarian"),
   LANG(LANG_CATALAN, SUBLANG_NEUTRAL, "Catalan"),

   LANG(LANG_CHINESE, SUBLANG_NEUTRAL, "Chinese"),
   LANG(LANG_CHINESE, SUBLANG_CHINESE_TRADITIONAL, "Chinese (Taiwan Region)"),
   LANG(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED, "Chinese (PR China)"),
   LANG(LANG_CHINESE, SUBLANG_CHINESE_HONGKONG, "Chinese (Hong Kong)"),
   LANG(LANG_CHINESE, SUBLANG_CHINESE_SINGAPORE, "Chinese (Singapore)"),
   LANG(LANG_CHINESE, SUBLANG_CHINESE_MACAU, "Chinese (Macau)"),

   LANG(LANG_CROATIAN, SUBLANG_NEUTRAL, "Croatian"),
   LANG(LANG_CZECH, SUBLANG_NEUTRAL, "Czech"),
   LANG(LANG_DANISH, SUBLANG_NEUTRAL, "Danish"),

   LANG(LANG_DUTCH, SUBLANG_NEUTRAL, "Hollandais"),
   LANG(LANG_DUTCH, SUBLANG_DUTCH, "Hollandais (Hollande)"),
   LANG(LANG_DUTCH, SUBLANG_DUTCH_BELGIAN, "Hollandais (Belgique)"),

   LANG(LANG_ENGLISH, SUBLANG_NEUTRAL, "English"),
   LANG(LANG_ENGLISH, SUBLANG_ENGLISH_US, "English (USA)"),
   LANG(LANG_ENGLISH, SUBLANG_ENGLISH_UK, "English (UK)"),
   LANG(LANG_ENGLISH, SUBLANG_ENGLISH_AUS, "English (Australian)"),
   LANG(LANG_ENGLISH, SUBLANG_ENGLISH_CAN, "English (Canadian)"),
   LANG(LANG_ENGLISH, SUBLANG_ENGLISH_NZ, "English (New Zealand)"),
   LANG(LANG_ENGLISH, SUBLANG_ENGLISH_EIRE, "English (Irish)"),
   LANG(LANG_ENGLISH, SUBLANG_ENGLISH_SOUTH_AFRICA, "English (South Africa)"),
   LANG(LANG_ENGLISH, SUBLANG_ENGLISH_JAMAICA, "English (Jamaica)"),
   LANG(LANG_ENGLISH, SUBLANG_ENGLISH_CARIBBEAN, "English (Caribbean)"),
   LANG(LANG_ENGLISH, SUBLANG_ENGLISH_BELIZE, "English (Belize)"),
   LANG(LANG_ENGLISH, SUBLANG_ENGLISH_TRINIDAD, "English (Trinidad)"),
   LANG(LANG_ENGLISH, SUBLANG_ENGLISH_ZIMBABWE, "English (Zimbabwe)"),
   LANG(LANG_ENGLISH, SUBLANG_ENGLISH_PHILIPPINES, "English (Philippines)"),

   LANG(LANG_ESTONIAN, SUBLANG_NEUTRAL, "Estonian"),
   LANG(LANG_FAEROESE, SUBLANG_NEUTRAL, "Faeroese"),
   LANG(LANG_FARSI, SUBLANG_NEUTRAL, "Farsi"),
   LANG(LANG_FINNISH, SUBLANG_NEUTRAL, "Finnish"),

   LANG(LANG_FRENCH, SUBLANG_NEUTRAL, "Français"),
   LANG(LANG_FRENCH, SUBLANG_FRENCH, "Français (France)"),
   LANG(LANG_FRENCH, SUBLANG_FRENCH_BELGIAN, "Français (Belgique)"),
   LANG(LANG_FRENCH, SUBLANG_FRENCH_CANADIAN, "Français (Canada)"),
   LANG(LANG_FRENCH, SUBLANG_FRENCH_SWISS, "Français (Suisse)"),
   LANG(LANG_FRENCH, SUBLANG_FRENCH_LUXEMBOURG, "Français (Luxembourg)"),
   LANG(LANG_FRENCH, SUBLANG_FRENCH_MONACO, "Français (Monaco)"),

   LANG(LANG_GEORGIAN, SUBLANG_NEUTRAL, "Georgian"),

   LANG(LANG_GERMAN, SUBLANG_NEUTRAL, "Allemand"),
   LANG(LANG_GERMAN, SUBLANG_GERMAN, "Allemand (Allemagne)"),
   LANG(LANG_GERMAN, SUBLANG_GERMAN_SWISS, "Allemand (Suisse)"),
   LANG(LANG_GERMAN, SUBLANG_GERMAN_AUSTRIAN, "Allemand (Austriche)"),
   LANG(LANG_GERMAN, SUBLANG_GERMAN_LUXEMBOURG, "Allemand (Luxembourg)"),
   LANG(LANG_GERMAN, SUBLANG_GERMAN_LIECHTENSTEIN, "Allemand (Liechtenstein)"),

   LANG(LANG_GREEK, SUBLANG_NEUTRAL, "Greek"),
   LANG(LANG_GUJARATI, SUBLANG_NEUTRAL, "Gujarati"),
   LANG(LANG_HEBREW, SUBLANG_NEUTRAL, "Hebrew"),
   LANG(LANG_HINDI, SUBLANG_NEUTRAL, "Hindi"),
   LANG(LANG_HUNGARIAN, SUBLANG_NEUTRAL, "Hungarian"),
   LANG(LANG_ICELANDIC, SUBLANG_NEUTRAL, "Icelandic"),
   LANG(LANG_INDONESIAN, SUBLANG_NEUTRAL, "Indonesian"),

   LANG(LANG_ITALIAN, SUBLANG_NEUTRAL, "Italien"),
   LANG(LANG_ITALIAN, SUBLANG_ITALIAN, "Italien (Italie)"),
   LANG(LANG_ITALIAN, SUBLANG_ITALIAN_SWISS, "Italien (Suisse)"),

   LANG(LANG_JAPANESE, SUBLANG_NEUTRAL, "Japonese"),
   LANG(LANG_KANNADA, SUBLANG_NEUTRAL, "Kanada"),

   LANG(LANG_KASHMIRI, SUBLANG_NEUTRAL, "Kashmiri"),
   LANG(LANG_KASHMIRI, SUBLANG_KASHMIRI_INDIA, "Kashmiri (India)"),

   LANG(LANG_KAZAK, SUBLANG_NEUTRAL, "Kazak"),
   LANG(LANG_KONKANI, SUBLANG_NEUTRAL, "Konkani"),

   LANG(LANG_KOREAN, SUBLANG_NEUTRAL, "Korean"),
   LANG(LANG_KOREAN, SUBLANG_KOREAN, "Korean (Extended Wansung)"),

   LANG(LANG_LATVIAN, SUBLANG_NEUTRAL, "Latvian"),

   LANG(LANG_LITHUANIAN, SUBLANG_NEUTRAL, "Lithuanian"),
   LANG(LANG_LITHUANIAN, SUBLANG_LITHUANIAN, "Lithuanian"),
//   LANG(LANG_LITHUANIAN, SUBLANG_LITHUANIAN_CLASSIC, "Lithuanian (Classic)"),

   LANG(LANG_MACEDONIAN, SUBLANG_NEUTRAL, "Macedonian"),

   LANG(LANG_MALAY, SUBLANG_NEUTRAL, "Malay"),
   LANG(LANG_MALAY, SUBLANG_MALAY_MALAYSIA, "Malay (Malaysia)"),
   LANG(LANG_MALAY, SUBLANG_MALAY_BRUNEI_DARUSSALAM, "Malay (Brunei Darussalam)"),

   LANG(LANG_MALAYALAM, SUBLANG_NEUTRAL, "Malayalam"),
   LANG(LANG_MANIPURI, SUBLANG_NEUTRAL, "Manipuri"),
   LANG(LANG_MARATHI, SUBLANG_NEUTRAL, "Marathi"),

   LANG(LANG_NEPALI, SUBLANG_NEUTRAL, "Nepali"),
   LANG(LANG_NEPALI, SUBLANG_NEPALI_INDIA, "Nepali (India)"),

   LANG(LANG_NORWEGIAN, SUBLANG_NEUTRAL, "Norwegian"),
   LANG(LANG_NORWEGIAN, SUBLANG_NORWEGIAN_BOKMAL, "Norwegian (Bokmal)"),
   LANG(LANG_NORWEGIAN, SUBLANG_NORWEGIAN_NYNORSK, "Norwegian (Nynorsk)"),

   LANG(LANG_ORIYA, SUBLANG_NEUTRAL, "Oriya"),
   LANG(LANG_POLISH, SUBLANG_NEUTRAL, "Polish"),

   LANG(LANG_PORTUGUESE, SUBLANG_NEUTRAL, "Portugais"),
   LANG(LANG_PORTUGUESE, SUBLANG_PORTUGUESE, "Portugais (Portugal)"),
   LANG(LANG_PORTUGUESE, SUBLANG_PORTUGUESE_BRAZILIAN, "Portugais (Bresil)"),

   LANG(LANG_PUNJABI, SUBLANG_NEUTRAL, "Punjabi"),
   LANG(LANG_ROMANIAN, SUBLANG_NEUTRAL, "Romanian"),
   LANG(LANG_RUSSIAN, SUBLANG_NEUTRAL, "Russian"),
   LANG(LANG_SANSKRIT, SUBLANG_NEUTRAL, "Sanskrit"),

   LANG(LANG_SERBIAN, SUBLANG_NEUTRAL, "Serbian"),
   LANG(LANG_SERBIAN, SUBLANG_SERBIAN_LATIN, "Serbian (Latin)"),
   LANG(LANG_SERBIAN, SUBLANG_SERBIAN_CYRILLIC, "Serbian (Cyrillic)"),

   LANG(LANG_SINDHI, SUBLANG_NEUTRAL, "Sindhi"),
   LANG(LANG_SLOVAK, SUBLANG_NEUTRAL, "Slovak"),
   LANG(LANG_SLOVENIAN, SUBLANG_NEUTRAL, "Slovenian"),

   LANG(LANG_SPANISH, SUBLANG_NEUTRAL, "Spanish"),
   LANG(LANG_SPANISH, SUBLANG_SPANISH, "Spanish (Castilian)"),
   LANG(LANG_SPANISH, SUBLANG_SPANISH_MEXICAN, "Spanish (Mexican)"),
   LANG(LANG_SPANISH, SUBLANG_SPANISH_MODERN, "Spanish (Modern)"),
   LANG(LANG_SPANISH, SUBLANG_SPANISH_GUATEMALA, "Spanish (Guatemala)"),
   LANG(LANG_SPANISH, SUBLANG_SPANISH_COSTA_RICA, "Spanish (Costa Rica)"),
   LANG(LANG_SPANISH, SUBLANG_SPANISH_PANAMA, "Spanish (Panama)"),
   LANG(LANG_SPANISH, SUBLANG_SPANISH_DOMINICAN_REPUBLIC, "Spanish (Dominican Republic)"),
   LANG(LANG_SPANISH, SUBLANG_SPANISH_VENEZUELA, "Spanish (Venezuela)"),
   LANG(LANG_SPANISH, SUBLANG_SPANISH_COLOMBIA, "Spanish (Colombia)"),
   LANG(LANG_SPANISH, SUBLANG_SPANISH_PERU, "Spanish (Peru)"),
   LANG(LANG_SPANISH, SUBLANG_SPANISH_ARGENTINA, "Spanish (Argentina)"),
   LANG(LANG_SPANISH, SUBLANG_SPANISH_ECUADOR, "Spanish (Ecuador)"),
   LANG(LANG_SPANISH, SUBLANG_SPANISH_CHILE, "Spanish (Chile)"),
   LANG(LANG_SPANISH, SUBLANG_SPANISH_URUGUAY, "Spanish (Uruguay)"),
   LANG(LANG_SPANISH, SUBLANG_SPANISH_PARAGUAY, "Spanish (Paraguay)"),
   LANG(LANG_SPANISH, SUBLANG_SPANISH_BOLIVIA, "Spanish (Bolivia)"),
   LANG(LANG_SPANISH, SUBLANG_SPANISH_EL_SALVADOR, "Spanish (El Salvador)"),
   LANG(LANG_SPANISH, SUBLANG_SPANISH_HONDURAS, "Spanish (Honduras)"),
   LANG(LANG_SPANISH, SUBLANG_SPANISH_NICARAGUA, "Spanish (Nicaragua)"),
   LANG(LANG_SPANISH, SUBLANG_SPANISH_PUERTO_RICO, "Spanish (Puerto Rico)"),

   LANG(LANG_SWAHILI, SUBLANG_NEUTRAL, "Swahili"),

   LANG(LANG_SWEDISH, SUBLANG_NEUTRAL, "Suèdois"),
   LANG(LANG_SWEDISH, SUBLANG_SWEDISH, "Suèdois (Suède)"),
   LANG(LANG_SWEDISH, SUBLANG_SWEDISH_FINLAND, "Suèdois (Finland)"),

   LANG(LANG_TAMIL, SUBLANG_NEUTRAL, "Tamil"),
   LANG(LANG_TATAR, SUBLANG_NEUTRAL, "Tatar"),
   LANG(LANG_TELUGU, SUBLANG_NEUTRAL, "Telugu"),
   LANG(LANG_THAI, SUBLANG_NEUTRAL, "Thai"),
   LANG(LANG_TURKISH, SUBLANG_NEUTRAL, "Turkish"),
   LANG(LANG_UKRAINIAN, SUBLANG_NEUTRAL, "Ukrainian"),

   LANG(LANG_URDU, SUBLANG_NEUTRAL, "Urdu"),
   LANG(LANG_URDU, SUBLANG_URDU_PAKISTAN, "Urdu (Pakistan)"),
   LANG(LANG_URDU, SUBLANG_URDU_INDIA, "Urdu (India)"),

   LANG(LANG_UZBEK, SUBLANG_NEUTRAL, "Uzbek"),
   LANG(LANG_UZBEK, SUBLANG_UZBEK_LATIN, "Uzbek (Latin)"),
   LANG(LANG_UZBEK, SUBLANG_UZBEK_CYRILLIC, "Uzbek (Cyrillic)"),

   LANG(LANG_VIETNAMESE, SUBLANG_NEUTRAL, "Vietnamese"),

   {0, NULL}
};

LPCTSTR GetLang(WORD code)
{
   t_LANGTBL* pL = langTbl;
   while (pL->msg) {
      if (pL->code == code) return pL->msg;
      pL++;
   }
   pL = langTbl;
   while (pL->msg) {
      if ( PRIMARYLANGID(pL->code ^ code) == 0 ) return pL->msg;
      pL++;
   }
   return "";
}
///////////////////////////////////////////////////////////////////////////////
// Accelerator

typedef struct {
	WORD    wKey;
	LPCTSTR szKey;
} MAPVIRTKEYS;

const static MAPVIRTKEYS mapVirtKeys[] = {
   {VK_LBUTTON, "LBUTTON"},   {VK_RBUTTON, "RBUTTON"},   {VK_CANCEL,  "CANCEL"},
   {VK_MBUTTON, "MBUTTON"},   {VK_BACK,    "BACK"},      {VK_TAB,     "TAB"},
   {VK_CLEAR,   "CLEAR"},     {VK_RETURN,  "RETURN"},    {VK_SHIFT,   "SHIFT"},
   {VK_CONTROL, "CONTROL"},   {VK_MENU,    "MENU"},      {VK_PAUSE,   "PAUSE"},
   {VK_CAPITAL, "CAPITAL"},   {VK_ESCAPE,  "ESCAPE"},    {VK_SPACE,   "SPACE"},
   {VK_PRIOR,   "PRIOR"},     {VK_NEXT,    "NEXT"},      {VK_END,     "END"},
   {VK_HOME,    "HOME"},      {VK_LEFT,    "LEFT"},      {VK_UP,      "UP"},
   {VK_RIGHT,   "RIGHT"},     {VK_DOWN,    "DOWN"},      {VK_SELECT,  "SELECT"},
   {VK_PRINT,   "PRINT"},     {VK_EXECUTE, "EXECUTE"},   {VK_SNAPSHOT,"SNAPSHOT"},
   {VK_INSERT,  "INSERT"},    {VK_DELETE,  "DELETE"},    {VK_HELP,    "HELP"},
   {'0', "0"},  {'1', "1"},   {'2', "2"},  {'3', "3"},   {'4', "4"},  {'5', "5"},
   {'6', "6"},  {'7', "7"},   {'8', "8"},  {'9', "9"},   {'A', "A"},  {'B', "B"},
   {'C', "C"},  {'D', "D"},   {'E', "E"},  {'F', "F"},   {'G', "G"},  {'H', "H"},
   {'I', "I"},  {'J', "J"},   {'K', "K"},  {'L', "L"},   {'M', "M"},  {'N', "N"},
	{'O', "O"},  {'P', "P"},   {'Q', "Q"},  {'R', "R"},   {'S', "S"},  {'T', "T"},
	{'U', "U"},  {'V', "V"},   {'W', "W"},  {'X', "X"},   {'Y', "Y"},  {'Z', "Z"},
	{VK_LWIN,    "LWIN"},      {VK_RWIN,    "RWIN"},      {VK_APPS,    "APPS"},
   {VK_NUMPAD0, "NUMPAD0"},   {VK_NUMPAD1, "NUMPAD1"},   {VK_NUMPAD2, "NUMPAD2"},
   {VK_NUMPAD3, "NUMPAD3"},   {VK_NUMPAD4, "NUMPAD4"},   {VK_NUMPAD5, "NUMPAD5"},
   {VK_NUMPAD6, "NUMPAD6"},   {VK_NUMPAD7, "NUMPAD7"},   {VK_NUMPAD8, "NUMPAD8"},
	{VK_NUMPAD9, "NUMPAD9"},   {VK_MULTIPLY,"MULTIPLY"},  {VK_ADD,     "ADD"},
   {VK_SUBTRACT,"SUBTRACT"},  {VK_DECIMAL, "DECIMAL"},   {VK_DIVIDE,  "DIVIDE"},
   {VK_NUMLOCK, "NUMLOCK"},   {VK_SCROLL,  "SCROLL"},    {VK_ATTN,    "ATTN"},
   {VK_CRSEL,   "CRSEL"},     {VK_EXSEL,   "EXSEL"},     {VK_EREOF,   "EREOF"},
   {VK_PLAY,    "PLAY"},      {VK_ZOOM,    "ZOOM"},      {VK_PA1,     "PA1"},
	{VK_SEPARATOR,"SEPARATOR"},{VK_NONAME,  "NONAME"},    {VK_OEM_CLEAR,"OEM_CLEAR"},
   {VK_F1, "F1" }, {VK_F2, "F2" }, {VK_F3, "F3" }, {VK_F4, "F4" }, {VK_F5,  "F5"},
   {VK_F6, "F6" }, {VK_F7, "F7" }, {VK_F8, "F8" }, {VK_F9, "F9" }, {VK_F10,"F10"},
   {VK_F11,"F11"}, {VK_F12,"F12"}, {VK_F13,"F13"}, {VK_F14,"F14"}, {VK_F15,"F15"},
   {VK_F16,"F16"}, {VK_F17,"F17"}, {VK_F18,"F18"}, {VK_F19,"F19"}, {VK_F20,"F20"},
   {VK_F21,"F21"}, {VK_F22,"F22"}, {VK_F23,"F23"}, {VK_F24,"F24"}, {0, NULL}
};

CString GetKey(RACCEL *pAc, BOOL quote)
{
   // virtual key
   CString tmp;
   for (const MAPVIRTKEYS *pt = mapVirtKeys; pt->szKey; pt++)
      if (pt->wKey == pAc->key) {
         tmp.Format(quote? "VK_%s": "%s", pt->szKey);
         return tmp;
      }
   // codes speciaux
   tmp.Format(quote? "\"\\x%02X\"": "\\x%02X", pAc->key);
   return tmp;
}

CString DisplayKbType(RACCEL *pAc)
{
   if (pAc->fVirt & FVIRTKEY) return "VIRTKEY";
   else return "ASCII";
}

CString GetControl(RACCEL *pAc)
{
   CString control = DisplayKbType(pAc);
   if (pAc->fVirt & FCONTROL) control += ", CONTROL"; 
   if (pAc->fVirt & FALT)     control += ", ALT";
   if (pAc->fVirt & FSHIFT)   control += ", SHIFT";
   return control;
}

CString DisplayKbStr(RACCEL *pAc)
{
   CString strKbd;
   // modifiers
   if (pAc->fVirt & FCONTROL)  strKbd = "Ctrl";
   if (pAc->fVirt & FALT)      strKbd = strKbd.IsEmpty()? "Alt":   strKbd + " + Alt";
   if (pAc->fVirt & FSHIFT)    strKbd = strKbd.IsEmpty()? "Shift": strKbd + " + Shift";

   // virtual key
   CString key = GetKey(pAc, FALSE);
   strKbd = strKbd.IsEmpty()? key: strKbd + " + " + key;

   return strKbd;
}

CString* DumpAccel(HINSTANCE hinst, LPCTSTR resName)
{
   static CString buffer;

   buffer.Empty();

   HRSRC hsrc = ::FindResource(hinst, resName, RT_ACCELERATOR);
   if (hsrc) {
      HGLOBAL hg = ::LoadResource(hinst, hsrc);
      int nb = ::SizeofResource(hinst, hsrc) / sizeof(RACCEL);
      RACCEL* pAc = (RACCEL*)::LockResource(hg);

      buffer.Format("IDC_%s", GetResStr(resName));
      buffer += " ACCELERATORS PRELOAD MOVEABLE PURE\r\nBEGIN\r\n";
      for ( ; --nb >= 0; pAc++) {
         Tab(buffer, 4);
         buffer += GetKey(pAc, TRUE);
         Tab(buffer, 18);
         buffer += FormatId(pAc->cmd);
         Tab(buffer, 36);
         buffer += GetControl(pAc) + "\r\n";
      }
      buffer += "END\r\n";
   }
   return &buffer;
}
