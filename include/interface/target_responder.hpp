#ifndef _TARGETRESPONDER_H_
#define _TARGETRESPONDER_H_

#include <pcompiler/output.hpp>
#include <QByteArray>
#include <QList>

namespace Kiss
{
	namespace Target
	{
		class Target;
		
		class Responder
		{
		public:
			enum AuthenticateReturn {
				AuthWillNotAccept,
				AuthTryAgain,
				AuthSuccess
			};

			virtual void availableResponse(Target *target, const bool& available) = 0; 
			virtual void compileResponse(Target *target, const Compiler::OutputList& results) = 0;
			virtual void downloadResponse(Target *target, const bool& success) = 0;
			virtual void runResponse(Target *target, const bool& success) = 0;
			virtual void listResponse(Target *target, const QStringList& programs) = 0;
			virtual void deleteResponse(Target *target, const bool& success) = 0;
			virtual void interactionResponse(Target *target, const QString& ret) = 0;
			virtual void unknownResponse(Target *target, const QByteArray& payload) = 0;
			virtual void customResponse(Target *target, const QString& type, const QByteArray& payload) = 0;
			virtual void communicationError(Target *target) = 0;
			virtual void connectionError(Target *target) = 0;
			virtual void notAuthenticatedError(Target *target) = 0;
			virtual void authenticationResponse(Target *target, const Responder::AuthenticateReturn& response) = 0;
		};

		typedef QList<Responder *> ResponderPtrList;
	}
}

#endif
