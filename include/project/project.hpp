#ifndef _PROJECT_HPP_
#define _PROJECT_HPP_

#include "target.hpp"
#include <kar.hpp>
#include <pcompiler/options.hpp>

#include <QSharedPointer>

#define PROJECT_EXT "kissproj"
#define LINKS_EXT "links"

namespace Kiss
{
	namespace Project
	{
		class Project;
		
		typedef QSharedPointer<Project> ProjectPtr;
		
		class Project
		{
		public:
			static ProjectPtr load(const QString &location);
			bool save();

			bool addAsCopy(const QString &path);
			bool addAsMovedCopy(const QString &path);
			bool removeFile(const QString &path);
			QStringList files() const;

			bool addAsLink(const QString &path);
			bool addAsRelativeLink(const QString &path);
			bool removeLink(const QString &path);
			QStringList links() const;
			const QString linksFilePath() const;
	
			void setSetting(const QString &key, const QString &value);
			void setSettings(const Compiler::Options &settings);
			void removeSetting(const QString &key);
			const Compiler::Options &settings() const;
	
			const QString &location() const;			
			virtual KarPtr archive() const;

			void setName(const QString &name);
			const QString &name() const;
			
			void setTarget(const Target::TargetPtr &target);
			Target::TargetPtr target() const;

			const bool download() const;
			const bool compile() const;
			const bool run() const;
			
		private:
			Project(const QString &location);
	
			QString m_name;
			QString m_location;
			Target::TargetPtr m_target;
			Compiler::Options m_settings;
		};
	}
}

#endif
