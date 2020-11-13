
// Serial_Port_TestDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Serial_Port_Test.h"
#include "Serial_Port_TestDlg.h"
#include "afxdialogex.h"


#include "Serial_Port.h"
#include "GPIB.h"

#include <iostream>
#include <string>

#define MODE_ELEMENT_INT 0
#define MODE_ELEMENT_HEX 1
#define MODE_ELEMENT_STR 2

#define MODE_PACKET_END_NONE 0
#define MODE_PACKET_END_RETURN 1
#define MODE_PACKET_END_CHKSUM 2
#define MODE_PACKET_END_CRC16 3


#define SZ_TLB_COMBO_END_STR 4
const TCHAR* str_Combo_End[SZ_TLB_COMBO_END_STR]={_T("없음"),_T("CR(\\r\\n)"),_T("체크섬(16)"),_T("CRC(16)")};

#ifdef _DEBUG
#define new DEBUG_NEW
#endif




const unsigned short crctable[256] = {
    0x0000,    0x1021,    0x2042,    0x3063,    0x4084,    0x50a5,    0x60c6,    0x70e7,
    0x8108,    0x9129,    0xa14a,    0xb16b,    0xc18c,    0xd1ad,    0xe1ce,    0xf1ef,
    0x1231,    0x0210,    0x3273,    0x2252,    0x52b5,    0x4294,    0x72f7,    0x62d6,
    0x9339,    0x8318,    0xb37b,    0xa35a,    0xd3bd,    0xc39c,    0xf3ff,    0xe3de,
    0x2462,    0x3443,    0x0420,    0x1401,    0x64e6,    0x74c7,    0x44a4,    0x5485,
    0xa56a,    0xb54b,    0x8528,    0x9509,    0xe5ee,    0xf5cf,    0xc5ac,    0xd58d,
    0x3653,    0x2672,    0x1611,    0x0630,    0x76d7,    0x66f6,    0x5695,    0x46b4,
    0xb75b,    0xa77a,    0x9719,    0x8738,    0xf7df,    0xe7fe,    0xd79d,    0xc7bc,
    0x48c4,    0x58e5,    0x6886,    0x78a7,    0x0840,    0x1861,    0x2802,    0x3823,
    0xc9cc,    0xd9ed,    0xe98e,    0xf9af,    0x8948,    0x9969,    0xa90a,    0xb92b,
    0x5af5,    0x4ad4,    0x7ab7,    0x6a96,    0x1a71,    0x0a50,    0x3a33,    0x2a12,
    0xdbfd,    0xcbdc,    0xfbbf,    0xeb9e,    0x9b79,    0x8b58,    0xbb3b,    0xab1a,
    0x6ca6,    0x7c87,    0x4ce4,    0x5cc5,    0x2c22,    0x3c03,    0x0c60,    0x1c41,
    0xedae,    0xfd8f,    0xcdec,    0xddcd,    0xad2a,    0xbd0b,    0x8d68,    0x9d49,
    0x7e97,    0x6eb6,    0x5ed5,    0x4ef4,    0x3e13,    0x2e32,    0x1e51,    0x0e70,
    0xff9f,    0xefbe,    0xdfdd,    0xcffc,    0xbf1b,    0xaf3a,    0x9f59,    0x8f78,
    0x9188,    0x81a9,    0xb1ca,    0xa1eb,    0xd10c,    0xc12d,    0xf14e,    0xe16f,
    0x1080,    0x00a1,    0x30c2,    0x20e3,    0x5004,    0x4025,    0x7046,    0x6067,
    0x83b9,    0x9398,    0xa3fb,    0xb3da,    0xc33d,    0xd31c,    0xe37f,    0xf35e,
    0x02b1,    0x1290,    0x22f3,    0x32d2,    0x4235,    0x5214,    0x6277,    0x7256,
    0xb5ea,    0xa5cb,    0x95a8,    0x8589,    0xf56e,    0xe54f,    0xd52c,    0xc50d,
    0x34e2,    0x24c3,    0x14a0,    0x0481,    0x7466,    0x6447,    0x5424,    0x4405,
    0xa7db,    0xb7fa,    0x8799,    0x97b8,    0xe75f,    0xf77e,    0xc71d,    0xd73c,
    0x26d3,    0x36f2,    0x0691,    0x16b0,    0x6657,    0x7676,    0x4615,    0x5634,
    0xd94c,    0xc96d,    0xf90e,    0xe92f,    0x99c8,    0x89e9,    0xb98a,    0xa9ab,
    0x5844,    0x4865,    0x7806,    0x6827,    0x18c0,    0x08e1,    0x3882,    0x28a3,
    0xcb7d,    0xdb5c,    0xeb3f,    0xfb1e,    0x8bf9,    0x9bd8,    0xabbb,    0xbb9a,
    0x4a75,    0x5a54,    0x6a37,    0x7a16,    0x0af1,    0x1ad0,    0x2ab3,    0x3a92,
    0xfd2e,    0xed0f,    0xdd6c,    0xcd4d,    0xbdaa,    0xad8b,    0x9de8,    0x8dc9,
    0x7c26,    0x6c07,    0x5c64,    0x4c45,    0x3ca2,    0x2c83,    0x1ce0,    0x0cc1,
    0xef1f,    0xff3e,    0xcf5d,    0xdf7c,    0xaf9b,    0xbfba,    0x8fd9,    0x9ff8,
    0x6e17,    0x7e36,    0x4e55,    0x5e74,    0x2e93,    0x3eb2,    0x0ed1,    0x1ef0
};
#define CRC16(CRC,VAL)  CRC=((CRC<<8)^crctable[((CRC>>8)^VAL)&0xff])&0xffff
#define CRC16_INIT(CRC) CRC=0xffff
#define CRC16_END(CRC)	CRC^=0x0000


#define ID_KEYSIGHT_54832D_SCPI 7
#define ID_KEITHLEY_2280S_SCPI 5
#define ID_KEITHLEY_DMM7510_SCPI 16
#define CMD_KEITHLEY_RESET						_T("*RST")

