//Download by http://www.NewXing.com
// COMTOOLDlg.cpp : implementation file
//

#include "stdafx.h"
#include "COMTOOL.h"
#include "COMTOOLDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define EditBoxWidth 50

int BaudRate[]={300,600,1200,2400,4800,9600,14400,19200,38400,56000,57600,115200,230400,460800,921600};
int ParitySelNum=5;
CString Parity[]={_T("None"),_T("Odd"),_T("Even"),_T("Mark"),_T("Space")};
int DataBits[]={5,6,7,8};
int StopBits[]={1,2};
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
// CCOMTOOLDlg dialog

CCOMTOOLDlg::CCOMTOOLDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCOMTOOLDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCOMTOOLDlg)
	m_strSend = _T("");
	m_strReceive = _T("");
	m_bHexR = true;
	m_bHexS = true;
	m_bAutoSend = FALSE;
	m_nSendPeriod = 1000;
	m_strStatus = _T("关闭");
	m_strSendBytes = _T("0");
	m_strReceiveBytes = _T("0");
	m_strPortNO = _T("");
	m_strBaudRate = _T("");
	m_strDataBits = _T("");
	m_strStopBits = _T("");
	m_strParity = _T("");
	m_nSendBytes=0;
	m_nReceiveBytes=0;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCOMTOOLDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCOMTOOLDlg)
	DDX_Control(pDX, IDC_Description, m_DescriptionCtrl);
	DDX_Control(pDX, IDC_Send, m_SendCtrl);
	DDX_Control(pDX, IDC_OpenClose, m_OpenCloseCtrl);
	DDX_Control(pDX, IDC_StopBits, m_StopBits);
	DDX_Control(pDX, IDC_ParityBits, m_Parity);
	DDX_Control(pDX, IDC_COM, m_PortNO);
	DDX_Control(pDX, IDC_Baudrate, m_BaudRate);
	DDX_Control(pDX, IDC_DataBits, m_DataBits);
	DDX_Control(pDX, IDC_SendPeriod, m_SendPeriodCtrl);
	DDX_Text(pDX, IDC_SendEdit, m_strSend);
	DDX_Text(pDX, IDC_ReceiveEdit, m_strReceive);
	DDX_Check(pDX, IDC_BHexR, m_bHexR);
	DDX_Check(pDX, IDC_BHexS, m_bHexS);
	DDX_Check(pDX, IDC_BAutoSend, m_bAutoSend);
	DDX_Text(pDX, IDC_SendPeriod, m_nSendPeriod);
	DDX_Text(pDX, IDC_Status, m_strStatus);
	DDX_Text(pDX, IDC_SendBytes, m_strSendBytes);
	DDX_Text(pDX, IDC_ReceiveBytes, m_strReceiveBytes);
	DDX_CBString(pDX, IDC_COM, m_strPortNO);
	DDX_CBString(pDX, IDC_Baudrate, m_strBaudRate);
	DDX_CBString(pDX, IDC_DataBits, m_strDataBits);
	DDX_CBString(pDX, IDC_StopBits, m_strStopBits);
	DDX_CBString(pDX, IDC_ParityBits, m_strParity);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CCOMTOOLDlg, CDialog)
	//{{AFX_MSG_MAP(CCOMTOOLDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_ABOUT, OnAbout)
	ON_BN_CLICKED(IDC_QUIT, OnQuit)
	ON_BN_CLICKED(IDC_ClearS, OnClearSendEdit)
	ON_BN_CLICKED(IDC_ClearR, OnClearReceiveEdit)
	ON_BN_CLICKED(IDC_BAutoSend, OnBAutoSend)
	ON_BN_CLICKED(IDC_OpenClose, OnOpenClose)
	ON_BN_CLICKED(IDC_ClearCounter, OnClearCounter)
	ON_MESSAGE(WM_COMM_RXCHAR,OnReceiveChar)
	ON_BN_CLICKED(IDC_Send, OnSend)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BHexS, OnBHexS)
	ON_BN_CLICKED(IDC_BHexR, OnBHexR)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCOMTOOLDlg message handlers

