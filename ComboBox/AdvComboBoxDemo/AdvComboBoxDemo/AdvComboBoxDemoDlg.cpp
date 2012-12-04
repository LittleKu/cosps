/////////////////////////////////////////////////////////////////////////////
// AdvComboBoxDemoDlg.cpp : implementation file
// 
// CAdvComboBox Control
// Version: 2,0
// Date: August 2002
// Author: Mathias Tunared
// Email: Mathias@inorbit.com
// Copyright (c) 2002. All Rights Reserved.
//
// This code, in compiled form or as source code, may be redistributed 
// unmodified PROVIDING it is not sold for profit without the authors 
// written consent, and providing that this notice and the authors name 
// and all copyright notices remains intact.
//
// This file is provided "as is" with no expressed or implied warranty.
// The author accepts no liability for any damage/loss of business that
// this product may cause.
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AdvComboBoxDemo.h"
#include "AdvComboBoxDemoDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAdvComboBoxDemoDlg dialog

CAdvComboBoxDemoDlg::CAdvComboBoxDemoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAdvComboBoxDemoDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAdvComboBoxDemoDlg)
	m_bCheckedList = FALSE;
	m_bFlatStyle = FALSE;
	m_bDisabledIndex = FALSE;
	m_bCheckedIndex = FALSE;
	m_nIndex = 0;
	m_bAutoAppend = FALSE;
	m_bAutoSuggest = FALSE;
	m_nVisibleItems = -1;
	//}}AFX_DATA_INIT
	m_nAdvCombo = 0;
	m_strAdvCombo = "";
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CAdvComboBoxDemoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAdvComboBoxDemoDlg)
	DDX_Control(pDX, IDC_INDEX_SPIN, m_ctlIndexSpin);
	DDX_Control(pDX, IDC_VISIBLE_ITEMS_SPIN, m_ctlVisibleItemsSpin);
	DDX_Control(pDX, IDC_VISIBLE_ITEMS_EDIT, m_ctlVisibleItemsEdit);
	DDX_Control(pDX, IDC_INDEX_EDIT, m_ctlIndexEdit);
	DDX_Control(pDX, IDC_ADV_COMBO, m_ctlAdvCombo);
	DDX_Check(pDX, IDC_CHECKED_CHECK, m_bCheckedList);
	DDX_Check(pDX, IDC_FLAT_CHECK, m_bFlatStyle);
	DDX_Check(pDX, IDC_IDX_DISABLED_CHECK, m_bDisabledIndex);
	DDX_Check(pDX, IDC_IDX_CHECKED_CHECK, m_bCheckedIndex);
	DDX_Text(pDX, IDC_INDEX_EDIT, m_nIndex);
	DDV_MinMaxInt(pDX, m_nIndex, 0, 20);
	DDX_Check(pDX, IDC_AUTOAPPEND_CHECK, m_bAutoAppend);
	DDX_Check(pDX, IDC_AUTOSUGGEST_CHECK, m_bAutoSuggest);
	DDX_Text(pDX, IDC_VISIBLE_ITEMS_EDIT, m_nVisibleItems);
	DDV_MinMaxInt(pDX, m_nVisibleItems, -1, 20);
	//}}AFX_DATA_MAP
	DDX_ACBIndex( pDX, IDC_ADV_COMBO, m_nAdvCombo );
	DDX_ACBString( pDX, IDC_ADV_COMBO, m_strAdvCombo );
}

BEGIN_MESSAGE_MAP(CAdvComboBoxDemoDlg, CDialog)
	//{{AFX_MSG_MAP(CAdvComboBoxDemoDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_CHECKED_CHECK, OnCheckedCheck)
	ON_BN_CLICKED(IDC_FLAT_CHECK, OnFlatCheck)
	ON_BN_CLICKED(IDC_IDX_DISABLED_CHECK, OnIdxDisabledCheck)
	ON_BN_CLICKED(IDC_IDX_CHECKED_CHECK, OnIdxCheckedCheck)
	ON_EN_CHANGE(IDC_INDEX_EDIT, OnChangeIndexEdit)
	ON_BN_CLICKED(IDC_GETTEXT_BUTTON, OnGettextButton)
	ON_BN_CLICKED(IDC_GETIDX_BUTTON, OnGetidxButton)
	ON_BN_CLICKED(IDC_BTN_ENABLE, OnBtnEnable)
	ON_BN_CLICKED(IDC_AUTOSUGGEST_CHECK, OnAutosuggestCheck)
	ON_BN_CLICKED(IDC_AUTOAPPEND_CHECK, OnAutoappendCheck)
	ON_EN_CHANGE(IDC_VISIBLE_ITEMS_EDIT, OnChangeVisibleItemsEdit)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAdvComboBoxDemoDlg message handlers

BOOL CAdvComboBoxDemoDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	m_ctlVisibleItemsSpin.SetRange32(-1,20);
	m_ctlIndexSpin.SetRange32(0,20);

	InitAdvComboBox();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CAdvComboBoxDemoDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CAdvComboBoxDemoDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CAdvComboBoxDemoDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CAdvComboBoxDemoDlg::OnCheckedCheck() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	if( m_bCheckedList )
	{
		m_ctlAdvCombo.ModifyACBStyle( 0, ACBS_CHECKED );
	}
	else
	{
		m_ctlAdvCombo.ModifyACBStyle( ACBS_CHECKED, 0 );
	}
	
}

void CAdvComboBoxDemoDlg::OnFlatCheck() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	if( m_bFlatStyle )
	{
		m_ctlAdvCombo.ModifyACBStyle( ACBS_STANDARD, ACBS_FLAT );
	}
	else
	{
		m_ctlAdvCombo.ModifyACBStyle( ACBS_FLAT, ACBS_STANDARD );
	}
}

void CAdvComboBoxDemoDlg::OnIdxDisabledCheck() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	BOOL bDisabled = m_ctlAdvCombo.GetItemDisabled( m_nIndex );
	m_ctlAdvCombo.SetItemDisabled( m_nIndex, !bDisabled );
}

void CAdvComboBoxDemoDlg::OnIdxCheckedCheck() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	BOOL bChecked = m_ctlAdvCombo.GetItemChecked( m_nIndex );
	m_ctlAdvCombo.SetItemChecked( m_nIndex, !bChecked );
}


void CAdvComboBoxDemoDlg::OnChangeIndexEdit() 
{
	// TODO: Add your control notification handler code here
	if( m_ctlIndexEdit.GetSafeHwnd() )
	{
		UpdateData();
		m_bDisabledIndex = m_ctlAdvCombo.GetItemDisabled( m_nIndex );
		m_bCheckedIndex = m_ctlAdvCombo.GetItemChecked( m_nIndex );
		UpdateData( FALSE );
	}
}

void CAdvComboBoxDemoDlg::OnGettextButton() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	AfxMessageBox( m_strAdvCombo, MB_OK );
}

void CAdvComboBoxDemoDlg::OnGetidxButton() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	CString str;
	str.Format( "Selected combo index: #%d", m_nAdvCombo );
	AfxMessageBox( str, MB_OK );
}


void CAdvComboBoxDemoDlg::OnBtnEnable() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	m_ctlAdvCombo.EnableWindow(!m_ctlAdvCombo.IsWindowEnabled());
}


void CAdvComboBoxDemoDlg::OnAutosuggestCheck() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	if( m_bAutoSuggest )
	{
		m_ctlAdvCombo.ModifyACBStyle( 0, ACBS_AUTOSUGGEST );
	}
	else
	{
		m_ctlAdvCombo.ModifyACBStyle( ACBS_AUTOSUGGEST, 0 );
	}
	
}

void CAdvComboBoxDemoDlg::OnAutoappendCheck() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	if( m_bAutoAppend )
	{
		m_ctlAdvCombo.ModifyACBStyle( 0, ACBS_AUTOAPPEND );
	}
	else
	{
		m_ctlAdvCombo.ModifyACBStyle( ACBS_AUTOAPPEND, 0 );
	}
	
}

void CAdvComboBoxDemoDlg::OnChangeVisibleItemsEdit() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	if( m_ctlVisibleItemsEdit.GetSafeHwnd() )
	{
		UpdateData();
		m_ctlAdvCombo.SetDefaultVisibleItems( m_nVisibleItems );
	}
}


void CAdvComboBoxDemoDlg::InitAdvComboBox()
{
	//m_ctlAdvCombo.LoadString();
	PLIST_ITEM pItem = NULL, pChild = NULL;
	LIST_ITEM item, item2;
	for(int i = 0; i < 5; i++)
	{
		item.strText.Format(_T("Parent node (%d) -- something to make the item wider"), i + 1);
		//item.hImage = GetSysResMgr()->GetBitmap(BR_TEMP1 + (i % 5));
		item.iImage = i % 5;
		pItem = m_ctlAdvCombo.AddItem(&item);
		if(i <= 2)
		{
			pItem->state |= ACBIS_COLLAPSED;
		}

		for(int j = 0; j < 3; j++)
		{
			item2.strText.Format(_T("p(%d) - child(%d)"), i + 1, j + 1);
			pChild = m_ctlAdvCombo.AddItem(&item2, pItem);
			if(pItem->state & ACBIS_COLLAPSED)
			{
				pChild->state |= ACBIS_INVISIBLE;
			}
		}
	}
	m_ctlAdvCombo.SetCurSel(0);
}
