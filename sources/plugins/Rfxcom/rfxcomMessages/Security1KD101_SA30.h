#pragma once

#include <shared/plugin/yPluginApi/IYPluginApi.h>
#include <shared/DataContainer.h>
#include "ISecurity1Subtype.h"

namespace yApi = shared::plugin::yPluginApi;

namespace rfxcomMessages
{
   //--------------------------------------------------------------
   /// \brief	The Scurity1 KD101 subtype
   //--------------------------------------------------------------
   class CSecurity1KD101_SA30 : public ISecurity1Subtype
   {
   public:
      //--------------------------------------------------------------
      /// \brief	                        The RFXCom subtype value
      //--------------------------------------------------------------
      enum { rfxValueKD101 = sTypeKD101, rfxValueSA30 = sTypeSA30 };

   public:
      //--------------------------------------------------------------
      /// \brief	                        Constructor
      /// \param[in] model                The device model
      //--------------------------------------------------------------
      explicit CSecurity1KD101_SA30(const std::string& model);

      //--------------------------------------------------------------
      /// \brief	Destructor
      //--------------------------------------------------------------
      virtual ~CSecurity1KD101_SA30();
      
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
      /// \brief	                        The device model
      //--------------------------------------------------------------
      const std::string m_model;

      //--------------------------------------------------------------
      /// \brief	                        The keyword
      //--------------------------------------------------------------
      yApi::historization::CSwitch m_alarm;
   };

} // namespace rfxcomMessages
