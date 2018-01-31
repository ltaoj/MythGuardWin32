/***********************************************************************
 * Module:  FaceDetect.h
 * Author:  lenovo
 * Modified: 2018Äê1ÔÂ31ÈÕ 16:30:02
 * Purpose: Declaration of the class FaceDetect
 ***********************************************************************/

#ifndef _FACEDETECT_
#define _FACEDETECT_

class Mat;

#include "fd\arcsoft_fsdk_face_detection.h"
#include "fd\merror.h"

class FaceDetect
{
public:
   FaceDetect();
   ~FaceDetect();
   void detect(Mat & imgData, vector<RECT> & rcFace);
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