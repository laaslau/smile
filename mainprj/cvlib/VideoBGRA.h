#pragma once
#include "common.h"
#include "Toolbox.h"
#include "VideoDevice.h"
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>


namespace My::CvLib
{

	class VideoBGRA : public My::Toolbox::OneFunThrd, public IFrameProcessor, public My::Common::IVideoSource
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
		bool frame(My::Common::frameCallback) override;
		float frameRate() override;
	};
}

