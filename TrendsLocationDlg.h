#pragma once

#include "Trends.h"
#include "ComboBoxExt.h"

// CTrendsLocationDlg dialog

class CTrendsLocationDlg final : public CDialogEx
{
	DECLARE_DYNAMIC(CTrendsLocationDlg)

public:
	CTrendsLocationDlg(CWnd* pParent = nullptr);   // standard constructor
	~CTrendsLocationDlg() override = default;

protected:
	HICON m_hIcon;
	CComboBoxExt m_Combo;
	std::unordered_map<std::string, std::string> m_geo{};

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_GEO };
#endif

protected:
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
