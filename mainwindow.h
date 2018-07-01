#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <memory>
#include <QMainWindow>
#include <QCamera>
#include <QImage>
#include <QCloseEvent>
#include "singleton.h"

class CGLWidget;

const size_t MAX_WIDTH = 640;
const size_t MAX_HEIGHT = 480;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
	void closeEvent(QCloseEvent *event);

signals:
	void stopWorkers();

public slots:
    void displayImage(const QImage&);

private:
    Ui::MainWindow *ui;
    CGLWidget *mCGLWidget;
};

#endif // MAINWINDOW_H
