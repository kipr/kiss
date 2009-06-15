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

#ifndef __SERIAL_PORT_H__
#define __SERIAL_PORT_H__

/* Currenty hard-wired for ReadWrite at 38400 */

#include <QIODevice>
#include <QString>

#ifdef WIN32
#include <windows.h>
#define OSHANDLE HANDLE
#else
#define OSHANDLE int
#endif

class QSerialPort : public QIODevice
{
public:
    QSerialPort(QString filename, QObject *parent = 0);
    ~QSerialPort();
    
    bool open(OpenMode mode = 0);
    void close();
    
    bool isSequential() { return true; }
    
#ifndef WIN32
    bool select(int msecs);
#endif
    
    void setPort(QString port);

protected:
    void configurePort();
    
    qint64 readData(char *data, qint64 maxSize);
    qint64 writeData(const char *data, qint64 maxSize);
    
private:
    QString m_name;
    OSHANDLE m_handle;
};
#endif
