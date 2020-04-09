#pragma once

#include <QtWidgets>
#include "ui_stdtable.h"
#include "mysortmodel.h"

class StdTable : public QWidget
{
	Q_OBJECT

public:

	enum Covert
	{
		Hex,
		Int,
		Qstring
	};

	StdTable(QWidget *parent = Q_NULLPTR);
	~StdTable();
	QModelIndex GetIndexForCurRowCol(int colIndex);
	ULONG_PTR GetColDataFromHex(int Col);
	ULONG_PTR GetColDataFromInt(int Col);
	QString GetColDataFromString(int Col);
private:
	void InitStdTableView();

public:
	Ui::StdTable ui;
	QStandardItemModel *mSourceModel;
	MySortModel *mSortModel;
	QTableView *mTableView;
};
