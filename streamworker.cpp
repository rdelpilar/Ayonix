#include "streamworker.h"
#include "AyonixFaceId.h"
#include "controller.h"

StreamWorker::StreamWorker(const Controller *c, QObject *parent)
    : QObject(parent),
	ctrlr(c),
	mAyonixEngine(0),
	mAyonixImage(new AyonixImage),
	mAyonixFaces(0),
	mImage(nullptr),
	mPainter(nullptr),
	nFaces(0)
{
	// Connect worker errors to controller
	connect(this, SIGNAL(error(const QString&)), ctrlr, SLOT(error(const QString&)), Qt::DirectConnection);

	init();
}

void StreamWorker::init()
{
	// Initialize Ayonix engine
	TCHAR mess[1024] = TEXT("data\\engine");

	size_t len = _tcslen(mess);
	if (mess[len - 1] == _T('\n'))
		mess[len - 1] = 0;

	int res = AFIDInitialize(mess, &mAyonixEngine);

	if (res != AYONIX_OK) {
		emit error("Failed to initialize Ayonix engine!");
		AFIDFinalize(mAyonixEngine);
		mAyonixEngine = 0;
		exit(-1);
	}
	else {
		size_t v1, v2, v3, v4;
		AFIDGetVersion(&v1, &v2, &v3, &v4);
	}

	mLabels.insert("Gender", "");
	mLabels.insert("Age", "");
	mLabels.insert("Smile", "");
}

StreamWorker::~StreamWorker()
{
	if(mAyonixEngine) {
		AFIDFinalize(mAyonixEngine);
		mAyonixEngine = 0;
	}

	if(mAyonixImage)
		delete mAyonixImage;
}

void StreamWorker::process(const int &width, const int &height, const int &lineSize, const int &size, void *data)
{
	if (size <= 0 || width <= 0 || height <= 0)
		return;

	// Fill AyonixImage struct
	mAyonixImage->width = width;
	mAyonixImage->height = height;
	mAyonixImage->iscolor = 1;
	mAyonixImage->step = lineSize;
	mAyonixImage->size = size;
	mAyonixImage->data = data;

	// Create QImage the first time
	if (mImage == NULL) {
		mImage = std::make_shared<QImage>(width, height, QImage::Format_RGB888);
		mPainter = std::make_shared<QPainter>(mImage.get());
		
		// Qt drawing tools
		mPen.setWidth(2);
		mPen.setColor(Qt::black);
		mPainter->setPen(mPen);

		mFont.setPointSize(7);
		mPainter->setFont(mFont);
	}

	// Detect faces
	int ret = AFIDDetectFaces(mAyonixEngine, mAyonixImage, 64, &mAyonixFaces, &nFaces, 0);
	if (ret != AYONIX_OK) {
		AFIDReleaseImage(mAyonixImage);
	}

	// Copy AVPicture to QImage
	for (int y = 0; y < height; ++y) {
		memcpy(mImage->scanLine(y), (unsigned char *)data + y * lineSize, width * 3);
	}

	// Pre-process all detected faces
	if (mAyonixFaces != NULL && nFaces > 0) {

		// Preprocess face
		ret = AFIDPreprocessFace(mAyonixEngine, mAyonixFaces);
		if (ret != AYONIX_OK) {
			AFIDReleaseImage(mAyonixImage);
		}

		for (int i = 0; i < nFaces; ++i) {
			// Draw circle on each face
			mRect.setRect(mAyonixFaces[i].location.x, mAyonixFaces[i].location.y, mAyonixFaces[i].location.w, mAyonixFaces[i].location.h);
			mPainter->drawEllipse(mRect);

			// Draw labels inside each circle
			mRect.setTopLeft(QPoint(mRect.x(), mRect.y() + 10));
			mLabels["Gender"] = mAyonixFaces[i].gender == 1 ? "F" : "M";
			mLabels["Age"] = QString::number(mAyonixFaces[i].age);
			mLabels["Smile"] = mAyonixFaces[i].expression.smile > 0.6 ? "Smiling" : "Not Smiling";
			mPainter->drawText(mRect, Qt::AlignHCenter | Qt::AlignTop, "Gender: " + mLabels["Gender"] + "\nAge: " + mLabels["Age"] + "\n" + mLabels["Smile"]);
		}
	}

	// Send unprocessed or processed image to main window
	emit processedImage(*mImage);

	// Clear QPainter rectangle coordinates
	for (int i = 0; i < nFaces; ++i) {
		mPainter->eraseRect(mAyonixFaces[i].location.x, mAyonixFaces[i].location.y, mAyonixFaces[i].location.w, mAyonixFaces[i].location.h);
	}

	// Cleanup
	AFIDReleaseFaces(mAyonixFaces, nFaces);
	mAyonixFaces = 0;
	nFaces = 0;	
}
