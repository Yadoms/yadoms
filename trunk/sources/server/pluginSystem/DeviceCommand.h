#pragma once
#include <shared/plugin/yadomsApi/IDeviceCommand.h>
#include "CommandHistorizer.h"

namespace pluginSystem
{
   //-----------------------------------------------------
   ///\brief The IDeviceCommand implementation
   //-----------------------------------------------------
   class CDeviceCommand : public shared::plugin::yadomsApi::IDeviceCommand
   {
   public:
      //-----------------------------------------------------
      ///\brief                  Constructor
      ///\param[in] targetDevice The target device (ex "Switch")
      ///\param[in] keyword      The target keyword (ex "dim")
      ///\param[in] body         The details of the command (value)
      //-----------------------------------------------------
      CDeviceCommand(const std::string& targetDevice, const std::string& keyword, const shared::CDataContainer & body);

      //-----------------------------------------------------
      ///\brief               Destructor
      //-----------------------------------------------------
      virtual ~CDeviceCommand();

      // IDeviceCommand implementation
      virtual const std::string& getTargetDevice() const;
      virtual const std::string& getKeyword() const;
      virtual const shared::CDataContainer& getBody() const;
      virtual const std::string toString() const;
      virtual const shared::plugin::yadomsApi::commands::IHistorizable& getHistorizableObject() const;
      // [END] IDeviceCommand implementation

   private:
      //-----------------------------------------------------
      ///\brief               Target device
      //-----------------------------------------------------
      std::string m_targetDevice;

      //-----------------------------------------------------
      ///\brief               Target keyword
      //-----------------------------------------------------
      std::string m_keyword;

      //-----------------------------------------------------
      ///\brief               Command body
      //-----------------------------------------------------
      shared::CDataContainer m_body;

      //-----------------------------------------------------
      ///\brief               Historizable object
      //-----------------------------------------------------
      CCommandHistorizer m_historizableObject;
   };  
} // namespace pluginSystem	
	
	