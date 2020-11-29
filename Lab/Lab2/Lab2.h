
// Lab2.h : main header file for the Lab2 application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CLab2App:
// See Lab2.cpp for the implementation of this class
//

class CLab2App : public CWinApp
{
public:
	CLab2App() noexcept;


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CLab2App theApp;
