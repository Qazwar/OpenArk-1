#pragma once
#include <qtabwidget.h>
class MyTabWidget : public QTabWidget
{
	QOBJECT_H
public:
	MyTabWidget(QWidget * parent = 0);
	void OnTabChanged(bool checked);
	~MyTabWidget();
};

