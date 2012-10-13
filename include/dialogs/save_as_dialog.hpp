
#ifndef _SAVE_AS_DIALOG_HPP_
#define _SAVE_AS_DIALOG_HPP_

#include <QDialog>
#include <QItemSelection>

class QFileSystemModel;

namespace Ui
{
	class SaveAsDialog;
}

namespace Kiss
{
	namespace Dialog
	{
		class SaveAs : public QDialog
		{
		Q_OBJECT
		public:
			SaveAs(QWidget *parent = 0);
			~SaveAs();
			
			void setNameFilters(const QStringList& filters);
			QStringList nameFilters() const;
			
			void setFileName(const QString& name);
			QString fileName() const;
			
			void setRootPath(const QString& path);
			QString rootPath() const;
			
			QString filePath() const;
			
		private slots:
			void fileNameChanged(const QString& name);
			void selectionChanged(const QModelIndex& index);
			
		private:
			QFileSystemModel *m_model;
			Ui::SaveAsDialog *ui;
		};
	}
}

#endif