#ifndef _TEMPLATE_PACK_HPP_
#define _TEMPLATE_PACK_HPP_

#include "template_file.hpp"

#include <QString>
#include <QIcon>
#include <QStringList>

namespace Kiss
{
	class Kar;
	
	namespace Template
	{
		class Pack : public QObject
		{
		Q_OBJECT
		Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
		public:
			~Pack();
			
			void setName(const QString& name);
			QString name() const;
			
			const QString& loadedFrom() const;
			
			bool addFile(const QString& path, const Template::File& file);
			void setFile(const QString& path, const Template::File& file);
			bool removeFile(const QString& path);
			bool hasFile(const QString& path);
			bool rename(const QString& path, const QString& newPath);
			
			QStringList templates() const;
			QStringList allFiles() const;
 			Template::File file(const QString& path) const;
			
			bool setDescription(const QString& path, const QString& description);
			bool setDescription(const QString& description);
			bool hasDescription(const QString& path) const;
			bool hasDescription() const;
			QString description(const QString& path) const;
			QString description() const;
			
			bool setLexer(const QString& path, const QString& lexer);
			QString lexer(const QString& path) const;
			
			static Pack *create();
			static Pack *load(const QString& path);
			bool save(const QString& path) const;
			
		signals:
			void fileAdded(const QString& path, const Kiss::Template::File& file);
			void fileRemoved(const QString& path);
			void nameChanged(const QString& newName);
			
		private:
			Pack(Kar *archive, const QString& loadedFrom);
			bool isSpecialFile(const QString& path) const;
			
			Kar *m_archive;
			QString m_loadedFrom;
		};
	}
}

#endif
