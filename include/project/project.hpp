#ifndef _PROJECT_HPP_
#define _PROJECT_HPP_

#include "unit.hpp"
#include <kar.hpp>
#include <pcompiler/options.hpp>

#include <QSharedPointer>

#define PROJECT_EXT "kissproj"
#define LINKS_FILE "project.links"

namespace Kiss
{
	namespace Project
	{
		class Project;
		
		typedef QSharedPointer<Project> ProjectPtr;
		
		class Project : public Unit
		{
		public:
			bool addAsCopy(const QString& path);
			bool removeFile(const QString& path);
			QStringList files() const;

			bool addAsLink(const QString &path);
			bool addAsRelativeLink(const QString &path);
			bool removeLink(const QString &path);
			QStringList links() const;
	
			void setSetting(const QString& key, const QString& value);
			void setSettings(const Compiler::Options& settings);
			void removeSetting(const QString& key);
			const Compiler::Options& settings() const;
	
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
	
			QStringList m_files;
			QString m_location;
			Compiler::Options m_settings;
		};
	}
}

#endif
