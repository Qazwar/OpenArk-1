#include "HookView.h"




void HookView::OnRefresh()
{
}

HookView::HookView(QWidget *parent) :StdDialog(parent)
{

	InitView();
}


HookView::~HookView()
{
}

void HookView::InitView()
{
	mSourceModel->setHorizontalHeaderLabels(QStringList() << tr("serial num") << tr("fun name") << tr("cur funaddr")
		<< "Hook" << tr("original funaddr") << tr("in module"));
	mSortModel->mColSortFunctions.resize(Col::LastCol);

	mSortModel->mColSortFunctions[Col::CurFunAddr] = MySortModel::SortBy::AsHex;
	mSortModel->mColSortFunctions[Col::FunName] = MySortModel::SortBy::AsText;
	mSortModel->mColSortFunctions[Col::Hook] = MySortModel::SortBy::AsText;
	mSortModel->mColSortFunctions[Col::InMod] = MySortModel::SortBy::AsText;
	mSortModel->mColSortFunctions[Col::OriginalAddr] = MySortModel::SortBy::AsHex;
	mSortModel->mColSortFunctions[Col::Serial] = MySortModel::SortBy::AsInt;

	//SetContextMenu();



}

//void HookView::SetContextMenu()
//{
//
//	mMenu.addAction(tr("refresh"), this, &HookView::OnRefresh);
//
//	setContextMenuPolicy(Qt::ContextMenuPolicy::CustomContextMenu);
//	connect(this, &HookView::customContextMenuRequested, this, [=](const QPoint &pos)
//		{
//			mMenu.exec(QCursor::pos());
//
//		}
//	);
//
//
//}




void HookView::OnNouse()
{
}