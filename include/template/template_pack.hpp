#ifndef _TEMPLATE_PACK_HPP_
#define _TEMPLATE_PACK_HPP_

#include "template_file.hpp"
#include <kar/kar.hpp>

#include <QString>
#include <QIcon>
#include <QStringList>
#include <QSharedPointer>

namespace kiss
{
	namespace templates
	{
		class Pack;
		
		typedef QSharedPointer<kiss::templates::Pack> PackPtr;
		
		class Pack : public QObject
		{
		Q_OBJECT
		Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
		public:
			~Pack();
			
			void setName(const QString &name);
			QString name() const;
			
			const QString &loadedFrom() const;
      
      bool isInternal() const;
      bool isAdvanced() const;
			
			bool addFile(const QString &path, const templates::File &file);
			void setFile(const QString &path, const templates::File &file);
			bool removeFile(const QString &path);
			bool hasFile(const QString &path);
			bool rename(const QString &path, const QString &newPath);
			
			QStringList templates() const;
			QStringList allFiles() const;
 			templates::File file(const QString &path) const;
			
			bool setDescription(const QString &path, const QString &description);
			bool setDescription(const QString &description);
			bool hasDescription(const QString &path) const;
			bool hasDescription() const;
			QString description(const QString &path) const;
			QString description() const;
			
			bool setLexer(const QString &path, const QString &lexer);
			QString lexer(const QString &path) const;
			
			static PackPtr create();
			static PackPtr load(const QString &path);
			bool save(const QString &path) const;
			
		signals:
			void fileAdded(const QString &path, const kiss::templates::File &file);
			void fileRemoved(const QString &path);
			void nameChanged(const QString &newName);
			
		private:
			Pack(const KarPtr &archive, const QString &loadedFrom);
			bool isSpecialFile(const QString &path) const;
			
			KarPtr m_archive;
			QString m_loadedFrom;
		};
	}
}

#endif
