#ifndef _TEMPLATE_FILE_HPP_
#define _TEMPLATE_FILE_HPP_

#include <QString>
#include <QByteArray>

namespace Kiss
{
	namespace Template
	{
		class File
		{
		public:
			File();
			File(const QByteArray& data, const QString& lexer = QString());
			
			bool isNull() const;
			
			bool hasLexer() const;
			const QString& lexer() const;
			const QByteArray& data() const;
			
			QString resolvedData() const;
			
			void setLexer(const QString& lexer);
			void setData(const QByteArray& data);
			
		private:
			QString m_lexer;
			QByteArray m_data;
		};
	}
}

#endif
