#pragma once
#include "common.h"

namespace My::CvLib
{
	using namespace My::Common;

	IStreamData* getStreamData();
	IStreamControl* getStreamControl();
	IVideoSource* getVideoSource();
	IVideoSource* getSmiledSource();
	IVideoSource* getSmilePictogram();
}

