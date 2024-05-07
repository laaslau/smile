#include "SmileDetector.h"
#include <vector>
#include <ranges>
#include <filesystem>

namespace My::CvLib
{
	static constexpr char FACE[]{ "haarcascade_frontalface_default.xml" };
	static constexpr char SMILE[]{ "haarcascade_smile.xml" };

	SmileDetector::SmileDetector() 
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
		for (const auto& smile : m_smiles)
		{
			rectangle(frame, smile, cv::Scalar(0, 0, 255, 255), 4, 8, 0);
		}
		return static_cast<int>(m_smiles.size());
	}


	bool SmileDetector::onDataApplied(std::unique_lock<std::mutex>& lock)
	{
		if (m_inputMat.empty())
		{
			return true;
		}
		cv::Mat proc;
		cv::Mat procColor;
		std::vector<cv::Rect> faces;
		std::vector<cv::Rect> smiles;

		cv::cvtColor(m_inputMat, proc, cv::COLOR_BGR2GRAY);
		
		{
			My::Toolbox::unlocker ulock(lock);
			
			cv::cvtColor(proc, procColor, cv::COLOR_GRAY2BGRA);

			m_faceCascade.detectMultiScale(proc, faces, 1.3, 5);
			for (const auto& rect : faces)
			{
				cv::Mat faceMat = proc(rect);
				
				m_smileCascade.detectMultiScale(faceMat, smiles, 1.8, 20);

				for (auto& smile : smiles)
				{
					smile.x += rect.x;
					smile.y += rect.y;
					rectangle(procColor, smile, cv::Scalar(0, 0, 255, 255), 4, 8, 0);
				}

				rectangle(procColor, rect, cv::Scalar(0, 255, 0, 255), 4, 8, 0);

			};


		}
		
		procColor.copyTo(m_mat);
		m_smiles = smiles;
		m_frameId++;
		m_rate.hit();
		return true;
	}
}

