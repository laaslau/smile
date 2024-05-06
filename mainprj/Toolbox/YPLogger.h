#pragma once
#include "plog/Log.h"
#include "plog/Initializers/RollingFileInitializer.h"
#include <filesystem>


namespace Y::Logger
{


	static constexpr char LOG_SUBFOLDER[] = { "log" };
	static constexpr char LOG_EXTENSION[]{ "log" };

	class Log : public plog::RollingFileAppender<plog::TxtFormatter>
	{
		std::string m_namePfx;
		std::string m_appPath;
		uint32_t m_logFileTimeDays{ 0 };

	public:
		Log(const std::string& appPath, const std::string& namePfx) :
			m_appPath{ appPath }, m_namePfx{ namePfx }, 
			plog::RollingFileAppender<plog::TxtFormatter>(getLogFilePath(appPath, namePfx).c_str(), 0x40000000, 10)
			
		{
			m_logFileTimeDays = getTimeDays();
		}

		void write(const plog::Record& record) override
		{
			rollLogFile();
			plog::RollingFileAppender<plog::TxtFormatter>::write(record);
		}

	private:

		std::string getLogFilePath(const std::string& path, const std::string& pfx)
		{
			std::filesystem::path logPath{ path };
			logPath.remove_filename();
			logPath.append(LOG_SUBFOLDER);

			auto const time = std::chrono::current_zone()->to_local(std::chrono::system_clock::now());
			auto name = std::format("{}{:%Y-%m-%d}", pfx, time).append(".").append(LOG_EXTENSION);

			if (validateFolder(logPath))
			{
				logPath.append(name);
				return std::string(logPath.string());
			}

			return name;
		}

		uint32_t getTimeDays()
		{
			const auto tt = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
			tm ltt{};
			localtime_s(&ltt, &tt);
			return static_cast<uint32_t>(ltt.tm_mday);
		}

		void rollLogFile()
		{
			auto h = getTimeDays();
			if (h != m_logFileTimeDays)
			{
				m_logFileTimeDays = h;
				setFileName(getLogFilePath(m_appPath, m_namePfx).c_str());
				PLOGI << "Logfile rolled....";
			}
		}

		bool validateFolder(const std::filesystem::path& folder)
		{
			if (std::filesystem::exists(folder))
			{
				return true;
			}
			return std::filesystem::create_directory(folder);
		}
	};

}

