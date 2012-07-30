#ifndef _TARGETRESPONDER_H_
#define _TARGETRESPONDER_H_

#include <kiss-compiler/Compiler.h>
#include <QByteArray>

class Target;

class TargetResponder
{
public:
	enum AuthenticateReturn {
		AuthWillNotAccept,
		AuthTryAgain,
		AuthSuccess
	};
	
	virtual void availableResponse(Target *target, bool available) = 0; 
	virtual void compileResponse(Target *target, CompileResult results) = 0;
	virtual void downloadResponse(Target *target, bool success) = 0;
	virtual void runResponse(Target *target, bool success) = 0;
	virtual void unknownResponse(Target *target, const QByteArray& payload) = 0;
	virtual void customResponse(Target *target, const QString& type, const QByteArray& payload) = 0;
	virtual void communicationError(Target *target) = 0;
	virtual void connectionError(Target *target) = 0;
	virtual void notAuthenticatedError(Target *target) = 0;
	virtual void authenticationResponse(Target *target, const TargetResponder::AuthenticateReturn& response) = 0;
};

#endif
