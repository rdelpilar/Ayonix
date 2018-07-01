#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <memory>
#include <QObject>

class MainWindow;
class QThread;
class StreamWorker;
class Camera;

typedef std::shared_ptr<MainWindow> MAIN_WINDOW;

class Controller : public QObject
{
    Q_OBJECT

public:
    explicit Controller(QObject *parent = nullptr);
    void init();

private:
    void createWorkers();

public slots:
	void error(const QString&);
    void stopWorkers();

private:
    MAIN_WINDOW mMainWindow;
    QThread *mCameraThread;
	QThread *mWorkerThread;
    StreamWorker *mWorker;
	Camera *mCamera;
};

#endif // CONTROLLER_H
