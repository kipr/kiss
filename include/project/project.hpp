#ifndef _PROJECT_HPP_
#define _PROJECT_HPP_

#include "unit.hpp"
#include <kar.hpp>

#include <QMap>
#include <QString>
#include <QStringList>
#include <QSharedPointer>

namespace Kiss
{
	namespace Project
	{
		class Project;
		
		typedef QSharedPointer<Project> ProjectPtr;
		
		class Project : public Unit
		{
		public:
			typedef QMap<QString, QString> Settings;
			
			bool linkFile(const QString &path);
			bool copyFile(const QString& path);
			bool removeFile(const QString& path);
			QStringList files() const;
			QStringList links() const;
	
			void setSettings(const Settings& settings);
			const Settings& settings() const;
	
			void setSetting(const QString& key, const QString& value);
			void removeSetting(const QString& key);
	
			const QString &location() const;
	
			bool save();
			static ProjectPtr create(const QString& location);
			static ProjectPtr load(const QString& location);
	
			void refresh();
			
			virtual KarPtr archive() const;
			
		protected:
			bool visitSelf(const Kiss::KarPtr &archive) const;
			
		private:
			Project(const QString& location);
			void refresh(const QString& location);
	
			QString m_location;
			Settings m_settings;
			Kiss::KarPtr m_archive;
		};
	}
}

#endif
