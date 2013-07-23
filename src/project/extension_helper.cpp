#include "extension_helper.hpp"

#include <QFileInfo>

using namespace kiss::project;

QString ExtensionHelper::icon(const QString &file)
{
	QFileInfo fileInfo(file);
	if(fileInfo.isDir()) return "brick.png";
	if(libExtensions().contains(fileInfo.suffix())) return "package.png";
	return "page_white.png";
}

bool ExtensionHelper::isFileEditable(const QString &file)
{
	// TODO: better way to determine editable files
	return !libExtensions().contains(QFileInfo(file).suffix());
}

const QStringList ExtensionHelper::libExtensions()
{
	return QStringList() << "a" << "so" << "dylib" << "dll";
}
