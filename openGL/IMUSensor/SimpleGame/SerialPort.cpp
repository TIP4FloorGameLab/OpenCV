#include "stdafx.h"
#include "SerialPort.h"
#include <Windows.h>


using namespace std;


SerialPort::SerialPort()
{
}


SerialPort::~SerialPort()
{
}


void SerialPort::ClosePort() {
	CloseHandle(m_hComm);
	return;
}

bool SerialPort::ReadByteStr(string& buffer) {
	string rx;
	char c = 0;
	DWORD dwBytesTransferred = 0;

	while (true) {
		if (ReadFile(m_hComm, &c, 1, &dwBytesTransferred, 0)) {
			if (dwBytesTransferred == 1) {
				if (c == '\n') {
					buffer.push_back(c);
					break;
				}
				else {
					buffer.push_back(c);
				}
			}
		}
	}
	return false;
}

// SerialPort 클래스 내부에 있는 parsedata에 데이터를 파싱해서
// 넣어준다.
void SerialPort::Parsing(string& buffer) {
	// 파싱포인트
	string delimiter = ",";
	int parse_count = 0;

	size_t pos = 0;
	string token;

	while ((pos = buffer.find(delimiter)) != string::npos) {
		token = buffer.substr(0, pos);
		if (parse_count == 0)
			parsedata.str_id = token;
		else if (parse_count == 1)
			parsedata.distance_x = (float)atof(token.c_str());
		else if (parse_count == 2)
			parsedata.distance_y = (float)atof(token.c_str());
		else if (parse_count == 3)
			parsedata.distance_z = (float)atof(token.c_str());
		else if (parse_count == 4)
			parsedata.euler_x = (float)atof(token.c_str());
		else if (parse_count == 5)
			parsedata.euler_y = (float)atof(token.c_str());
		else if (parse_count == 6) {
			parsedata.euler_z = (float)atof(token.c_str());
			parse_count = -1;
			cout << parsedata.distance_x << "\t" << parsedata.distance_y << "\t" << parsedata.distance_z << "\n";
		}

		/*else if (parse_count == 4)
			parsedata.gyro_x = (float)atof(token.c_str());
		else if (parse_count == 5)
			parsedata.gyro_y = (float)atof(token.c_str());
		else if (parse_count == 6)
			parsedata.gyro_z = (float)atof(token.c_str());

		else if (parse_count == 7)
			parsedata.acc_x = (float)atof(token.c_str());
		else if (parse_count == 8)
			parsedata.acc_y = (float)atof(token.c_str());
		else if (parse_count == 9) 
			parsedata.acc_z = (float)atof(token.c_str());*/

		
			

		parse_count++;
		buffer.erase(0, pos + delimiter.length());
	}

	parsedata.battery = atoi(buffer.c_str());

}

bool SerialPort::WriteByte(BYTE bybyte){
	m_iBytesWritten = 0;
	if (WriteFile(m_hComm, &bybyte, 1, &m_iBytesWritten, NULL) == 0)
		return false;
	else
		return true;
}

bool SerialPort::OpenPort(CString portname) {
	m_hComm = CreateFile(L"//./" + portname,
		GENERIC_READ | GENERIC_WRITE, // 읽어오기만 할 예정이므로
		0,
		0,
		OPEN_EXISTING,
		0,
		0);
	if (m_hComm == INVALID_HANDLE_VALUE)
	{
		return false;
	}
	else
		return true;
}

bool SerialPort::SetCommunicationTimeouts(DWORD ReadIntervalTimeout,
	DWORD ReadTotalTimeoutMultiplier, DWORD ReadTotalTimeoutConstant,
	DWORD WriteTotalTimeoutMultiplier, DWORD WriteTotalTimeoutConstant) {
	if ((m_bPortReady = GetCommTimeouts(m_hComm, &m_CommTimeouts)) == 0)
		return false;

	m_CommTimeouts.ReadIntervalTimeout = ReadIntervalTimeout;
	m_CommTimeouts.ReadTotalTimeoutConstant = ReadTotalTimeoutConstant;
	m_CommTimeouts.ReadTotalTimeoutMultiplier = ReadTotalTimeoutMultiplier;
	m_CommTimeouts.WriteTotalTimeoutConstant = WriteTotalTimeoutConstant;
	m_CommTimeouts.WriteTotalTimeoutMultiplier = WriteTotalTimeoutMultiplier;

	m_bPortReady = SetCommTimeouts(m_hComm, &m_CommTimeouts);

	if (m_bPortReady == 0)
	{
		printf("StCommTimeouts function failed\n");
		CloseHandle(m_hComm);
		return false;
	}

	return true;
}

bool SerialPort::ConfigurePort(DWORD BaudRate, BYTE ByteSize, DWORD fParity,
	BYTE  Parity, BYTE StopBits) {
	if ((m_bPortReady = GetCommState(m_hComm, &m_dcb)) == 0)
	{
		printf("오류\n");
		CloseHandle(m_hComm);
		return false;
	}

	m_dcb.BaudRate = BaudRate;
	m_dcb.ByteSize = ByteSize;
	m_dcb.Parity = Parity;
	m_dcb.StopBits = StopBits;
	m_dcb.fBinary = true;
	m_dcb.fDsrSensitivity = false;
	m_dcb.fParity = fParity;
	m_dcb.fOutX = false;
	m_dcb.fInX = false;
	m_dcb.fNull = false;
	m_dcb.fAbortOnError = true;
	m_dcb.fOutxCtsFlow = false;
	m_dcb.fOutxDsrFlow = false;
	m_dcb.fDtrControl = DTR_CONTROL_DISABLE;
	m_dcb.fDsrSensitivity = false;
	m_dcb.fRtsControl = RTS_CONTROL_DISABLE;
	m_dcb.fOutxCtsFlow = false;
	m_dcb.fOutxCtsFlow = false;

	m_bPortReady = SetCommState(m_hComm, &m_dcb);

	if (m_bPortReady == 0)
	{
		printf("SetCommState Error\n");
		CloseHandle(m_hComm);
		return false;
	}

	return true;

}
