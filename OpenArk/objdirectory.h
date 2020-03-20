#pragma once

#include <QWidget>
#include "ui_objdirectory.h"
#include "common.h"


enum {
	Name,
	Type,
	LinkSym
};

class ObjectView : public QWidget
{
	Q_OBJECT
public:
	typedef QList<ObInfo>* Oblist;

	ObjectView(QWidget *parent = Q_NULLPTR);
	~ObjectView();

	void UpdataObTable(QTreeWidgetItem *current, QTreeWidgetItem *previous);//切换目录项

	void SetTreeItemRecurSion(PObInfo pObInfo, QTreeWidgetItem *parent);//添加树项
	QList<Oblist> mObListList;
private slots:
	void OnRefresh();
private:
	Ui::objdirectory ui;
};
