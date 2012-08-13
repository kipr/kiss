#include "QtTargetResponder.h"

void QtTargetResponder::availableResponse(Target *target, bool available)
{
	emit availableFinished(available);
}

void QtTargetResponder::compileResponse(Target *target, CompileResult results)
{
	emit compileFinished(results);
}

void QtTargetResponder::downloadResponse(Target *target, bool success)
{
	emit downloadFinished(success);
}

void QtTargetResponder::runResponse(Target *target, bool success)
{
	emit runFinished(success);
}

void QtTargetResponder::listResponse(Target *target, const QStringList& programs)
{
	emit listFinished(programs);
}

void QtTargetResponder::deleteResponse(Target *target, bool success)
{
	emit deleteFinished(success);
}

void QtTargetResponder::interactionResponse(Target *target, const QString& ret)
{
	emit interactionFinished(ret);
}

void QtTargetResponder::customResponse(Target *target, const QString& type, const QByteArray& payload)
{
	emit customFinished(type, payload);
}

void QtTargetResponder::unknownResponse(Target *target, const QByteArray& payload)
{
	emit unknownFinished(payload);
}

void QtTargetResponder::communicationError(Target *target)
{
	emit communicationError();
}

void QtTargetResponder::connectionError(Target *target)
{
	emit connectionError();
}

void QtTargetResponder::notAuthenticatedError(Target *target)
{
	emit notAuthenticatedError();
}

void QtTargetResponder::authenticationResponse(Target *target, const TargetResponder::AuthenticateReturn& response)
{
	emit authenticationResponse(response);
}
