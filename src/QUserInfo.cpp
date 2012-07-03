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
	LPTSTR str = malloc(SIZE);
	LPDWORD size = SIZE;
#ifdef UNICODE
	if(!GetUserName(str, size)) ret = "";
	else ret = QString::fromUtf16(str);
#else
	if(!GetUserName(str, size)) ret = "";
	else ret = QString::fromAscii(str);
#endif
	free(str);
#else
	ret = QString::fromUtf8(getlogin());
#endif
	return ret;
}