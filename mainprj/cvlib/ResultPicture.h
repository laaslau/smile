#pragma once
#include "common.h"
#include "Toolbox.h"
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>

#include <tuple>
#include <vector>
#include <chrono>

namespace My::CvLib
{

	class ResultPicture : public My::Common::IVideoSource
	{
		cv::Mat m_matSmile;
		cv::Mat m_matSerious;

		std::chrono::steady_clock::time_point m_smiledTime;
		bool m_smiled{};

		My::Common::IStreamData* m_data{};


		bool load();

	public:

		ResultPicture(My::Common::IStreamData* data);
		std::tuple<uint8_t*, uint32_t, uint32_t> getPic(bool smile);

		bool frame(My::Common::frameCallback callback) override;
		float frameRate() override { return 1.0f; }
	};

}

