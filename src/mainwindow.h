/*
 * baracuda 0.5
 * Author : phil estival <phil.estival@free.fr>
 * Date:<2016/05/17 15:45:06>
 * Source distributed under the terms of the GPL v2 license.
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QMessageBox>
#include "gpu.h"
#include "moduleslistmodel.h"
#include "brick.h"
#include "bartime.h"

namespace Ui {
class MainWindow;
class Dialog;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(GPU*, QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QDialog * aboutDlg;
    Ui::Dialog * about;
    QFileDialog * fileDialog;
    //ModulesListModel * moduleListModel;

    GPU * gpu;
    Time * time;

public slots:

    void loadKernel();
    void selectKernelInList(const QModelIndex & idx); /// show the source when clicking a kernel in the list
    void brickSelected(Brick * b); /// show the source when selecting a the brick in the composer
    void renderOp(Brick * b);
    void compileKernel( const QString & s ) const;
    void updateBrick( Brick *, String );
    void kernelSelectionFinished(int);
    void compilationDone( GPUModule * module );
    void stopRendering();
    void showAboutDlg();
    void exit();
};

#endif // MAINWINDOW_H
