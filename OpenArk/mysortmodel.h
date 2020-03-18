#pragma once
#include <QSortFilterProxyModel>


class MySortModel : public QSortFilterProxyModel
{
	Q_OBJECT
public:
	
	 // Sorting
	struct SortBy
	{
		typedef std::function<bool(const QString &, const QString &)> t;
		static bool AsText(const QString & a, const QString & b);
		static bool AsInt(const QString & a, const QString & b);
		static bool AsHex(const QString & a, const QString & b);
	};

	MySortModel(QObject *parent);
	~MySortModel();
	bool lessThan(const QModelIndex &left, const QModelIndex &right) const override;
	void AddColSortFun(SortBy::t pfun);
	
	
	QVector<SortBy::t> mColSortFunctions;
};
