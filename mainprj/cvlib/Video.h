#pragma once
#include "common.h"
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>

#include <thread>
#include <mutex>
#include <atomic>
#include <memory>
#include <vector>
#include <condition_variable>
#include <chrono>

namespace My::CvLib
{
	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//
	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	class VideoSource : public My::Common::IVideoSource
	{

		std::thread m_thread{};
		std::atomic_flag m_exit{};

		cv::VideoCapture m_source;
		std::string m_sourceName;
		cv::Mat m_mat{};
		void threadFn();
		bool startStream();

	public:

		VideoSource();
		~VideoSource();

		bool start(const std::string& name) override;
		void stop() override;


	};

}

