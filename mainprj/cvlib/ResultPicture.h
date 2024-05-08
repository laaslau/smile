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
		static constexpr int ANIM_COLUMNS = 2;
		static constexpr int ANIM_ROWS = 5;

		cv::Mat m_mat[ANIM_COLUMNS][ANIM_ROWS];

		std::chrono::steady_clock::time_point m_smiledTime;
		bool m_smiled{};

		My::Common::IStreamData* m_data{};


		bool load();
		std::tuple<uint8_t*, uint32_t, uint32_t> getPic(bool smile, int row);

	public:

		ResultPicture(My::Common::IStreamData* data);

		bool frame(My::Common::frameCallback callback) override;
		float frameRate() override { return 1.0f; }
	};

}

