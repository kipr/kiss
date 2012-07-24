#ifndef _DEVICERESPONDER_H_
#define _DEVICERESPONDER_H_

#include <kiss-compiler/Compiler.h>
#include <QByteArray>

class Device;

class DeviceResponder
{
public:
	enum AuthenticateReturn {
		AuthWillNotAccept,
		AuthTryAgain,
		AuthSuccess
	};
	
	virtual void availableResponse(Device *device, bool available) = 0; 
	virtual void compileResponse(Device *device, CompileResult results) = 0;
	virtual void downloadResponse(Device *device, bool success) = 0;
	virtual void runResponse(Device *device, bool success) = 0;
	virtual void unknownResponse(Device *device, const QByteArray& payload) = 0;
	virtual void customResponse(Device *device, const QString& type, const QByteArray& payload) = 0;
	virtual void communicationError(Device *device) = 0;
	virtual void connectionError(Device *device) = 0;
	virtual void notAuthenticatedError(Device *device) = 0;
	virtual void authenticationResponse(Device *device, const DeviceResponder::AuthenticateReturn& response) = 0;
};

#endif
