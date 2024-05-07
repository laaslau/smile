#pragma once
#include "common.h"
#include "Toolbox.h"
#include "VideoDevice.h"
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>


namespace My::CvLib
{

	class SmileDetector : public My::Toolbox::OneFunThrd, public IFrameProcessor, public My::Common::IVideoSource, public IContourAdder
	{

		My::Toolbox::HitRate m_rate;
		uint64_t m_frameId{};

		std::vector<cv::Rect> m_smiles;
		cv::Mat m_mat{};
		cv::Mat m_inputMat{};

		My::Common::IStreamData* m_streamData{};

		// My::Toolbox::OneFunThrd
		bool onDataApplied(std::unique_lock<std::mutex>& lock) override;

		// data
		cv::CascadeClassifier m_faceCascade;
		cv::CascadeClassifier m_smileCascade;

		bool m_cascadesLoaded{};

		bool loadData();

	public:
		SmileDetector(My::Common::IStreamData* data);
		~SmileDetector();

		bool isCascadesLoaded() const { return m_cascadesLoaded; }

		// public IFrameProcessor
		void onFrame(const cv::Mat& frame) override;

		// public IVideoSource
		bool frame(My::Common::frameCallback) override;
		float frameRate() override;

		// public IContourAdder
		int addContours(cv::Mat& frame) const override;
	};
}

