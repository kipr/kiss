#ifndef _QTDEVICERESPONDER_H_
#define _QTDEVICERESPONDER_H_

#include "DeviceResponder.h"

class QtDeviceResponder : public QObject, public DeviceResponder
{
Q_OBJECT
public:
	virtual void availableResponse(Device *device, bool available); 
	virtual void compileResponse(Device *device, CompileResult results);
	virtual void downloadResponse(Device *device, bool success);
	virtual void runResponse(Device *device, bool success);
	virtual void unknownResponse(Device *device, const QByteArray& payload);
	virtual void customResponse(Device *device, const QString& type, const QByteArray& payload);
	virtual void communicationError(Device *device);
	virtual void connectionError(Device *device);
	virtual void notAuthenticatedError(Device *device);
	virtual void authenticationResponse(Device *device, const DeviceResponder::AuthenticateReturn& response);
	
signals:
	void availableFinished(bool available);
	void compileFinished(CompileResult results);
	void downloadFinished(bool success);
	void runFinished(bool success);
	void customFinished(const QString& type, const QByteArray& payload);
	void unknownFinished(const QByteArray& payload);
	void communicationError();
	void connectionError();
	void notAuthenticatedError();
	void authenticationResponse(const DeviceResponder::AuthenticateReturn& response);

private:
	
};

#endif
