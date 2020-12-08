
// Lab3.h : main header file for the Lab3 application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CLab3App:
// See Lab3.cpp for the implementation of this class
//

class CLab3App : public CWinApp
{
public:
	CLab3App() noexcept;


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CLab3App theApp;
