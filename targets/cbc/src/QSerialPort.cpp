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

#include "QSerialPort.h"

#ifndef WIN32
#include <fcntl.h>
#include <termios.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <sys/select.h>
#include <errno.h>
#else
#include <windows.h>
#endif

#include <unistd.h>
#include <string.h>


QSerialPort::QSerialPort(QString filename, QObject *parent) : QIODevice(parent), 
                                                              m_name(filename),
                                                              #ifdef WIN32
                                                              m_handle(INVALID_HANDLE_VALUE)
                                                              #else 
                                                              m_handle(-1)
                                                              #endif
{
}

QSerialPort::~QSerialPort()
{
    close();
}

bool QSerialPort::open(OpenMode)
{
    #ifdef WIN32
    COMMTIMEOUTS cto;
    char realPortname[10];
    memset(realPortname, 0, 10);
    strncpy(realPortname, "\\\\.\\", 4);
    strncpy(realPortname+4, m_name.toLocal8Bit(), 5);
    
    m_handle = CreateFileA(realPortname, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, 0);
    
    if(m_handle == INVALID_HANDLE_VALUE)
            return false;
            
    cto.ReadIntervalTimeout = 500;
    cto.ReadTotalTimeoutMultiplier = 0;
    cto.ReadTotalTimeoutConstant = 500;
    cto.WriteTotalTimeoutMultiplier = 0;
    cto.WriteTotalTimeoutConstant = 0;
    
    if(!SetCommTimeouts(m_handle, &cto)) {
        CloseHandle(m_handle);
        m_handle = INVALID_HANDLE_VALUE;
        return false;
    }
    
    if(!SetupComm(m_handle, 2048, 2048)) {
        CloseHandle(m_handle);
        m_handle = INVALID_HANDLE_VALUE;
        return false;
    }
    
    if(!SetCommMask(m_handle, 0)) {
        CloseHandle(m_handle);
        m_handle = INVALID_HANDLE_VALUE;
        return false;
    }
    configurePort();
    return QIODevice::open(ReadWrite);
    #else
    m_handle = ::open(m_name.toLocal8Bit(), O_RDWR | O_NDELAY | O_NOCTTY);
    
    if(m_handle > 0) {
        ::fcntl(m_handle, F_SETFL, ~O_NDELAY & ::fcntl(m_handle, F_GETFL, 0));
        configurePort();
        return QIODevice::open(ReadWrite);
    }
    #endif
    return false;
}

void QSerialPort::setPort(QString port)
{
    m_name = port;
}

void QSerialPort::close()
{
#ifdef WIN32
    if(m_handle != INVALID_HANDLE_VALUE) {
        CloseHandle(m_handle);
        m_handle = INVALID_HANDLE_VALUE;
        QIODevice::close();
    }
#else
     if(m_handle > 0) {
        ::close(m_handle);
        m_handle = -1;
        QIODevice::close();
    }
#endif
}

void QSerialPort::configurePort()
{
#ifdef WIN32
    DCB dcb;
    
    memset(&dcb, sizeof(dcb), 0);
    dcb.DCBlength = sizeof(dcb);
    
    if(!GetCommState(m_handle, &dcb))
        return;
    
    dcb.DCBlength = sizeof(dcb);
    dcb.BaudRate = 38400;
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
    
    SetCommState(m_handle, &dcb);

#else
    struct termios tio;
    
    tcgetattr(m_handle, &tio);
    
    tio.c_iflag = IGNBRK;
    tio.c_oflag = 0;
    tio.c_cflag = CREAD | CLOCAL | CS8;
    tio.c_lflag = 0;
    bzero(&tio.c_cc, sizeof(tio.c_cc));
    
    cfsetspeed(&tio, B38400);
    
    tcsetattr(m_handle, TCSADRAIN, &tio);
#endif
}

qint64 QSerialPort::readData(char *data, qint64 maxSize)
{
#ifdef WIN32
    if(m_handle != INVALID_HANDLE_VALUE) {
        DWORD read_count = 0;
        qWarning("ReadFile...");
        if(!ReadFile(m_handle, data, maxSize, &read_count, NULL)) {
            qWarning("Readfile failed...");
            return 0;
        }
            qWarning("Readfile returned %d", read_count);
        return read_count;
    }
#else
    if(m_handle > 0) {
        if(select(500)) {
            int ret = ::read(m_handle, data, maxSize);
            if(ret >= 0) return ret;
        }
        return 0;
    }
    return -1;
#endif
}

qint64 QSerialPort::writeData(const char *data, qint64 maxSize)
{
#ifdef WIN32
    if(m_handle != INVALID_HANDLE_VALUE) {
        DWORD write_count;
        
        if(!WriteFile(m_handle, data, maxSize, &write_count, NULL))
            return 0;
        return write_count;
    }
#else
    if(m_handle > 0) {
        int ret = ::write(m_handle, data, maxSize);
        if(ret >= 0) return ret;
        if(ret == -EAGAIN) return 0;
    }
    return -1;
#endif
}

#ifndef WIN32
bool QSerialPort::select(int msecs)
{
    fd_set fds;
    struct timeval timeout;
    
    FD_ZERO(&fds);
    FD_SET(m_handle, &fds);
    
    timeout.tv_sec  = (msecs / 1000);
    timeout.tv_usec = (msecs % 1000) * 1000;
    
    int n = ::select(m_handle + 1, &fds, 0, 0, &timeout);
    
    if(n == 1) return true;
    return false;
}
#endif
