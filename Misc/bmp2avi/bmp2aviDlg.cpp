// bmp2aviDlg.cpp : implementation file
//

#include "stdafx.h"
#include "bmp2avi.h"
#include "bmp2aviDlg.h"
#include "BitmapAvi.h"
#include "AviBitmap.h"

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
// CBmp2aviDlg dialog

CBmp2aviDlg::CBmp2aviDlg(CWnd* pParent /*=NULL*/)
: CDialog(CBmp2aviDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBmp2aviDlg)
	// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CBmp2aviDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBmp2aviDlg)
	// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CBmp2aviDlg, CDialog)
//{{AFX_MSG_MAP(CBmp2aviDlg)
ON_WM_SYSCOMMAND()
ON_WM_PAINT()
ON_WM_QUERYDRAGICON()
ON_BN_CLICKED(IDC_AVI_TO_BMP, OnAviToBmp)
ON_BN_CLICKED(IDC_BMP_TO_AVI, OnBmpToAvi)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBmp2aviDlg message handlers

BOOL CBmp2aviDlg::OnInitDialog()
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
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CBmp2aviDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CBmp2aviDlg::OnPaint() 
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
HCURSOR CBmp2aviDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CBmp2aviDlg::OnAviToBmp() 
{
	CAviBitmap aviBitmap(_T(".\\avi\\bmp2avi.avi"));
	HRESULT hr = S_OK;
	
	do 
	{
		hr = aviBitmap.Init();
		if(hr != S_OK)
		{
			break;
		}

		hr = aviBitmap.GetAllFrames(_T(".\\bmp2"));
		if(hr != S_OK)
		{
			break;
		}
	} while (FALSE);

	CString szMsg = _T("OK");
	if(hr != S_OK)
	{
		szMsg.Format(_T("0x%08X: %s"), hr, aviBitmap.GetLastErrorMessage());
	}
	AfxMessageBox(szMsg);
}

void CBmp2aviDlg::OnBmpToAvi() 
{
	BMPtoAVI(_T(".\\avi\\bmp2avi.avi"), _T(".\\bmp"));
}

void CBmp2aviDlg::AVItoBmp(CString strAVIFileName, CString strBmpDir)
{
	AVIFileInit();
 PAVIFILE avi;
 int res=AVIFileOpen(&avi, strAVIFileName, OF_READ, NULL);
 int n = GetLastError();
 if (res!=AVIERR_OK)
 {
  //an error occures
  if (avi!=NULL)
   AVIFileRelease(avi);
  return ;
 }
 AVIFILEINFO avi_info;
 AVIFileInfo(avi, &avi_info, sizeof(AVIFILEINFO));
 PAVISTREAM pStream;
 res=AVIFileGetStream(avi, &pStream, streamtypeVIDEO /*video stream*/, 
   0 /*first stream*/);
 if (res!=AVIERR_OK)
 {
  if (pStream!=NULL)
   AVIStreamRelease(pStream);
   AVIFileExit();
  return ;
 }

 //do some task with the stream
 int iNumFrames;
 int iFirstFrame;
 iFirstFrame=AVIStreamStart(pStream);
 if (iFirstFrame==-1)
 {
  //Error getteing the frame inside the stream
  if (pStream!=NULL)
   AVIStreamRelease(pStream);
  AVIFileExit();
  return ;
 }
 iNumFrames=AVIStreamLength(pStream);
 if (iNumFrames==-1)
 {
  //Error getteing the number of frames inside the stream
  if (pStream!=NULL)
   AVIStreamRelease(pStream);
  AVIFileExit();
  return ;
 }

 //getting bitmap from frame
 BITMAPINFOHEADER bih;
 ZeroMemory(&bih, sizeof(BITMAPINFOHEADER));

 bih.biBitCount=24; //24 bit per pixel
 bih.biClrImportant=0;
 bih.biClrUsed = 0;
 bih.biCompression = BI_RGB;
 bih.biPlanes = 1;
 bih.biSize = 40;
 bih.biXPelsPerMeter = 0;
 bih.biYPelsPerMeter = 0;
 //calculate total size of RGBQUAD scanlines (DWORD aligned)
 bih.biSizeImage = (((bih.biWidth * 3) + 3) & 0xFFFC) * bih.biHeight ;

 PGETFRAME pFrame;
 pFrame=AVIStreamGetFrameOpen(pStream, NULL );

 AVISTREAMINFO streaminfo;
 AVIStreamInfo(pStream,&streaminfo,sizeof(AVISTREAMINFO));

 //Get the first frame
 BITMAPINFOHEADER bih2;
 long lsize = sizeof(bih2);
 int index=0;
 for (int i=iFirstFrame; i<iNumFrames; i++)
 {
  index= i-iFirstFrame;
  BYTE* pDIB = (BYTE*) AVIStreamGetFrame(pFrame, index); //
  AVIStreamReadFormat(pStream,index,&bih2,&lsize);
  BITMAPFILEHEADER stFileHdr;

  BYTE* Bits=new BYTE[bih2.biSizeImage];
  AVIStreamRead(pStream,index,1,Bits,bih2.biSizeImage,NULL,NULL);
  //RtlMoveMemory(Bits, pDIB + sizeof(BITMAPINFOHEADER), bih2.biSizeImage);

  bih2.biClrUsed =0;
  stFileHdr.bfOffBits=sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER);
  stFileHdr.bfSize=sizeof(BITMAPFILEHEADER);
  stFileHdr.bfType=0x4d42; 

  CString FileName;
  FileName.Format("Frame-%05d.bmp", index);
  CString strtemp = strBmpDir;
  strtemp += "";
  strtemp += FileName;
  FILE* fp=_tfopen(strtemp ,_T("wb"));
  fwrite(&stFileHdr,1,sizeof(BITMAPFILEHEADER),fp);
  fwrite(&bih2,1,sizeof(BITMAPINFOHEADER),fp);
  int ff = fwrite(Bits,1,bih2.biSizeImage,fp);
  int e = GetLastError();
  fclose(fp);
  /////
  delete Bits;
  //CreateFromPackedDIBPointer(pDIB, index);
 }

 AVIStreamGetFrameClose(pFrame);

 //close the stream after finishing the task
 if (pStream!=NULL)
  AVIStreamRelease(pStream);
 AVIFileExit();
}

