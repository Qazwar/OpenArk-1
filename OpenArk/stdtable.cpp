#include "stdtable.h"
#include <qfont.h>

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
	 mTableView->setSelectionBehavior(QAbstractItemView::SelectionBehavior::SelectRows);
	 mTableView->verticalHeader()->setDefaultSectionSize(10);//����Ĭ���и߶�
	 mTableView->setWordWrap(false);//��ֹ����
	 mTableView->setEditTriggers(QAbstractItemView::NoEditTriggers);//���ɱ༭
	 mTableView->horizontalHeader()->setStretchLastSection(true);//���һ����չ
	 mTableView->verticalHeader()->setHidden(true);//�����б���
	 mTableView->setFont(QFont(QString::fromLocal8Bit("΢���ź�")));
	 mTableView->horizontalHeader()->setSortIndicatorShown(false);//����ʾ����ָʾ��
	 mTableView->horizontalHeader()->setFixedHeight(20);//����߶�
	 mTableView->horizontalHeader()->setBackgroundRole(QPalette::Base);//������ɫ
	 mTableView->horizontalHeader()->setHighlightSections(false);//����������
	
	
}
