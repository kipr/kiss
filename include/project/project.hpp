#ifndef _PROJECT_HPP_
#define _PROJECT_HPP_

#include "target.hpp"
#include <kar/kar.hpp>
#include <pcompiler/options.hpp>

#include <QSharedPointer>

#define PROJECT_EXT "kissproj"
#define KEY_LINKS "LINKS"
#define KEY_AUTO_COMPILE_DEPS "AUTO_COMPILE_DEPS"

namespace kiss
{
	namespace project
	{	
		class Project;
		
		typedef QSharedPointer<Project> ProjectPtr;
		
		class Project
		{
		public:
			virtual ~Project();
      
			static ProjectPtr load(const QString &location);
			bool save();

			const bool download() const;
			const bool compile() const;
			const bool run() const;

			bool addFileAsCopy(const QString &path, const QString &dest);
			bool addFileAsMovedCopy(const QString &path, const QString &dest);
			bool removeFile(const QString &path);
			QStringList files() const;

			bool addFileAsLink(const QString &path);
			bool addFileAsRelativeLink(const QString &path);
			bool removeLink(const QString &path);
			QStringList links() const;
      
      bool addFolder(const QString &path, const QString &folderName);
      bool removeFolder(const QString &path);
			
			void setDeps(const QStringList &deps);
			QStringList deps() const;
			
			void setAutoCompileDeps(bool autoCompileDeps);
			bool autoCompileDeps() const;
	
			void setCompilerFlag(const QString &flag, const QString &value);
			void setCompilerFlags(const Compiler::Options &flags);
			void removeCompilerFlag(const QString &flag);
			const Compiler::Options compilerFlags() const;
			
			void setCompileLib(bool lib);
			bool compileLib() const;
			
			void setTarget(const target::TargetPtr &target);
			target::TargetPtr target() const;

			void setName(const QString &name);
			const QString &name() const;
			const QString &projectFilename() const;
	
			const QString &location() const;
			virtual KarPtr archive() const;
      
      static QString associatedProject(const QString &path);
			
		private:
			Project(const QString &location);
			QStringList reservedKeys() const;
	
			QString m_name;
			QString m_location;
			QString m_projectFilename;
			target::TargetPtr m_target;
			Compiler::Options m_options;
		};
	}
}

#endif
