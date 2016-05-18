/*
 * baracuda 0.5
 * Author : phil estival <phil.estival@free.fr>
 * Date:<2016/05/17 15:45:06>
 * Source distributed under the terms of the GPL v2 license.

recompile the UI with : `uic mainwindow.ui>ui_mainwindow.h`

 */

#include <QStyleFactory>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ui_about.h"
#include "cudacompil.h"

#include "compilation.h"
#include "moduleslistmodel.h"
#include <QMessageBox>
//#include <QErrorMessage>
#include "histogram.h"
#include "bartime.h"


MainWindow::MainWindow(GPU * _gpu, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    gpu(_gpu)
{
    qApp->setStyle(QStyleFactory::create("Fusion"));
    QPalette darkPalette;
    darkPalette.setColor(QPalette::Window, QColor(53,53,53));
    darkPalette.setColor(QPalette::WindowText, Qt::white);
    darkPalette.setColor(QPalette::Base, QColor(25,25,25));
    darkPalette.setColor(QPalette::AlternateBase, QColor(53,53,53));
    darkPalette.setColor(QPalette::ToolTipBase, Qt::white);
    darkPalette.setColor(QPalette::ToolTipText, Qt::white);
    darkPalette.setColor(QPalette::Text, Qt::white);
    darkPalette.setColor(QPalette::Button, QColor(53,53,53));
    darkPalette.setColor(QPalette::ButtonText, Qt::white);
    darkPalette.setColor(QPalette::BrightText, Qt::red);
    darkPalette.setColor(QPalette::Link, QColor(42, 130, 218));
    darkPalette.setColor(QPalette::Highlight, QColor(42, 130, 218));
    darkPalette.setColor(QPalette::HighlightedText, Qt::black);
    qApp->setPalette(darkPalette);
    qApp->setStyleSheet("QToolTip { color: #ffffff; background-color: #2a82da; border: 1px solid white; }");


    ui->setupUi(this);
    ui->renderer->setGPU(gpu);

    about=new Ui::Dialog();
    aboutDlg = new QDialog(this);
    about->setupUi(aboutDlg);

    ui->opcompositer->composer->setList( ui->moduleListView );
    ui->opcompositer->setList(ui->moduleListView);

    connect( ui->actionAbout, SIGNAL( triggered() ), this, SLOT( showAboutDlg() ) );
    connect( ui->actionOpenImage, SIGNAL( triggered() ), this, SLOT( openImage() ) );
    connect( ui->actionLoadKernel, SIGNAL( triggered() ), this, SLOT( loadKernel() ) );
    connect( ui->actionExit, SIGNAL( triggered() ), this, SLOT( exit() ) );
    connect( ui->actionStop, SIGNAL( triggered() ), this, SLOT( stopRendering() ) );
    //connect( ui->actionMaximizeRendererWindow, SIGNAL( triggered() ), this, SLOT( maximizeRenderWindow() ) );

    // when selecting a kernel in the list
    connect(ui->moduleListView,
            SIGNAL(clicked(QModelIndex)),
            this, SLOT(selectKernelInList(QModelIndex)));
    
    connect( (OpComposer*) ui->opcompositer->scene(),
             SIGNAL( BrickSelected( Brick *) ),
             this, SLOT( brickSelected( Brick *) ) );

    connect( (OpComposer*) ui->opcompositer->scene(),
             SIGNAL( RenderOp( Brick *) ),
             this, SLOT( renderOp( Brick *) ) );

    connect( (OpComposer*) ui->opcompositer->scene(),
             SIGNAL( Recompile( Brick *) ),
             this, SLOT( recompileOp( Brick *) ) );

   connect( ui->textEdit,
             SIGNAL( UpdateBrick (Brick *, String) ),
             this, SLOT( updateBrick(Brick *, String))
             );
    //*/
    fileDialog = new QFileDialog ( this, "Open a kernel", QDir::currentPath()+"/kernels",
                               "Cuda kernels [*.cu] (*.cu);;All [*] (*);;");
    fileDialog->setFileMode(QFileDialog::ExistingFiles);
    connect(fileDialog, SIGNAL(finished (int)), this, SLOT(kernelSelectionFinished(int)));


    /**
     * meant this way,
     * the renderer2 (an histogram for instance)
     * would execute an op on the shared pbo, but modifying the pbo itself
     * if it's not writing on an other devptr segment.
     * This is the situation in which we want to preserve the input data,
     * by examining the op signature so it has input and output channels separated
     */

/*
    GLRendererWidget* renderer2 = new GLRendererWidget(ui->splitter,ui->renderer);
    ui->renderer->setCopyRenderer(renderer2);

    renderer2->setObjectName(QStringLiteral("renderer2"));
    QSizePolicy sizePolicy1(QSizePolicy::Maximum, QSizePolicy::Preferred);
    sizePolicy1.setHorizontalStretch(0);
    sizePolicy1.setVerticalStretch(0);
    sizePolicy1.setHeightForWidth(renderer2->sizePolicy().hasHeightForWidth());
    renderer2->setSizePolicy(sizePolicy1);
    renderer2->setMinimumSize(QSize(128, 0));
    renderer2->setStyleSheet(QStringLiteral("background-color:red"));
    ui->splitter->addWidget(renderer2);

    CudaCompiler compil;
    GPUModule * module = compil.createGPUModuleFromFile( "kernels/invert24.cu", gpu );
    OP * op = new OP(module);
    renderer2->setOp(op);
    ui->renderer->setCopyRenderer(renderer2);
*/

/*
Histogram* histogram = new Histogram(ui->splitter,ui->renderer);
histogram->init(gpu);
histogram->setObjectName("Histogram");
QSizePolicy sizePolicy2(QSizePolicy::Maximum, QSizePolicy::Preferred);
sizePolicy2.setHorizontalStretch(0);
sizePolicy2.setVerticalStretch(0);
sizePolicy2.setHeightForWidth(histogram->sizePolicy().hasHeightForWidth());
histogram->setSizePolicy(sizePolicy2);
histogram->setMinimumSize(QSize(128, 0));

ui->splitter->addWidget(histogram);

ui->renderer->setCopyRenderer(histogram);
//*/

    // test purpose

    compileKernel("kernels/ribbons.cu");
    compileKernel("kernels/ribbons-glsl.cu");
    compileKernel("kernels/grayscale.cu");
    compileKernel("kernels/invert.cu");
    compileKernel("kernels/test.cu");

    //time = new Time();
    //Time::time.start();

}


void MainWindow::selectKernelInList(const QModelIndex & idx) {
/*
 * show the source when selecting in the brick in the list
 **/
    int row = idx.row();
    //GPUModule * module = ui->moduleListView->getBrick(row)->getOp()->getModule();
    ui->textEdit->setEditingBrick( ui->moduleListView->getBrick(row) );

    // THIS IS A WORKAROUND
    // to keep coding while the libqxcb.so bug in dragitem + primus is unsolved
    Brick * b = ui->moduleListView->getBrick(row);
    ui->opcompositer->composer->addBrick( b );
    ui->opcompositer->composer->connectBrick( b );
    //ui->textEdit->setText( module->getSrc());
}

/**
 * show the source when selecting a brick
 * examine the signature.
 * width, height and time are reserved parameters
 * all others parameters can have external input
 */
void MainWindow::brickSelected(Brick * b) {

    //ui->textEdit->setEditingModule( b->getOp()->getModule() );
    ui->textEdit->setEditingBrick( b );
    // this is where the signature gets read and select
    // the widgets to show up in order to manipulate the inputs
    qDebug() << b->getOp()->getModule()->getSignature();

    ui->signatureParamsLayout;
}

void MainWindow::renderOp(Brick * b)
{
    ui->renderer->running = true;
    ui->renderer->setOp( b->getOp() );
}

void MainWindow::kernelSelectionFinished(int result) {
    if(result)
    foreach(String file, fileDialog->selectedFiles()) {
       ui->statusBar->showMessage("Compiling " + file);
       Compilation *compil = new Compilation(file,gpu);
       connect(
                   compil,
                   &Compilation::done,
                   this,
                   &MainWindow::compilationDone );
       connect(
                   compil, &Compilation::finished,
                   compil, &QObject::deleteLater);
       compil->start();      
    }
}

MainWindow::~MainWindow()
{
    delete ui;
    delete gpu;
}


void MainWindow::stopRendering()
{
    ui->renderer->running = !ui->renderer->running;

    if(ui->renderer->running) {
        ui->actionStop->setText("stop");
        //Time::time.
    }
    else
        ui->actionStop->setText("start");
}


void MainWindow::compilationDone( GPUModule * module )
{
    if(module==NULL) {
        ui->statusBar->showMessage("module compilation error");
    }
    else {
        ui->statusBar->showMessage("Kernel compiled");
        OP * op = new OP(module);
        Brick * b = new Brick(op);
        ui->moduleListView->addBrick(b);
    }
    delete (QObject::sender()); // destroy the compil object
}


void MainWindow::compileKernel( const QString & filename ) const
{
    ui->statusBar->showMessage("Compiling kernel...");

    CudaCompiler compil;
    GPUModule * module = compil.createGPUModuleFromFile( filename, gpu );

    if(module==NULL) {
        // output the log buffer please
        ui->statusBar->showMessage("Kernel compilation failed");
        QMessageBox::critical(NULL, "Error","Module compilation failed "+filename,
                              QMessageBox::Ok );
                              //Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint);
        qDebug() << filename <<" : compilation failed!\n";
    }
    else {
        OP * op = new OP( module );
        Brick * brick = new Brick( op );
        ui->statusBar->showMessage("Kernel compilation done");
        // switch to a list of module to a list of bricks
        ui->moduleListView->addBrick(brick);
        ui->renderer->setOp(brick->getOp());
        ui->renderer->running = true;
    }
}

void MainWindow::updateBrick( Brick * brick, String filename )
{
    ui->statusBar->showMessage("Compiling kernel " + filename);

    CudaCompiler compil;
    GPUModule * module = compil.createGPUModuleFromFile( filename, gpu );

    if(module==NULL) {
        // output the log buffer please
        ui->statusBar->showMessage("Kernel compilation failed");
        QMessageBox::critical(NULL, "Error","Module compilation failed "+filename,
                              QMessageBox::Ok );
                              //Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint);
        // todo retrieve error and output in window
    }
    else {
        ui->statusBar->showMessage("Kernel compilation done");
        OP * op = new OP( module );
        brick->~Brick(); // Destroy the object but leave the space allocated.
        brick = new (brick) Brick(op);// Create a new object in the same space
        ui->renderer->setOp(brick->getOp());
    }
}

// status please
void MainWindow::loadKernel() { fileDialog->exec(); }
void MainWindow::showAboutDlg() { aboutDlg->show(); }

void MainWindow::exit() {
    stopRendering();
    delete gpu;
    QCoreApplication::quit();
}
