#include "qt_target_responder.hpp"

using namespace Kiss::Target;

void QtResponder::availableResponse(Target *target, const bool& available)
{
	emit availableFinished(available);
}

void QtResponder::compileResponse(Target *target, const Compiler::OutputList& results)
{
	emit compileFinished(results);
}

void QtResponder::downloadResponse(Target *target, const bool& success)
{
	emit downloadFinished(success);
}

void QtResponder::runResponse(Target *target, const bool& success)
{
	emit runFinished(success);
}

void QtResponder::listResponse(Target *target, const QStringList& programs)
{
	emit listFinished(programs);
}

void QtResponder::deleteResponse(Target *target, const bool& success)
{
	emit deleteFinished(success);
}

void QtResponder::interactionResponse(Target *target, const QString& ret)
{
	emit interactionFinished(ret);
}

void QtResponder::customResponse(Target *target, const QString& type, const QByteArray& payload)
{
	emit customFinished(type, payload);
}

void QtResponder::unknownResponse(Target *target, const QByteArray& payload)
{
	emit unknownFinished(payload);
}

void QtResponder::communicationError(Target *target)
{
	emit communicationError();
}

void QtResponder::connectionError(Target *target)
{
	emit connectionError();
}

void QtResponder::notAuthenticatedError(Target *target)
{
	emit notAuthenticatedError();
}

void QtResponder::authenticationResponse(Target *target, const Responder::AuthenticateReturn& response)
{
	emit authenticationResponse(response);
}
