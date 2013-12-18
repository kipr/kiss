#include "file_utils.hpp"

#include <QFile>
#include <QFileInfo>
#include <QSettings>

#define OPEN_PATH "openpath"
#define SAVE_PATH "savepath"

QString kiss::FileUtils::absolutePath(const QString &path, const QDir &originDir)
{
	if(QFileInfo(path).isAbsolute()) return path;
	return QDir::cleanPath(originDir.absoluteFilePath(path));
}

QString kiss::FileUtils::relativePath(const QString &path, const QDir &originDir)
{
	if(QFileInfo(path).isRelative()) return path;
	return originDir.relativeFilePath(path);
}

bool kiss::FileUtils::remove(const QString &path)
{
	QDir dir(path);
	if(!dir.exists()) return true;

	QFileInfoList entries = dir.entryInfoList(QDir::NoDotAndDotDot
		| QDir::System | QDir::Hidden | QDir::AllDirs | QDir::Files,
		QDir::DirsFirst);

	foreach(const QFileInfo &entry, entries) {
		const QString entryPath = entry.absoluteFilePath();
		if(!(entry.isDir() ? remove(entryPath) : QFile::remove(entryPath))) return false;
	}

	if(!dir.rmdir(path)) return false;

	return true;
}

QString kiss::FileUtils::getExistingDirectory(QWidget *parent, const QString &caption, QFileDialog::Options options)
{
	QString dirPath = QFileDialog::getExistingDirectory(parent, caption, FileUtils::openPath() + "/*", options);
	if(!dirPath.isEmpty()) QSettings().setValue(OPEN_PATH, QDir(dirPath).absolutePath());

	return dirPath;
}

QString kiss::FileUtils::getOpenFileName(QWidget *parent, const QString &caption, const QString &filter,
	QString *selectedFilter, QFileDialog::Options options)
{
	QString filePath = QFileDialog::getOpenFileName(parent, caption, FileUtils::openPath() + "/*", filter, selectedFilter, options);
	if(!filePath.isEmpty()) QSettings().setValue(OPEN_PATH, QFileInfo(filePath).absolutePath());

	return filePath;
}

QStringList kiss::FileUtils::getOpenFileNames(QWidget *parent, const QString &caption, const QString &filter,
	QString *selectedFilter, QFileDialog::Options options)
{
	QStringList filePaths = QFileDialog::getOpenFileNames(parent, caption, FileUtils::openPath() + "/*", filter, selectedFilter, options);
	if(!filePaths.isEmpty()) QSettings().setValue(OPEN_PATH, QFileInfo(filePaths.at(0)).absolutePath());

	return filePaths;
}

QString kiss::FileUtils::getSaveFileName(QWidget *parent, const QString &caption, const QString &filter,
	QString *selectedFilter, QFileDialog::Options options)
{
	QString filePath = QFileDialog::getSaveFileName(parent, caption, FileUtils::savePath() + "/*", filter, selectedFilter, options);
	if(!filePath.isEmpty()) QSettings().setValue(SAVE_PATH, QFileInfo(filePath).absolutePath());

	return filePath;
}

const QString kiss::FileUtils::openPath()
{
	return QSettings().value(OPEN_PATH, QDir::homePath()).toString();
}

const QString kiss::FileUtils::savePath()
{
	return QSettings().value(SAVE_PATH, QDir::homePath()).toString();
}
