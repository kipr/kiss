#ifndef _KOVAN_SERIAL_INTERFACE_HPP_
#define _KOVAN_SERIAL_INTERFACE_HPP_

#include <QtPlugin>
#include <QRunnable>
#include <QList>
#include "interface.hpp"

class UdpAdvertiser;
class QSemaphore;

namespace kiss
{
	namespace target
	{
    
		class PortSampler : public QObject, public QRunnable
		{
		Q_OBJECT
		public:
			PortSampler(const QString &path, QSemaphore *const sema);
			~PortSampler();
			void run();

		signals:
			void found(const QString &port);
      
    private:
      QString _path;
      QSemaphore *_sema;
		};
    
    class SamplerFinished : public QObject, public QRunnable
    {
    Q_OBJECT
    public:
      SamplerFinished(const int n, QSemaphore *const sema);
      
      void run();
      
    signals:
      void runFinished();
  
    private:
      int _n;
      QSemaphore *_sema;
    };

		class KovanSerialInterface : public kiss::target::Interface
		{
		Q_OBJECT
		Q_INTERFACES(kiss::target::Interface)
		public:
			KovanSerialInterface();
			~KovanSerialInterface();
			
			virtual kiss::target::TargetPtr createTarget(const QString &address);
			virtual const bool scan(kiss::target::InterfaceResponder *responder);
			virtual void invalidateResponder();

		private slots:
			void scanStarted();
			void found(const QString &port);

		private:
			kiss::target::InterfaceResponder *m_responder;
		};
	}
}


#endif
