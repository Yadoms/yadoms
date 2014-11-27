#pragma once

#include <shared/plugin/yadomsApi/IYadomsApi.h>
#include <shared/DataContainer.h>
#include "IThermostat1Subtype.h"

namespace yApi = shared::plugin::yadomsApi;

namespace rfxcomMessages
{
   //--------------------------------------------------------------
   /// \brief	The Thermostat1 Digimax keyword
   //--------------------------------------------------------------
   class CThermostat1Digimax : public IThermostat1Subtype
   {
   public:
      //--------------------------------------------------------------
      /// \brief	                        Constructor
      //--------------------------------------------------------------
      CThermostat1Digimax();
      //--------------------------------------------------------------
      /// \brief	Destructor
      //--------------------------------------------------------------
      virtual ~CThermostat1Digimax();
      
      // ILighting2Subtype implementation
      virtual std::string getModel() const;
      virtual void declare(boost::shared_ptr<yApi::IYadomsApi> context, const std::string& deviceName) const;
      virtual void historize(boost::shared_ptr<yApi::IYadomsApi> context, const std::string& deviceName) const;
      virtual void set(const std::string& yadomsCommand);
      virtual void reset();
      virtual void setFromProtocolState(const RBUF& thermostat1Rbuf);
      virtual void toProtocolState(RBUF& thermostat1Rbuf) const;
      // [END] ILighting2Subtype implementation

   private:
      //--------------------------------------------------------------
      /// \brief	                        The keyword
      //--------------------------------------------------------------
      yApi::historization::CTemperature m_currentTemperature;
      yApi::historization::CTemperature m_setPoint;
   };

} // namespace rfxcomMessages