#define CMD_KEITHLEY_2280S_VOLT_5				_T(":VOLT 5")
#define CMD_KEITHLEY_2280S_VOLT_12				_T(":VOLT 12")
#define CMD_KEITHLEY_2280S_VOLT_LIMIT_10		_T(":VOLT : LIM 10") 
#define CMD_KEITHLEY_2280S_RANGE_CURR_AUTO		_T(":SENS:CONC:RANG:AUTO ON") 
#define CMD_KEITHLEY_2280S_RANGE_CURR_10		_T(":SENS:CONC:RANG 10") 
#define CMD_KEITHLEY_2280S_LIMIT_CURR_3			_T(":CURR 0.3") 
#define CMD_KEITHLEY_2280S_OUTPUT_ON        	_T(":OUTP ON") 
#define CMD_KEITHLEY_2280S_OUTPUT_OFF        	_T(":OUTP OFF") 
#define CMD_KEITHLEY_2280S_INIT        	        _T(":INIT:CONT ON") 
#define CMD_KEITHLEY_2280S_READ					_T("READ?") 


//#define CMD_KEITHLEY_DMM7510_MDE_CONT			_T("")
#define CMD_KEITHLEY_DMM7510_MODE_CURR			_T("SENS:FUNC 'CURR'") 
#define CMD_KEITHLEY_DMM7510_MODE_RES		    _T("SENS:FUNC 'RES'") 
#define CMD_KEITHLEY_DMM7510_MODE_VOLT			_T("SENS:FUNC 'VOLT'") 
#define CMD_KEITHLEY_DMM7510_CURR_RANGE			_T("SENS:CURR:RANG:1e-9") 
#define CMD_KEITHLEY_DMM7510_AUTO_CURR_RANGE	_T("SENS:CURR:RANG:AUTO ON") 
#define CMD_KEITHLEY_DMM7510_AUTO_VOLT_RANGE	_T("SENS:VOLT:RANG:AUTO ON") 
#define CMD_KEITHLEY_DMM7510_AUTO_RES_RANGE  	_T("SENS:RES:RANG:AUTO ON") 
#define CMD_KEITHLEY_DMM7510_READ				_T(":READ?") 


#define CMD_KEITHLEY_2700_MODE_CURR				_T(":SENS:FUNC 'CURR'") 
#define CMD_KEITHLEY_2700_MODE_VOLT				_T(":SENS:FUNC 'VOLT:DC'") 
#define CMD_KEITHLEY_2700_CURR_RANGE			_T(":SENS:CURR:RANG:1e-9") 
#define CMD_KEITHLEY_2700_AUTO_CURR_RANGE		_T(":SENS:CURR:RANG:AUTO ON") 
#define CMD_KEITHLEY_2700_AUTO_VOLT_RANGE		_T(":SENS:VOLT:DC:RANG:AUTO ON") 
#define CMD_KEITHLEY_2700_READ					_T(":READ?") 



// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CSerial_Port_TestDlg dialog

CSerial_Port_TestDlg *pMainDlg;



CSerial_Port_TestDlg::CSerial_Port_TestDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSerial_Port_TestDlg::IDD, pParent)
	, m_Test_Type(0)
	, m_Relay_Type(0)
	, m_Voltage(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	pMainDlg=this;
	str_Port_Name=_T("");	
	i_Rx_Line_Count=0;
	i_Index_Tx_Buff=0;
	i_Packet_End_Class=0;
	m_strJigRxData = "";
	
}
CSerial_Port_TestDlg::~CSerial_Port_TestDlg()
{
	return ;
}
void CSerial_Port_TestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//  DDX_Radio(pDX, IDC_EE2, m_Relay_Type);
	DDX_Control(pDX, IDC_LISTCTRL, m_List);
	//  DDX_Radio(pDX, IDC_5V, m_Volt);
	//  DDX_Radio(pDX, IDC_BULK, m_Test_Type);
	DDX_Radio(pDX, IDC_BULK, m_Test_Type);
	DDX_Radio(pDX, IDC_EE2, m_Relay_Type);
	DDX_Radio(pDX, IDC_5V, m_Voltage);
}

BEGIN_MESSAGE_MAP(CSerial_Port_TestDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()	
	ON_EN_CHANGE(IDC_EDIT1_TX, &CSerial_Port_TestDlg::OnEnChangeEditTx)
	ON_BN_CLICKED(IDC_BTN_OPEN, &CSerial_Port_TestDlg::OnBnClickedBtnOpen)
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_CBN_SELCHANGE(IDC_COMBO_END, &CSerial_Port_TestDlg::OnCbnSelchangeComboEnd)		
	ON_WM_CTLCOLOR()	
	ON_BN_CLICKED(IDC_BTN_SEND, &CSerial_Port_TestDlg::OnBnClickedBtnSend)
	ON_BN_CLICKED(IDC_BTN_POWER_ON, &CSerial_Port_TestDlg::OnBnClickedBtnPowerOn)
	ON_BN_CLICKED(IDC_BTN_POWER_OFF, &CSerial_Port_TestDlg::OnBnClickedBtnPowerOff)
	ON_BN_CLICKED(IDC_BTN_MEAS_READ, &CSerial_Port_TestDlg::OnBnClickedBtnMeasRead)
	ON_BN_CLICKED(IDC_TEST, &CSerial_Port_TestDlg::OnBnClickedTest)
	ON_BN_CLICKED(IDC_SAVE, &CSerial_Port_TestDlg::OnBnClickedSave)
	ON_BN_CLICKED(IDC_LOAD, &CSerial_Port_TestDlg::OnBnClickedLoad)
	ON_BN_CLICKED(IDC_CLEAR, &CSerial_Port_TestDlg::OnBnClickedClear)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_LISTCTRL, OnCustomdrawTcpList)
	ON_NOTIFY(NM_CLICK, IDC_LISTCTRL, &CSerial_Port_TestDlg::OnNMClickListctrl)
END_MESSAGE_MAP()

void CALLBACK On_Msg_Serial_Port(long port_num,long status)
{
	if(pMainDlg->GetSafeHwnd()!=NULL)pMainDlg->On_Msg_Serial_Port(port_num,status);
	return ;
	
}

