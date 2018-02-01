/***********************************************************************
 * Module:  FaceDetect.h
 * Author:  lenovo
 * Modified: 2018Äê1ÔÂ31ÈÕ 16:30:02
 * Purpose: Declaration of the class FaceDetect
 ***********************************************************************/

#ifndef _FACEDETECT_
#define _FACEDETECT_

#include "fd/arcsoft_fsdk_face_detection.h"
#include "fd/merror.h"
#include <opencv2/core/core.hpp>

typedef vector<RECT> VRECT;

using namespace cv;

class FaceDetect
{
public:
   FaceDetect();
   ~FaceDetect();
   void detect(Mat & imgData, VRECT & rcFace);
   bool convertFromMat(Mat & matData, LPASVLOFFSCREEN offInput);
   bool init(MInt32 nScale, MInt32 nMaxFace);

protected:
	bool destroy(void);

private:
   MRESULT nRet;
   MHandle hEngine;
   MByte * pWorkMem;
   ASVLOFFSCREEN offInput;
};

#endif