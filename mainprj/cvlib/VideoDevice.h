#pragma once
#include "common.h"
#include "Toolbox.h"
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>

#include <thread>
#include <mutex>
#include <atomic>
#include <memory>
#include <vector>

namespace My::CvLib
{

	class IFrameProcessor
	{
	public:
		virtual void onFrame(const cv::Mat& frame) = 0;
	};


	class IContourAdder
	{
	public:
		virtual int addContours(cv::Mat& frame) const = 0;
	};

	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//
	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	class VideoDevice : public My::Common::IVideoDevice
	{

		std::thread m_thread{};
		std::atomic_flag m_exit{};
		cv::VideoCapture m_source;
		std::string m_sourceName;

		std::vector<IFrameProcessor*> m_processors;

		void threadFn();
		bool startStream();

	public:

		VideoDevice() = default;
		VideoDevice(const std::vector<IFrameProcessor*>& processors);
		~VideoDevice();

		bool start(const std::string& name) override;
		void stop() override;


	};

}

