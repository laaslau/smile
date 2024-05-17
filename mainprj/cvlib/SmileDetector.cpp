#include "SmileDetector.h"
#include <vector>
#include <ranges>
#include <filesystem>
#include <cmath>
#include <algorithm>

namespace My::CvLib
{
	static constexpr char FACE[]{ "haarcascade_frontalface_default.xml" };
	static constexpr char SMILE[]{ "haarcascade_smile.xml" };

	SmileDetector::SmileDetector(My::Common::IStreamData* data) : m_streamData{ data }
	{
		loadData();
		start();
	}

	SmileDetector::~SmileDetector()
	{
		stop();
	}

	bool SmileDetector::loadData()
	{
		const auto& path = My::Toolbox::exePathA().parent_path();
		m_cascadesLoaded = m_faceCascade.load((path / FACE).string());
		m_cascadesLoaded &= m_smileCascade.load((path / SMILE).string());
		return m_cascadesLoaded;
	}

	void SmileDetector::onFrame(const cv::Mat& frame)
	{
		if (!m_cascadesLoaded)
		{
			return;
		}

		{
			std::lock_guard lock(m_mtx);
			frame.copyTo(m_inputMat);
		}

		hit();
	}

	bool SmileDetector::frame(My::Common::frameCallback callback)
	{
		std::lock_guard lock(m_mtx);
		if (m_mat.empty())
		{
			return false;
		}

		callback(m_frameId, m_mat.ptr(), static_cast<uint32_t>(m_mat.cols), static_cast<uint32_t>(m_mat.rows));
		return true;
	}

	float SmileDetector::frameRate()
	{
		return m_rate;
	}

	int SmileDetector::addContours(cv::Mat& frame) const
	{
		std::lock_guard lock(m_mtx);
		for (const auto& smile : m_smiles)
		{
			rectangle(frame, smile, cv::Scalar(0, 0, 255, 255), 4, 8, 0);
		}
		return static_cast<int>(m_smiles.size());
	}

	namespace
	{
		cv::Rect& operator  << (cv::Rect& me, const cv::Rect& another) { me.x += another.x; me.y += another.y; return me; }
	}


	bool SmileDetector::onDataApplied(std::unique_lock<std::mutex>& lock)
	{
		if (m_inputMat.empty())
		{
			return true;
		}
		cv::Mat proc;
		cv::Mat procColor;


		cv::cvtColor(m_inputMat, proc, cv::COLOR_BGR2GRAY);

		{
			My::Toolbox::unlocker ulock(lock);

			cv::cvtColor(proc, procColor, cv::COLOR_GRAY2BGRA);

			std::vector<cv::Rect> faces;
			m_faceCascade.detectMultiScale(proc, faces, 1.3, 5);
			std::ranges::sort(faces, [](const auto& f1, const auto& f2) {return f1.area() > f2.area(); });

			for (const auto [inx, rect] : faces | std::views::enumerate)
			{
				std::vector<cv::Rect> smiles;
				m_smileCascade.detectMultiScale(proc(rect), smiles, 1.8, 20);

				std::ranges::for_each(smiles, [&procColor, &rect](auto& smile) { rectangle(procColor, smile << rect, cv::Scalar(0, 0, 255, 255), 4, 8, 0); });
				rectangle(procColor, rect, cv::Scalar(0, 255, 0, 255), 4, 8, 0);

				if (inx == 0)
				{
					auto horCent = static_cast<int>((static_cast<float>(rect.x + rect.width / 2) / static_cast<float>(proc.cols)) * 100);
					m_streamData->setFaceDir({ horCent });
					m_streamData->setSmiling(!smiles.empty());
					m_smiles = smiles;
				}
			}
		}

		procColor.copyTo(m_mat);
		m_frameId++;
		m_rate.hit();
		return true;
	}
}


