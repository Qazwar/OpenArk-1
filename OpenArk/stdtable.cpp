#include "stdtable.h"


StdTable::StdTable(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	//��ʼ������
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

	//��ʾ��ʽ
	 mTableView->setSortingEnabled(true);
	
	 mTableView->setWordWrap(false);//��ֹ����
	 mTableView->setEditTriggers(QAbstractItemView::NoEditTriggers);//���ɱ༭
	 mTableView->horizontalHeader()->setStretchLastSection(true);//���һ����չ
	 mTableView->verticalHeader()->setSectionsMovable(false);
	
}
