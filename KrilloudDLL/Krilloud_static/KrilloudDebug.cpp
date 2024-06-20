#include "KrilloudDebug.h"

bool debugActive = false;
std::string outputFileName;
const size_t max_debug_sentences = 1;
std::vector<std::string> debug_sentences;

namespace KrilloudDebug
{
	void SetDebugFilename(std::string filename)
	{
		outputFileName = filename;
	}

	void ActivateDebug(bool active)
	{
#ifndef __ANDROID__
		debugActive = active;
#endif
	}

	bool GetDebugStatus()
	{
		return debugActive;
	}

	const std::string currentDateTime() {
		
		boost::posix_time::ptime timeLocal = boost::posix_time::second_clock::local_time();

		std::stringstream ss;
		ss << timeLocal.date() << " - " << timeLocal.time_of_day();
		
		return ss.str();
	}

	void DebugWrite(std::string debugText)
	{
		if (debugActive)
		{
			std::stringstream sentence;
			sentence << currentDateTime() << " -- " << debugText;
			debug_sentences.push_back(sentence.str());

			if (debug_sentences.size() >= max_debug_sentences)
			{
				DumpDebug();
			}
		}
	}

	void DebugWrite(std::vector<std::string> debugText)
	{
		if (debugActive)
		{
			std::stringstream sentence;
			sentence << currentDateTime() << " -- ";

			for (size_t x = 0; x < debugText.size(); x++)
			{
				sentence << debugText[x];
			}

			debug_sentences.push_back(sentence.str());

			if (debug_sentences.size() >= max_debug_sentences)
			{
				DumpDebug();
			}
		}
	}

	void DumpDebug()
	{
		if (debugActive)
		{
			try
			{
				std::ofstream myfile;
				myfile.open(outputFileName, std::ofstream::app);

				for (size_t x = 0; x < debug_sentences.size(); x++)
				{
					myfile << debug_sentences[x] << "\n";
				}
				debug_sentences.clear();

				myfile.close();
			}
			catch (...)
			{

			}
		}
	}
}