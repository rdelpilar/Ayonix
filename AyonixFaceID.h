
#ifndef __AYNX_FACEID_DLL_H__
#define __AYNX_FACEID_DLL_H__

#ifdef __cplusplus
extern "C" {
#endif 

#include "AyonixTypes.h"

int AFIDGetVersion(size_t *major, size_t *minor, size_t *build, size_t *revision);

int AFIDInitialize(const TCHAR *enginePath, void **engine);
int AFIDFinalize(void *engine);

int AFIDLoadImage(const TCHAR *filename, AyonixImage **img);
int AFIDDecodeImage(void *data, size_t len, AyonixImage **img);
int AFIDReleaseImage(AyonixImage *img);

int AFIDDetectFaces(void *engine, AyonixImage *img, int minsize, AyonixFace **faces, size_t *nFaces, AyonixRect *roi);
int AFIDMarkFace(void *engine, AyonixImage *img, AyonixPoint lEye, AyonixPoint rEye, AyonixFace **face);
int AFIDReleaseFaces(AyonixFace *faces, size_t nFaces);

int AFIDPreprocessFace(void *engine, AyonixFace *face);

int AFIDCreateAfid(void *engine, AyonixFace *face, void **afid, size_t *afidSize);
int AFIDUpdateAfid(void *engine, AyonixFace *face, void *afidOld, void **afidNew, size_t *afidSize);
int AFIDMergeAfids(void *engine, void *afid1, void *afid2, void **afidNew, size_t *afidSize);
int AFIDReleaseAfid(void *afid);

int AFIDMatchAfids(void *engine, void *afid, void **afids, size_t nAfids, float *scores, int *indexes);

int AFIDTrackerInit(void *engine, int detectionInterval, int minsize, void **tracker);
int AFIDTrackerFinalize(void *tracker);

int AFIDTrackerUpdate(void *engine, void *tracker, AyonixImage *frame, uint64_t timeStamp, AyonixFace **faces, size_t *nFaces);


#ifdef __cplusplus
}
#endif 

#endif
