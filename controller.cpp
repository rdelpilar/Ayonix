#include "controller.h"
#include "mainwindow.h"
#include "streamworker.h"
#include "camera.h"
#include <QtCore>
#include <QMessageBox>

Controller::Controller(QObject *parent)
    : QObject(parent),
      mMainWindow(std::make_shared<MainWindow>()),
      mCameraThread(nullptr),
	  mWorkerThread(nullptr),
	  mCamera(nullptr),
      mWorker(nullptr)
{
    // Register meta types for Qt signals and slots
    qRegisterMetaType<QImage>("const QImage&");
	qRegisterMetaType<QString>("const QString&");

    init();
}

void Controller::init()
{
    // Create workers
    createWorkers();

	// Connect worker and GL widget in main window
	connect(mWorker,
		SIGNAL(processedImage(const QImage&)),
		mMainWindow.get(),
		SLOT(displayImage(const QImage&)));

	// Connect camera and worker
	connect(mCamera,
		SIGNAL(sendData(const int&, const int&, const int&, const int&, void*)),
		mWorker,
		SLOT(process(const int&, const int&, const int&, const int&, void*)));

	// Connect window close button event to workers
	connect(mMainWindow.get(), SIGNAL(stopWorkers()), this, SLOT(stopWorkers()));

	mMainWindow->show();
}

// Separate threads for Camera object
// and StreamWorker object
void Controller::createWorkers()
{
	mWorkerThread = new QThread();
	mCameraThread = new QThread();

	mWorker = new StreamWorker(this);
	mWorker->moveToThread(mWorkerThread);

	// Start worker thread first before camera
	// starts streaming frames
	mWorkerThread->start();

	mCamera = new Camera(this);
	mCamera->moveToThread(mCameraThread);

	// Begin camera stream as soon as camera thread starts
	connect(mCameraThread, SIGNAL(started()), mCamera, SLOT(run()));
	
	mCameraThread->start();
}

void Controller::stopWorkers()
{
	// Stop camera thread first
	if (mCameraThread->isRunning()) {
		mCameraThread->requestInterruption();
		mCamera->deleteLater();		
		mCameraThread->quit();
		mCameraThread->wait();
	}

	if (mWorkerThread->isRunning()) {
		mWorker->deleteLater();
		mWorkerThread->quit();
		mWorkerThread->wait();
	}
}

void Controller::error(const QString &msg)
{
	QMessageBox::critical(mMainWindow.get(), tr("Ayonix"), msg, QMessageBox::Ok);
	stopWorkers();
}
