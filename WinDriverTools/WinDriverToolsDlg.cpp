
// WinDriverToolsDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "WinDriverTools.h"
#include "WinDriverToolsDlg.h"
#include "afxdialogex.h"
#include <commdlg.h>
#include <Shellapi.h>
#include <winsvc.h>
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CWinDriverToolsDlg 对话框

int CWinDriverToolsDlg::glo_set = 1;
TCHAR CWinDriverToolsDlg::errmsg[MAX_PATH] = {0};
CWinDriverToolsDlg::CWinDriverToolsDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_WINDRIVERTOOLS_DIALOG, pParent),Install(0) ,Start(1),Stop(2),Uninstall(3)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	regListCtlNum = 0;
	driverFile = new DriverFile();
}
CWinDriverToolsDlg::~CWinDriverToolsDlg()
{
	delete driverFile;
	driverFile = NULL;
}
void CWinDriverToolsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_REG, regListCtl);
}

BEGIN_MESSAGE_MAP(CWinDriverToolsDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_DROPFILES()
	ON_BN_CLICKED(IDC_BUTTON_INSTALL, &CWinDriverToolsDlg::OnBnClickedButtonInstall)
	ON_BN_CLICKED(IDC_BUTTON_START, &CWinDriverToolsDlg::OnBnClickedButtonStart)
	ON_BN_CLICKED(IDC_BUTTON_STOP, &CWinDriverToolsDlg::OnBnClickedButtonStop)
	ON_BN_CLICKED(IDC_BUTTON_UNINSTALL, &CWinDriverToolsDlg::OnBnClickedButtonUninstall)
	ON_BN_CLICKED(IDC_BUTTON_CLOSE, &CWinDriverToolsDlg::OnBnClickedButtonClose)
	ON_BN_CLICKED(IDC_BUTTON_FILE, &CWinDriverToolsDlg::OnBnClickedButtonFile)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BUTTON_ADD, &CWinDriverToolsDlg::OnBnClickedButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON_DEL, &CWinDriverToolsDlg::OnBnClickedButtonDel)
END_MESSAGE_MAP()


// CWinDriverToolsDlg 消息处理程序

BOOL CWinDriverToolsDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);
	CString title("DriverInstaller");
	this->SetWindowTextW(title);
	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}
	/* Active Terminal For MFC Application Debug In the Debug Module */
	#ifdef DEBUG
		::AllocConsole();
		FILE * fp;
		freopen_s(&fp, "CONOUT$", "w+t", stdout);
	#endif // DEBUG
	fontColor = RGB(0, 0, 0);
	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标
	//Init Dialog ListCtl
	DWORD listStyle = regListCtl.GetExtendedStyle();
	listStyle |= LVS_EX_FULLROWSELECT;
	regListCtl.SetExtendedStyle(listStyle);
	regListCtl.InsertColumn(0, _T("Path"), LVCFMT_LEFT, 280);
	regListCtl.InsertColumn(1, _T("Key"), LVCFMT_LEFT, 90);
	regListCtl.InsertColumn(2, _T("Value"), LVCFMT_LEFT, 280);

	ShowWindow(SW_NORMAL);
	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CWinDriverToolsDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}
/* Drop Files to the Edit Text */
void CWinDriverToolsDlg::OnDropFiles(HDROP hDropInfo) 
{
	TCHAR fullPath[MAX_PATH] = { 0 };
	DragQueryFile(hDropInfo,0,LPWSTR(fullPath),sizeof(fullPath));
	SetDlgItemText(IDC_EDIT_PATH, LPWSTR(fullPath));
	DragFinish(hDropInfo);
	CDialog::OnDropFiles(hDropInfo);
	driverFile->setFullPath(fullPath);
	cout << "Drop Finish" << endl;
}
// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CWinDriverToolsDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CWinDriverToolsDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

/* driver install*/
void CWinDriverToolsDlg::OnBnClickedButtonInstall()
{
	getTitleName(driverFile->getFullPath());
	cout << "install" << endl;
	bool result =  OnNotiftyServiceAction(Install);
}

/* driver start */
void CWinDriverToolsDlg::OnBnClickedButtonStart()
{
	getTitleName(driverFile->getFullPath());
	// TODO: 在此添加控件通知处理程序代码
	bool result =  OnNotiftyServiceAction(Start);
	cout << "start" << endl;
}

