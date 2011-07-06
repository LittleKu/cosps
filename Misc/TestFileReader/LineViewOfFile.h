/***************************************************************************/
/* NOTE:                                                                   */
/* This document is copyright (c) by Oz Solomon, and is bound by the MIT   */
/* open source license (www.opensource.org/licenses/mit-license.html).     */
/* See License.txt for more information.                                   */
/***************************************************************************/

#ifndef __LINEVIEWOFFILE_H
#define __LINEVIEWOFFILE_H

#include "ReadOnlyMemMappedFile.h"

class CLineViewOfFile : public CReadOnlyMemMappedFile
{
protected:
    DWORD m_iFilePtr;
    enum { 
        Unknown = -1,
        NonUnicode = 0,
        UTF16_LittleEndian = 1,
        UTF16_BigEndian = 2,
        UTF8 = 3
    } m_Unicodeness;
    CString m_Line;

    bool GetNextChar(unsigned char& ch);
    void DetectUnicodeness();

public:
    CLineViewOfFile(LPCTSTR pszFileName);

    bool GetNextLine(const class CString*& outLine);
};


#endif // __LINEVIEWOFFILE_H
