#include "language_helper_c.hpp"

#include <QDebug>
#include <QFileInfo>
#include <QDir>
#include <QtPlugin>

using namespace Kiss;

Compiler::OutputList LanguageHelperC::preprocess(const Kiss::KarPtr &archive, const QStringList &searchPaths) const
{
	const static QString exts[] = {
		".c", ".cpp", ".cc", ".cxx",
		".h", ".hpp", ".hh", ".hxx",
		""
	};
	const QStringList files = archive->files();
	QStringList pertinentFiles;
	foreach(const QString &file, files) {
		const QString lowerFile = file.toLower();
		quint8 i = 0;
		bool found = false;
		while(!exts[i].isEmpty()) {
			if(lowerFile.endsWith(exts[i])) {
				found = true;
				break;
			}
			++i;
		}
		if(!found) continue;
		
		pertinentFiles << file;
	}
	
	const static QRegExp includeRexp("^\\s*#include.*\".+\"");
	
	Compiler::OutputList out;
	
	const QStringList searchPathsAndAbsolute = searchPaths + (QStringList() << "");
	
	qDebug() << "Language helper found the following pertinent files:" << pertinentFiles;
	foreach(const QString &file, pertinentFiles) {
		
		const QString raw = archive->data(file);
		QStringList lines = raw.split("\n");
		QString result;
		
		foreach(const QString &line, lines) {
			// Is this an #include?
			if(!includeRexp.exactMatch(line)) {
				result += line + "\n";
				continue;
			}
			
			int start = line.indexOf('\"');
			int end = line.indexOf('\"', start + 1);
			const QString include = line.mid(start + 1, end - start - 1);
			bool found = false;
			foreach(const QString &path, searchPathsAndAbsolute) {
				QFileInfo fileLocation(path, include);
				qDebug() << fileLocation.filePath();
				if(!fileLocation.exists()) continue;
				QString absPath = fileLocation.absoluteFilePath().mid(1);
				result += "#include \"" + absPath + "\"\n";
				QFile headerFile(fileLocation.filePath());
				if(!headerFile.open(QIODevice::ReadOnly)) {
					out << Compiler::Output(file, 1, "", tr("error: Couldn't open \"%1\" for reading").arg(include).toUtf8());
					break;
				}
				archive->setFile(absPath, headerFile.readAll());
				headerFile.close();
				qDebug() << ("#include \"" + absPath + "\"\n");
				found = true;
				break;
			}
			if(!found) out << Compiler::Output(file, 1, "", tr("error: Couldn't find locally included file \"%1\"").arg(include).toUtf8());
		}
		archive->setFile(file, result.toUtf8());
	}
	
	return out;
}

Q_EXPORT_PLUGIN2(language_helper_c, Kiss::LanguageHelperC);