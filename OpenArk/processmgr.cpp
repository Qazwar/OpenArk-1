#include "processmgr.h"

ProcessMgr::ProcessMgr(QWidget *parent)
	: StdTable(parent)
{
	InitProcessView();
}

ProcessMgr::~ProcessMgr()
{
}


void ProcessMgr::InitProcessView()
{

	mSourceModel->setHorizontalHeaderLabels(QStringList() << tr("Process") << tr("PID") << tr("PPID") << tr("Path") << tr("Eprocess") << tr("Access") << tr("Company Name"));
	mSortModel->AddColSortFun(MySortModel::SortBy::AsText);
	mSortModel->AddColSortFun(MySortModel::SortBy::AsInt);
	mSortModel->AddColSortFun(MySortModel::SortBy::AsInt);
	mSortModel->AddColSortFun(MySortModel::SortBy::AsText);
	mSortModel->AddColSortFun(MySortModel::SortBy::AsHex);
	mSortModel->AddColSortFun(MySortModel::SortBy::AsText);
	mSortModel->AddColSortFun(MySortModel::SortBy::AsText);
	

	


	
}

