#include "qtwrapper.h"

QString ToHexQstring(qulonglong num)
{
	return "0x" + QString("%1").arg(num, 16, 16, QLatin1Char('0')).toUpper();
}