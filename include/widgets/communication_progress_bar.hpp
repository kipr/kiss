#ifndef _COMMUNICATION_PROGRESS_BAR_HPP_
#define _COMMUNICATION_PROGRESS_BAR_HPP_

#include <QProgressBar>

namespace Kiss
{
	namespace Target
	{
		class CommunicationManager;
	}
	
	namespace Widget
	{
		class CommunicationProgressBar : public QProgressBar
		{
		Q_OBJECT
		public:
			CommunicationProgressBar(Target::CommunicationManager *manager, QWidget *parent = 0);
			
			Target::CommunicationManager *manager() const;
			
		signals:
			void clicked();
			
		protected:
			void paintEvent(QPaintEvent *event);
			void mousePressEvent(QMouseEvent *event);
			
		private slots:
			void admitted();
			void finished();
			void queueFinished();
			
		private:
			Target::CommunicationManager *m_manager;
			quint32 m_done;
			quint32 m_total;
		};
	}
}

#endif
