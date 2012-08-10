#include "QUserInfo.h"

#ifdef Q_OS_WIN
#include <windows.h>
#else
#include <unistd.h>
#endif

#define SIZE 256

QString QUserInfo::username()
{
	QString ret;
#ifdef Q_OS_WIN
	TCHAR str[SIZE + 1];
	DWORD size = SIZE + 1;
#ifdef UNICODE
	if(!GetUserName((TCHAR*)str, &size)) ret = "";
	else ret = QString::fromUtf16(str);
#else
	if(!GetUserName((TCHAR*)str, &size)) ret = "";
	else ret = QString::fromAscii(str);
#endif

#else
	ret = QString::fromUtf8(getlogin());
#endif
	return ret;
}
