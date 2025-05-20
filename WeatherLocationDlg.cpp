// CWeatherLocationDlg.cpp : implementation file
//

#include "pch.h"
#include "Brief.h"
#include "WeatherLocationDlg.h"
#include "afxdialogex.h"

#define WEATHERLOCATIONDLG_DEFAULTMINCLIENTSIZEX		750
#define WEATHERLOCATIONDLG_DEFAULTMINCLIENTSIZEY		250

// CWeatherLocationDlg dialog

IMPLEMENT_DYNAMIC(CWeatherLocationDlg, CDialogEx)

CWeatherLocationDlg::CWeatherLocationDlg(CBriefDoc* pDoc, CWnd* pParent/* = nullptr*/)
	:CDialogEx(IDD_DIALOG_LOCATION, pParent)
	,m_pDoc(pDoc)
{

}

void CWeatherLocationDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST, m_List);
	DDX_Text(pDX, IDC_EDIT_TEXT, m_sText);
}

BEGIN_ANCHOR_MAP(CWeatherLocationDlg)
	ANCHOR_MAP_ENTRY(IDC_EDIT_TEXT, AMF_TOP | AMF_LEFT | AMF_RIGHT)
	ANCHOR_MAP_ENTRY(IDC_LIST, AMF_TOP | AMF_LEFT | AMF_RIGHT | AMF_BOTTOM)
	ANCHOR_MAP_ENTRY(IDOK, AMF_AUTOMATIC)
	ANCHOR_MAP_ENTRY(IDCANCEL, AMF_AUTOMATIC)
END_ANCHOR_MAP()

BEGIN_MESSAGE_MAP(CWeatherLocationDlg, CDialogEx)
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_TIMER()
	ON_WM_GETMINMAXINFO()
	ON_EN_CHANGE(IDC_EDIT_TEXT, &CWeatherLocationDlg::OnEnChangeEditText)
    ON_NOTIFY(NM_DBLCLK, IDC_LIST, &CWeatherLocationDlg::OnDblclkList)
END_MESSAGE_MAP()

// CWeatherLocationDlg message handlers

BOOL CWeatherLocationDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class

	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_DOWN && GetDlgItem(IDC_EDIT_TEXT) == GetFocus())
		{
			if (m_List.GetItemCount() > 0)
			{
				if (0 == m_List.GetSelectedCount())
				{
					m_List.SetItemState(0, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
					m_List.SetFocus();
				}
				else
				{
					NextDlgCtrl();
				}
				return TRUE;
			}
		}
		if (pMsg->wParam == VK_ESCAPE && GetDlgItem(IDC_LIST) == GetFocus())
		{
			PrevDlgCtrl();
			return TRUE;
		}
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}

BOOL CWeatherLocationDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO: Add extra initialization here

	m_hIcon = AfxGetApp()->LoadIcon(IDR_BriefTYPE);
	SetIcon(m_hIcon, FALSE);

	DWORD dwExStyle = LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_INFOTIP;
	m_List.SetExtendedStyle(dwExStyle);

	m_List.InsertColumn(0, _T("Name"), LVCFMT_LEFT, 170);
	m_List.InsertColumn(1, _T("Adm_area1"), LVCFMT_LEFT, 170);
	m_List.InsertColumn(2, _T("Adm_area2"), LVCFMT_LEFT, 170);
	m_List.InsertColumn(3, _T("Country"), LVCFMT_LEFT, 170);
	m_List.InsertColumn(4, _T("Place ID"), LVCFMT_LEFT, 0);

	InitAnchors(AMIF_SIZEGRIP);

	UINT nBytes = 0;
	std::unique_ptr<WINDOWPLACEMENT> lwp;
	if (AfxGetApp()->GetProfileBinary(_T("Settings"),
		_T("WindowPlacementWeatherLocationDlg"), reinterpret_cast<LPBYTE*>(&lwp), &nBytes))
		SetWindowPlacement(lwp.get());

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}

void CWeatherLocationDlg::OnDestroy()
{
	CDialog::OnDestroy();

	// TODO: Add your message handler code here

	WINDOWPLACEMENT wp;
	GetWindowPlacement(&wp);
	AfxGetApp()->WriteProfileBinary(_T("Settings"), _T("WindowPlacementWeatherLocationDlg"), 
		reinterpret_cast<LPBYTE>(&wp), sizeof(wp));
}

void CWeatherLocationDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here

	CRect rcWnd;
	GetWindowRect(&rcWnd);

	HandleAnchors(&rcWnd);
	Invalidate(FALSE);
}

void CWeatherLocationDlg::OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI)
{
	// TODO: Add your message handler code here and/or call default

	lpMMI->ptMinTrackSize.x = WEATHERLOCATIONDLG_DEFAULTMINCLIENTSIZEX;
	lpMMI->ptMinTrackSize.y = WEATHERLOCATIONDLG_DEFAULTMINCLIENTSIZEY;

	CDialog::OnGetMinMaxInfo(lpMMI);
}

void CWeatherLocationDlg::PopulateList(std::vector<SWeatherLocation> locations)
{
	CUtils utils;
	int nCount{};
	for (const auto& it : locations)
	{
		nCount = m_List.GetItemCount();
		m_List.InsertItem(nCount, utils.utf8_utf16(it.name.c_str()));
		m_List.SetItemText(nCount, 1, utils.utf8_utf16(it.adm_area1.c_str()));
		m_List.SetItemText(nCount, 2, utils.utf8_utf16(it.adm_area2.c_str()));
		m_List.SetItemText(nCount, 3, utils.utf8_utf16(it.country.c_str()));
		m_List.SetItemText(nCount, 4, utils.utf8_utf16(it.place_id.c_str()));
	}
}

void CWeatherLocationDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default

	if (ID_TIMER_LOCSEARCH == nIDEvent)
	{
		KillTimer(ID_TIMER_LOCSEARCH);
		UpdateData();
		m_List.DeleteAllItems();
		if (m_sText.GetLength() > 1)
			PopulateList(m_pDoc->GetWeatherLoc(m_sText));
	}

	CDialogEx::OnTimer(nIDEvent);
}

void CWeatherLocationDlg::OnEnChangeEditText()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here

	SetTimer(ID_TIMER_LOCSEARCH, 330, nullptr);
}

void CWeatherLocationDlg::OnDblclkList(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: Add your control notification handler code here

	if (1 == m_List.GetSelectedCount())	// if one single line is selected
		OnOK();

	*pResult = 0;
}

void CWeatherLocationDlg::OnOK()
{
	// TODO: Add extra validation here

	UpdateData();
	const int nIndex = m_List.GetSelectionMark();
	if (nIndex < 0 || nIndex >= m_List.GetItemCount())
		return;

	CUtils utils;
	theApp.WriteProfileString(_T("Settings"), _T("WeatherLocationPlaceId"), m_List.GetItemText(nIndex, 4));
	theApp.WriteProfileString(_T("Settings"), _T("WeatherLocationName"), CString(utils.utf16_utf8(m_List.GetItemText(nIndex, 0)).GetString()));
	theApp.WriteProfileString(_T("Settings"), _T("WeatherLocationCountry"), CString(utils.utf16_utf8(m_List.GetItemText(nIndex, 3))));

	CDialog::OnOK();
}
