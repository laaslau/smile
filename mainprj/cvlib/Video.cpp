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

	VideoSource::VideoSource()
	{}

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
			cv::Mat matHalf;

			if (m_source.read(mat))
			{

				//m_procs.frame(mat);

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


}


