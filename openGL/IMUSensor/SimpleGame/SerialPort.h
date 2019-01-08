#pragma once
#include <atlstr.h>

class SerialPort
{
public:
	SerialPort();
	~SerialPort();
private:
	HANDLE  m_hComm;
	DCB     m_dcb;
	COMMTIMEOUTS m_CommTimeouts;
	bool    m_bPortReady;
	bool    m_bWriteRC;
	bool    m_bReadRC;
	DWORD   m_iBytesWritten;
	DWORD   m_iBytesRead;
	DWORD   m_dwBytesRead;

public:
	ParseNode parsedata;
	string filter_sensor_id;

	void ClosePort();
	bool WriteByte(BYTE bybyte);
	bool ReadByteStr(string& buffer);


	bool OpenPort(CString portname);
	bool SetCommunicationTimeouts(DWORD ReadIntervalTimeout,
		DWORD ReadTotalTimeoutMultiplier, DWORD ReadTotalTimeoutConstant,
		DWORD WriteTotalTimeoutMultiplier, DWORD WriteTotalTimeoutConstant);
	bool ConfigurePort(DWORD BaudRate, BYTE ByteSize, DWORD fParity,
		BYTE  Parity, BYTE StopBits);

	void Parsing(string& buffer);
};
