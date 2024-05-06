#pragma once
#include "common.h"

namespace My::CvLib
{
	using namespace My::Common;

	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//
	///////////////////////////////////////////////////////////////////////////////////////////////////////
	class StreamData : public IStreamData
	{
		StringList m_streamNames;
		void store() {}
		void retieve() {}
	public:
		const std::string& getStreamName() const override;
		void setStreamName(const std::string& name) override;
		const StringList& getStreamNames() const override;
	};

	class StreamControl : public IStreamControl
	{
		IVideoDevice* m_videoPtr{};
		IStreamData* m_dataPtr{};
		bool m_running{};
	public:
		StreamControl() = delete;
		StreamControl(IStreamData* dataPtr, IVideoDevice* videoPtr) : m_dataPtr{dataPtr}, m_videoPtr { videoPtr } {}

		bool run(bool start) override;
		bool isRunning() const override;
	};
}

