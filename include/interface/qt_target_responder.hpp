#ifndef _QT_TARGET_RESPONDER_HPP_
#define _QT_TARGET_RESPONDER_HPP_

#include "target_responder.hpp"

namespace Kiss
{
	namespace Target
	{
		class QtResponder : public QObject, public Responder
		{
		Q_OBJECT
		public:
			virtual void availableResponse(Target *target, const bool& available); 
			virtual void compileResponse(Target *target, const Compiler::OutputList& results);
			virtual void downloadResponse(Target *target, const bool& success);
			virtual void runResponse(Target *target, const bool& success);
			virtual void listResponse(Target *target, const QStringList& programs);
			virtual void deleteResponse(Target *target, const bool& success);
			virtual void interactionResponse(Target *target, const QString& ret);
			virtual void unknownResponse(Target *target, const QByteArray& payload);
			virtual void customResponse(Target *target, const QString& type, const QByteArray& payload);
			virtual void communicationError(Target *target);
			virtual void connectionError(Target *target);
			virtual void notAuthenticatedError(Target *target);
			virtual void authenticationResponse(Target *target, const Responder::AuthenticateReturn& response);

		signals:
			void availableFinished(bool available);
			void compileFinished(const Compiler::OutputList& results);
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
			void authenticationResponse(const Responder::AuthenticateReturn& response);
		};
	}
}

#endif