BOOL CCOMTOOLDlg::OnInitDialog()
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
	m_SendPeriodCtrl.EnableWindow(m_bAutoSend);
	m_OpenCloseCtrl.SetWindowText(_T("打开串口"));
	m_DescriptionCtrl.SetWindowText("");
	if(m_bHexS)
		GetDlgItem(IDC_SendEdit)->ModifyStyle(0,ES_UPPERCASE,0);
	else
		GetDlgItem(IDC_SendEdit)->ModifyStyle(ES_UPPERCASE,0,0);

	if(m_bHexR)
		GetDlgItem(IDC_ReceiveEdit)->ModifyStyle(0,ES_UPPERCASE,0);
	else
		GetDlgItem(IDC_ReceiveEdit)->ModifyStyle(ES_UPPERCASE,0,0);
	CString temp;
	
	//显示波特率
	for(int i=0;i<sizeof(BaudRate)/sizeof(int);i++)
	{
		temp.Format("%d",BaudRate[i]);
		m_BaudRate.AddString((LPCTSTR)temp);
	}
	temp.Format("%d",9600);
	m_BaudRate.SetCurSel(m_BaudRate.FindString(0,temp));
	
	
	//显示奇偶校验
	for (i=0;i<ParitySelNum;i++)
	{
		m_Parity.AddString((LPCTSTR) Parity[i]);
	}
	m_Parity.SetCurSel(m_Parity.FindString(0,_T("None")));
	//显示停止位
	for(i=0;i<sizeof(StopBits)/sizeof(int);i++)
	{
		temp.Format("%d",StopBits[i]);
		m_StopBits.AddString((LPCTSTR)temp);
	}
	temp.Format("%d",1);
	m_StopBits.SetCurSel(m_StopBits.FindString(0,(LPCTSTR)temp));
	
	//显示数据位
	for(i=0;i<sizeof(DataBits)/sizeof(int);i++)
	{
		temp.Format("%d",DataBits[i]);
		m_DataBits.AddString((LPCTSTR)temp);
	}
	temp.Format("%d",8);
	m_DataBits.SetCurSel(m_DataBits.FindString(0,(LPCTSTR)temp));
	
	//显示串口设置
	for(i=1;i<=MaxSerialPortNum-1;i++)
	{
		if(m_SerialPort.InitPort(this,i))
		{
			temp.Format("COM%d",i);		
			m_PortNO.AddString((LPCTSTR)temp);
		}
	}
	if(m_PortNO.GetCount())
	{
		m_SerialPort.InitPort(this,MaxSerialPortNum);
		m_PortNO.SetCurSel(0);
	}

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CCOMTOOLDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CCOMTOOLDlg::OnPaint() 
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
HCURSOR CCOMTOOLDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CCOMTOOLDlg::OnAbout() 
{
	// TODO: Add your control notification handler code here
	CAboutDlg dlg;
	dlg.DoModal();
}

void CCOMTOOLDlg::OnQuit() 
{
	// TODO: Add your control notification handler code here
	m_SerialPort.InitPort(this,MaxSerialPortNum);
	PostQuitMessage(0);
	
}

void CCOMTOOLDlg::OnClearSendEdit() 
{
	// TODO: Add your control notification handler code here
	UpdateData(true);
	m_strSend=_T("");
	UpdateData(false);
}

void CCOMTOOLDlg::OnClearReceiveEdit() 
{
	// TODO: Add your control notification handler code here
	UpdateData(true);
	m_strReceive=_T("");
	UpdateData(false);
}

void CCOMTOOLDlg::OnBAutoSend() 
{
	// TODO: Add your control notification handler code here
	UpdateData(true);
	m_SendPeriodCtrl.EnableWindow(m_bAutoSend);
	if(m_bAutoSend)	
	{
		m_SendCtrl.SetWindowText("开始自动发送");
	}
	else
	{
		m_SendCtrl.SetWindowText("手动发送");
		KillTimer(1);
	}
}

