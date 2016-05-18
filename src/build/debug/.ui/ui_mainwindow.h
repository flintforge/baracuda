/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.3.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include <modulelist.h>
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
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(603, 564);
        MainWindow->setStyleSheet(QLatin1String("margin:0px;\n"
"padding:0px;"));
        actionLoadKernel = new QAction(MainWindow);
        actionLoadKernel->setObjectName(QStringLiteral("actionLoadKernel"));
        actionStop = new QAction(MainWindow);
        actionStop->setObjectName(QStringLiteral("actionStop"));
        actionOpenImage = new QAction(MainWindow);
        actionOpenImage->setObjectName(QStringLiteral("actionOpenImage"));
        actionAbout = new QAction(MainWindow);
        actionAbout->setObjectName(QStringLiteral("actionAbout"));
        actionExit = new QAction(MainWindow);
        actionExit->setObjectName(QStringLiteral("actionExit"));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        verticalLayout_5 = new QVBoxLayout(centralWidget);
        verticalLayout_5->setSpacing(6);
        verticalLayout_5->setContentsMargins(11, 11, 11, 11);
        verticalLayout_5->setObjectName(QStringLiteral("verticalLayout_5"));
        splitter_4 = new QSplitter(centralWidget);
        splitter_4->setObjectName(QStringLiteral("splitter_4"));
        splitter_4->setOrientation(Qt::Horizontal);
        verticalLayoutWidget = new QWidget(splitter_4);
        verticalLayoutWidget->setObjectName(QStringLiteral("verticalLayoutWidget"));
        Editor = new QVBoxLayout(verticalLayoutWidget);
        Editor->setSpacing(6);
        Editor->setContentsMargins(11, 11, 11, 11);
        Editor->setObjectName(QStringLiteral("Editor"));
        Editor->setContentsMargins(0, 0, 0, 0);
        textEdit = new TextEdit(verticalLayoutWidget);
        textEdit->setObjectName(QStringLiteral("textEdit"));

        Editor->addWidget(textEdit);

        compileBtn = new QPushButton(verticalLayoutWidget);
        compileBtn->setObjectName(QStringLiteral("compileBtn"));

        Editor->addWidget(compileBtn);

        splitter_4->addWidget(verticalLayoutWidget);
        horizontalLayoutWidget_4 = new QWidget(splitter_4);
        horizontalLayoutWidget_4->setObjectName(QStringLiteral("horizontalLayoutWidget_4"));
        rightside = new QHBoxLayout(horizontalLayoutWidget_4);
        rightside->setSpacing(6);
        rightside->setContentsMargins(11, 11, 11, 11);
        rightside->setObjectName(QStringLiteral("rightside"));
        rightside->setContentsMargins(0, 0, 0, 0);
        splitter_3 = new QSplitter(horizontalLayoutWidget_4);
        splitter_3->setObjectName(QStringLiteral("splitter_3"));
        splitter_3->setOrientation(Qt::Vertical);
        splitter_3->setHandleWidth(2);
        horizontalLayoutWidget = new QWidget(splitter_3);
        horizontalLayoutWidget->setObjectName(QStringLiteral("horizontalLayoutWidget"));
        renderer_2 = new QHBoxLayout(horizontalLayoutWidget);
        renderer_2->setSpacing(1);
        renderer_2->setContentsMargins(11, 11, 11, 11);
        renderer_2->setObjectName(QStringLiteral("renderer_2"));
        renderer_2->setContentsMargins(0, 0, 0, 0);
        splitter = new QSplitter(horizontalLayoutWidget);
        splitter->setObjectName(QStringLiteral("splitter"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(splitter->sizePolicy().hasHeightForWidth());
        splitter->setSizePolicy(sizePolicy);
        splitter->setOrientation(Qt::Horizontal);
        renderer = new GLRendererWidget(splitter);
        renderer->setObjectName(QStringLiteral("renderer"));
        renderer->setEnabled(true);
        QSizePolicy sizePolicy1(QSizePolicy::Maximum, QSizePolicy::Minimum);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(renderer->sizePolicy().hasHeightForWidth());
        renderer->setSizePolicy(sizePolicy1);
        renderer->setMinimumSize(QSize(128, 128));
        renderer->setMaximumSize(QSize(2048, 2048));
        renderer->setStyleSheet(QStringLiteral(""));
        splitter->addWidget(renderer);

        renderer_2->addWidget(splitter);

        splitter_3->addWidget(horizontalLayoutWidget);
        horizontalLayoutWidget_2 = new QWidget(splitter_3);
        horizontalLayoutWidget_2->setObjectName(QStringLiteral("horizontalLayoutWidget_2"));
        Hcompositor = new QHBoxLayout(horizontalLayoutWidget_2);
        Hcompositor->setSpacing(3);
        Hcompositor->setContentsMargins(11, 11, 11, 11);
        Hcompositor->setObjectName(QStringLiteral("Hcompositor"));
        Hcompositor->setContentsMargins(0, 0, 0, 0);
        splitter_2 = new QSplitter(horizontalLayoutWidget_2);
        splitter_2->setObjectName(QStringLiteral("splitter_2"));
        splitter_2->setOrientation(Qt::Horizontal);
        splitter_2->setHandleWidth(1);
        moduleListView = new ModuleList(splitter_2);
        moduleListView->setObjectName(QStringLiteral("moduleListView"));
        moduleListView->setMaximumSize(QSize(128, 16777215));
        splitter_2->addWidget(moduleListView);
        opcompositer = new OpComposerView(splitter_2);
        opcompositer->setObjectName(QStringLiteral("opcompositer"));
        opcompositer->setMaximumSize(QSize(2048, 2048));
        splitter_2->addWidget(opcompositer);

        Hcompositor->addWidget(splitter_2);

        splitter_3->addWidget(horizontalLayoutWidget_2);
        horizontalLayoutWidget_3 = new QWidget(splitter_3);
        horizontalLayoutWidget_3->setObjectName(QStringLiteral("horizontalLayoutWidget_3"));
        HLayout = new QHBoxLayout(horizontalLayoutWidget_3);
        HLayout->setSpacing(1);
        HLayout->setContentsMargins(11, 11, 11, 11);
        HLayout->setObjectName(QStringLiteral("HLayout"));
        HLayout->setContentsMargins(0, 0, 0, 0);
        signatureParamsLayout = new QVBoxLayout();
        signatureParamsLayout->setSpacing(6);
        signatureParamsLayout->setObjectName(QStringLiteral("signatureParamsLayout"));

        HLayout->addLayout(signatureParamsLayout);

        splitter_3->addWidget(horizontalLayoutWidget_3);

        rightside->addWidget(splitter_3);

        splitter_4->addWidget(horizontalLayoutWidget_4);

        verticalLayout_5->addWidget(splitter_4);

        graphicsView = new TimelineView(centralWidget);
        graphicsView->setObjectName(QStringLiteral("graphicsView"));
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
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 603, 19));
        menuFIle = new QMenu(menuBar);
        menuFIle->setObjectName(QStringLiteral("menuFIle"));
        menuRendering = new QMenu(menuBar);
        menuRendering->setObjectName(QStringLiteral("menuRendering"));
        menuAbout = new QMenu(menuBar);
        menuAbout->setObjectName(QStringLiteral("menuAbout"));
        MainWindow->setMenuBar(menuBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
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
