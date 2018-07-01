#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "cglwidget.h"
#include <QCameraInfo>

Q_DECLARE_METATYPE(QCameraInfo)

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    mCGLWidget = new CGLWidget();
    mCGLWidget->setMinimumSize(MAX_WIDTH, MAX_HEIGHT);
    mCGLWidget->setMaximumSize(MAX_WIDTH, MAX_HEIGHT);
    ui->verticalLayout->addWidget(mCGLWidget);
    ui->verticalLayout->addStretch();
}

MainWindow::~MainWindow()
{
    delete mCGLWidget;
    delete ui;
}

// Send a request interrupt to both threads when
// the user clicks on the X button to close the window
void MainWindow::closeEvent(QCloseEvent *event)
{
	emit stopWorkers();
	event->accept();
}

// Send processed image to OpenGL widget for display
void MainWindow::displayImage(const QImage &image)
{
    mCGLWidget->displayImage(image);
}
