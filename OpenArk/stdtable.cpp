#include "stdtable.h"
#include <qfont.h>

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
	 mTableView->setSelectionBehavior(QAbstractItemView::SelectionBehavior::SelectRows);
	 mTableView->verticalHeader()->setDefaultSectionSize(10);//设置默认行高度
	 mTableView->setWordWrap(false);//禁止换行
	 mTableView->setEditTriggers(QAbstractItemView::NoEditTriggers);//不可编辑
	 mTableView->horizontalHeader()->setStretchLastSection(true);//最后一行扩展
	 mTableView->verticalHeader()->setHidden(true);//隐藏行标题
	 mTableView->setFont(QFont(QString::fromLocal8Bit("微软雅黑")));
	 mTableView->horizontalHeader()->setSortIndicatorShown(false);//不显示排序指示器
	 mTableView->horizontalHeader()->setFixedHeight(20);//标题高度
	 mTableView->horizontalHeader()->setBackgroundRole(QPalette::Base);//标题颜色
	 mTableView->horizontalHeader()->setHighlightSections(false);//不高亮标题
	
	
}
