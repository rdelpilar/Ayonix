#ifndef CAMERA_H
#define CAMERA_H

#include <memory>
#include <QObject>
#include <QtCore>

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavdevice/avdevice.h>
#include <libswscale/swscale.h>
#include <libavutil/avutil.h>
}

class Controller;

#if LIBAVCODEC_VERSION_INT < AV_VERSION_INT(55,28,1)
#define av_frame_alloc avcodec_alloc_frame
#define av_frame_free avcodec_free_frame
#endif

class Camera : public QObject
{
	Q_OBJECT

public:
	explicit Camera(const Controller*, QObject *parent = nullptr);
	~Camera();
	
private:
	void initCamera();
	void initFFMpeg();

signals:
	void error(const QString&);
	void sendData(const int&, const int&, const int&, const int&, void*);

public slots:
	void run();

private:
	const Controller *ctrlr;
	QList<QString> mCameraNames;

	QString mCameraName;
	AVCodecContext *mCodecCtx;
	AVFormatContext *mFormatCtx;
	AVCodec *mCodec;
	AVInputFormat *mIformat;
	AVFrame *mFrame, *mFrameRGB;
	AVPixelFormat mPixelFormat;
	AVPacket mPacket;
	int mVideoStream;
	int mNumBytes;
};

#endif // !CAMERA_H