BOOL CSerial_Port_TestDlg::OnInitDialog()
{
	int int00;
	TCHAR name_device[255];
	BYTE  value_device[255];
	long ret;
	CComboBox *pcombobox;	
	DWORD size_name,size_value,type_value;
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
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
	pcombobox=(CComboBox *)GetDlgItem(IDC_COMBO_PORT);
	pcombobox->ResetContent();
	
	

	HKEY regkey00;
	if(RegOpenKey(HKEY_LOCAL_MACHINE,_T("HARDWARE\\DEVICEMAP\\SERIALCOMM"),&regkey00)==ERROR_SUCCESS)
	{
		int00=0;
		do{
		
		size_name=255;
		size_value=255;
		type_value=REG_SZ;
		ret=RegEnumValue(regkey00,int00,name_device,&size_name,NULL,&type_value,value_device,&size_value);
		if(ret==ERROR_SUCCESS)
		{
			pcombobox->InsertString(int00,(TCHAR *) value_device );
			

		}
		int00++;
		TRACE(_T("name = %s,  val=%s\n"),name_device,value_device);
		}while(ret!=ERROR_NO_MORE_ITEMS);
		RegCloseKey(regkey00);

	}

	if(pcombobox->GetCount()>0)
	{
		pcombobox->SetCurSel(0);
	}
	
	
	
	
	//pcombobox=(CComboBox *)GetDlgItem(IDC_COMBO_END);
	//pcombobox->ResetContent();
	//for(int00=0;int00<SZ_TLB_COMBO_END_STR;int00++)
	//{
	//	pcombobox->InsertString(int00,str_Combo_End[int00] );
	//}
	//if(pcombobox->GetCount()>0)
	//{
	//	i_Packet_End_Class=0;
	//	pcombobox->SetCurSel(i_Packet_End_Class);
	//}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	


	//계측기DMM7510 ON
	if (GPIBOpen(ID_KEITHLEY_DMM7510_SCPI) == 0)
	{
		GPIBWrite(CMD_KEITHLEY_RESET);		
		GPIBWrite(CMD_KEITHLEY_DMM7510_MODE_RES);		
		GPIBWrite(CMD_KEITHLEY_DMM7510_AUTO_RES_RANGE);
		



	}
	//계측기2280S ON
	if (GPIBOpen(ID_KEITHLEY_2280S_SCPI) == 0)
	{
		GPIBWrite(CMD_KEITHLEY_RESET);
		GPIBWrite(CMD_KEITHLEY_2280S_VOLT_5);
		GPIBWrite(CMD_KEITHLEY_2280S_VOLT_LIMIT_10);
		GPIBWrite(CMD_KEITHLEY_2280S_LIMIT_CURR_3);
		GPIBWrite(CMD_KEITHLEY_2280S_RANGE_CURR_AUTO);
		//GPIBWrite(CMD_KEITHLEY_2280S_OUTPUT_ON);
		//GPIBWrite(CMD_KEITHLEY_2280S_INIT);

	}	

	ConResInit();

	CRect rt;
	m_List.GetWindowRect(&rt);
	m_List.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);

	m_List.InsertColumn(0, _T("No."), LVCFMT_LEFT, rt.Width()*0.1);
	m_List.InsertColumn(1, _T("NC1"), LVCFMT_LEFT, rt.Width()*0.2);
	m_List.InsertColumn(2, _T("NC2"), LVCFMT_LEFT, rt.Width()*0.2);
	m_List.InsertColumn(3, _T("NO1"), LVCFMT_LEFT, rt.Width()*0.2);
	m_List.InsertColumn(4, _T("NO2"), LVCFMT_LEFT, rt.Width()*0.2);

	return TRUE;  // returnz TRUE  unless you set the focus to a control
}

int CSerial_Port_TestDlg::EnInstrument2280s()
{

	//계측기2280S ON
	GPIBOpen(ID_KEITHLEY_2280S_SCPI);	
	return 1;
}
int CSerial_Port_TestDlg::EnInstrumentDMM7510()
{
	//계측기DMM7510 ON
	GPIBOpen(ID_KEITHLEY_DMM7510_SCPI);	
	return 2;

}

void CSerial_Port_TestDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	
	if (nID == SC_CLOSE)
	{
		//순차적으로 무조건 종료
		if (GPIBOpen(ID_KEITHLEY_2280S_SCPI) == 0)
		{
			GPIBWrite(CMD_KEITHLEY_2280S_OUTPUT_OFF);
			Wait(100);
			GPIBClose(ID_KEITHLEY_2280S_SCPI);
		}
		if (GPIBOpen(ID_KEITHLEY_DMM7510_SCPI) == 0)
		{			
			GPIBClose(ID_KEITHLEY_DMM7510_SCPI);
		}
		
	}

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

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CSerial_Port_TestDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

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
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CSerial_Port_TestDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CSerial_Port_TestDlg::On_Msg_Serial_Port(long port_num,long status)
{
	CString str00,str01,str02,str_temp;
	BYTE buff[1024*sizeof(BYTE)];
	int int00;
	int read_size;
	BYTE byte00;
	CEdit *pedit;			
	
	switch(HIWORD(port_num))
	{
		case 0: //rx
			if((status&EV_RXCHAR)==EV_RXCHAR)
			{
				
				str00.Format(_T("COM%d"),port_num);
				pedit=(CEdit *)GetDlgItem(IDC_EDIT_RX);
				pedit->GetWindowText(str01);
				if(str01.GetLength()>=4096)
				{
						str01="";
						str02="";
				}
				pedit=(CEdit *)GetDlgItem(IDC_EDIT_RX_HEX);
				pedit->GetWindowText(str02);
				if(str02.GetLength()>=4096)
				{
						str01="";
						str02="";
				}
				do
				{
					read_size=Read_Serial_Port(str_Port_Name,buff,1024);
			
					for(int00=0;int00<read_size;int00++)
					{
						byte00=buff[int00];
						str_temp.Format(_T("[%02x]"),byte00);
												
						m_strJigRxData += byte00;
						
#if 1
						
						if(i_Rx_Line_Count<16)
						{
							switch((TCHAR)byte00)
							{
								case '\r':
								case '\n':
									//str01=_T("");
									str01+=_T(' ');
									break;
								default:
									str01+=(TCHAR)buff[int00];
									break;
							}
							str02+=str_temp;
							i_Rx_Line_Count++;
						}
						else
						{
							str01+=_T("\r\n");
							str02+=_T("\r\n");
							str01+=(TCHAR)buff[int00];
							str02+=str_temp;
							i_Rx_Line_Count=0;
						}					

#endif
									
					}
				}while(read_size>0);
												
				pedit=(CEdit *)GetDlgItem(IDC_EDIT_RX);
				pedit->SetWindowText(str01);
				pedit->LineScroll(pedit->GetLineCount());
				pedit=(CEdit *)GetDlgItem(IDC_EDIT_RX_HEX);
				pedit->SetWindowText(str02);
				pedit->LineScroll(pedit->GetLineCount());


				//RECEIVE DATA	
				
							
				
			}

			
			break;
		case 1: //tx
			break;
	}
	
	
}



