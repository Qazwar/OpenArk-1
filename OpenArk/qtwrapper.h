#pragma once
#include <qstring.h>
#include <qstandarditemmodel.h>
#include <Windows.h>
class qtwrapper
{
};

QString ToHexQstring(qulonglong num);
QStandardItem* MakeItem(PWSTR text);
QStandardItem * MakeItem(PVOID num);
QStandardItem * MakeItem(qulonglong num);