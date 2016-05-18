/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 4.8.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QAction>
#include <QApplication>
#include <QButtonGroup>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QPushButton>
#include <QSplitter>
#include <QStatusBar>
#include <QVBoxLayout>
#include <QWidget>
#include "modulelist.h"
#include "glrendererwidget.h"
#include "opcomposer.h"
#include "textedit.h"
#include "timelineview.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionLoadKernel;
    QAction *actionStop;
    QAction *actionOpenImage;
    QAction *actionAbout;
    QAction *actionExit;
    QWidget *centralWidget;
    QVBoxLayout *verticalLayout_5;
    QSplitter *splitter_4;
    QWidget *verticalLayoutWidget;
    QVBoxLayout *Editor;
    TextEdit *textEdit;
    QPushButton *compileBtn;
    QWidget *horizontalLayoutWidget_4;
    QHBoxLayout *rightside;
    QSplitter *splitter_3;
    QWidget *horizontalLayoutWidget;
    QHBoxLayout *renderer_2;
    QSplitter *splitter;
    GLRendererWidget *renderer;
    QWidget *horizontalLayoutWidget_2;
    QHBoxLayout *Hcompositor;
    QSplitter *splitter_2;
    ModuleList *moduleListView;
    OpComposerView *opcompositer;
    QWidget *horizontalLayoutWidget_3;
    QHBoxLayout *HLayout;
    QVBoxLayout *signatureParamsLayout;
    TimelineView *graphicsView;
    QMenuBar *menuBar;
    QMenu *menuFIle;
    QMenu *menuRendering;
    QMenu *menuAbout;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(603, 564);
        MainWindow->setStyleSheet(QString::fromUtf8("margin:0px;\n"
"padding:0px;"));
        actionLoadKernel = new QAction(MainWindow);
        actionLoadKernel->setObjectName(QString::fromUtf8("actionLoadKernel"));
        actionStop = new QAction(MainWindow);
        actionStop->setObjectName(QString::fromUtf8("actionStop"));
        actionOpenImage = new QAction(MainWindow);
        actionOpenImage->setObjectName(QString::fromUtf8("actionOpenImage"));
        actionAbout = new QAction(MainWindow);
        actionAbout->setObjectName(QString::fromUtf8("actionAbout"));
        actionExit = new QAction(MainWindow);
        actionExit->setObjectName(QString::fromUtf8("actionExit"));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        verticalLayout_5 = new QVBoxLayout(centralWidget);
        verticalLayout_5->setSpacing(6);
        verticalLayout_5->setContentsMargins(11, 11, 11, 11);
        verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));
        splitter_4 = new QSplitter(centralWidget);
        splitter_4->setObjectName(QString::fromUtf8("splitter_4"));
        splitter_4->setOrientation(Qt::Horizontal);
        verticalLayoutWidget = new QWidget(splitter_4);
        verticalLayoutWidget->setObjectName(QString::fromUtf8("verticalLayoutWidget"));
        Editor = new QVBoxLayout(verticalLayoutWidget);
        Editor->setSpacing(6);
        Editor->setContentsMargins(11, 11, 11, 11);
        Editor->setObjectName(QString::fromUtf8("Editor"));
        Editor->setContentsMargins(0, 0, 0, 0);
        textEdit = new TextEdit(verticalLayoutWidget);
        textEdit->setObjectName(QString::fromUtf8("textEdit"));

        Editor->addWidget(textEdit);

        compileBtn = new QPushButton(verticalLayoutWidget);
        compileBtn->setObjectName(QString::fromUtf8("compileBtn"));

        Editor->addWidget(compileBtn);

        splitter_4->addWidget(verticalLayoutWidget);
        horizontalLayoutWidget_4 = new QWidget(splitter_4);
        horizontalLayoutWidget_4->setObjectName(QString::fromUtf8("horizontalLayoutWidget_4"));
        rightside = new QHBoxLayout(horizontalLayoutWidget_4);
        rightside->setSpacing(6);
        rightside->setContentsMargins(11, 11, 11, 11);
        rightside->setObjectName(QString::fromUtf8("rightside"));
        rightside->setContentsMargins(0, 0, 0, 0);
        splitter_3 = new QSplitter(horizontalLayoutWidget_4);
        splitter_3->setObjectName(QString::fromUtf8("splitter_3"));
        splitter_3->setOrientation(Qt::Vertical);
        splitter_3->setHandleWidth(2);
        horizontalLayoutWidget = new QWidget(splitter_3);
        horizontalLayoutWidget->setObjectName(QString::fromUtf8("horizontalLayoutWidget"));
        renderer_2 = new QHBoxLayout(horizontalLayoutWidget);
        renderer_2->setSpacing(1);
        renderer_2->setContentsMargins(11, 11, 11, 11);
        renderer_2->setObjectName(QString::fromUtf8("renderer_2"));
        renderer_2->setContentsMargins(0, 0, 0, 0);
        splitter = new QSplitter(horizontalLayoutWidget);
        splitter->setObjectName(QString::fromUtf8("splitter"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(splitter->sizePolicy().hasHeightForWidth());
        splitter->setSizePolicy(sizePolicy);
        splitter->setOrientation(Qt::Horizontal);
        renderer = new GLRendererWidget(splitter);
        renderer->setObjectName(QString::fromUtf8("renderer"));
        renderer->setEnabled(true);
        QSizePolicy sizePolicy1(QSizePolicy::Maximum, QSizePolicy::Minimum);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(renderer->sizePolicy().hasHeightForWidth());
        renderer->setSizePolicy(sizePolicy1);
        renderer->setMinimumSize(QSize(128, 128));
        renderer->setMaximumSize(QSize(2048, 2048));
        renderer->setStyleSheet(QString::fromUtf8(""));
        splitter->addWidget(renderer);

        renderer_2->addWidget(splitter);

        splitter_3->addWidget(horizontalLayoutWidget);
        horizontalLayoutWidget_2 = new QWidget(splitter_3);
        horizontalLayoutWidget_2->setObjectName(QString::fromUtf8("horizontalLayoutWidget_2"));
        Hcompositor = new QHBoxLayout(horizontalLayoutWidget_2);
        Hcompositor->setSpacing(3);
        Hcompositor->setContentsMargins(11, 11, 11, 11);
        Hcompositor->setObjectName(QString::fromUtf8("Hcompositor"));
        Hcompositor->setContentsMargins(0, 0, 0, 0);
        splitter_2 = new QSplitter(horizontalLayoutWidget_2);
        splitter_2->setObjectName(QString::fromUtf8("splitter_2"));
        splitter_2->setOrientation(Qt::Horizontal);
        splitter_2->setHandleWidth(1);
        moduleListView = new ModuleList(splitter_2);
        moduleListView->setObjectName(QString::fromUtf8("moduleListView"));
        moduleListView->setMaximumSize(QSize(128, 16777215));
        splitter_2->addWidget(moduleListView);
        opcompositer = new OpComposerView(splitter_2);
        opcompositer->setObjectName(QString::fromUtf8("opcompositer"));
        opcompositer->setMaximumSize(QSize(2048, 2048));
        splitter_2->addWidget(opcompositer);

        Hcompositor->addWidget(splitter_2);

        splitter_3->addWidget(horizontalLayoutWidget_2);
        horizontalLayoutWidget_3 = new QWidget(splitter_3);
        horizontalLayoutWidget_3->setObjectName(QString::fromUtf8("horizontalLayoutWidget_3"));
        HLayout = new QHBoxLayout(horizontalLayoutWidget_3);
        HLayout->setSpacing(1);
        HLayout->setContentsMargins(11, 11, 11, 11);
        HLayout->setObjectName(QString::fromUtf8("HLayout"));
        HLayout->setContentsMargins(0, 0, 0, 0);
        signatureParamsLayout = new QVBoxLayout();
        signatureParamsLayout->setSpacing(6);
        signatureParamsLayout->setObjectName(QString::fromUtf8("signatureParamsLayout"));

        HLayout->addLayout(signatureParamsLayout);

        splitter_3->addWidget(horizontalLayoutWidget_3);

        rightside->addWidget(splitter_3);

        splitter_4->addWidget(horizontalLayoutWidget_4);

        verticalLayout_5->addWidget(splitter_4);

        graphicsView = new TimelineView(centralWidget);
        graphicsView->setObjectName(QString::fromUtf8("graphicsView"));
        graphicsView->setEnabled(true);
        QSizePolicy sizePolicy2(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(10);
        sizePolicy2.setHeightForWidth(graphicsView->sizePolicy().hasHeightForWidth());
        graphicsView->setSizePolicy(sizePolicy2);
        graphicsView->setMinimumSize(QSize(128, 8));
        graphicsView->setMaximumSize(QSize(16777215, 20));

        verticalLayout_5->addWidget(graphicsView);

        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 603, 19));
        menuFIle = new QMenu(menuBar);
        menuFIle->setObjectName(QString::fromUtf8("menuFIle"));
        menuRendering = new QMenu(menuBar);
        menuRendering->setObjectName(QString::fromUtf8("menuRendering"));
        menuAbout = new QMenu(menuBar);
        menuAbout->setObjectName(QString::fromUtf8("menuAbout"));
        MainWindow->setMenuBar(menuBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        MainWindow->setStatusBar(statusBar);

        menuBar->addAction(menuFIle->menuAction());
        menuBar->addAction(menuRendering->menuAction());
        menuBar->addAction(menuAbout->menuAction());
        menuFIle->addAction(actionLoadKernel);
        menuFIle->addAction(actionOpenImage);
        menuFIle->addAction(actionExit);
        menuRendering->addAction(actionStop);
        menuAbout->addAction(actionAbout);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0));
        actionLoadKernel->setText(QApplication::translate("MainWindow", "load kernel...", 0));
        actionStop->setText(QApplication::translate("MainWindow", "stop", 0));
        actionOpenImage->setText(QApplication::translate("MainWindow", "open image...", 0));
        actionAbout->setText(QApplication::translate("MainWindow", "About Baracuda...", 0));
        actionExit->setText(QApplication::translate("MainWindow", "exit", 0));
        actionExit->setShortcut(QApplication::translate("MainWindow", "Ctrl+W", 0));
        compileBtn->setText(QApplication::translate("MainWindow", "compile", 0));
        menuFIle->setTitle(QApplication::translate("MainWindow", "File", 0));
        menuRendering->setTitle(QApplication::translate("MainWindow", "Rendering", 0));
        menuAbout->setTitle(QApplication::translate("MainWindow", "Help", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