void CSerial_Port_TestDlg::OnEnChangeEditTx()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here

	int int00;
	int len;
	TCHAR ch00;
	CString str00;
	CEdit *pedit;
	int00=1;
	if(str_Port_Name.IsEmpty())
	{
		int00=0;
		
	}
	else 
	{
		if(IsOpen_Serial_Port(str_Port_Name)<1)int00=0;
	}
	if(int00==0) 
	{
		str00.Format(_T("Serial Port %s  Not Open!!\n"),str_Port_Name);
		return;
	}
	pedit=(CEdit *)GetDlgItem(IDC_EDIT1_TX);
	pedit->GetWindowText(str00);
	len=str00.GetLength();
	for(int00=0;int00<len;int00++)
	{
		ch00=str00.GetAt(int00);
		if((ch00=='\r')||(ch00=='\n'))
		{
			str00=str00.Left(int00);
			str00+="\r\n";
			Write_Serial_Port(str_Port_Name,str00.GetBuffer(),str00.GetLength());
			str00.ReleaseBuffer();
			str00="";
			pedit->SetWindowText(str00);
			return ;
			
		}
	}
}


void CSerial_Port_TestDlg::OnBnClickedBtnOpen()
{
	// TODO: Add your control notification handler code here
	CString str00;

	CComboBox *pcombobox;
	pcombobox=(CComboBox *)GetDlgItem(IDC_COMBO_PORT);
	pcombobox->GetLBText(pcombobox->GetCurSel(),str00);
	
	COMMCONFIG st_com_config;
	memset(&st_com_config,0,sizeof(COMMCONFIG));
	st_com_config.dwSize=sizeof(COMMCONFIG);
	st_com_config.dwProviderSubType=PST_RS232;
	st_com_config.dwProviderOffset=0;
	st_com_config.dwProviderSize=0;
	st_com_config.wVersion=1;
	st_com_config.dcb.DCBlength=sizeof(DCB);
	if(CommConfigDialog(str00,NULL,&st_com_config))
	{
		if(str_Port_Name.IsEmpty()==FALSE)
		{
			if(str_Port_Name.Compare(str00)==0)
			{
				Close_Serial_Port(str_Port_Name);
			}
			else 
			{
				Free_Serial_Port(str_Port_Name);
				str_Port_Name=_T("");
				if(Create_Serial_Port(str00)>0)str_Port_Name=str00;
			}
		}
		else
		{
			if(Create_Serial_Port(str00)>0)str_Port_Name=str00;
		}
		
		Create_Serial_Port(str_Port_Name);
		Set_Callback_Serial_Port(str_Port_Name,::On_Msg_Serial_Port);
		//Open_Serial_Port(str_Port_Name,st_com_config.dcb);
		Set_SW_Control_Serial_Port(str_Port_Name,
			st_com_config.dcb.fTXContinueOnXoff,
			st_com_config.dcb.fInX,
			st_com_config.dcb.fOutX,
			CONST_UNUSED,CONST_UNUSED,CONST_UNUSED,CONST_UNUSED);
		
		Set_HW_Control_Serial_Port(str_Port_Name,st_com_config.dcb.fDtrControl,st_com_config.dcb.fDsrSensitivity,st_com_config.dcb.fRtsControl);
		Open_Serial_Port(str_Port_Name,
				st_com_config.dcb.BaudRate,
				st_com_config.dcb.Parity,
				st_com_config.dcb.StopBits,
				st_com_config.dcb.ByteSize);
	}
}



int CSerial_Port_TestDlg::Insert_Tx_Buff(BYTE data)
{
	if(i_Index_Tx_Buff<SZ_TX_BUFF)
	{
		ab8_Tx_Buff[i_Index_Tx_Buff++]=data;
		return 1;
	}
	return 0;
}
int CSerial_Port_TestDlg::Convert_Tx_Element(CString str,char mode)
{
	
	int count;
	int len;
	int int00;
	char ch00;
	BYTE byte00;
	TCHAR tchar00;
	len = str.GetLength();
	if(len)
	{
		switch(mode)
		{
			
			case MODE_ELEMENT_HEX://hex;
				byte00=0;
				for(count=0;count<len;count++) 
				{
					tchar00=str.GetAt(count);
					
					if((tchar00>=_T('0'))&&(tchar00<='9'))
					{
						byte00<<=4;
						byte00|=tchar00-_T('0');
					}
					else if((tchar00>=_T('A'))&&(tchar00<=_T('F')))
					{
						byte00<<=4;
						byte00|=tchar00-_T('A')+10;
					}
					else if((tchar00>=_T('a'))&&(tchar00<=_T('f')))
					{
						byte00<<=4;
						byte00|=tchar00-_T('a')+10;
					}
					

				}
				Insert_Tx_Buff(byte00);
				break;
			case MODE_ELEMENT_STR://string;
				for(count=0;count<len;count++) 
				{
					Insert_Tx_Buff((BYTE)str.GetAt(count));
				}
				break;
			default://int
				byte00=0;
				for(count=0;count<len;count++) 
				{
					tchar00=str.GetAt(count);
					
					if((tchar00>=_T('0'))&&(tchar00<='9'))
					{
						byte00*=10;
						byte00+=tchar00-_T('0');
					}
					
					

				}
				Insert_Tx_Buff(byte00);
				break;
		}
		return 1;
	}
	return 0;
	
}
int CSerial_Port_TestDlg::Convert_Tx_String(CString str)
{
	CString str_temp;
	TCHAR tchar00;
	int intex;
	intex=0;
	int char_mode=0;
	//0=int,1=hex,2 = string, 
	i_Index_Tx_Buff=0;
	str_temp=_T("");
	do{
		tchar00=str.GetAt(intex);
		if(tchar00=='\0')break;
		intex++;
		switch(tchar00)
		{
			case '"':
				Convert_Tx_Element(str_temp,char_mode);
				if(char_mode==MODE_ELEMENT_STR)char_mode=MODE_ELEMENT_INT;
				else char_mode=MODE_ELEMENT_STR;
				str_temp=_T("");
				break;
			case '+':
			case ',':
				if(char_mode==MODE_ELEMENT_STR)
				{
					str_temp+=tchar00;
					
					break;
				}
				Convert_Tx_Element(str_temp,char_mode);
				str_temp=_T("");
				char_mode=MODE_ELEMENT_INT;
				break;
			case 'X':
			case 'x':
				if(char_mode==MODE_ELEMENT_STR)
				{
					str_temp+=tchar00;
					
					break;
				}
				char_mode=MODE_ELEMENT_HEX;
				str_temp=_T("");
				break;
			default:
				str_temp+=tchar00;
				break;
		}
	}while(tchar00);
	if(str_temp.IsEmpty()==0)Convert_Tx_Element(str_temp,char_mode);		
	return 1;
}

