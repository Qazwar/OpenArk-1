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

QModelIndex StdTable::GetIndexForCurRowCol(int colIndex)
{
	auto index = mTableView->currentIndex();
	int row = index.row();
	QModelIndex idIndex = mSortModel->index(row, colIndex);
	
	
	return idIndex;
}

ULONG_PTR StdTable::GetColDataFromHex(int Col)
{
	ULONG_PTR retValue = 0;
	auto modeIndex = GetIndexForCurRowCol(Col);
	
	retValue = mSortModel->data(modeIndex).toString().toULongLong(0,16);

	return retValue;
}

ULONG_PTR StdTable::GetColDataFromInt(int Col)
{
	ULONG_PTR retValue = 0;
	auto modeIndex = GetIndexForCurRowCol(Col);

	retValue = mSortModel->data(modeIndex).toULongLong();

	return retValue;
}

QString StdTable::GetColDataFromString(int Col)
{
	auto modeIndex = GetIndexForCurRowCol(Col);
	return mSortModel->data(modeIndex).toString();

}

void StdTable::InitStdTableView()
{
	mSourceModel = new QStandardItemModel;
	mSortModel = new MySortModel(this);

	mSortModel->setSourceModel(mSourceModel);
	mTableView->setModel(mSortModel);
	mTableView->clearSelection();
	
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
	 mTableView->setSelectionMode(QAbstractItemView::SingleSelection);//ֻ��ѡ����
	
	
	
}
