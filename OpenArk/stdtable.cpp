#include "stdtable.h"


StdTable::StdTable(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	//初始化数据
	mTableView = ui.tableView;
	InitStdTableView();
}

StdTable::~StdTable()
{

}

void StdTable::InitStdTableView()
{
	mSourceModel = new QStandardItemModel;
	mSortModel = new MySortModel(this);

	mSortModel->setSourceModel(mSourceModel);
	
	mTableView->setModel(mSortModel);
	
}