int CSerial_Port_TestDlg::Convert_Tx_String2(CString str)
{
	
	CString str_temp;	
	str_temp = _T("");
	int index = 0;
	index = str.GetLength();
	for (int i = 0; i < index; i++)
	{
		str_temp = str.GetAt(i);
		Convert_Tx_Element(str_temp, MODE_ELEMENT_STR);
	}
	
	return 1;
}

int CSerial_Port_TestDlg::Send_Packet(void)
{
	unsigned int crc;
	unsigned int checksum;
	int int00;
	switch(i_Packet_End_Class)
	{
		case MODE_PACKET_END_RETURN:
			Insert_Tx_Buff('\r');
			Insert_Tx_Buff('\n');
			break;
		case MODE_PACKET_END_CRC16:
			CRC16_INIT(crc);
			for(int00=0;int00<i_Index_Tx_Buff;int00++)
			{
				CRC16(crc,ab8_Tx_Buff[int00]);
			}
			CRC16_END(crc);
			Insert_Tx_Buff((BYTE)(crc>>8));
			Insert_Tx_Buff((BYTE)crc);
			break;
		case MODE_PACKET_END_CHKSUM:
			checksum=0;
			for(int00=0;int00<i_Index_Tx_Buff;int00++)
			{
				checksum+=ab8_Tx_Buff[int00];
			}
			Insert_Tx_Buff((BYTE)(checksum>>8));
			Insert_Tx_Buff((BYTE)checksum);
			break;
		
	}
	Send_Buff(str_Port_Name,ab8_Tx_Buff,i_Index_Tx_Buff);
	i_Index_Tx_Buff=0;
	return 1;
}


int CSerial_Port_TestDlg::Send_Buff(CString portname,void *data,DWORD size)
{
	int ret;
	Wait_Write(portname,5000);
	ret=Write_Serial_Port(portname,data,size);
	
	i_Index_Tx_Buff=0;
	return ret;
}
int CSerial_Port_TestDlg::Send_Frame(UINT id)
{
	CString str00;
	CEdit *pedit;
	pedit=(CEdit *)GetDlgItem(IDC_EDIT_PACKET_START);
	pedit->GetWindowText(str00);
	if(str00.IsEmpty()==0)
	{
		Convert_Tx_String(str00);
		Send_Buff(str_Port_Name,ab8_Tx_Buff,i_Index_Tx_Buff);
	}
	pedit=(CEdit *)GetDlgItem(id);
	pedit->GetWindowText(str00);
	Convert_Tx_String(str00);
	Send_Packet();

	pedit=(CEdit *)GetDlgItem(IDC_EDIT_PACKET_END);
	pedit->GetWindowText(str00);
	if(str00.IsEmpty()==0)
	{
		Convert_Tx_String(str00);
		Send_Buff(str_Port_Name,ab8_Tx_Buff,i_Index_Tx_Buff);
	}
	return 1;
}





void CSerial_Port_TestDlg::OnClose()
{
	// TODO: Add your message handler code here and/or call default
	CDialogEx::OnClose();
}


void CSerial_Port_TestDlg::OnDestroy()
{
	CDialogEx::OnDestroy();
	// TODO: Add your message handler code here
}


void CSerial_Port_TestDlg::OnCbnSelchangeComboEnd()
{
	// TODO: Add your control notification handler code here
	CString str00;
	int int00;
	CComboBox *pcombobox;
	pcombobox=(CComboBox *)GetDlgItem(IDC_COMBO_END);
	
	
	int00=pcombobox->GetCurSel();
	switch(int00)
	{
		case MODE_PACKET_END_NONE:
			AfxMessageBox(_T("패킷뒤에 아무것도 붙지 않습니다"));
			i_Packet_End_Class=MODE_PACKET_END_NONE;
			break;
		case MODE_PACKET_END_RETURN:
			AfxMessageBox(_T("패킷뒤에 CR(\\r\\n) 이 추가 됩니다"));
			i_Packet_End_Class=MODE_PACKET_END_RETURN;
			break;
		case MODE_PACKET_END_CHKSUM:
			AfxMessageBox(_T("패킷뒤에 체크섬(16bit)이 추가 됩니다"));
			i_Packet_End_Class=MODE_PACKET_END_CHKSUM;
			break;
		case MODE_PACKET_END_CRC16:
			AfxMessageBox(_T("패킷뒤에 CRC(16bit)가 추가 됩니다"));
			i_Packet_End_Class=MODE_PACKET_END_CRC16;
			break;

	}
}






int CSerial_Port_TestDlg::Open_Dev(int id)
{
	int ret = GPIBOpen(id);

	return ret;
}
int CSerial_Port_TestDlg::Close_Dev(int id)
{
	GPIBClose(id);

	return 0;
}
int CSerial_Port_TestDlg::Command_Write(CString command)
{
	char* cbuff = LPSTR(LPCTSTR(command));
	GPIBWrite(cbuff);

	return 0;

}
char* CSerial_Port_TestDlg::Command_Read(CString command)
{
	char* cbuff = LPSTR(LPCTSTR(command));
	GPIBWrite(cbuff);
	char* cret = GPIBRead();

	return cret;
}

int CSerial_Port_TestDlg::Read_Measurement(double &val)
{

	GPIBWrite(CMD_KEITHLEY_DMM7510_READ);
	char* cret = GPIBRead();
	CString str00 = (CString)cret;

	int len = str00.GetLength();
	int step = 0;
	TCHAR ch00;
	CString str01 = _T("");
	double db00;

	for (int int00 = 0; int00 <= len; int00++)
	{
		ch00 = str00.GetAt(int00);
		switch (ch00)
		{
		case ',':
		case _T('\0'):
		case '\r':
		case '\n':
			if (str01.IsEmpty() == FALSE)
			{
				db00 = atof(CStringA(str01));
				switch (step)
				{
					//step=0
				case 0:
					val = db00;
					break;
					//step=1
				case 1:
					break;
				}
				step++;
			}
			str01 = "";
			break;
		default:
			str01 += ch00;
			break;
		}
	}

	return 0;
}



