#include "extension_helper.hpp"

#include <QFileInfo>

using namespace kiss::project;

const QString ExtensionHelper::icon(const QString &file)
{
  QFileInfo fileInfo(file);
  if(fileInfo.isDir()) return "brick.png";
  return iconFromExtension(fileInfo.suffix());
}

const QString ExtensionHelper::iconFromExtension(const QString &ext)
{
  if(libExtensions().contains(ext)) return "package.png";
  if(ext == "c") return "page_white_c.png";
  if(ext == "cpp") return "page_white_cpp.png";
  if(ext == "h" || ext == "hpp") return "page_white_h.png";
  if(ext == "board") return "world.png";
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
