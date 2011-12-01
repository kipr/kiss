#ifndef _MACROSTRING_H_
#define _MACROSTRING_H_

#include <QString>
#include <QStringList>
#include <QMap>
#include <QDate>

struct Macro
{
	virtual QString macro(const QString& with) const = 0;
};

struct StringMacro : Macro
{
	StringMacro(QString string) : m_string(string) {}
	
	QString macro(const QString&) const {
		return m_string;
	}
private:
	QString m_string;
};

struct DateMacro : Macro
{
	QString macro(const QString&) const {
		return QDate::currentDate().toString(Qt::TextDate);
	}
};

struct MacroString : QMap<QString, Macro*>
{
	~MacroString() {
		while (!isEmpty()) { delete begin().value(); erase(begin()); }
	}
	
	QString apply(const QString& original) const {
		QString ret = original;
		foreach(const QString& key, keys())
			ret = ret.replace(key, value(key)->macro(original));
		return ret;
	}
	
	bool macroExists(const QString source, const QString& key, const QString& stop = "") const {
		if(!stop.isEmpty() && !source.contains(stop)) return false;
		QString meta = stop.isEmpty() ? source : source.section(stop, 0, 0);
		foreach(const QString& metaLine, meta.split("\n")) {
			QStringList parts;
			foreach(const QString& part, metaLine.split(" ")) parts << part.trimmed();
			if(parts[0] == key) return true;
		}
		return false;
	}
	
	QStringList macroArguments(const QString& source, const QString& key, const QString& stop = "") const {
		if(!stop.isEmpty() && !source.contains(stop)) return QStringList();
		QString meta = stop.isEmpty() ? source : source.section(stop, 0, 0);
		foreach(const QString& metaLine, meta.split("\n")) {
			QStringList parts;
			foreach(const QString& part, metaLine.split(" ")) parts << part.trimmed();
			
			if(parts[0] == key) return parts.mid(1);
		}
		return QStringList();
	}
	
	QString nonMeta(const QString& source, const QString& stop) const {
		return source.contains(stop) ? source.section(stop, 1, 1) : source;
	}
};

#endif