//delay함수..먹통방지
void CSerial_Port_TestDlg::Wait(DWORD dwMillisecond)
{
	MSG msg;
	DWORD dwStart;
	dwStart = GetTickCount();

	while (GetTickCount() - dwStart < dwMillisecond)
	{
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
}

void CSerial_Port_TestDlg::OnBnClickedBtnSend()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString str00;
	GetDlgItem(IDC_EDIT_SEND)->GetWindowText(str00);	
	//릴레이초기화
	Convert_Tx_String2(str00);
	Send_Packet();
	Wait(1500);	
}


void CSerial_Port_TestDlg::OnBnClickedBtnPowerOn()
{
	EnInstrument2280s();
	GPIBWrite(CMD_KEITHLEY_2280S_OUTPUT_ON);
	GPIBWrite(CMD_KEITHLEY_2280S_INIT);
}

void CSerial_Port_TestDlg::PowerOn()
{
	EnInstrument2280s();
	GPIBWrite(CMD_KEITHLEY_2280S_OUTPUT_ON);
	GPIBWrite(CMD_KEITHLEY_2280S_INIT);
}

void CSerial_Port_TestDlg::OnBnClickedBtnPowerOff()
{
	EnInstrument2280s();
	GPIBWrite(CMD_KEITHLEY_2280S_OUTPUT_OFF);
	//GPIBWrite(CMD_KEITHLEY_2280S_INIT);
}

void CSerial_Port_TestDlg::PowerOff()
{
	EnInstrument2280s();
	GPIBWrite(CMD_KEITHLEY_2280S_OUTPUT_OFF);
}

void CSerial_Port_TestDlg::OnBnClickedBtnMeasRead()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	GetDlgItem(IDC_EDIT_RECEIVE2)->SetWindowText(_T(""));	
	double dbVal;
	CString str;

	EnInstrumentDMM7510();			
	Read_Measurement(dbVal);	
	str.Format(_T("%.3e"), dbVal);	
	GetDlgItem(IDC_EDIT_RECEIVE2)->SetWindowText(str);	
}

double CSerial_Port_TestDlg::MeasRead()
{
	GetDlgItem(IDC_EDIT_RECEIVE2)->SetWindowText(_T(""));
	double dbVal;
	CString str;

	EnInstrumentDMM7510();
	Read_Measurement(dbVal);
	return dbVal;
}

void CSerial_Port_TestDlg::ConResInit()
{
	i_Now_Row = 0;
}

void CSerial_Port_TestDlg::Relsult_Bulk(int row, int col)
{
	CString Val_B = (_T(""));
	CString str01 = (_T(""));
	CString str00 = (_T(""));

	str01.Format(_T("%d"), row + 1);
	str00 = _T("1.9");  //Bulk Calibration value

	switch (col){
	case NUM:
		m_List.InsertItem(row, str01);
		break;
	case NC1:
		Val_B.Format(_T("%f"), (MeasRead() - (atof(str00)) - 0.03));   //NC1 평균보다 0.03 높음
		str_NC1_Buff[i_Now_Row] = Val_B;
		m_List.SetItem(row, col, LVIF_TEXT, Val_B, NULL, NULL, NULL, NULL);
		break;
	case NC2:
		Val_B.Format(_T("%f"), (MeasRead() - (atof(str00)) + 0.09));  //NC2 평균보다 0.09 낮음
		str_NC2_Buff[i_Now_Row] = Val_B;
		m_List.SetItem(row, col, LVIF_TEXT, Val_B, NULL, NULL, NULL, NULL);
		break;
	case NO1:
		Val_B.Format(_T("%f"), (MeasRead() - (atof(str00))));
		str_NO1_Buff[i_Now_Row] = Val_B;
		m_List.SetItem(row, col, LVIF_TEXT, Val_B, NULL, NULL, NULL, NULL);
		break;
	case NO2:
		Val_B.Format(_T("%f"), (MeasRead() - (atof(str00))));
		str_NO2_Buff[i_Now_Row] = Val_B;
		m_List.SetItem(row, col, LVIF_TEXT, Val_B, NULL, NULL, NULL, NULL);
		break;
	default:
		break;
	}
}

void CSerial_Port_TestDlg::Relsult_Card(int row, int col)
{
	CString Val_B = (_T(""));
	CString str01 = (_T(""));
	CString str00 = (_T(""));

	str01.Format(_T("%d"), row + 1);
	str00 = _T("1.9");  //Card Calibration value

	switch (col){
	case NUM:
		m_List.InsertItem(row, str01);
		break;
	case NC1:
		Val_B.Format(_T("%f"), (MeasRead() - (atof(str00)) - 0.19));   //NC1 평균보다 0.19 높음
		str_NC1_Buff[i_Now_Row] = Val_B;
		m_List.SetItem(row, col, LVIF_TEXT, Val_B, NULL, NULL, NULL, NULL);
		break;
	case NC2:
		Val_B.Format(_T("%f"), (MeasRead() - (atof(str00)) - 0.24));  //NC2 평균보다 0.24 높음
		str_NC2_Buff[i_Now_Row] = Val_B;
		m_List.SetItem(row, col, LVIF_TEXT, Val_B, NULL, NULL, NULL, NULL);
		break;
	case NO1:
		Val_B.Format(_T("%f"), (MeasRead() - (atof(str00)) - 0.29));  //NO1 평균보다 0.29 높음
		str_NO1_Buff[i_Now_Row] = Val_B;
		m_List.SetItem(row, col, LVIF_TEXT, Val_B, NULL, NULL, NULL, NULL);
		break;
	case NO2:
		Val_B.Format(_T("%f"), (MeasRead() - (atof(str00)) - 0.34));  //NO2 평균보다 0.34 높음
		str_NO2_Buff[i_Now_Row] = Val_B;
		m_List.SetItem(row, col, LVIF_TEXT, Val_B, NULL, NULL, NULL, NULL);
		break;
	default:
		break;
	}
}

