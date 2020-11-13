
// Serial_Port_TestDlg.h : header file
//

#pragma once
#include "afxwin.h"
#include "GridCtrl.h"
#include "GridCellCombo.h"
#include <stdio.h>						/* location of: printf() */
#include <stdlib.h>						/* location of: atof(), atoi() */
#include "afxcmn.h"

// CSerial_Port_TestDlg dialog
#define SZ_TX_BUFF 1024

#define CONT_RES_MAXROW 20
//Realy Pin
#define NUM 0
#define NC1 1
#define NC2 2
#define NO1 3
#define NO2 4
//Relay Type
#define EE2 0
#define UD2 1
#define TLP3107_3100 2
//Test Type
#define BULK 0
#define CARD 1



struct Relay
{
	CString Relay_data_No;
	CString Relay_data_NC1;
	CString Relay_data_NC2;
	CString Relay_data_NO1;
	CString Relay_data_NO2;

	Relay()
	{
		Relay_data_No.Empty();
		Relay_data_NC1.Empty();
		Relay_data_NC2.Empty();
		Relay_data_NO2.Empty();
		Relay_data_NO1.Empty();
	}
};
class CSerial_Port_TestDlg : public CDialogEx
{
public:
	CGridCtrl m_Grid;

	CString str_Coil_Buff[CONT_RES_MAXROW];
	CString str_NC1_Buff[CONT_RES_MAXROW];
	CString str_NC2_Buff[CONT_RES_MAXROW];
	CString str_NO1_Buff[CONT_RES_MAXROW];
	CString str_NO2_Buff[CONT_RES_MAXROW];

	CArray<Relay*, Relay*> m_arrRelay;
public:
	void On_Msg_Serial_Port(long port_num,long status);
	int Convert_Tx_String(CString str);
	int Convert_Tx_Element(CString str,char mode);	
	int Insert_Tx_Buff(BYTE data);
	int Send_Buff(CString portname,void *data,DWORD size);
	int Send_Packet(void);  	
	int Send_Frame(UINT id);
	CString str_Port_Name;	
	BYTE ab8_Tx_Buff[SZ_TX_BUFF];
	int i_Index_Tx_Buff;
	int i_Packet_End_Class;
	int i_Now_Row;
protected:
	int i_Rx_Line_Count;
// Construction
public:
	CSerial_Port_TestDlg(CWnd* pParent = NULL);	// standard constructor
	~CSerial_Port_TestDlg();
// Dialog Data
	enum { IDD = IDD_SERIAL_PORT_TEST_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:	
	afx_msg void OnEnChangeEditTx();
	afx_msg void OnBnClickedBtnOpen();	
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	afx_msg void OnCbnSelchangeComboEnd();	

	afx_msg void OnCustomdrawTcpList(NMHDR* pNMHDR, LRESULT* pResult);
	
public:	
	int Open_Dev(int id);
	int Close_Dev(int id);
	int Command_Write(CString command);
	char * Command_Read(CString command);
	int Read_Measurement(double & val);		
	void Wait(DWORD dwMillisecond);	
	
	int Convert_Tx_String2(CString str);
	CString m_strJigRxData;
	int EnInstrument2280s();
	int EnInstrumentDMM7510();				

	void Relsult_Bulk(int row, int col);
	void Relsult_Card(int row, int col);
	void ConResInit();


	void PowerOn();
	void PowerOff();
	double MeasRead();

	void EE2_Type();

	afx_msg void OnBnClickedBtnSend();
	afx_msg void OnBnClickedBtnPowerOn();
	afx_msg void OnBnClickedBtnPowerOff();
	afx_msg void OnBnClickedBtnMeasRead();

public:
	afx_msg void OnBnClickedTest();
	CListCtrl m_List;
	afx_msg void OnBnClickedSave();
	afx_msg void OnBnClickedLoad();

	void RemoveAll();
	void Listclear();

	void OnSaveContRes(FILE* fp);
	afx_msg void OnBnClickedClear();

	int m_Test_Type;
	int m_Relay_Type;
	int m_Voltage;
	int i_Row_Index;

	afx_msg void OnNMClickListctrl(NMHDR *pNMHDR, LRESULT *pResult);
};
