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

#ifndef __SERIAL_CLIENT_H__
#define __SERIAL_CLIENT_H__

#include <QSerialPort.h>
#include <QDataStream>
#include <QStringList>

#define SERIAL_MAX_RETRY 5
#define SERIAL_TIMEOUT 500
#define SERIAL_START (quint16)(0xCBC)
#define SERIAL_KEY (quint32)(0xB07BA11)

#define SERIAL_MESSAGE_OK   ((quint8)1)
#define SERIAL_MESSAGE_FAIL ((quint8)2)

class SerialClient : QObject
{
Q_OBJECT

public:
    SerialClient(QString port = QString(), QObject *parent = 0);
    ~SerialClient();
    
    bool sendFile(QString name, QStringList deps);
    
    void setPort(QString port);
    
private:
    QSerialPort m_serialPort;
    QDataStream m_stream;
    
    bool checkOk();
    bool writePacket(QByteArray& data);
    
};

#endif