LONG CBmp2aviDlg::BMPtoAVI(CString szAVIName, CString strBmpDir)
{
	CBitmapAvi bitmapAvi(szAVIName, 0, 25, "audio.wav");

	CFileFind finder;
	strBmpDir += _T("\\*.bmp");

	CString szMsg = _T("OK");
	HRESULT hr = S_OK; 
	
	BOOL bFind = finder.FindFile(strBmpDir);
	while(bFind)
	{
		bFind = finder.FindNextFile();
		if(!finder.IsDots() && !finder.IsDirectory())
		{
			CString str = finder.GetFilePath();
			hr = bitmapAvi.AddFrame(str);
			if(hr != S_OK)
			{
				break;
			}
		}
	}

	if(hr == S_OK)
	{
		hr = bitmapAvi.Done();
	}

	if(hr != S_OK)
	{
		szMsg = bitmapAvi.GetLastErrorMessage();
	}

	AfxMessageBox(szMsg);

	return hr;
}
/*
LONG CBmp2aviDlg::BMPtoAVI(CString szAVIName, CString strBmpDir)
{
	if(::PathFileExists(szAVIName))
	{
		::DeleteFile(szAVIName);
	}
	CFileFind finder;
	strBmpDir += _T("\\*.bmp"); 
	AVIFileInit(); 
	AVISTREAMINFO strhdr;
	PAVIFILE pfile;
	PAVISTREAM ps; 
	int nFrames =0; 
	HRESULT hr; 
	
	BOOL bFind = finder.FindFile(strBmpDir);
	while(bFind)
	{
		bFind = finder.FindNextFile();
		if(!finder.IsDots() && !finder.IsDirectory())
		{
			CString str = finder.GetFilePath();
			FILE *fp = fopen(str,"rb");
			BITMAPFILEHEADER bmpFileHdr;
			BITMAPINFOHEADER bmpInfoHdr;
			fseek( fp,0,SEEK_SET);
			fread(&bmpFileHdr,sizeof(BITMAPFILEHEADER),1, fp);
			fread(&bmpInfoHdr,sizeof(BITMAPINFOHEADER),1, fp);
			
			BYTE *tmp_buf = NULL;
			if(nFrames ==0 )
			{
				hr = AVIFileOpen(&pfile,szAVIName,OF_WRITE | OF_CREATE,NULL);
				if(hr != 0)
				{
					return hr;
				}
				memset(&strhdr, 0, sizeof(strhdr));
				strhdr.fccType = streamtypeVIDEO;// stream type
				strhdr.fccHandler = 0;
				strhdr.dwScale = 1;
				strhdr.dwRate = 15; // 15 fps
				strhdr.dwSuggestedBufferSize = bmpInfoHdr.biSizeImage ;
				SetRect(&strhdr.rcFrame, 0, 0, bmpInfoHdr.biWidth, bmpInfoHdr.biHeight);
				
				// And create the stream;
				hr = AVIFileCreateStream(pfile,&ps,&strhdr);
				if(hr != 0)
				{
					return hr;
				}
				// hr = AVIStreamSetFormat(ps,nFrames,&bmpInfoHdr,sizeof(bmpInfoHdr));
			}
			DWORD dwBytesCount = (((bmpInfoHdr.biWidth * bmpInfoHdr.biBitCount) + 31) & ~31) >> 3;
			tmp_buf = new BYTE[dwBytesCount];
			fread(tmp_buf, 1, dwBytesCount, fp);
			hr = AVIStreamSetFormat(ps,nFrames,&bmpInfoHdr,sizeof(bmpInfoHdr));
			if(hr != 0)
			{
				return hr;
			}
			hr = AVIStreamWrite(ps, nFrames, 1, (LPBYTE) tmp_buf, dwBytesCount, AVIIF_KEYFRAME, NULL, NULL);
			if(hr != 0)
			{
				return hr;
			}
			delete [] tmp_buf;

			nFrames ++; 
			fclose(fp);
		}
	}
	
	AVIStreamClose(ps);
	
	if(pfile != NULL)
		AVIFileRelease(pfile);
	AVIFileExit();

	return 0;
}
*/