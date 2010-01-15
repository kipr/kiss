/**************************************************************************
 *  Copyright 2007,2008,2009 KISS Institute for Practical Robotics        *
 *                                                                        *
 *  This file is part of KISS (Kipr's Instructional Software System).     *
 *                                                                        *
 *  KISS is free software: you can redistribute it and/or modify          *
 *  it under the terms of the GNU General Public License as published by  *
 *  the Free Software Foundation, either version 2 of the License, or     *
 *  (at your option) any later version.                                   *
 *                                                                        *
 *  KISS is distributed in the hope that it will be useful,               *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *  GNU General Public License for more details.                          *
 *                                                                        *
 *  You should have received a copy of the GNU General Public License     *
 *  along with KISS.  Check the LICENSE file in the project root.         *
 *  If not, see <http://www.gnu.org/licenses/>.                           *
 **************************************************************************/

#include "kiss_serial.h"
#include "serial_interface.h"

#include <string.h>

OS_HANDLE kiss_platform_serial_open(char *portname)
{
	HANDLE handle;
	COMMTIMEOUTS cto;
	char realPortname[10];

	memset(realPortname, 0, 10);
	strncpy(realPortname,"\\\\.\\", 4);
	strncpy(realPortname+4, portname, 5);

	handle = CreateFileA(realPortname,
						GENERIC_READ | GENERIC_WRITE,
						0,
						NULL,
						OPEN_EXISTING,
						0,
						0);

	if(handle == INVALID_HANDLE_VALUE)
		return handle;

	cto.ReadIntervalTimeout = MAXDWORD;
	cto.ReadTotalTimeoutMultiplier = 0;
	cto.ReadTotalTimeoutConstant = 0;
	cto.WriteTotalTimeoutMultiplier = 0;
	cto.WriteTotalTimeoutConstant = 0;

	if(!SetCommTimeouts(handle, &cto)) {
		kiss_platform_serial_close(handle);
		return INVALID_HANDLE_VALUE;
	}

	if(!SetupComm(handle, 2048, 2048)) {
		kiss_platform_serial_close(handle);
		return INVALID_HANDLE_VALUE;
	}

	if(!SetCommMask(handle, 0)) {
		kiss_platform_serial_close(handle);
		return INVALID_HANDLE_VALUE;
	}

	return handle;
}

void kiss_platform_serial_close(OS_HANDLE handle)
{
	CloseHandle(handle);
}

int kiss_platform_serial_set_baud(OS_HANDLE handle, int baud)
{
	int baud_rate;
	DCB dcb;

	switch(baud) {
		case BAUD_9600:
			baud_rate=9600;
			break;
		case BAUD_19K:
			baud_rate=19200;
			break;
		case BAUD_38K:
			baud_rate=38400;
			break;
		case BAUD_57K:
			baud_rate=57600;
			break;
		default:
			return 0;
	}

	memset(&dcb, sizeof(dcb), 0);
	dcb.DCBlength = sizeof(dcb);

	if(!GetCommState(handle, &dcb))
		return 0;

	dcb.DCBlength = sizeof(DCB);
	dcb.BaudRate = baud_rate;
	dcb.fBinary = 1;
	dcb.fOutxCtsFlow = 0;
	dcb.fOutxDsrFlow = 0;
	dcb.fDtrControl = DTR_CONTROL_DISABLE;
	dcb.fDsrSensitivity = 0;
	dcb.fTXContinueOnXoff = 0;
	dcb.fOutX = 0;
	dcb.fInX = 0;
	dcb.fErrorChar = 0;
	dcb.fNull = 0;
	dcb.fRtsControl = RTS_CONTROL_TOGGLE;
	dcb.fAbortOnError = FALSE;
	dcb.ByteSize = 8;
	dcb.Parity = NOPARITY;
	dcb.fParity = 0;
	dcb.StopBits = ONESTOPBIT;
	
	if(!SetCommState(handle, &dcb))
		return 0;

	return 1;
}

int kiss_platform_serial_read(OS_HANDLE handle, unsigned char *data, int count)
{
	DWORD read_count = 0;

	if(!ReadFile(handle, data, count, &read_count, NULL))
		return 0;

	return read_count;
}

int kiss_platform_serial_write(OS_HANDLE handle, unsigned char *data, int count)
{
	DWORD write_count;

	if(!WriteFile(handle, data, count, &write_count, NULL))
		return 0;

	return write_count;
}

int kiss_platform_serial_check_handle(OS_HANDLE handle)
{
	if(handle == INVALID_HANDLE_VALUE)
		return 0;
	return 1;
}

int kiss_platform_serial_iflush(OS_HANDLE handle)
{
	if(!PurgeComm(handle, PURGE_RXABORT | PURGE_RXCLEAR))
		return 0;
	return 1;
}

int kiss_platform_serial_oflush(OS_HANDLE handle)
{
	if(!PurgeComm(handle, PURGE_TXABORT | PURGE_TXCLEAR))
		return 0;
	return 1;
}
