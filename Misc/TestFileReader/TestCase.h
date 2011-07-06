#ifndef _TEST_CASE_H_
#define _TEST_CASE_H_

#pragma once


#define RF_IN_PLC		0
#define RF_IN_STL		1
#define RF_IN_MFC		2
#define RF_IN_MFC2		3


BOOL ReadFile(LPCTSTR lpFileName, int nMethod);

#endif