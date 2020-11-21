
// Lab1.h : main header file for the Lab1 application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CLab1App:
// See Lab1.cpp for the implementation of this class
//

class CLab1App : public CWinApp
{
public:
	CLab1App() noexcept;


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CLab1App theApp;
