#pragma once
#include <qstring.h>
#include <qstandarditemmodel.h>
#include <Windows.h>
class qtwrapper
{
};
QString ToHexQstring(qulonglong num);
QStandardItem * MakeItem(PCHAR text);
QStandardItem* MakeItem(PWSTR text);
QStandardItem * MakeItem(PVOID num);
QStandardItem * MakeItem(qulonglong num);
QStandardItem * MakeItem(QString text);
PWSTR GetFileNameFromFullPath(PWSTR Path);
int ArkGetFileNameLenFromParam(PWSTR FileName);
PWSTR ArkGetCovertPath(PWSTR Path);