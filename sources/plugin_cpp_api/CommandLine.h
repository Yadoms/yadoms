#pragma once
#include "ICommandLine.h"

namespace plugin_cpp_api
{
	//--------------------------------------------------------------
	///\brief The command line parameter extraction
	//--------------------------------------------------------------
	class CCommandLine : public ICommandLine
	{
	public:
		CCommandLine(int argc, char** argv);

		virtual ~CCommandLine();

		std::string yPluginApiAccessorId() const override;

	private:
		
		std::string m_yPluginApiAccessorId;
	};
} // namespace plugin_cpp_api
