#include "Factory.h"
#include "VideoBGRA.h"
#include "VideoDevice.h"
#include "SmileDetector.h"
#include "Data.h"
#include "YPLogger.h"
#include "Toolbox.h"
#include <algorithm>
#include <format>
#include <vector>
#include <ranges>

namespace My::CvLib
{

	static StreamData g_streamData;
	static SmileDetector g_smileDetector;
	static VideoBGRA g_videoBGRA({ &g_smileDetector });
	static VideoDevice g_videoSource({ &g_videoBGRA, &g_smileDetector });
	static StreamControl g_streamControl(&g_streamData, &g_videoSource);
	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//
	///////////////////////////////////////////////////////////////////////////////////////////////////////

	IStreamData* getStreamData()
	{
		return &g_streamData;
	}

	IStreamControl* getStreamControl()
	{
		return &g_streamControl;
	}

	IVideoSource* getVideoSource()
	{
		return &g_videoBGRA;
	}

	IVideoSource* getSmiledSource()
	{
		return &g_smileDetector;
	}

}


