// BriefView.cpp : implementation of the CBriefView class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Brief.h"
#endif

#include "BriefDoc.h"
#include "BriefView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CBriefView

IMPLEMENT_DYNCREATE(CBriefView, CHtmlView)

BEGIN_MESSAGE_MAP(CBriefView, CHtmlView)
	ON_WM_TIMER()
	ON_COMMAND(ID_FILE_MOVE, &CBriefView::OnFileMove)
//	ON_COMMAND(ID_FILE_KEEP, &CBriefView::OnFileKeep)
	ON_COMMAND(ID_VIEW_REFRESH, &CBriefView::OnViewRefresh)
	ON_MESSAGE(WMU_THREADDATA, &CBriefView::OnThreadData)
	ON_MESSAGE(WMU_POSTINIT, &CBriefView::OnPostInit)
END_MESSAGE_MAP()

// CBriefView construction/destruction

CBriefView::CBriefView() noexcept
{
	// TODO: add construction code here

}

CBriefView::~CBriefView()
{
//	if (GetDocument()->m_bKeep)
//		SetThreadExecutionState(ES_CONTINUOUS);
}

void CBriefView::DoDataExchange(CDataExchange* pDX)
{
	CHtmlView::DoDataExchange(pDX);
}

BOOL CBriefView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CHtmlView::PreCreateWindow(cs);
}

void CBriefView::OnInitialUpdate()
{
	CHtmlView::OnInitialUpdate();

	CBriefDoc* pDoc = GetDocument();
	if (pDoc->GenerateMessagePage("Loading ..."))
		Navigate2(pDoc->GetAppPathTemp() + _T("loading.html"));
	else
		::PostMessage(AfxGetMainWnd()->GetSafeHwnd(), WMU_SETSTATUSTEXT, STATUS_ERROR, 5);

	PostMessage(WMU_POSTINIT);
}

// CBriefView printing

void CBriefView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
//	AFXPrintPreview(this);
#endif
}

BOOL CBriefView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CBriefView::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	// TODO: add extra initialization before printing
}

void CBriefView::OnEndPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	// TODO: add cleanup after printing
}

void CBriefView::OnDraw(CDC* pDC)
{
	CBriefDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (! pDoc)
		return;
}

// CBriefView diagnostics

#ifdef _DEBUG
void CBriefView::AssertValid() const
{
	CHtmlView::AssertValid();
}

void CBriefView::Dump(CDumpContext& dc) const
{
	CHtmlView::Dump(dc);
}

CBriefDoc* CBriefView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CBriefDoc)));
	return (CBriefDoc*)m_pDocument;
}
#endif //_DEBUG

// CBriefView message handlers

void CBriefView::OnUpdate(CHtmlView* pSender, LPARAM lHint, CObject* pHint)
{
	// TODO: Add your specialized code here and/or call the base class

	switch (lHint)
	{
	case 0:
		break;
	default:
		break;
	}
}

void CBriefView::OnDocumentComplete(LPCTSTR lpszURL)
{
	// TODO: Add your specialized code here and/or call the base class

	if (m_bInit && -1 != CString(lpszURL).Find(_T("loading.html")))
		SetTimer(ID_TIMER_GETDATA, 5 * TIME_SECOND, nullptr);

	if (m_bInit && -1 != CString(lpszURL).Find(_T("index.html")) && !GetDocument()->m_sError.IsEmpty())
		::PostMessage(AfxGetMainWnd()->GetSafeHwnd(), WMU_SETSTATUSTEXT, STATUS_ERROR, 0);

	CHtmlView::OnDocumentComplete(lpszURL);
}

void CBriefView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default

	if (ID_TIMER_GETDATA == nIDEvent)
	{
		Refresh();
	}

	CHtmlView::OnTimer(nIDEvent);
}

void CBriefView::OnFileMove()
{
	// TODO: Add your command handler code here

	CComVariant varRes;
	CStringArray arrArgs;
	CBriefDoc* pDoc = GetDocument();
	pDoc->m_bMove = !pDoc->m_bMove;

	if (pDoc->m_bMove)
	{
		AfxGetMainWnd()->SetTimer(ID_TIMER_MOVE, theApp.GetProfileInt(_T("Settings"), _T("MoveIntervalSeconds"), 77) * 1000, nullptr);
		arrArgs.Add(L"ON");
	}
	else
	{
		AfxGetMainWnd()->KillTimer(ID_TIMER_MOVE);
		arrArgs.Add(L"OFF");
	}

	if (!CallClientScript(L"Move", &arrArgs, &varRes))
		pDoc->m_sError = _T("Failed to call Move client script function");
}

void CBriefView::OnFileKeep()
{
	// TODO: Add your command handler code here

/*	CBriefDoc* pDoc = GetDocument();
	pDoc->m_bKeep = !pDoc->m_bKeep;

	if (pDoc->m_bKeep)
		SetThreadExecutionState(ES_CONTINUOUS | ES_AWAYMODE_REQUIRED | ES_SYSTEM_REQUIRED | ES_DISPLAY_REQUIRED);
	else
		SetThreadExecutionState(ES_CONTINUOUS);

	Refresh();*/
}

