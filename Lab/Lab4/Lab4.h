
// Lab4.h : main header file for the Lab4 application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CLab4App:
// See Lab4.cpp for the implementation of this class
//

class CLab4App : public CWinApp
{
public:
	CLab4App() noexcept;


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CLab4App theApp;
