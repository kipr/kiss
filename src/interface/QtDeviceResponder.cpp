#include "QtDeviceResponder.h"

void QtDeviceResponder::availableResponse(Device *device, bool available)
{
	emit availableFinished(available);
}

void QtDeviceResponder::compileResponse(Device *device, CompileResult results)
{
	emit compileFinished(results);
}

void QtDeviceResponder::downloadResponse(Device *device, bool success)
{
	emit downloadFinished(success);
}

void QtDeviceResponder::runResponse(Device *device, bool success)
{
	emit runFinished(success);
}

void QtDeviceResponder::customResponse(Device *device, const QString& type, const QByteArray& payload)
{
	emit customFinished(type, payload);
}

void QtDeviceResponder::unknownResponse(Device *device, const QByteArray& payload)
{
	emit unknownFinished(payload);
}

void QtDeviceResponder::communicationError(Device *device)
{
	emit communicationError();
}

void QtDeviceResponder::connectionError(Device *device)
{
	emit connectionError();
}

void QtDeviceResponder::notAuthenticatedError(Device *device)
{
	emit notAuthenticatedError();
}

void QtDeviceResponder::authenticationResponse(Device *device, bool success)
{
	emit authenticationResponse(success);
}