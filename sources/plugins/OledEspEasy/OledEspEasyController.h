#pragma once

#include <shared/plugin/yPluginApi/YPluginConfiguration.hpp>
#include "OledEspEasyConfiguration.h"
#include <shared/enumeration/EnumHelpers.hpp>

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;


//--------------------------------------------------------------
/// \brief	Configuration of the empty plugin
//--------------------------------------------------------------
class COledEspEasyController
{
public:
	enum E_StartResult
	{
		kSuccess = 0,      /// started successfully
		kSerialPortError,  /// in case of serial port access problem
		kControllerError,  /// in case of controller problem
		kUnknownError      /// in case of any other problem      
	};

	//--------------------------------------------------------------
	/// \brief	    Contructor
	//--------------------------------------------------------------
	COledEspEasyController(const COledEspEasyConfiguration& configuration);

   //--------------------------------------------------------------
   /// \brief	    Destructor
   //--------------------------------------------------------------
   virtual ~COledEspEasyController();

   
   void switch_on();
   void switch_off();
   void clear_screen();
   void update_line(int iLine, int iCol, const std::string& text);


protected:
	DECLARE_ENUM_HEADER(ECommands,
		((Oled))
		((OledCommand))
	)


	template<class ... Types>
	void sendCommand(const ECommands& command, Types... args)
	{
		sendCommand(command, { args... });
	}

	void sendCommand(const ECommands& command, const std::vector<std::string> & args) const;

private:
   //--------------------------------------------------------------
   /// \brief	    Configuration getter
   //--------------------------------------------------------------
   const COledEspEasyConfiguration & m_configuration;
};

