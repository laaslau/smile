#include "VideoBGRA.h"
#include "YPLogger.h"
#include "Toolbox.h"
#include <ranges>


namespace My::CvLib
{

	void VideoBGRA::onFrame(const cv::Mat& frame)
	{
		{
			std::lock_guard lock(m_mtx);
			frame.copyTo(m_inputMat);
		}

		hit();
	}

	bool VideoBGRA::frame(My::Common::frameCallback callback)
	{
		std::lock_guard lock(m_mtx);
		if (m_mat.empty())
		{
			return false;
		}

		callback(m_frameId, m_mat.ptr(), static_cast<uint32_t>(m_mat.cols), static_cast<uint32_t>(m_mat.rows));
		return true;
	}

	float VideoBGRA::frameRate()
	{
		return m_rate;
	}


	bool VideoBGRA::onDataApplied(std::unique_lock<std::mutex>& /*lock*/)
	{
		if (m_inputMat.empty())
		{
			return true;
		}
		cv::cvtColor(m_inputMat, m_mat, cv::COLOR_BGR2BGRA);
		std::ranges::for_each(m_contours, [&mat = m_mat](const auto* contour) {contour->addContours(mat); });
		m_frameId++;
		m_rate.hit();
		return true;
	}
}


