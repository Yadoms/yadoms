#pragma once

#include <shared/plugin/yPluginApi/IYPluginApi.h>
#include <shared/DataContainer.h>
#include "IThermostat3Subtype.h"

namespace yApi = shared::plugin::yPluginApi;

namespace rfxcomMessages
{
   //--------------------------------------------------------------
   /// \brief	The Thermostat Mertik G6RH4TB keyword
   //--------------------------------------------------------------
   class CThermostat3MertikG6RH4TB : public IThermostat3Subtype
   {
   public:
      //--------------------------------------------------------------
      /// \brief	                        Constructor
      //--------------------------------------------------------------
      CThermostat3MertikG6RH4TB();

      // ILighting2Subtype implementation
      virtual std::string getModel() const;
      virtual void declare(boost::shared_ptr<yApi::IYPluginApi> api, const std::string& deviceName) const;
      virtual void historize(boost::shared_ptr<yApi::IYPluginApi> api, const std::string& deviceName) const;
      virtual void set(const std::string& keyword, const std::string& yadomsCommand);
      virtual void reset();
      virtual void setFromProtocolState(unsigned char cmd);
      virtual void toProtocolState(unsigned char& cmd) const;
      // [END] ILighting2Subtype implementation

   private:
      //--------------------------------------------------------------
      /// \brief	                        The keywords
      //--------------------------------------------------------------
      yApi::historization::CSwitch m_onOff;
      yApi::historization::CSwitch m_onOff2;
      yApi::historization::CUpDownStop m_UpDown;

      //--------------------------------------------------------------
      /// \brief	                        Status byte buffering
      //--------------------------------------------------------------
      unsigned char m_statusByte;
   };

} // namespace rfxcomMessages