/* driver stop */
void CWinDriverToolsDlg::OnBnClickedButtonStop()
{
	getTitleName(driverFile->getFullPath());
	// TODO: 在此添加控件通知处理程序代码
	bool result =  OnNotiftyServiceAction(Stop);
	cout << "stop" << endl;
}

/* driver uninstall*/
void CWinDriverToolsDlg::OnBnClickedButtonUninstall()
{
	getTitleName(driverFile->getFullPath());
	// TODO: 在此添加控件通知处理程序代码
	bool result =  OnNotiftyServiceAction(Uninstall);
	cout << "uninstall" << endl;
}

/* close */
void CWinDriverToolsDlg::OnBnClickedButtonClose()
{
	//Test
	getTitleName(driverFile->getFullPath());
	// TODO: 在此添加控件通知处理程序代码
	cout << "close" << endl;
}
bool CWinDriverToolsDlg::OnNotiftyServiceAction(int operatorType)
{
	switch(operatorType)
	{
		case 0:
			doServiceOperation(driverFile -> getFullPath(), driverFile -> getDriverTitle(),Install);
			break;
		case 1:
			doServiceOperation(driverFile -> getFullPath(), driverFile -> getDriverTitle(),Start);
			break;
		case 2:
			doServiceOperation(driverFile -> getFullPath(), driverFile -> getDriverTitle(),Stop);
			break;
		case 3:
			doServiceOperation(driverFile -> getFullPath(), driverFile -> getDriverTitle(),Uninstall);
			break;
		default:
			return false;
	}
	return true;
}

