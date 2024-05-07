#include "ResultPicture.h"
#include <filesystem>

namespace My::CvLib
{

	ResultPicture::ResultPicture(My::Common::IStreamData* data) : m_data{ data }
	{
		load();
	}

	std::tuple<uint8_t*, uint32_t, uint32_t> ResultPicture::getPic(bool smile)
	{
		if (smile)
		{
			return { m_matSmile.ptr<uchar>(0), static_cast<uint32_t>(m_matSmile.cols), static_cast<uint32_t>(m_matSmile.rows) };
		}
		return { m_matSerious.ptr<uchar>(0), static_cast<uint32_t>(m_matSerious.cols), static_cast<uint32_t>(m_matSerious.rows) };

	}

	bool ResultPicture::frame(My::Common::frameCallback callback)
	{
		if (m_data->getFaces() == 0)
		{
			return false;
		}
		int smileInx = (m_data->getSmiles() == 0) ? 1 : 0;
		auto [p, w, h] = getPic((smileInx == 0));
		callback(static_cast<uint64_t>(smileInx), p, w, h);
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

		int midpoint = image.cols / 2;


		cv::Rect leftHalf(0, 0, midpoint, image.rows);
		cv::Rect rightHalf(midpoint, 0, image.cols - midpoint, image.rows);

		m_matSmile = image(leftHalf);
		m_matSerious = image(rightHalf);


		cv::cvtColor(m_matSmile, m_matSmile, cv::COLOR_BGR2BGRA);
		cv::cvtColor(m_matSerious, m_matSerious, cv::COLOR_BGR2BGRA);

		return true;

	}



}