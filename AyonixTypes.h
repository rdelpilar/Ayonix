
#ifndef __AYONIX_TYPES_H__
#define __AYONIX_TYPES_H__

#include <inttypes.h>

#ifdef __cplusplus
extern "C" {
#endif 

/*
        Error Codes
*/

// General errors
#define AYONIX_OK                                0      //!< Succesful return

#define AYONIX_ERR_FAILED                       -1      //!< General/unknown error
#define AYONIX_ERR_OUT_OF_MEM                   -2      //!< Out of memory
#define AYONIX_ERR_INVALID_PARAM                -3      //!< Invalid parameter is sent to a function
#define AYONIX_ERR_FILE_OPEN                    -10     //!< A file cannot be opened
#define AYONIX_ERR_FILE_READ                    -11     //!< File contents cannot be read from disk
#define AYONIX_ERR_FILE_WRITE                   -12     //!< File contents cannot be written to disk
#define AYONIX_ERR_FILE_CORRUPTED               -14     //!< A file is corrupted
#define AYONIX_ERR_FILE_NOT_SUPPORTED           -15     //!< An input file format is not supported
#define AYONIX_ERR_IMAGE_TOO_SMALL              -1001   //!< Image is too small for face detection/recognition
#define AYONIX_ERR_OPERATION_NOT_AVAILABLE      -1002   //!< Operation cannot be completed due to permission or licensing limitation

// Licensing errors
#define AYONIX_ERR_LICENSING_SDKVER_MISMATCH    -701    //!< License file is for a different version of the SDK
#define AYONIX_ERR_LICENSING_HWID               -702    //!< Cannot acquire hardware id
#define AYONIX_ERR_LICENSING_HWID_MISMATCH      -703    //!< License file has a different hardware id
#define AYONIX_ERR_LICENSING_DONGLE             -704    //!< Cannot find or connect to dongle
#define AYONIX_ERR_LICENSING_ACTIVATION_EXPIRED -705    //!< Used side license activation has expired
#define AYONIX_ERR_LICENSING_LICENSE_EXPIRED    -706    //!< License duration is up

/*
        Structures
*/

typedef struct {
        int width;      //!< Width of image in pixels
        int height;     //!< Height of image in pixels
        int iscolor;    //!< 1: image is color, 0: image is grayscale
        int step;       //!< Size (in bytes) of a row of image in memory
        int size;       //!< Size (in bytes) of data in memory
        void *data;     //!< 32-bit aligned image data
} AyonixImage;
        
typedef struct {
        float x;        //!< X coordinate of the point
        float y;        //!< Y coordinate of the point
} AyonixPoint;

typedef struct {
        int x;          //!< X coordinate of the rectangle
        int y;          //!< Y coordinate of the rectangle
        int w;          //!< Width component of the rectangle
        int h;          //!< Height component of the rectangle
} AyonixRect;

typedef struct {        
        AyonixPoint lEye;              //!< The center of the left eye in the coordinates of the source image 
        AyonixPoint rEye;              //!< The center of the right eye in the coordinates of the source image
        AyonixPoint nose;              //!< The nose location in the source image 
        AyonixPoint mouth;             //!< The mouth location in the source image 
        AyonixPoint lEyeLCorner;       //!< The left corner of the left eye in the coordinates of the source image 
        AyonixPoint lEyeRCorner;       //!< The right corner of the left eye in the coordinates of the source image 
        AyonixPoint lEyeTop;           //!< The top of the left eye in the coordinates of the source image 
        AyonixPoint lEyeBottom;        //!< The bottom of the left eye in the coordinates of the source image 
        AyonixPoint rEyeLCorner;       //!< The left corner of the right eye in the coordinates of the source image 
        AyonixPoint rEyeRCorner;       //!< The right corner of the right eye in the coordinates of the source image 
        AyonixPoint rEyeTop;           //!< The top of the right eye in the coordinates of the source image 
        AyonixPoint rEyeBottom;        //!< the bottom of the right eye in the coordinates of the source image 
        AyonixPoint bEyes;             //!< The point in the middle between the eyes in the coordinates of the source image 
        AyonixPoint mouthLCorner;      //!< The left corner of the mouth in the coordinates of the source image 
        AyonixPoint mouthRCorner;      //!< The right corner of the mouth in the coordinates of the source image 
        AyonixPoint mouthTop;          //!< The top of the mouth in the coordinates of the source image 
        AyonixPoint mouthBottom;       //!< The bottom of the mouth in the coordinates of the source image 
        AyonixPoint chin;              //!< The center of the chin in the coordinates of the source image
        AyonixPoint lEyebrow[3];       //!< 3 points spanning the left eyebrow
        AyonixPoint rEyebrow[3];       //!< 3 points spanning the right eyebrow
        AyonixPoint lNoseWing;         //!< The left corner of the nosewing
        AyonixPoint rNoseWing;         //!< The right corner of the nosewing
} AyonixFaceFeatures;

typedef struct {
        float smile;            //!< Smile amount
        float lEyeBlink;        //!< Blink amount of left eye
        float rEyeBlink;        //!< Blink amount of right eye
        float mouthOpen;        //!< Openness amount of mouth
} AyonixFaceExpression;

typedef struct {
        AyonixRect location;      //!< Face mugshot location in the image
        AyonixImage *image;       //!< Face mugshot image
} AyonixFaceMugshot;

typedef struct {
        AyonixRect location;                   //!< Detected face location in the image
        AyonixFaceMugshot mugshot;             //!< Face mugshot
        AyonixFaceFeatures features;           //!< Detected face features (in mugshot coordinates)
        AyonixFaceExpression expression;       //!< Detected facial expressions

        int isValid;                            //!< Validity set after preprocess operation
        float quality;                          //!< Face quality ration in [0, 1], 1: best quality
        float gender;                           //!< Gender ratio in [-1, 1], -1: male, 1: female
        int age;                                //!< Estimated age in [10, 65]
        float roll, pitch, yaw;                 //!< Face orientation

        int trackerId;                          //!< A unique id assigned by the tracker
        int trackerCount;                       //!< Number of frames this face was tracked
        
        void *reserved;
} AyonixFace;


/*
        Char/string Definition
        Windows: always widechar (UTF16), define UNICODE in project settings
        Linux: UTF8
*/
#if defined(_WIN32) || defined(_WIN64)
        #include <Windows.h>        
        #include <tchar.h>
#elif defined(__linux__)
        typedef char TCHAR;
        #define _T(x)           x
#endif


#ifdef __cplusplus
}
#endif 

#endif

