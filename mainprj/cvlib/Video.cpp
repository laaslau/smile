#include "Video.h"
#include "YPLogger.h"
#include "Toolbox.h"
#include <algorithm>
#include <format>
#include <vector>



namespace My::CvLib
{

	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//
	///////////////////////////////////////////////////////////////////////////////////////////////////////

	VideoSource::VideoSource(const std::vector<IFrameProcessor*>& processors) : m_processors{ processors }
	{
	}

	VideoSource::~VideoSource()
	{
		stop();
	}

	using namespace std::chrono_literals;

	void VideoSource::threadFn()
	{

		while (!m_exit.test())
		{
			cv::Mat mat;

			if (m_source.read(mat))
			{
				std::ranges::for_each(m_processors, [&mat](auto* processor) {processor->onFrame(mat); });
			}
			else
			{
				m_source.release();
				startStream();
			}
		}
		m_exit.clear();
	}

	bool VideoSource::startStream()
	{
		std::optional<int> num = std::nullopt;
		try
		{
			num = std::stoi(m_sourceName);
		}
		catch (const std::exception&)
		{
		}
		bool started{};

		if (num)
		{
			started = m_source.open(*num);
		}
		else
		{
			started = m_source.open(m_sourceName, cv::CAP_FFMPEG);
		}

		if (!started)
		{
			PLOGE << "Failed to open video stream [" << m_sourceName << "]";
			return false;
		}
		return true;
	}

	bool VideoSource::start(const std::string& name)
	{
		m_sourceName = name;

		startStream();
		m_thread = std::thread(&VideoSource::threadFn, this);
		return true;
	}

	void VideoSource::stop()
	{
		if (m_thread.joinable())
		{
			m_exit.test_and_set();
			m_thread.join();
		}

		m_source.release();
	}


	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//
	///////////////////////////////////////////////////////////////////////////////////////////////////////

	void VideoBGRA::onFrame(const cv::Mat& frame)
	{
		{
			std::lock_guard lock(m_mtx);
			frame.copyTo(m_inputMat);
			m_data.test_and_set();
		}
		m_condition.notify_one();
	}

	bool VideoBGRA::frame(frameCallback callback)
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
		m_data.clear();
		if (m_inputMat.empty())
		{
			return true;
		}
		cv::cvtColor(m_inputMat, m_mat, cv::COLOR_BGR2BGRA);
		m_frameId++;
		m_rate.hit();
		return true;
	}
}