void CWinDriverToolsDlg::OnBnClickedButtonFile()
{
	TCHAR szFilter[] = _T("Sys Files(*.sys)|*.sys|All Files(*.*)|*.*||");
	CFileDialog fileDlg(TRUE, _T("sys"), NULL, 0, szFilter, this);
	CString strFilePath;

	if (IDOK == fileDlg.DoModal())
	{
		strFilePath = fileDlg.GetPathName();
		SetDlgItemText(IDC_EDIT_PATH, strFilePath);
		_tcscpy(driverFile->getFullPath(), strFilePath);
	}
}
/*Get .sys file name from full path */
TCHAR* CWinDriverToolsDlg::getTitleName(TCHAR* filePath)
{	
	TCHAR path[MAX_PATH] = { 0 };
	TCHAR title[MAX_PATH] = { 0 };
	TCHAR* pStr = NULL;
	int gType = 0;
	::GetDlgItemText(AfxGetMainWnd()->m_hWnd,IDC_EDIT_PATH,path,MAX_PATH);
	CString temp;
	temp.Format(_T("%s"), path);
	pStr = path + lstrlen(path);
	while (*pStr != '\\' && pStr - path != 0)
		pStr--;
	if (pStr != path)
		pStr++;
	for (gType = 0; *(pStr + gType); gType++)
	{
		title[gType] = *(pStr + gType);
	}
	title[gType] = '\0';
	driverFile->setDriverTitle(title);
	return driverFile->getDriverTitle();
}
/*Show Msg infomation at Dialog static area */
void CWinDriverToolsDlg::doReport(HWND hwnd, TCHAR* str,int type) 
{	
	//ERROR
	if (!type) 
	{
		LPWSTR pBuffer = NULL;
		DWORD getLastError = GetLastError();
		FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER |
			FORMAT_MESSAGE_FROM_SYSTEM |
			FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL,
			getLastError,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			(LPTSTR)&pBuffer,
			0,
			NULL);
		_stprintf_s(errmsg, sizeof(errmsg), _T("%d(0x%x):%s\n"), getLastError, getLastError, str);
		GetDlgItem(IDC_STATIC_ERRMSG)->SetWindowTextW(errmsg);
		LocalFree(pBuffer);
	}
	else //REPORT
	{
		GetDlgItem(IDC_STATIC_ERRMSG)->SetWindowTextW(str);
	}

}
/*Do Service Operation Install/Start/Stop/Uninstall*/
BOOL CWinDriverToolsDlg::doServiceOperation(TCHAR *szFullPath, TCHAR *szName, int iType)
{
	SC_HANDLE shOSCM = NULL, shCS = NULL;
	SERVICE_STATUS ss;
	DWORD dwErrorCode = 0;
	BOOL bSuccess = FALSE;

	shOSCM = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if (!shOSCM)
	{
		fontColor = RGB(255, 0, 0);
		doReport(AfxGetMainWnd()->m_hWnd, TEXT("OpenSCManager Failed!"),ERROR);
		return FALSE;
	}
	if (iType != Install)
	{
		shCS = OpenService(shOSCM, szName, SERVICE_ALL_ACCESS);
		if (!shCS)
		{
			fontColor = RGB(255, 0, 0);
			doReport(AfxGetMainWnd()->m_hWnd, TEXT("OpenService Failed!"),ERROR);
			CloseServiceHandle(shOSCM); 
			shOSCM = NULL;
			return FALSE;
		}
	}

	switch (iType)
	{
	case 0: 
		shCS = CreateService(shOSCM, szName, szName,
			SERVICE_ALL_ACCESS, SERVICE_KERNEL_DRIVER,
			SERVICE_DEMAND_START, SERVICE_ERROR_NORMAL,
			szFullPath, NULL, NULL, NULL, NULL, NULL);

		if (!shCS)
		{
			fontColor = RGB(255, 0, 0);
			doReport(AfxGetMainWnd()->m_hWnd,TEXT("CreateService Failed！"),ERROR);
			bSuccess = FALSE;
			break;
		}
		bSuccess = TRUE;
		fontColor = RGB(0, 0, 0);
		doReport(NULL, TEXT("Install Service Successfully！"), REPORT);
		break;

	case 1:
		if (StartService(shCS, 0, NULL))
		{
			fontColor = RGB(0, 0, 0);
			doReport(NULL, TEXT("Start Service Successfully！"), REPORT);
		}
		else
		{
			fontColor = RGB(255, 0, 0);
			doReport(AfxGetMainWnd()->m_hWnd,TEXT("Start Service Failed！"),ERROR);
			bSuccess = FALSE;
			break;
		}
		bSuccess = TRUE;
		break;

	case 2: 
		if (!ControlService(shCS, SERVICE_CONTROL_STOP, &ss))
		{
			fontColor = RGB(255, 0, 0);
			doReport(AfxGetMainWnd()->m_hWnd, TEXT("StopService Failed！"),ERROR);
			bSuccess = FALSE;
			break;
		}
		fontColor = RGB(0, 0, 0);
		doReport(NULL, TEXT("Stop Service finish！"), REPORT);
		bSuccess = TRUE;
		break;

	case 3: 
		if (!DeleteService(shCS))
		{
			fontColor = RGB(255, 0, 0);
			doReport(AfxGetMainWnd()->m_hWnd, TEXT("DeleteService Failed！"),ERROR);
			bSuccess = FALSE;
			break;
		}
		fontColor = RGB(0, 0, 0);
		doReport(NULL, TEXT("Uninstall Service finish！"), REPORT);
		bSuccess = TRUE;
		break;

	default:
		break;
	}

	if (shCS)
	{
		CloseServiceHandle(shCS);
		shCS = NULL;
	}
	if (shOSCM)
	{
		CloseServiceHandle(shOSCM);
		shOSCM = NULL;
	}

	return bSuccess;
}

HBRUSH CWinDriverToolsDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何特性
	switch (pWnd->GetDlgCtrlID())
	{	
		case IDC_STATIC_ERRMSG:
			pDC->SetTextColor(fontColor);
			return hbr;
		default:
			return hbr;
	}

}

/*Add item to Register List */
void CWinDriverToolsDlg::OnBnClickedButtonAdd()
{
	RegisterDlg registerDlg;
	registerDlg.DoModal();
	// TODO: 在此添加控件通知处理程序代码
	 
}

/*Delete Item from Register List*/
void CWinDriverToolsDlg::OnBnClickedButtonDel()
{

	int row = regListCtl.GetSelectionMark();
	CString path = regListCtl.GetItemText(row, 0);
	// TODO: 在此添加控件通知处理程序代码
	if (row == -1) 
	{
		doReport(NULL, TEXT("Please Select an item to delete"), REPORT);
	}
	else
	{
		if (ERROR_SUCCESS == RegDeleteKey(HKEY_LOCAL_MACHINE, path))
		{
			regListCtl.DeleteItem(row);
			regListCtlNum--;
		}
		else
		{
			doReport(NULL, TEXT("Delete Registry Content fail！"), REPORT);
		}
	}
	
}

