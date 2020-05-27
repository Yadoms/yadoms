#pragma once

#include <shared/plugin/yPluginApi/YPluginConfiguration.hpp>
#include "Configuration.h"
#include <shared/enumeration/EnumHelpers.hpp>

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;


//--------------------------------------------------------------
/// \brief	Controller which send commands to screen (http api)
//--------------------------------------------------------------
class CHttpController
{
public:
	//--------------------------------------------------------------
	/// \brief	    Send a command to ESPEasy oled api
	/// \param [in]	command	The command
	/// \param [in]	args	The arguments
	//--------------------------------------------------------------
	void sendCommand(const std::string& ip, const std::string & commandMode, const std::string & commandData) const;

	//--------------------------------------------------------------
	/// \brief	    Analyse and adjust text to be corectly handled by Http API
	/// \param [in]	text	The text to check
	/// \return				The corrected text
	//--------------------------------------------------------------
	static std::string processText(const std::string& text);

};

