#ifndef STREAMWORKER_H
#define STREAMWORKER_H

#include <memory>
#include <QObject>
#include <QtGui>
#include <QtCore>
#include "AyonixTypes.h"

class Controller;

typedef std::shared_ptr<QPainter> PAINTER;
typedef std::shared_ptr<QImage> IMAGE;

class StreamWorker : public QObject
{
    Q_OBJECT

public:
    explicit StreamWorker(const Controller* = nullptr, QObject *parent = nullptr);
    ~StreamWorker();

private:
	void init();

signals:
	void error(const QString&);
    void processedImage(const QImage&);

public slots:
    void process(const int&, const int&, const int&, const int&, void*);

private:
	const Controller *ctrlr;

	void *mAyonixEngine;
	AyonixImage *mAyonixImage;
	AyonixFace *mAyonixFaces;
	size_t nFaces;

	IMAGE mImage;
	PAINTER mPainter;
	QPen mPen;
	QRect mRect;
	QFont mFont;
	QMap<QString, QString> mLabels;
};

#endif // STREAMWORKER_H
