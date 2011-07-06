/***************************************************************************/
/* NOTE:                                                                   */
/* This document is copyright (c) by Oz Solomon, and is bound by the MIT   */
/* open source license (www.opensource.org/licenses/mit-license.html).     */
/* See License.txt for more information.                                   */
/***************************************************************************/

#include "stdafx.h"
#include "LineViewOfFile.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

static const short int utf8_byte_count[] = {
    // 192 - 223 - 2 bytes
          2, 2, 2, 2, 2, 2, 2, 2,
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    2, 2, 2, 2,

    // 224 - 239 - 3 bytes
                3, 3, 3, 3, 3, 3,
    3, 3, 3, 3, 3, 3, 3, 3, 3, 3,

    // 240 - 247 - 4 bytes
    4, 4, 4, 4, 4, 4, 4, 4,

    // 248 - 251 - 5 bytes
                            5, 5,
    5, 5,

    // 252 - 253 - 6 bytes
           6, 6
};
    

CLineViewOfFile::CLineViewOfFile(LPCTSTR pszFileName) : 
    CReadOnlyMemMappedFile(pszFileName), m_iFilePtr(0), m_Unicodeness(Unknown)
{
    DetectUnicodeness();
}

// tries to figure out if this is a Unicode file by reading the first 3 
// bytes to look for a byte-order mask
void CLineViewOfFile::DetectUnicodeness()
{
    ASSERT(m_iFilePtr == 0);

    const BYTE * const m_pbCurr = m_pBuf + m_iFilePtr;
    const int bytes_left = GetFileSize() - m_iFilePtr;

    if (bytes_left >= 2)
    {
        const BYTE ch1 = *(m_pbCurr);
        const BYTE ch2 = *(m_pbCurr + 1);

        if (ch1 == 0xFE  &&  ch2 == 0xFF)
        {
            m_Unicodeness = UTF16_BigEndian;
            m_iFilePtr += 2;
            return;
        }

        if (ch1 == 0xFF  &&  ch2 == 0xFE)
        {
            m_Unicodeness = UTF16_LittleEndian;
            m_iFilePtr += 2;
            return;
        }

        if (bytes_left >= 3)
        {
            const BYTE ch3 = *(m_pbCurr + 2);
            if (ch1 == 0xEF  &&  ch2 == 0xBB  &&  ch3 == 0xBF)
            {
                m_Unicodeness = UTF8;
                m_iFilePtr += 3;
                return;
            }
        }
    }

    m_Unicodeness = NonUnicode;
}

// This function will return the next character in the file.  It also tries
// to figure out if the file is Unicode encoded.
// The Unicode strategy is to return only characters in the ASCII range 
// (<=127) and to return 0xFF for any character out of that range.  This is
// fine since the characters we really care about for counting are in the
// ASCII range.
bool CLineViewOfFile::GetNextChar(unsigned char& outChar)
{
    const BYTE * const m_pbCurr = m_pBuf + m_iFilePtr;
    const int bytes_left = GetFileSize() - m_iFilePtr;

    if (bytes_left < 1) return false;

    switch (m_Unicodeness)
    {
        case Unknown:
        {
            ASSERT(false);
            outChar = 0xFF;
            return false;
        }

        case NonUnicode:
        {
            outChar = *(m_pbCurr);
            ++m_iFilePtr;
            break;
        }

        case UTF16_BigEndian:
        {
            if (bytes_left < 2) return false;
            outChar = (*(m_pbCurr) == 0)? *(m_pbCurr + 1) : 0xFF;
            m_iFilePtr += 2;
            break;
        }

        case UTF16_LittleEndian:
        {
            if (bytes_left < 2) return false;
            outChar = (*(m_pbCurr + 1) == 0)? *(m_pbCurr) : 0xFF;
            m_iFilePtr += 2;
            break;
        }

        case UTF8:
        {
            outChar = *m_pbCurr;
            // figure out how many bytes this character was encoded with
            if (outChar >= 192  &&  outChar <= 253)
            {
                const int num_bytes = utf8_byte_count[outChar - 192];
                if (bytes_left < num_bytes)
                {
                    return false;
                }
                m_iFilePtr += num_bytes;
                outChar = (unsigned char)0xFF;
            }
            else
            {
                // 1 byte
                ++m_iFilePtr;
            }
            break;
        }

    }
    return true;
}

bool CLineViewOfFile::GetNextLine(const CString *& outLine)
{
    unsigned char ch;
    bool is_eof;
    m_Line.Empty();
    outLine = &m_Line;

    // find first end of line character
    for (;;)
    {
        is_eof = !GetNextChar(ch);
        if (is_eof) break;
        if (ch == '\n'  ||  ch == '\r') break;
        m_Line += ch;
    }

    // peek ahead for \r\n or \n\r combination
    DWORD iCurrFilePtr = m_iFilePtr;
    unsigned char ch_next;
    if (GetNextChar(ch_next))
    {
        if (!(ch == '\r'  &&  ch_next == '\n')  &&
            !(ch == '\n'  &&  ch_next == '\r'))
        {
            // backtrack
            m_iFilePtr = iCurrFilePtr;
        }
    }

    // post-condition assertion
    ASSERT(m_iFilePtr <= GetFileSize() &&  m_iFilePtr > 0);

    return !is_eof  ||  !m_Line.IsEmpty();
}