void CBriefView::Refresh()
{
	if (m_InternetDataStatus.any())
		return;	// at least one thread is still running

	SetTimer(ID_TIMER_GETDATA, 13 * TIME_MINUTE, nullptr);

	m_InternetDataStatus.set();

	CBriefDoc* pDoc = GetDocument();
	pDoc->InitializeDownload();
	pDoc->StartGettingWeather(this);
	pDoc->StartGettingGoogleTrends(this);
	pDoc->StartGettingYoutubeTrends(this);
}

void CBriefView::OnViewRefresh()
{
	// TODO: Add your command handler code here

	Refresh();
}

void CBriefView::DoPageRefresh()
{
	CBriefDoc* pDoc = GetDocument();
	if (pDoc->CreateIndexPage())
		Navigate2(pDoc->GetAppPathTemp() + _T("index.html"));
	else
		::PostMessage(AfxGetMainWnd()->GetSafeHwnd(), WMU_SETSTATUSTEXT, STATUS_ERROR, 5);
}

LRESULT CBriefView::OnPostInit(WPARAM wParam, LPARAM lParam)
{
	Refresh();

	m_bInit = TRUE;

	return 0;
}

LRESULT CBriefView::OnThreadData(WPARAM wParam, LPARAM lParam)
{
	CBriefDoc* pDoc = GetDocument();

	switch (wParam)
	{
	case MESSAGE_EXTERNALIP_DONE:
		break;
	case MESSAGE_GETLOCATION_DONE:
		break;
	case MESSAGE_GETWEATHER_DONE:
		pDoc->JointThread(0);
		m_InternetDataStatus.reset(0);
		break;
	case MESSAGE_GETGOOGLETRENDS_DONE:
		pDoc->JointThread(1);
		m_InternetDataStatus.reset(1);
		break;
	case MESSAGE_GETYOUTUBETRENDS_DONE:
		pDoc->JointThread(2);
		m_InternetDataStatus.reset(2);
		break;
	}

	if (m_InternetDataStatus.none())
	{
		if (!pDoc->IsDownloadAborted())
		{
			DoPageRefresh();
			pDoc->RetrieveThreadsErrors();
		}
	}

	return 0;
}

BOOL CBriefView::CallClientScript(LPCTSTR pStrFuncName, CStringArray* pArrFuncArgs, CComVariant* pOutVarRes)
{
	//Call client function in HTML
	//'pStrFuncName' = client script function name
	//'pArrFuncArgs' = if not NULL, list of arguments
	//'pOutVarRes' = if not NULL, will receive the return value
	// RETURN:
	// = TRUE if done
	BOOL bRes{ FALSE };
	CComVariant vaResult{};
	CComPtr<IHTMLDocument2> spDoc2{};
	const auto spDisp = GetHtmlDocument();
	if (spDisp && SUCCEEDED(spDisp->QueryInterface(IID_IHTMLDocument2, reinterpret_cast<void**>(&spDoc2))))
	{
		CComPtr<IDispatch> spScript{};
		if (SUCCEEDED(spDoc2->get_Script(&spScript)))
		{
			DISPID dispid{ NULL };
			//Find dispid for given function in the object
			CComBSTR bstrMember{ pStrFuncName };
			if (SUCCEEDED(spScript->GetIDsOfNames(IID_NULL, &bstrMember, 1, LOCALE_USER_DEFAULT, &dispid)))
			{
				const auto arraySize = pArrFuncArgs ? pArrFuncArgs->GetSize() : 0;
				//Putting parameters
				DISPPARAMS dispparams;
				memset(&dispparams, 0, sizeof dispparams);
				dispparams.cArgs = static_cast<UINT>(arraySize);
				dispparams.rgvarg = new VARIANT[dispparams.cArgs];
				dispparams.cNamedArgs = 0;

				for (int i = 0; i < arraySize; ++i)
				{
					CComBSTR bstr = static_cast<CComBSTR>(pArrFuncArgs->GetAt(arraySize - 1 - i));	// back reading
					bstr.CopyTo(&dispparams.rgvarg[i].bstrVal);
					dispparams.rgvarg[i].vt = VT_BSTR;
				}

				EXCEPINFO excepInfo;
				memset(&excepInfo, 0, sizeof excepInfo);
				UINT nArgErr = (UINT)-1;  // initialize to invalid arg
				//Call JavaScript function       
				if (SUCCEEDED(spScript->Invoke(dispid, IID_NULL, 0, DISPATCH_METHOD,
												&dispparams, &vaResult, &excepInfo, &nArgErr)))
				{
					//Done!
					bRes = TRUE;
				}
				//Free mem
				delete[] dispparams.rgvarg;
			}
		}
	}

	if (pOutVarRes)
		*pOutVarRes = vaResult;

	return bRes;
}
