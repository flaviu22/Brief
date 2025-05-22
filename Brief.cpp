// Brief.cpp : Defines the class behaviors for the application.
//

#include "pch.h"
#include <afx.h>
#include "framework.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
#include "Brief.h"
#include "MainFrm.h"

#include "BriefDoc.h"
#include "BriefView.h"

#pragma comment(lib, "version.lib")

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CBriefApp

BEGIN_MESSAGE_MAP(CBriefApp, CWinAppEx)
	ON_COMMAND(ID_APP_ABOUT, &CBriefApp::OnAppAbout)
	// Standard file based document commands
//	ON_COMMAND(ID_FILE_NEW, &CWinAppEx::OnFileNew)
//	ON_COMMAND(ID_FILE_OPEN, &CWinAppEx::OnFileOpen)
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, &CWinAppEx::OnFilePrintSetup)
END_MESSAGE_MAP()

// CBriefApp construction

CBriefApp::CBriefApp() noexcept
{
	m_bHiColorIcons = TRUE;

	m_nAppLook = 0;
	// support Restart Manager
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_ALL_ASPECTS;
#ifdef _MANAGED
	// If the application is built using Common Language Runtime support (/clr):
	//     1) This additional setting is needed for Restart Manager support to work properly.
	//     2) In your project, you must add a reference to System.Windows.Forms in order to build.
	System::Windows::Forms::Application::SetUnhandledExceptionMode(System::Windows::Forms::UnhandledExceptionMode::ThrowException);
#endif

	// TODO: replace application ID string below with unique ID string; recommended
	// format for string is CompanyName.ProductName.SubProduct.VersionInformation
	SetAppID(_T("Brief.AppID.NoVersion"));

	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

// The one and only CBriefApp object

CBriefApp theApp;


// CBriefApp initialization

BOOL CBriefApp::InitInstance()
{
	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();

	// Initialize OLE libraries
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}

	AfxEnableControlContainer();

	EnableTaskbarInteraction(FALSE);

	// AfxInitRichEdit2() is required to use RichEdit control
	// AfxInitRichEdit2();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T("Sinis"));
	LoadStdProfileSettings(16);  // Load standard INI file options (including MRU)
	InitContextMenuManager();
	InitKeyboardManager();
	InitTooltipManager();

	CMFCToolTipInfo ttParams;
	ttParams.m_bVislManagerTheme = TRUE;
	theApp.GetTooltipManager()->SetTooltipParams(AFX_TOOLTIP_TYPE_ALL,
		RUNTIME_CLASS(CMFCToolTipCtrl), &ttParams);

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views
	CSingleDocTemplate* pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CBriefDoc),
		RUNTIME_CLASS(CMainFrame),       // main SDI frame window
		RUNTIME_CLASS(CBriefView));
	if (! pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);

#ifdef _DEBUG
	AfxEnableMemoryLeakDump(FALSE);
#endif
	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// Dispatch commands specified on the command line.  Will return FALSE if
	// app was launched with /RegServer, /Register, /Unregserver or /Unregister.
	if (! ProcessShellCommand(cmdInfo))
		return FALSE;

	// The one and only window has been initialized, so show and update it
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();
	return TRUE;
}

int CBriefApp::ExitInstance()
{
	//TODO: handle additional resources you may have added
	AfxOleTerm(FALSE);

	return CWinAppEx::ExitInstance();
}

// CBriefApp message handlers

// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg() noexcept;

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	CString GetAppVersion(CString sFileName, BOOL bShortVersion = TRUE) const;

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() noexcept : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BOOL CAboutDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here

	CString sFileName, sText;
	GetModuleFileName(NULL, sFileName.GetBuffer(_MAX_PATH), _MAX_PATH);
	sFileName.ReleaseBuffer();
	sText.Format(_T("%s, Version %s"), theApp.m_pszAppName, GetAppVersion(sFileName));
	GetDlgItem(IDC_STATIC2)->SetWindowText(sText);

	sText.Format(_T("Copyright (C) %s"), COleDateTime::GetCurrentTime().Format(_T("%Y")));
	GetDlgItem(IDC_STATIC3)->SetWindowText(sText);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

CString CAboutDlg::GetAppVersion(CString sFileName, BOOL bShortVersion/* = TRUE*/) const
{
	CString sVersion;
	DWORD dwDummy = 0;

	try
	{
		const auto size = ::GetFileVersionInfoSize(sFileName.GetBuffer(_MAX_PATH), &dwDummy);
		sFileName.ReleaseBuffer();
		std::vector<LPBYTE> lpData(size);
		if (::GetFileVersionInfo(sFileName, 0, size, lpData.data()))
		{
			UINT nLenght = 0;
			VS_FIXEDFILEINFO* lpFfi;
			VerQueryValue(lpData.data(), _T("\\"), (LPVOID*)&lpFfi, &nLenght);
			DWORD dwFileVersionMS = lpFfi->dwFileVersionMS;
			DWORD dwFileVersionLS = lpFfi->dwFileVersionLS;
			DWORD dwLeftMost = HIWORD(dwFileVersionMS);
			DWORD dwSecondLeft = LOWORD(dwFileVersionMS);
			DWORD dwSecondRight = HIWORD(dwFileVersionLS);
			DWORD dwRightMost = LOWORD(dwFileVersionLS);
			if (bShortVersion)
				sVersion.Format(_T("%d.%d"), dwLeftMost, dwSecondLeft);
			else
				sVersion.Format(_T("%d.%d.%d.%d"), dwLeftMost, dwSecondLeft, dwSecondRight, dwRightMost);
		}
	}
	catch (CException* pException)
	{
		pException->Delete();
	}

	return sVersion;
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

// App command to run the dialog
void CBriefApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// CBriefApp customization load/save methods

void CBriefApp::PreLoadState()
{
	BOOL bNameValid;
	CString strName;
	bNameValid = strName.LoadString(IDS_EDIT_MENU);
	ASSERT(bNameValid);
	GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EDIT);
}

void CBriefApp::LoadCustomState()
{
}

void CBriefApp::SaveCustomState()
{
}

// CBriefApp message handlers
