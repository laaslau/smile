#include "Data.h"
#include "Video.h"
#include "YPLogger.h"
#include "Toolbox.h"
#include <algorithm>
#include <format>
#include <vector>
#include <ranges>




namespace My::CvLib
{
	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//
	///////////////////////////////////////////////////////////////////////////////////////////////////////
	namespace
	{
		constexpr std::size_t STREAM_NAMES_MAX = 10;
		constexpr char STREAMS_SECTION[] = "Streams";
		constexpr char STREAM_PAR[] = "Stream";
	}


	StreamData::StreamData()
	{
		retrieve();
	}
	void StreamData::store()
	{
		for (auto [i, d] : m_streamNames | std::views::enumerate)
		{
			if (!d.empty())
			{
				My::Toolbox::writeStringPar(STREAMS_SECTION, std::format("{}{}", STREAM_PAR, i).c_str(), d);
			}
		}
	}

	void StreamData::retrieve()
	{
		for (int inx = 0; inx < STREAM_NAMES_MAX; inx++)
		{
			const auto& s = My::Toolbox::readStringPar(STREAMS_SECTION, std::format("{}{}", STREAM_PAR, inx).c_str(), "");
			if (s.empty())
			{
				break;
			}
			m_streamNames.push_back(s);
		}
	}



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