void CSerial_Port_TestDlg::EE2_Type()
{
	switch (m_Test_Type)
	{
	case BULK:
		if (i_Row_Index == 0)
		{
			Relsult_Bulk(i_Now_Row, 0);

			PowerOff();
			Convert_Tx_String2(_T("0"));
			Send_Packet();
			Wait(500);
			Relsult_Bulk(i_Now_Row, 1);
			Wait(100);

			Convert_Tx_String2(_T("1"));
			Send_Packet();
			Wait(500);
			Relsult_Bulk(i_Now_Row, 2);
			Wait(100);

			PowerOn();
			Convert_Tx_String2(_T("2"));
			Send_Packet();
			Wait(500);
			Relsult_Bulk(i_Now_Row, 3);
			Wait(100);

			Convert_Tx_String2(_T("3"));
			Send_Packet();
			Wait(500);
			Relsult_Bulk(i_Now_Row, 4);
			Wait(600);

			PowerOff();

			i_Now_Row++;
			break;
		}
		else
		{
			PowerOff();
			Convert_Tx_String2(_T("0"));
			Send_Packet();
			Wait(500);
			Relsult_Bulk(i_Row_Index, 1);
			Wait(100);

			Convert_Tx_String2(_T("1"));
			Send_Packet();
			Wait(500);
			Relsult_Bulk(i_Row_Index, 2);
			Wait(100);

			PowerOn();
			Convert_Tx_String2(_T("2"));
			Send_Packet();
			Wait(500);
			Relsult_Bulk(i_Row_Index, 3);
			Wait(100);

			Convert_Tx_String2(_T("3"));
			Send_Packet();
			Wait(500);
			Relsult_Bulk(i_Row_Index, 4);
			Wait(600);

			PowerOff();
			
			i_Row_Index = 0;
			break;
		}
	case CARD:
		Relsult_Card(i_Now_Row, 0);

		PowerOff();
		Convert_Tx_String2(_T("0"));
		Send_Packet();
		Wait(500);
		Relsult_Card(i_Now_Row, 1);
		Wait(100);

		Convert_Tx_String2(_T("1"));
		Send_Packet();
		Wait(500);
		Relsult_Card(i_Now_Row, 2);
		Wait(100);

		PowerOn();
		Convert_Tx_String2(_T("2"));
		Send_Packet();
		Wait(500);
		Relsult_Card(i_Now_Row, 3);
		Wait(100);

		Convert_Tx_String2(_T("3"));
		Send_Packet();
		Wait(500);
		Relsult_Card(i_Now_Row, 4);
		Wait(600);

		PowerOff();

		i_Now_Row++;

		break;
	}
}
	
void CSerial_Port_TestDlg::OnBnClickedTest()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(true);

	if (m_Voltage == 0)
	{
		GPIBWrite(CMD_KEITHLEY_2280S_VOLT_5);
	}
	else if (m_Voltage == 1)
	{
		GPIBWrite(CMD_KEITHLEY_2280S_VOLT_12);
	}

	if (IDYES == AfxMessageBox(_T("Test Type 확인했습니까 ?"), MB_YESNO))
	{
		if (IDYES == AfxMessageBox(_T("Relay Type 확인했습니까 ?"), MB_YESNO))
		{
			if (IDYES == AfxMessageBox(_T("Voltage 확인했습니까 ?"), MB_YESNO))
			{
				for (int i = 0; i < 2; i++)
				{
					switch (m_Relay_Type)
					{
					case EE2:
						if (IDYES == AfxMessageBox(_T("Test 하시겠습니까?"), MB_YESNO))
						{
							EE2_Type();
							i = 0;
							break;
						}
						else{ break; }

					case UD2:
						m_List.InsertItem(1, _T("1"));
						m_List.SetItem(1, 1, LVIF_TEXT, TEXT("Test"), NULL, NULL, NULL, NULL);
						MessageBox(TEXT("두번째 선택됨")); break;

					case TLP3107_3100:
						Convert_Tx_String2(_T("1"));
						Send_Packet();
						MessageBox(TEXT("세번째 선택됨")); break;
					default:
						break;
					}
				}
			}
		}
	}
}

void CSerial_Port_TestDlg::OnSaveContRes(FILE* fp)
{
	int i_ret = 0;

	fprintf(fp, "\n");
	fprintf(fp, "No.,NC1,NC2,NO1,NO2,\n");

	for (int i = 0; i<i_Now_Row; i++)
	{
		fprintf(fp, "%d,%s,%s,%s,%s,\n", i+1, str_NC1_Buff[i],
			str_NC2_Buff[i], str_NO1_Buff[i], str_NO2_Buff[i]);
	}

	fclose(fp);

	AfxMessageBox(_T("Save Ok"));
}

void CSerial_Port_TestDlg::OnBnClickedSave()
{
	wchar_t* wchar_str;
	char* char_str00;
	char* char_str;
	int char_str_len;

	CString str_file_name;
	TCHAR ach_filters[] = _T("Probe Card Data Files (*.csv)|*.csv||");
	CFileDialog file_dlg(FALSE, _T("csv"), NULL, OFN_HIDEREADONLY, ach_filters, this);

	if (file_dlg.DoModal() == IDOK)
	{
		str_file_name = file_dlg.GetPathName();
		str_file_name.Replace(_T("\\\\"), _T("\\"));


		char_str = str_file_name.GetBuffer(str_file_name.GetLength());
		////////////CString 을 char로 변환//////////////////////////////
		//wchar_str = str_file_name.GetBuffer(str_file_name.GetLength());
		//char_str_len = WideCharToMultiByte(CP_ACP, 0, wchar_str, -1, NULL, 0, NULL, NULL);
		//char_str = new char[char_str_len];
		//WideCharToMultiByte(CP_ACP, 0, wchar_str, -1, char_str, char_str_len, 0, 0);
		////////////////////////////////////////////////////

		FILE* fp;
		if ((fp = fopen(char_str, "w+")) == NULL)
		{
			AfxMessageBox(_T("CSV파일을 생성 할 수 없습니다."));
			fclose(fp);
			fp = NULL;
			return;
		}
		else
		{
			this->OnSaveContRes(fp);
		}
	}
}

void CSerial_Port_TestDlg::RemoveAll()
{
	//해당 배열의 사이즈 만큼 반복문을 돌림
	for (int i = 0; i < m_arrRelay.GetSize(); ++i)
	{
		Relay *pRelay = m_arrRelay.GetAt(i);    //해당 배열의 값을 매칭시켜준다.

		if (NULL == pRelay)     //pRelay 객체가 NULL이면 계속 진행하고
			continue;

		delete pRelay;    //그렇지 않을 경우 객체 삭제

		pRelay = NULL;
	}

	m_arrRelay.RemoveAll();  //배열 삭제
}

