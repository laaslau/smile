#pragma once
#include "common.h"
#include <mutex>
namespace My::CvLib
{
	using namespace My::Common;

	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//
	///////////////////////////////////////////////////////////////////////////////////////////////////////
	class StreamData : public IStreamData
	{
		mutable std::mutex m_mtx;
		std::optional<int> m_face{};
		int m_smiles{};

		StringList m_streamNames;
		void store();
		void retrieve();
	public:
		StreamData();
		const std::string& getStreamName() const override;
		void setStreamName(const std::string& name) override;
		const StringList& getStreamNames() const override;


		const std::optional<int> getFaceDir() const override;
		void setFaceDir(std::optional<int> f) override;

		const int getSmiles() const override;
		void setSmiles(int s) override;
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

