#include "cglwidget.h"
#include <QGLWidget>

CGLWidget::CGLWidget(QWidget *parent)
    : QOpenGLWidget(parent),
	mImage(nullptr)
{
}

void CGLWidget::displayImage(const QImage &image)
{
	// Viewport is an OpenGL widget,
	// conversion is necessary
    mImage = QGLWidget::convertToGLFormat(image);
    update();
}

void CGLWidget::initializeGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glViewport(0, 0, this->width(), this->height());
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-this->width() / 2, this->width() / 2, this->height() / 2, -this->height() / 2, -1, 1);
    glMatrixMode(GL_MODELVIEW);
}

void CGLWidget::resizeGL(int x, int y)
{
    glViewport(0, 0, x, y);
}

void CGLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT);
	glDrawPixels(mImage.width(), mImage.height(), GL_BGRA, GL_UNSIGNED_BYTE, mImage.bits());
	glFlush();
}

