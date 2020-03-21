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

	//显示方式
	 mTableView->setSortingEnabled(true);
	
	 mTableView->setWordWrap(false);//禁止换行
	 mTableView->setEditTriggers(QAbstractItemView::NoEditTriggers);//不可编辑
	 mTableView->horizontalHeader()->setStretchLastSection(true);//最后一行扩展
	 mTableView->verticalHeader()->setSectionsMovable(false);
	
}
