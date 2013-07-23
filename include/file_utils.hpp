#ifndef _FILE_UTILS_HPP_
#define _FILE_UTILS_HPP_

#include <QFileDialog>
#include <QDir>
#include <QString>

namespace kiss
{
	class FileUtils
	{
	public:
		static QString absolutePath(const QString &path, const QDir &originDir);
		static QString relativePath(const QString &path, const QDir &originDir);
		static bool remove(const QString &path);

		static QString getExistingDirectory(QWidget *parent = 0, const QString &caption = QString(),
			QFileDialog::Options options = QFileDialog::ShowDirsOnly);
		static QString getOpenFileName(QWidget *parent = 0, const QString &caption = QString(),
			const QString &filter = QString(), QString *selectedFilter = 0, QFileDialog::Options options = 0);
		static QStringList getOpenFileNames(QWidget *parent = 0, const QString &caption = QString(),
			const QString &filter = QString(), QString *selectedFilter = 0, QFileDialog::Options options = 0);
		static QString getSaveFileName(QWidget *parent = 0, const QString &caption = QString(),
			const QString &filter = QString(), QString *selectedFilter = 0, QFileDialog::Options options = 0);

	private:
		static const QString openPath();
		static const QString savePath();
	};
}

#endif
