#include "extension_helper.hpp"

#include <QFileInfo>

using namespace kiss::project;

const QString ExtensionHelper::icon(const QString &file)
{
	QFileInfo fileInfo(file);
  const QString &suffix = fileInfo.suffix();
	if(fileInfo.isDir()) return "brick.png";
	if(libExtensions().contains(suffix)) return "package.png";
  if(suffix == "c") return "page_white_c.png";
  if(suffix == "cpp") return "page_white_cpp.png";
  if(suffix == "h" || suffix == "hpp") return "page_white_h.png";
  if(suffix == "board") return "world.png";
  return "page_white.png";
}

const bool ExtensionHelper::isFileEditable(const QString &file)
{
	// TODO: better way to determine editable files
	return !libExtensions().contains(QFileInfo(file).suffix());
}

const QStringList ExtensionHelper::libExtensions()
{
	return QStringList() << "a" << "so" << "dylib" << "dll";
}
