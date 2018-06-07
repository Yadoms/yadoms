#pragma once
#include <shared/plugin/yPluginApi/IDeviceCommand.h>
#include "CommandHistorizer.h"
#include "database/common/requesters/Keyword.h"

namespace pluginSystem
{
   //-----------------------------------------------------
   ///\brief The IDeviceCommand implementation
   //-----------------------------------------------------
   class CDeviceCommand : public shared::plugin::yPluginApi::IDeviceCommand
   {
   public:
      //-----------------------------------------------------
      ///\brief                  Constructor
      ///\param[in] targetDevice The target device (ex "Switch")
      ///\param[in] keyword      The target keyword
      ///\param[in] body         The details of the command (value)
      //-----------------------------------------------------
      CDeviceCommand(const std::string& targetDevice,
                     boost::shared_ptr<const database::entities::CKeyword> keyword,
                     const std::string& body);

      //-----------------------------------------------------
      ///\brief               Destructor
      //-----------------------------------------------------
      virtual ~CDeviceCommand();

      // IDeviceCommand implementation
      const std::string& getDevice() const override;
      const std::string& getKeyword() const override;
      const std::string& getBody() const override;
      // [END] IDeviceCommand implementation

      virtual const shared::plugin::yPluginApi::historization::IHistorizable& getHistorizableObject() const;

   private:
      //-----------------------------------------------------
      ///\brief               Target device
      //-----------------------------------------------------
      std::string m_device;

      //-----------------------------------------------------
      ///\brief               Target keyword
      //-----------------------------------------------------
      boost::shared_ptr<const database::entities::CKeyword> m_keyword;

      //-----------------------------------------------------
      ///\brief               Command body
      //-----------------------------------------------------
      std::string m_body;

      //-----------------------------------------------------
      ///\brief               Historizable object
      //-----------------------------------------------------
      CCommandHistorizer m_historizableObject;
   };
} // namespace pluginSystem	


