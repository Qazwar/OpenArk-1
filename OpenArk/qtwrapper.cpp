#include "qtwrapper.h"
#include <Windows.h>
QString ToHexQstring(qulonglong num)
{
	return "0x" + QString("%1").arg(num, 16, 16, QLatin1Char('0')).toUpper();
}

QStandardItem * MakeItem(PWSTR text)
{
	return new  QStandardItem(QString::fromWCharArray(text));

}
QStandardItem * MakeItem(PVOID num)
{
	return new  QStandardItem(ToHexQstring((qulonglong)num));

}

QStandardItem * MakeItem(qulonglong num)
{
	return new  QStandardItem(QString::number(num));
}
