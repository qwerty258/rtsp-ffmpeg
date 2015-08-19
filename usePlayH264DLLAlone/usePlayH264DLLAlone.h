
// usePlayH264DLLAlone.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CusePlayH264DLLAloneApp:
// See usePlayH264DLLAlone.cpp for the implementation of this class
//

class CusePlayH264DLLAloneApp : public CWinApp
{
public:
	CusePlayH264DLLAloneApp();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CusePlayH264DLLAloneApp theApp;