/********************************************************************************
** Form generated from reading UI file 'processmgr.ui'
**
** Created by: Qt User Interface Compiler version 5.12.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PROCESSMGR_H
#define UI_PROCESSMGR_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QTreeView>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ProcessMgr
{
public:
    QGridLayout *gridLayout;
    QTreeView *viewProcess;

    void setupUi(QWidget *ProcessMgr)
    {
        if (ProcessMgr->objectName().isEmpty())
            ProcessMgr->setObjectName(QString::fromUtf8("ProcessMgr"));
        ProcessMgr->resize(400, 300);
        gridLayout = new QGridLayout(ProcessMgr);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        viewProcess = new QTreeView(ProcessMgr);
        viewProcess->setObjectName(QString::fromUtf8("viewProcess"));

        gridLayout->addWidget(viewProcess, 0, 0, 1, 1);


        retranslateUi(ProcessMgr);

        QMetaObject::connectSlotsByName(ProcessMgr);
    } // setupUi

    void retranslateUi(QWidget *ProcessMgr)
    {
        ProcessMgr->setWindowTitle(QApplication::translate("ProcessMgr", "Form", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ProcessMgr: public Ui_ProcessMgr {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PROCESSMGR_H
