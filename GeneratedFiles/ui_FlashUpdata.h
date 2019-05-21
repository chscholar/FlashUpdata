/********************************************************************************
** Form generated from reading UI file 'FlashUpdata.ui'
**
** Created by: Qt User Interface Compiler version 5.6.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FLASHUPDATA_H
#define UI_FLASHUPDATA_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_FlashUpdataClass
{
public:
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QWidget *centralWidget;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *FlashUpdataClass)
    {
        if (FlashUpdataClass->objectName().isEmpty())
            FlashUpdataClass->setObjectName(QStringLiteral("FlashUpdataClass"));
        FlashUpdataClass->resize(600, 400);
        menuBar = new QMenuBar(FlashUpdataClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        FlashUpdataClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(FlashUpdataClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        FlashUpdataClass->addToolBar(mainToolBar);
        centralWidget = new QWidget(FlashUpdataClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        FlashUpdataClass->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(FlashUpdataClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        FlashUpdataClass->setStatusBar(statusBar);

        retranslateUi(FlashUpdataClass);

        QMetaObject::connectSlotsByName(FlashUpdataClass);
    } // setupUi

    void retranslateUi(QMainWindow *FlashUpdataClass)
    {
        FlashUpdataClass->setWindowTitle(QApplication::translate("FlashUpdataClass", "FlashUpdata", 0));
    } // retranslateUi

};

namespace Ui {
    class FlashUpdataClass: public Ui_FlashUpdataClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FLASHUPDATA_H
