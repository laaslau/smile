#include "VideoDevice.h"
#include "YPLogger.h"
#include "Toolbox.h"
#include <algorithm>
#include <optional>

namespace My::CvLib
{

	VideoDevice::VideoDevice(const std::vector<IFrameProcessor*>& processors) : m_processors{ processors }
	{
	}

	VideoDevice::~VideoDevice()
	{
		stop();
	}

	void VideoDevice::threadFn()
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

	bool VideoDevice::startStream()
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

	bool VideoDevice::start(const std::string& name)
	{
		m_sourceName = name;

		startStream();
		m_thread = std::thread(&VideoDevice::threadFn, this);
		return true;
	}

	void VideoDevice::stop()
	{
		if (m_thread.joinable())
		{
			m_exit.test_and_set();
			m_thread.join();
		}

		m_source.release();
	}

}