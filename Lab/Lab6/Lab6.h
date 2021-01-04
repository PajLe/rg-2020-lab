
// Lab6.h : main header file for the Lab6 application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CLab6App:
// See Lab6.cpp for the implementation of this class
//

class CLab6App : public CWinApp
{
public:
	CLab6App() noexcept;


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CLab6App theApp;
