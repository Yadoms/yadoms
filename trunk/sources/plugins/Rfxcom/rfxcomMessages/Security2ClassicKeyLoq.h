#pragma once

#include <shared/plugin/yPluginApi/IYPluginApi.h>
#include <shared/DataContainer.h>
#include "ISecurity2Subtype.h"

namespace yApi = shared::plugin::yPluginApi;

namespace rfxcomMessages
{
   //--------------------------------------------------------------
   /// \brief	The Security2 Classic KeyLoq subtype
   //--------------------------------------------------------------
   class CSecurity2ClassicKeyLoq : public ISecurity2Subtype
   {
   public:
      //--------------------------------------------------------------
      /// \brief	                        The RFXCom subtype value
      //--------------------------------------------------------------
      enum { rfxValue = sTypeSec2Classic };

   public:
      //--------------------------------------------------------------
      /// \brief	                        Constructor
      //--------------------------------------------------------------
      CSecurity2ClassicKeyLoq();
      //--------------------------------------------------------------
      /// \brief	Destructor
      //--------------------------------------------------------------
      virtual ~CSecurity2ClassicKeyLoq();
      
      // ISecurity2Subtype implementation
      virtual std::string getModel() const;
      virtual void declare(boost::shared_ptr<yApi::IYPluginApi> context, const std::string& deviceName) const;
      virtual void historize(boost::shared_ptr<yApi::IYPluginApi> context, const std::string& deviceName) const;
      virtual void setId(unsigned int id);
      virtual unsigned int getId() const;
      virtual void set(const std::string& keyword, const std::string& yadomsCommand);
      virtual void resetState();
      virtual void setFromProtocolState(const RBUF& Security2);
      virtual void toProtocolState(RBUF& Security2) const;
      // [END] ISecurity2Subtype implementation

   private:
      //--------------------------------------------------------------
      /// \brief	The device id
      //--------------------------------------------------------------
      unsigned int m_id;

      //--------------------------------------------------------------
      /// \brief	                        The keywords
      //--------------------------------------------------------------
      yApi::historization::CSwitch m_button0;
      yApi::historization::CSwitch m_button1;
      yApi::historization::CSwitch m_button2;
      yApi::historization::CSwitch m_button3;
   };

} // namespace rfxcomMessages
