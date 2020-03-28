#pragma once

#include <QtWidgets>
#include "ui_stdtable.h"
#include "mysortmodel.h"

class StdTable : public QWidget
{
	Q_OBJECT

public:
	StdTable(QWidget *parent = Q_NULLPTR);
	~StdTable();
	QModelIndex GetIndexForCurRowCol(int colIndex);
private:
	void InitStdTableView();

public:
	Ui::StdTable ui;
	QStandardItemModel *mSourceModel;
	MySortModel *mSortModel;
	QTableView *mTableView;
};
