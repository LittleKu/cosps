// Converter.h: interface for the CConverter class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CONVERTER_H__FAA6CBD7_E27B_4014_ACD0_FDC87C86E696__INCLUDED_)
#define AFX_CONVERTER_H__FAA6CBD7_E27B_4014_ACD0_FDC87C86E696__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class OptionContext;

class CConverter  
{
public:
	CConverter();
	virtual ~CConverter();

	virtual int Convert(LPCTSTR lpszInputFile) = 0;
};

class CDefaultConverter : public CConverter
{
public:
	CDefaultConverter();
	virtual ~CDefaultConverter();

	virtual int Convert(LPCTSTR lpszInputFile);

private:
	bool ProcessSize(OptionContext* pContext);
};

#endif // !defined(AFX_CONVERTER_H__FAA6CBD7_E27B_4014_ACD0_FDC87C86E696__INCLUDED_)
