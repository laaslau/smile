#include "Factory.h"
#include "Video.h"
#include "YPLogger.h"
#include "Toolbox.h"
#include <algorithm>
#include <format>
#include <vector>
#include <ranges>




namespace My::CvLib
{

	static StreamData g_streamData;
	static VideoSource g_videoSource;
	static StreamControl g_streamControl{ &g_streamData, &g_videoSource };
	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//
	///////////////////////////////////////////////////////////////////////////////////////////////////////

	IStreamData* getStreamData()
	{
		return &g_streamData;
	}

	IStreamControl* getStreamControl()
	{
		return &g_streamControl;
	}


	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//
	///////////////////////////////////////////////////////////////////////////////////////////////////////
	static constexpr std::size_t STREAM_NAMES_MAX = 10;

	const std::string& StreamData::getStreamName() const
	{
		const static std::string EMPTY{};
		if (m_streamNames.empty())
		{
			return EMPTY;
		}

		return m_streamNames.front();
	}

	const StringList& StreamData::getStreamNames() const
	{
		return m_streamNames;
	}

	void StreamData::setStreamName(const std::string& name)
	{
		auto f = std::ranges::find(m_streamNames, name);

		if (f != m_streamNames.end())
		{
			auto top = *f;
			m_streamNames.erase(f);
			m_streamNames.emplace_front(top);
		}
		else
		{
			m_streamNames.emplace_front(name);
		}

		while (m_streamNames.size() > STREAM_NAMES_MAX)
		{
			m_streamNames.pop_back();
		}

		store();
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//
	///////////////////////////////////////////////////////////////////////////////////////////////////////

	bool StreamControl::run(bool start)
	{
		if (start)
		{
			m_running = m_videoPtr->start(m_dataPtr->getStreamName());
		}
		else
		{
			m_running = false;
			m_videoPtr->stop();
		}
		return m_running;
	}

	bool StreamControl::isRunning() const
	{
		return m_running;
	}

}


