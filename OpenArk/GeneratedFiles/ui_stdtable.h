/********************************************************************************
** Form generated from reading UI file 'stdtable.ui'
**
** Created by: Qt User Interface Compiler version 5.12.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_STDTABLE_H
#define UI_STDTABLE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QTableView>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_StdTable
{
public:
    QGridLayout *gridLayout;
    QTableView *tableView;

    void setupUi(QWidget *StdTable)
    {
        if (StdTable->objectName().isEmpty())
            StdTable->setObjectName(QString::fromUtf8("StdTable"));
        StdTable->resize(400, 300);
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(StdTable->sizePolicy().hasHeightForWidth());
        StdTable->setSizePolicy(sizePolicy);
        gridLayout = new QGridLayout(StdTable);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        tableView = new QTableView(StdTable);
        tableView->setObjectName(QString::fromUtf8("tableView"));

        gridLayout->addWidget(tableView, 0, 0, 1, 1);


        retranslateUi(StdTable);

        QMetaObject::connectSlotsByName(StdTable);
    } // setupUi

    void retranslateUi(QWidget *StdTable)
    {
        StdTable->setWindowTitle(QApplication::translate("StdTable", "StdTable", nullptr));
    } // retranslateUi

};

namespace Ui {
    class StdTable: public Ui_StdTable {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_STDTABLE_H
