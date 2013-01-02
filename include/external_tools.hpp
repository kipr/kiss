#ifndef _EXTERNAL_TOOLS_HPP_
#define _EXTERNAL_TOOLS_HPP_

#include <QString>
#include <QSettings>

namespace Kiss
{
	class ExternalTools
	{
	public:
		void setTool(const QString &name, const QString &path);
		QString tool(const QString &name);
		void removeTool(const QString &name);
		
		bool launch(const QString &name);
		
	private:
		QSettings m_settings;
	};
}

#endif
