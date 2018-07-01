#include "camera.h"
#include "controller.h"
#include <QGLWidget>
#include <QCameraInfo>

Q_DECLARE_METATYPE(QCameraInfo)

Camera::Camera(const Controller *c, QObject *parent)
	: QObject(parent),
	  ctrlr(c),
	  mPixelFormat(AV_PIX_FMT_BGR24),
	  mVideoStream(1)
{
	// Connect camera errors to controller
	connect(this, SIGNAL(error(const QString&)), ctrlr, SLOT(error(const QString&)), Qt::DirectConnection);

	initCamera();
	initFFMpeg();
}

Camera::~Camera()
{
	av_free_packet(&mPacket);
	avcodec_close(mCodecCtx);
	av_free(mFrame);
	av_free(mFrameRGB);
	avformat_close_input(&mFormatCtx);
}

void Camera::initCamera()
{
	// Using QT API to list all available camera device
	foreach(const QCameraInfo &cameraInfo, QCameraInfo::availableCameras()) {
		mCameraNames.append(cameraInfo.description());
	}

	if (mCameraNames.isEmpty()) {
		emit error("No camera found!");
		exit(-1);
	}

	// Use the first camera found
	mCameraName = mCameraNames.front();
}

// Initialize FFMpeg library
// Much of the initialization routine code here were taken from below website except for the openCV part
// http://hasanaga.info/how-to-capture-frame-from-webcam-witg-ffmpeg-api-and-display-image-with-opencv/
void Camera::initFFMpeg()
{
	// Register device and codec
	avdevice_register_all();
	avcodec_register_all();

	// Allocate memory for format context
	mFormatCtx = avformat_alloc_context();
	mIformat = av_find_input_format("dshow");

	// Video name must prefix "video="
	QString cameraName = "video=" + mCameraName;

	// Attempt to initialize and open camera device
	if (avformat_open_input(&mFormatCtx, cameraName.toStdString().c_str(), mIformat, NULL) != 0) {
		emit error("Unable to open camera " + mCameraName);
		avformat_close_input(&mFormatCtx);
		return;
	}

	// Collect camera stream information
	if (avformat_find_stream_info(mFormatCtx, NULL) < 0) {
		emit error("Unable to open stream info");
		avformat_close_input(&mFormatCtx);
		return;
	}

	av_dump_format(mFormatCtx, 0, cameraName.toStdString().c_str(), 0);

	// Find video stream type
	for (unsigned int i = 0; i < mFormatCtx->nb_streams; i++) {
		if (mFormatCtx->streams[i]->codec->coder_type == AVMEDIA_TYPE_VIDEO) {
			mVideoStream = i;
			break;
		}
	}

	if (mVideoStream == -1) {
		emit error("Invalid video stream");
		avformat_close_input(&mFormatCtx);
		return;
	}

	mCodecCtx = mFormatCtx->streams[mVideoStream]->codec;
	mCodec = avcodec_find_decoder(mCodecCtx->codec_id);

	// Find codec for device
	if (mCodec == NULL) {
		emit error("Codec not found!");
		avformat_close_input(&mFormatCtx);
		avcodec_close(mCodecCtx);
		return;
	}

	// Attempt to open codec
	if (avcodec_open2(mCodecCtx, mCodec, NULL) < 0) {
		emit error("Unable to open codec!");
		avformat_close_input(&mFormatCtx);
		avcodec_close(mCodecCtx);
		return;
	}

	// Allocate memory for frames
	mFrame = av_frame_alloc();
	mFrameRGB = av_frame_alloc();

	// Get size of frame
	mNumBytes = avpicture_get_size(mPixelFormat, mCodecCtx->width, mCodecCtx->height);
	uint8_t *buffer = (uint8_t *) av_malloc(mNumBytes * sizeof(uint8_t));

	// Create AVPicture
	if (avpicture_fill((AVPicture *)mFrameRGB, buffer, mPixelFormat, mCodecCtx->width, mCodecCtx->height) < 0) {
		emit error("Unable to fill AVPicture!");
		av_free(mFrame);
		av_free(mFrameRGB);
		avformat_close_input(&mFormatCtx);
		avcodec_close(mCodecCtx);
		return;
	}
}

void Camera::run()
{
	int frameFinished = 0;
	struct SwsContext *img_convert_ctx;

	// Read frames from camera
	while (av_read_frame(mFormatCtx, &mPacket) >= 0) {
		// Thread interrupt
		if (QThread::currentThread()->isInterruptionRequested()) {
			return;
		}

		// Decode valid frame
		if (mPacket.stream_index == mVideoStream) {
			avcodec_decode_video2(mCodecCtx, mFrame, &frameFinished, &mPacket);
			if (frameFinished) {
				// Convert frames to AVPicture
				img_convert_ctx = sws_getCachedContext(NULL, mCodecCtx->width, mCodecCtx->height, mCodecCtx->pix_fmt, mCodecCtx->width, mCodecCtx->height, mPixelFormat, SWS_BICUBIC, NULL, NULL, NULL);
				sws_scale(img_convert_ctx, ((AVPicture*)mFrame)->data, ((AVPicture*)mFrame)->linesize, 0, mCodecCtx->height, ((AVPicture *) mFrameRGB)->data, ((AVPicture *) mFrameRGB)->linesize);

				// Send AVPicture info to worker thread for processing
				emit sendData(mCodecCtx->width, mCodecCtx->height, mFrameRGB->linesize[0], mNumBytes, mFrameRGB->data[0]);
				
				// Cleanup
				av_free_packet(&mPacket);
				sws_freeContext(img_convert_ctx);
			}
		}
	}
}

