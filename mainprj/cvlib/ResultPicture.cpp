#include "ResultPicture.h"
#include <filesystem>

namespace My::CvLib
{

	ResultPicture::ResultPicture(My::Common::IStreamData* data) : m_data{ data }
	{
		load();
	}

	std::tuple<uint8_t*, uint32_t, uint32_t> ResultPicture::getPic(bool smile, int row)
	{
		int col = smile ? 0 : 1;
		return { m_mat[col][row].ptr<uchar>(0), static_cast<uint32_t>(m_mat[col][row].cols), static_cast<uint32_t>(m_mat[col][row].rows) };
	}

	
	bool ResultPicture::frame(My::Common::frameCallback callback)
	{
		if (m_data->getFaceDir() == std::nullopt)
		{
			return false;
		}
		m_smiled = m_data->getSmiling();
		if (m_smiled)
		{
			m_smiledTime = std::chrono::steady_clock::now();
		}
		else
		{
			using namespace std::chrono_literals;
			if (std::chrono::steady_clock::now() - m_smiledTime < 1000ms)
			{
				m_smiled = true;
			}
		}

		
		int row = (ANIM_ROWS * *m_data->getFaceDir()) / 100;
		auto [p, w, h] = getPic(m_smiled, row);
		callback((m_smiled ? 1 : 2) * row, p, w, h);
		return true;
	}

	static constexpr char SMILE_PIC[]{ "smile.jpg" };


	bool ResultPicture::load()
	{
		cv::VideoCapture source;
		const auto& path = My::Toolbox::exePathA().parent_path();


		cv::Mat image = cv::imread((path / SMILE_PIC).string().c_str());

		if (image.empty())
		{
			return false;
		}

		int colWidth = image.cols / ANIM_COLUMNS;
		int rowHeigh = image.rows / ANIM_ROWS;

		for (int c = 0; c < ANIM_COLUMNS; c++)
		{
			for (int r = 0; r < ANIM_ROWS; r++)
			{
				cv::Rect part(c * colWidth, r * rowHeigh, colWidth, rowHeigh);
				int destRow = ANIM_ROWS - r - 1;
				cv::cvtColor(image(part), m_mat[c][destRow], cv::COLOR_BGR2BGRA);
			}
		}
		return true;
	}



}