#ifndef _PROJECT_HPP_
#define _PROJECT_HPP_

#include <QMap>
#include <QString>
#include <QStringList>

namespace Kiss
{
	namespace Project
	{
		class Project
		{
		public:
			typedef QMap<QString, QString> Settings;
			
			QString name() const;
			
			bool addFile(const QString& path);
			bool removeFile(const QString& path);
			QStringList files() const;
	
			bool settingsEnabled() const;
			void setSettings(const Settings& settings);
			const Settings& settings() const;
	
			void setSetting(const QString& key, const QString& value);
			void removeSetting(const QString& key);
	
			const QString& location() const;
	
			bool save();
			static Project *load(const QString& location);
			static Project *loadSingleFile(const QString& location);
	
			void refresh();
	
		private:
			Project(const QString& location, bool singleFileMode);
		
			void refresh(const QString& location);
	
			QString m_location;
			bool m_singleFileMode;
		
			QStringList m_files;
			Settings m_settings;
		};
		
		typedef Project* ProjectPtr;
	}
}

#endif
