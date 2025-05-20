#pragma once

// CMoveDlg dialog

class CMoveDlg final : public CDialogEx
{
	DECLARE_DYNAMIC(CMoveDlg)

public:
	CMoveDlg(CWnd* pParent = nullptr);   // standard constructor
	~CMoveDlg() override = default;

protected:
	HICON m_hIcon;
	UINT m_nMove;
	CSpinButtonCtrl m_Spin;

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_MOVE };
#endif

protected:
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
