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
#include <condition_variable>
#include <chrono>

namespace My::CvLib
{
	using namespace My::Common;

	class IFrameProcessor
	{
	public:
		virtual void onFrame(const cv::Mat& frame) = 0;
	};

	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//
	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	class VideoSource : public My::Common::IVideoDevice
	{

		std::thread m_thread{};
		std::atomic_flag m_exit{};
		cv::VideoCapture m_source;
		std::string m_sourceName;

		std::vector<IFrameProcessor*> m_processors;

		void threadFn();
		bool startStream();

	public:

		VideoSource() = default;
		VideoSource(const std::vector<IFrameProcessor*>& processors);
		~VideoSource();

		bool start(const std::string& name) override;
		void stop() override;


	};

	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//
	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	class VideoBGRA : public My::Toolbox::OneFunThrd, public IFrameProcessor, public IVideoSource
	{

		My::Toolbox::HitRate m_rate;
		uint64_t m_frameId{};

		cv::Mat m_mat{};
		cv::Mat m_inputMat{};

		// My::Toolbox::OneFunThrd
		bool onDataApplied(std::unique_lock<std::mutex>& lock) override;

	public:
		VideoBGRA() { start(); }
		~VideoBGRA() { stop(); }
		// public IFrameProcessor
		void onFrame(const cv::Mat& frame) override;

		// public IVideoSource
		bool frame(frameCallback) override;
		float frameRate() override;
	};
}

