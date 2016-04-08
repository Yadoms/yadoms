#pragma once
#include "ICommandLine.h"

//TODO virer
namespace plugin_cpp_api
{
	//--------------------------------------------------------------
	/// \class The command line parameter extraction
	//--------------------------------------------------------------
	class CCommandLine : public ICommandLine
	{
	public:
		CCommandLine(int argc, char** argv);

		virtual ~CCommandLine();

		//--------------------------------------------------------------
		/// \brief              Get the plugin execution context (Yadoms API)
		/// \return             Plugin execution context (Yadoms API)
		//--------------------------------------------------------------
		std::string yPluginApiAccessorId() const override;

	private:
		
		std::string m_yPluginApiAccessorId;
	};
} // namespace plugin_cpp_api
