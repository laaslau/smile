#pragma once

#include <string>
#include <list>
#include <optional>

using StringList = std::list<std::string>;

namespace My::Common
{
	class IStreamData
	{
	public:
		virtual const std::string& getStreamName() const = 0;
		virtual void setStreamName(const std::string& name) = 0;

		virtual const StringList& getStreamNames() const = 0;
	};

	class IStreamControl
	{
	public:
		virtual bool run(bool start) = 0;
		virtual bool isRunning() const = 0;
	};

	class IVideoSource
	{
	public:
		virtual bool start(const std::string& name) = 0;
		virtual void stop() = 0;
	};

}
