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

   //--------------------------------------------------------------
   /// \brief	    Switch screen ON
   //--------------------------------------------------------------
   void switch_on();

   //--------------------------------------------------------------
   /// \brief	    Switch screen OFF
   //--------------------------------------------------------------
   void switch_off();

   //--------------------------------------------------------------
   /// \brief	    Clear all screen
   //--------------------------------------------------------------
   void clear_screen();

   //--------------------------------------------------------------
   /// \brief	    Update a line of text
   /// \param [in]	iLine	The line to update (form 1 to x)
   /// \param [in]	iCol	The starting column to update (from 1 to x)
   /// \param [in]	text	The text to display
   //--------------------------------------------------------------
   void update_line(int iLine, int iCol, const std::string& text);


protected:
	DECLARE_ENUM_HEADER(ECommands,
		((Oled))
		((OledCommand))
	)

	//--------------------------------------------------------------
	/// \brief	    Send a command to ESPEasy oled api (using ... args)
	/// \param [in]	command	The command
	/// \param [in]	args	The arguments
	//--------------------------------------------------------------
	template<class ... Types>
	void sendCommand(const ECommands& command, Types... args)
	{
		sendCommand(command, { args... });
	}

	//--------------------------------------------------------------
	/// \brief	    Send a command to ESPEasy oled api
	/// \param [in]	command	The command
	/// \param [in]	args	The arguments
	//--------------------------------------------------------------
	void sendCommand(const ECommands& command, const std::vector<std::string> & args) const;


	//--------------------------------------------------------------
	/// \brief	    Analyse and adjust text to be corectly handled by Oled API
	/// \param [in]	text	The text to check
	/// \return				The corrected text
	//--------------------------------------------------------------
	static std::string processText(const std::string& text);
private:
   //--------------------------------------------------------------
   /// \brief	    Configuration getter
   //--------------------------------------------------------------
   const COledEspEasyConfiguration & m_configuration;
};

