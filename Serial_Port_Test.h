
// Serial_Port_Test.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CSerial_Port_TestApp:
// See Serial_Port_Test.cpp for the implementation of this class
//

class CSerial_Port_TestApp : public CWinApp
{
public:
	CSerial_Port_TestApp();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CSerial_Port_TestApp theApp;