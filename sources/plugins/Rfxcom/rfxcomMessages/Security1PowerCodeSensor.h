#pragma once

#include <shared/plugin/yPluginApi/IYPluginApi.h>
#include <shared/DataContainer.h>
#include "ISecurity1Subtype.h"

namespace yApi = shared::plugin::yPluginApi;

namespace rfxcomMessages
{
   //--------------------------------------------------------------
   /// \brief	The Security1 PowerCode sensor subtype
   //--------------------------------------------------------------
   class CSecurity1PowerCodeSensor : public ISecurity1Subtype
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
      explicit CSecurity1PowerCodeSensor(bool secondaryContact);

      //--------------------------------------------------------------
      /// \brief	Destructor
      //--------------------------------------------------------------
      virtual ~CSecurity1PowerCodeSensor();
      
      // ISecurity1Subtype implementation
      virtual std::string getModel() const;
      virtual void declare(boost::shared_ptr<yApi::IYPluginApi> context, const std::string& deviceName) const;
      virtual void historize(boost::shared_ptr<yApi::IYPluginApi> context, const std::string& deviceName) const;
      virtual void set(const std::string& keyword, const std::string& yadomsCommand);
      virtual void reset();
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
