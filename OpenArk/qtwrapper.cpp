#include "qtwrapper.h"
#include <Windows.h>
#include <qdebug.h>
QString ToHexQstring(qulonglong num)
{
	return "0x" + QString("%1").arg(num, 16, 16, QLatin1Char('0')).toUpper();
}

QStandardItem * MakeItem(PWSTR text)
{
	return new  QStandardItem(QString::fromWCharArray(text));

}

QStandardItem * MakeItem(PCHAR text)
{
	return new  QStandardItem(QString::fromLocal8Bit(text));
}

QStandardItem * MakeItem(QString text)
{
	return new  QStandardItem(text);

}
PWSTR GetFileNameFromFullPath(PWSTR Path)
{
	auto p = wcsrchr(Path, L'\\');
	if (p)
	{
		return p + 1;
	}
	return PWSTR();
}
int ArkGetFileNameLenFromParam(PWSTR FileName)
{
	int i = 0;
	while (*FileName != L' ')
	{
		i++;
		FileName++;
	}
	return i;
}
QStandardItem * MakeItem(PVOID num)
{
	return new  QStandardItem(ToHexQstring((qulonglong)num));

}

QStandardItem * MakeItem(qulonglong num)
{

	return new  QStandardItem(QString::number(num));
}


PWSTR ArkGetCovertPath(PWSTR Path)
{
	static int sysLinkLen = wcslen(L"\\??\\");
	static int systemRootLen = wcslen(L"\\systemroot");
	static int windowsLen = wcslen(L"C:\\Windows");
	
	if (_wcsnicmp(Path, L"\\systemroot", systemRootLen) == 0)
	{
		
		
		memcpy(Path, L"C:\\Windows", windowsLen*2);
		wcscpy(&Path[windowsLen], &Path[systemRootLen]);
		
	}
	else if (_wcsnicmp(Path, L"\\??\\", sysLinkLen) == 0)
	{
		wcscpy(Path, &Path[sysLinkLen]);
		
	}



	return Path;
}