#ifndef _QTTARGETRESPONDER_H_
#define _QTTARGETRESPONDER_H_

#include "TargetResponder.h"

class QtTargetResponder : public QObject, public TargetResponder
{
Q_OBJECT
public:
	virtual void availableResponse(Target *target, bool available); 
	virtual void compileResponse(Target *target, CompileResult results);
	virtual void downloadResponse(Target *target, bool success);
	virtual void runResponse(Target *target, bool success);
	virtual void listResponse(Target *target, const QStringList& programs);
	virtual void deleteResponse(Target *target, bool success);
	virtual void interactionResponse(Target *target, const QString& ret);
	virtual void unknownResponse(Target *target, const QByteArray& payload);
	virtual void customResponse(Target *target, const QString& type, const QByteArray& payload);
	virtual void communicationError(Target *target);
	virtual void connectionError(Target *target);
	virtual void notAuthenticatedError(Target *target);
	virtual void authenticationResponse(Target *target, const TargetResponder::AuthenticateReturn& response);
	
signals:
	void availableFinished(bool available);
	void compileFinished(CompileResult results);
	void downloadFinished(bool success);
	void runFinished(bool success);
	void listFinished(const QStringList& programs);
	void deleteFinished(bool success);
	void interactionFinished(const QString& ret);
	void customFinished(const QString& type, const QByteArray& payload);
	void unknownFinished(const QByteArray& payload);
	void communicationError();
	void connectionError();
	void notAuthenticatedError();
	void authenticationResponse(const TargetResponder::AuthenticateReturn& response);

private:
	
};

#endif