void CCOMTOOLDlg::OnOpenClose() 
{
	// TODO: Add your control notification handler code here
	CString temp;
	m_OpenCloseCtrl.GetWindowText(temp);
	UpdateData(true);
	if(temp==_T("关闭串口"))
	{
		m_SerialPort.InitPort(this,MaxSerialPortNum);
		m_OpenCloseCtrl.SetWindowText(_T("打开串口"));
		m_strStatus=_T("关闭");
		UpdateData(false);
		m_DescriptionCtrl.SetWindowText("");
		m_SendCtrl.GetWindowText(temp);
		if(temp=="停止自动发送")
		{
			KillTimer(1);
			m_SendCtrl.SetWindowText("开始自动发送");
		}
	}
	else if( m_PortNO.GetCount())
	{	
		int SelPortNO,SelBaudRate,SelDataBits,SelStopBits;
		char SelParity;
		UpdateData(true);
		temp=m_strPortNO;
		temp.Delete(0,3);
		SelPortNO=atoi(temp);
		SelBaudRate=atoi(m_strBaudRate);
		SelDataBits=atoi(m_strDataBits);
		SelParity=m_strParity.GetAt(0);
		SelStopBits=atoi(m_strStopBits);
		if(m_SerialPort.InitPort(this,SelPortNO,SelBaudRate,SelParity,SelDataBits,SelStopBits,EV_RXCHAR|EV_CTS,512))		
		{
			m_SerialPort.StartMonitoring();
			m_OpenCloseCtrl.SetWindowText(_T("关闭串口"));
			m_strStatus=_T("打开");
			UpdateData(false);
			temp=m_strPortNO+"  , 波特率:  "+m_strBaudRate+"bps, 校验位:  "+m_strParity+
				", 数据为:  "+m_strDataBits+" , 停止位:  "+m_strStopBits;
			m_DescriptionCtrl.SetWindowText(temp);
		}
		else 
			AfxMessageBox("该串口已经被其他应用程序所占用!\n请选择其它的串口");
	}
}

void CCOMTOOLDlg::OnClearCounter() 
{
	// TODO: Add your control notification handler code here
	UpdateData(true);
	m_nSendBytes=0;
	m_nReceiveBytes=0;
	m_strSendBytes=_T("0");
	m_strReceiveBytes=_T("0");
	UpdateData(false);
}


void CCOMTOOLDlg::OnSend() 
{
	// TODO: Add your control notification handler code here
	CString temp;
	UpdateData(true);
	if(m_strStatus=="关闭")
	{
		AfxMessageBox("请首先打开串口");
		return;
	}
	if(!m_bAutoSend)
	{	
		temp=m_strSend;
		if(m_bHexS)
		temp=ChangeCharstr2Hexstr(temp);
		m_SerialPort.WriteToPort(temp.GetBuffer(temp.GetLength()),temp.GetLength());
		m_nSendBytes+=temp.GetLength();
		m_strSendBytes.Format("%d",m_nSendBytes);
		UpdateData(false);
	}
	else 
	{
		m_SendCtrl.GetWindowText(temp);
		if(temp=="停止自动发送")
		{
			KillTimer(1);
			m_SendCtrl.SetWindowText("开始自动发送");
			UpdateData(false);
		}
		else
		{
			SetTimer(1,m_nSendPeriod,NULL);
			m_SendCtrl.SetWindowText("停止自动发送");
		}
	}
	//temp.Format("%d",
	//AfxMessageBox(temp);
}

void CCOMTOOLDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	UpdateData(true);
	CString temp;
	temp=m_strSend;
	if(m_bHexS)
		temp=ChangeCharstr2Hexstr(temp);
	m_SerialPort.WriteToPort(temp.GetBuffer(temp.GetLength()),temp.GetLength());
	m_nSendBytes+=temp.GetLength();
	m_strSendBytes.Format("%d",m_nSendBytes);
	UpdateData(false);
	CDialog::OnTimer(nIDEvent);
}

void CCOMTOOLDlg::OnReceiveChar(UINT ch, LONG port)
{
	UpdateData(true);
	m_nReceiveBytes++;
	CString temp;
	temp.Format("%d",m_nReceiveBytes);
	m_strReceiveBytes=temp;
	if(m_bHexR)
		m_strReceive+=DevideHexChar(ch)+_T(" ");
	else
		m_strReceive+=ch;
	UpdateData(false);

	((CEdit*)GetDlgItem(IDC_ReceiveEdit))->LineScroll(
		m_strReceive.GetLength()/(((CEdit*)GetDlgItem(IDC_ReceiveEdit))->LineLength()));
}

CString CCOMTOOLDlg::ChangeCharstr2Hexstr(CString Charstr)
{
	CString Hexstr=_T("");
	Charstr.MakeUpper();
	HexStringFilter(Charstr);
	int Length=Charstr.GetLength();
	if(Length%2)
		Charstr.Delete(Length-1);
	Length=Charstr.GetLength();
	for(int i=0;i<Length/2;i++)
	{
		Hexstr+=CombineHexChar(Charstr.GetAt(i*2),Charstr.GetAt(i*2+1));
	}
	return Hexstr;
}

void CCOMTOOLDlg::OnBHexS() 
{
	// TODO: Add your control notification handler code here
	UpdateData(true);
	if(m_bHexS)
		GetDlgItem(IDC_SendEdit)->ModifyStyle(0,ES_UPPERCASE,0);
	else
		GetDlgItem(IDC_SendEdit)->ModifyStyle(ES_UPPERCASE,0,0);
}

void CCOMTOOLDlg::OnBHexR() 
{
	// TODO: Add your control notification handler code here
	UpdateData(true);
	if(m_bHexR)
		GetDlgItem(IDC_ReceiveEdit)->ModifyStyle(0,ES_UPPERCASE,0);
	else
		GetDlgItem(IDC_ReceiveEdit)->ModifyStyle(ES_UPPERCASE,0,0);
}

void CCOMTOOLDlg::HexStringFilter(CString &str)
{
	BOOL bOK;
	for(int i=0;i<str.GetLength();)
	{
		bOK=((str.GetAt(i)>='0')&&(str.GetAt(i)<='9'))||
			((str.GetAt(i)>='A')&&(str.GetAt(i)<='F'))||
			((str.GetAt(i)>='a')&&(str.GetAt(i)<='f'));
		if(!bOK)
			str.Delete(i);
		else i++;	
	}
}
char CCOMTOOLDlg::CombineHexChar(char CharH,char CharL)
{
	char result;
	CString temp;
	if(CharH>='0'&&CharH<='9')			result=(CharH-'0');
	else if(CharH>='a'&&CharH<='f')		result=(CharH-'a'+10);
	else if(CharH>='A'&&CharH<='F')		result=(CharH-'A'+10);
	else								result=0;
	result<<=4;	
	if(CharL>='0'&&CharL<='9')			result+=(CharL-'0');
	else if(CharL>='a'&&CharL<='f')		result+=(CharL-'a'+10);
	else if(CharL>='A'&&CharL<='F')		result+=(CharL-'A'+10);
	else								result+=0;
	return result;
}


CString CCOMTOOLDlg::DevideHexChar(char HexChar)
{
	CString result=_T("");
	int temp=(HexChar&0xF0)>>4;
	if(temp<10)
		result+=(temp+'0');
	else 
		result+=(temp+'A'-10);
	temp=HexChar&0x0F;
	if(temp<10)
		result+=(temp+'0');
	else 
		result+=(temp+'A'-10);
	return result;
}
