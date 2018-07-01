#ifndef CGLWIDGET_H
#define CGLWIDGET_H

#include <QObject>
#include <QOpenGLWidget>
#include <QImage>

class CGLWidget : public QOpenGLWidget
{
    Q_OBJECT

public:
    CGLWidget(QWidget *parent = NULL);
    void displayImage(const QImage&);

protected:
    void initializeGL();
    void resizeGL(int, int);
    void paintGL();

private:
	QImage mImage;
};

#endif // CGLWIDGET_H
