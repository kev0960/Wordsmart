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
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_WordsmartClass
{
public:
    QAction *actionVersion;
    QAction *actionFlash_Cards;
    QAction *actionYour_Word_List;
    QAction *actionDownload_Online;
    QWidget *centralWidget;
    QVBoxLayout *verticalLayout_2;
    QStackedWidget *stackedWidget;
    QWidget *page;
    QVBoxLayout *verticalLayout;
    QTextBrowser *textBrowser;
    QWidget *page_2;
    QHBoxLayout *horizontalLayout;
    QListWidget *listWidget_2;
    QListWidget *listWidget;
    QTextBrowser *textBrowser_2;
    QWidget *page_3;
    QHBoxLayout *horizontalLayout_2;
    QListWidget *listWidget_3;
    QListWidget *listWidget_4;
    QListWidget *listWidget_5;
    QMenuBar *menuBar;
    QMenu *menuWho;
    QMenu *menu;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *WordsmartClass)
    {
        if (WordsmartClass->objectName().isEmpty())
            WordsmartClass->setObjectName(QStringLiteral("WordsmartClass"));
        WordsmartClass->resize(869, 511);
        actionVersion = new QAction(WordsmartClass);
        actionVersion->setObjectName(QStringLiteral("actionVersion"));
        actionFlash_Cards = new QAction(WordsmartClass);
        actionFlash_Cards->setObjectName(QStringLiteral("actionFlash_Cards"));
        actionYour_Word_List = new QAction(WordsmartClass);
        actionYour_Word_List->setObjectName(QStringLiteral("actionYour_Word_List"));
        actionDownload_Online = new QAction(WordsmartClass);
        actionDownload_Online->setObjectName(QStringLiteral("actionDownload_Online"));
        centralWidget = new QWidget(WordsmartClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        verticalLayout_2 = new QVBoxLayout(centralWidget);
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        stackedWidget = new QStackedWidget(centralWidget);
        stackedWidget->setObjectName(QStringLiteral("stackedWidget"));
        page = new QWidget();
        page->setObjectName(QStringLiteral("page"));
        verticalLayout = new QVBoxLayout(page);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        textBrowser = new QTextBrowser(page);
        textBrowser->setObjectName(QStringLiteral("textBrowser"));
        QFont font;
        font.setFamily(QString::fromUtf8("\353\247\221\354\235\200 \352\263\240\353\224\225"));
        font.setPointSize(10);
        textBrowser->setFont(font);
        textBrowser->setTextInteractionFlags(Qt::NoTextInteraction);

        verticalLayout->addWidget(textBrowser);

        stackedWidget->addWidget(page);
        page_2 = new QWidget();
        page_2->setObjectName(QStringLiteral("page_2"));
        horizontalLayout = new QHBoxLayout(page_2);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        listWidget_2 = new QListWidget(page_2);
        listWidget_2->setObjectName(QStringLiteral("listWidget_2"));
        listWidget_2->setMaximumSize(QSize(400, 16777215));
        listWidget_2->setSelectionMode(QAbstractItemView::ExtendedSelection);
        listWidget_2->setSelectionRectVisible(false);

        horizontalLayout->addWidget(listWidget_2);

        listWidget = new QListWidget(page_2);
        listWidget->setObjectName(QStringLiteral("listWidget"));
        listWidget->setMaximumSize(QSize(400, 16777215));
        QFont font1;
        font1.setFamily(QStringLiteral("Verdana"));
        listWidget->setFont(font1);

        horizontalLayout->addWidget(listWidget);

        textBrowser_2 = new QTextBrowser(page_2);
        textBrowser_2->setObjectName(QStringLiteral("textBrowser_2"));
        textBrowser_2->setFont(font);

        horizontalLayout->addWidget(textBrowser_2);

        stackedWidget->addWidget(page_2);
        page_3 = new QWidget();
        page_3->setObjectName(QStringLiteral("page_3"));
        horizontalLayout_2 = new QHBoxLayout(page_3);
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        listWidget_3 = new QListWidget(page_3);
        listWidget_3->setObjectName(QStringLiteral("listWidget_3"));

        horizontalLayout_2->addWidget(listWidget_3);

        listWidget_4 = new QListWidget(page_3);
        listWidget_4->setObjectName(QStringLiteral("listWidget_4"));

        horizontalLayout_2->addWidget(listWidget_4);

        listWidget_5 = new QListWidget(page_3);
        listWidget_5->setObjectName(QStringLiteral("listWidget_5"));

        horizontalLayout_2->addWidget(listWidget_5);

        stackedWidget->addWidget(page_3);

        verticalLayout_2->addWidget(stackedWidget);

        WordsmartClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(WordsmartClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 869, 26));
        menuWho = new QMenu(menuBar);
        menuWho->setObjectName(QStringLiteral("menuWho"));
        menu = new QMenu(menuBar);
        menu->setObjectName(QStringLiteral("menu"));
        WordsmartClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(WordsmartClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        WordsmartClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(WordsmartClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        WordsmartClass->setStatusBar(statusBar);

        menuBar->addAction(menu->menuAction());
        menuBar->addAction(menuWho->menuAction());
        menuWho->addAction(actionVersion);
        menu->addAction(actionFlash_Cards);
        menu->addAction(actionYour_Word_List);
        menu->addAction(actionDownload_Online);

        retranslateUi(WordsmartClass);

        stackedWidget->setCurrentIndex(1);


        QMetaObject::connectSlotsByName(WordsmartClass);
    } // setupUi

    void retranslateUi(QMainWindow *WordsmartClass)
    {
        WordsmartClass->setWindowTitle(QApplication::translate("WordsmartClass", "Wordsmart", 0));
        actionVersion->setText(QApplication::translate("WordsmartClass", "Version", 0));
        actionFlash_Cards->setText(QApplication::translate("WordsmartClass", "Flash Cards", 0));
        actionYour_Word_List->setText(QApplication::translate("WordsmartClass", "Your Word List", 0));
        actionDownload_Online->setText(QApplication::translate("WordsmartClass", "Download Wordlist", 0));
        textBrowser->setHtml(QApplication::translate("WordsmartClass", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'\353\247\221\354\235\200 \352\263\240\353\224\225'; font-size:10pt; font-weight:400; font-style:normal;\">\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-family:'Verdana';\"><br /></p></body></html>", 0));
        menuWho->setTitle(QApplication::translate("WordsmartClass", "Help", 0));
        menu->setTitle(QApplication::translate("WordsmartClass", "Learn Words", 0));
    } // retranslateUi

};

namespace Ui {
    class WordsmartClass: public Ui_WordsmartClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WORDSMART_H
