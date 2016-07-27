/********************************************************************************
** Form generated from reading UI file 'wordsmart.ui'
**
** Created by: Qt User Interface Compiler version 5.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WORDSMART_H
#define UI_WORDSMART_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_WordsmartClass
{
public:
    QAction *actionVersion;
    QWidget *centralWidget;
    QTextBrowser *textBrowser;
    QLabel *label;
    QMenuBar *menuBar;
    QMenu *menuWho;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *WordsmartClass)
    {
        if (WordsmartClass->objectName().isEmpty())
            WordsmartClass->setObjectName(QStringLiteral("WordsmartClass"));
        WordsmartClass->resize(600, 400);
        actionVersion = new QAction(WordsmartClass);
        actionVersion->setObjectName(QStringLiteral("actionVersion"));
        centralWidget = new QWidget(WordsmartClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        textBrowser = new QTextBrowser(centralWidget);
        textBrowser->setObjectName(QStringLiteral("textBrowser"));
        textBrowser->setGeometry(QRect(20, 40, 391, 261));
        label = new QLabel(centralWidget);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(20, 10, 131, 16));
        WordsmartClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(WordsmartClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 600, 26));
        menuWho = new QMenu(menuBar);
        menuWho->setObjectName(QStringLiteral("menuWho"));
        WordsmartClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(WordsmartClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        WordsmartClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(WordsmartClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        WordsmartClass->setStatusBar(statusBar);

        menuBar->addAction(menuWho->menuAction());
        menuWho->addAction(actionVersion);

        retranslateUi(WordsmartClass);

        QMetaObject::connectSlotsByName(WordsmartClass);
    } // setupUi

    void retranslateUi(QMainWindow *WordsmartClass)
    {
        WordsmartClass->setWindowTitle(QApplication::translate("WordsmartClass", "Wordsmart", 0));
        actionVersion->setText(QApplication::translate("WordsmartClass", "Version", 0));
        label->setText(QApplication::translate("WordsmartClass", "Captured Clipboard", 0));
        menuWho->setTitle(QApplication::translate("WordsmartClass", "Help", 0));
    } // retranslateUi

};

namespace Ui {
    class WordsmartClass: public Ui_WordsmartClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WORDSMART_H
