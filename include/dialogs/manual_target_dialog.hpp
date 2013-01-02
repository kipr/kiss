#ifndef _MANUAL_TARGET_SELECTOR_HPP_
#define _MANUAL_TARGET_SELECTOR_HPP_

#include <QDialog>

#include "target.hpp"

namespace Ui
{
	class ManualTargetDialog;
}

namespace Kiss
{
	namespace Target
	{
		class InterfaceModel;
	}
	
	namespace Dialog
	{
		class ManualTarget : public QDialog
		{
		Q_OBJECT
		public:
			ManualTarget(QWidget *parent = 0);
			~ManualTarget();
			
			Kiss::Target::TargetPtr target() const;
			
		private:
			Ui::ManualTargetDialog *ui;
			Kiss::Target::InterfaceModel *m_model;
		};
	}
}

#endif
