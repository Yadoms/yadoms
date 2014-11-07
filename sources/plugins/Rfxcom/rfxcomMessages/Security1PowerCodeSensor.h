#pragma once

#include <shared/plugin/yadomsApi/IYadomsApi.h>
#include <shared/DataContainer.h>
#include "ISecurity1Subtype.h"

namespace yApi = shared::plugin::yadomsApi;

namespace rfxcomMessages
{
   //--------------------------------------------------------------
   /// \brief	The Security1 PowerCode sensor subtype
   //--------------------------------------------------------------
   class CSecurity1X10PowerCodeSensor : public ISecurity1Subtype
   {
   public:
      //--------------------------------------------------------------
      /// \brief	                        The RFXCom subtype value
      //--------------------------------------------------------------
      enum { rfxValuePowercodeSensor = sTypePowercodeSensor, rfxValuePowercodeSensorAux = sTypePowercodeAux };

   public:
      //--------------------------------------------------------------
      /// \brief	                        Constructor
      /// \param[in] secondaryContact     Secondary contact
      //--------------------------------------------------------------
      CSecurity1X10PowerCodeSensor(bool secondaryContact);

      // ISecurity1Subtype implementation
      virtual std::string getModel() const;
      virtual void declare(boost::shared_ptr<yApi::IYadomsApi> context, const std::string& deviceName) const;
      virtual void historize(boost::shared_ptr<yApi::IYadomsApi> context, const std::string& deviceName) const;
      virtual void set(const std::string& keyword, const shared::CDataContainer& yadomsCommand);
      virtual void default();
      virtual void setFromProtocolState(unsigned char statusByte);
      virtual unsigned char toProtocolState() const;
      // [END] ISecurity1Subtype implementation

   private:
      //--------------------------------------------------------------
      /// \brief	                        The keywords
      //--------------------------------------------------------------
      yApi::historization::CSwitch m_alarm;
      yApi::historization::CSwitch m_tamper;
   };

} // namespace rfxcomMessages
