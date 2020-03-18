#include "mysortmodel.h"

MySortModel::MySortModel(QObject *parent)
	: QSortFilterProxyModel(parent)
{


}

MySortModel::~MySortModel()
{
}

bool MySortModel::lessThan(const QModelIndex & left, const QModelIndex & right) const
{
	assert(mColSortFunctions.empty() == 0);
	auto s1 = sourceModel()->data(left).toString(); auto s2 = sourceModel()->data(right).toString();
	auto column = left.column();
	return mColSortFunctions[column](s1, s2);

}

void MySortModel::AddColSortFun(SortBy::t pfun)
{
	mColSortFunctions.push_back(pfun);
}
/************************************************************************************
								   Sorting
************************************************************************************/
bool MySortModel::SortBy::AsText(const QString & a, const QString & b)
{
	auto i = QString::compare(a, b);
	if (i < 0)
		return true;
	if (i > 0)
		return false;
	return (&a) < (&b);
}

bool MySortModel::SortBy::AsInt(const QString & a, const QString & b)
{
	if (a.toLongLong() < b.toLongLong())
		return true;
	if (a.toLongLong() > b.toLongLong())
		return false;
	return (&a) < (&b);
}

bool MySortModel::SortBy::AsHex(const QString & a, const QString & b)
{
	if (a.toLongLong(0, 16) < b.toLongLong(0, 16))
		return true;
	if (a.toLongLong(0, 16) > b.toLongLong(0, 16))
		return false;
	return (&a) < (&b);
}
