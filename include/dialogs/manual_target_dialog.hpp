#ifndef _MANUAL_TARGET_SELECTOR_HPP_
#define _MANUAL_TARGET_SELECTOR_HPP_

#include <QDialog>

#include "target.hpp"

namespace Ui
{
	class ManualTargetDialog;
}

namespace kiss
{
	namespace target
	{
		class InterfaceModel;
	}
	
	namespace dialog
	{
		class ManualTarget : public QDialog
		{
		Q_OBJECT
		public:
			ManualTarget(QWidget *parent = 0);
			~ManualTarget();
			
			kiss::target::TargetPtr target() const;
			
		private:
			Ui::ManualTargetDialog *ui;
			kiss::target::InterfaceModel *m_model;
		};
	}
}

#endif
