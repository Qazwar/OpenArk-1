#pragma once
#include "stdtable.h"
#include <qwidget.h>
class StdDialog :public StdTable
{
public:
	StdDialog(QWidget *parent = nullptr);
	~StdDialog();
};