void CSerial_Port_TestDlg::Listclear()
{
	i_Now_Row = 0; 

	for (size_t row = 0; row <= 50; row++)
	{
		for (size_t col = 0; col <= 5; col++)
		{
			m_List.SetItem(row, col, LVIF_TEXT, "", NULL, NULL, NULL, NULL);
		}
	}
}

void CSerial_Port_TestDlg::OnBnClickedLoad()
{
	int i_num=0;

	RemoveAll();    //배열 초기화 및 해제
	//Listclear();

	TCHAR ach_filters[] = _T("Probe Card Data Files (*.csv)|*.csv||");
	CFileDialog file_dlg(TRUE, "csv", "Probe Card Data Files", OFN_FILEMUSTEXIST | OFN_HIDEREADONLY, ach_filters, this);

	_tsetlocale(LC_ALL, "Korean");  //한국 지역을 설정

	CString str01;
	CString strFilePath = _T("");        //.csv 파일 읽을 위치 및 파일명 저장할 변수 선언 

	if (file_dlg.DoModal() == IDOK)
	{
		strFilePath = file_dlg.GetPathName();         //해당 csv 파일이 저장되어 있는 경로 탐색 후 추가

		FILE *fp = NULL;            //파일 객체 선언

		fopen_s(&fp, strFilePath, "r+b");       //해당 경로에 파일을 읽어라

		if (fp)    //해당 파일이 있고
		{
			char szContent[2048] = { 0, };
			memset(szContent, NULL, 2048);

			while (fgets(szContent, 2048, fp))   //파일을 한 라인씩 읽어라
			{
				CString strContent;

				strContent.Format("%s", szContent);
				memset(szContent, NULL, 2048);

				if (strContent.Find("#") >= 0)     //만약 csv 파일 안의 데이터 중 #이 있으면 건너 뛰어라
					continue;
				if (strContent.Find("No.") >= 0)   //만약 csv 파일 안의 데이터 중 No.(목차 데이터)가 있으면 건너 뛰어라
					continue;
				//i_num = strContent.Find("\n");
				if (strContent.Find("\n") == 1)   //만약 csv 파일 안의 데이터 중 공백이 있으면 건너 뛰어라
					continue;

				strContent.Remove('\r');
				strContent.Remove('\n');

				strContent = _T(strContent);         //유니코드 변환하여 저장
				
				Relay *pRelay = new Relay;

				CString rString;
				int nSubString = 0;

				//AfxExtractSubString() 함수를 이용하여 해당 csv 파일은 , 단위로
				while (AfxExtractSubString(rString, strContent, nSubString++, ','))
				{
					switch (nSubString)
					{
					case 1: pRelay->Relay_data_No = _T(rString); break;
					case 2: pRelay->Relay_data_NC1 = _T(rString); break;
					case 3: pRelay->Relay_data_NC2 = _T(rString); break;
					case 4: pRelay->Relay_data_NO1 = _T(rString); break;
					case 5: pRelay->Relay_data_NO2 = _T(rString); break;
					}
				}
				m_arrRelay.Add(pRelay);
			}
			fclose(fp);  //파일 닫기
		}

		Listclear();

		//CArray에서 현재 보관하고 있는 값을 출력하고 싶을 때 사용하면 됨.
		for (int i = 0; i < m_arrRelay.GetSize(); i++)
		{
			Relay *pRelay = m_arrRelay.GetAt(i);

			str01.Format(_T("%d"), i + 1);
			m_List.InsertItem(i, str01);
			m_List.SetItem(i, 1, LVIF_TEXT, pRelay->Relay_data_NC1, NULL, NULL, NULL, NULL);
			m_List.SetItem(i, 2, LVIF_TEXT, pRelay->Relay_data_NC2, NULL, NULL, NULL, NULL);
			m_List.SetItem(i, 3, LVIF_TEXT, pRelay->Relay_data_NO1, NULL, NULL, NULL, NULL);
			m_List.SetItem(i, 4, LVIF_TEXT, pRelay->Relay_data_NO2, NULL, NULL, NULL, NULL);
		}
	}
}

void CSerial_Port_TestDlg::OnBnClickedClear()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	Listclear();
}

void CSerial_Port_TestDlg::OnCustomdrawTcpList(NMHDR* pNMHDR, LRESULT* pResult)
{
	CString strNC1, strNC2, strNO1, strNO2;
	double defval = 0.4;

	BOOL bErrFlag = FALSE;

	NMLVCUSTOMDRAW* pLVCD = (NMLVCUSTOMDRAW*)pNMHDR;

	strNC1 = m_List.GetItemText(pLVCD->nmcd.dwItemSpec, 1);
	strNC2 = m_List.GetItemText(pLVCD->nmcd.dwItemSpec, 2);
	strNO1 = m_List.GetItemText(pLVCD->nmcd.dwItemSpec, 3);
	strNO2 = m_List.GetItemText(pLVCD->nmcd.dwItemSpec, 4);
	
	if (defval <= (atof(strNC1)))
	{
		bErrFlag = TRUE;
	}
	else if (defval <= (atof(strNC2)))
	{
		bErrFlag = TRUE;
	}
	else if (defval <= (atof(strNO1)))
	{
		bErrFlag = TRUE;
	}
	else if (defval <= (atof(strNO2)))
	{
		bErrFlag = TRUE;
	}

	*pResult = 0;

	if (CDDS_PREPAINT == pLVCD->nmcd.dwDrawStage)
		*pResult = CDRF_NOTIFYITEMDRAW;

	else if (CDDS_ITEMPREPAINT == pLVCD->nmcd.dwDrawStage)
	{
		if (bErrFlag)        // TCP && 긴급 플래그 일떄
		{
			pLVCD->clrText = RGB(255, 0, 0);
		}

		if (!bErrFlag)        // 긴급 플래그가 아닐때
		{
			pLVCD->clrText = RGB(0, 0, 0);
		}
		*pResult = CDRF_DODEFAULT;
	}
}

void CSerial_Port_TestDlg::OnNMClickListctrl(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

	i_Row_Index = pNMListView->iItem;
		
	*pResult = 0;
}
