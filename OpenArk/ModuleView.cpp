#include "ModuleView.h"



ModuleView::ModuleView(QWidget *parent):StdDialog(parent)
{
	InitView();


}


ModuleView::~ModuleView()
{
}

void ModuleView::InitView()
{
	mSourceModel->setHorizontalHeaderLabels(QStringList() << tr("mod path") << tr("base addr") << tr("mod size") << tr("company"));
	mSortModel->AddColSortFun(MySortModel::SortBy::AsText);
	mSortModel->AddColSortFun(MySortModel::SortBy::AsInt);
	mSortModel->AddColSortFun(MySortModel::SortBy::AsInt);
	mSortModel->AddColSortFun(MySortModel::SortBy::AsText);

	SetContextMenu();



}

void ModuleView::SetContextMenu()
{
}
