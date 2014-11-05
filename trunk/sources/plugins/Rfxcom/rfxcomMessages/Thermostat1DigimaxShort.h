#pragma once

#include <shared/plugin/yadomsApi/IYadomsApi.h>
#include <shared/DataContainer.h>
#include "IThermostat1Subtype.h"

namespace yApi = shared::plugin::yadomsApi;

namespace rfxcomMessages
{
   //--------------------------------------------------------------
   /// \brief	The Thermostat1 Digimax short keyword
   //--------------------------------------------------------------
   class CThermostat1DigimaxShort : public IThermostat1Subtype
   {
   public:
      //--------------------------------------------------------------
      /// \brief	                        Constructor
      //--------------------------------------------------------------
      CThermostat1DigimaxShort();

      // ILighting2Subtype implementation
      virtual std::string getModel() const;
      virtual void declare(boost::shared_ptr<yApi::IYadomsApi> context, const std::string& deviceName) const;
      virtual void historize(boost::shared_ptr<yApi::IYadomsApi> context, const std::string& deviceName) const;
      virtual void set(const shared::CDataContainer& yadomsCommand);
      virtual void default();
      virtual void setFromProtocolState(const RBUF& thermostat1Rbuf);
      virtual void toProtocolState(RBUF& thermostat1Rbuf) const;
      // [END] ILighting2Subtype implementation

   private:
      //--------------------------------------------------------------
      /// \brief	                        The keyword
      //--------------------------------------------------------------
      yApi::historization::CTemperature m_currentTemperature;
   };

} // namespace rfxcomMessages
