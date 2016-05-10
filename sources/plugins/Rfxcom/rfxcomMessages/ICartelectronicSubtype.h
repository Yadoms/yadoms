#pragma once

#include "IRfxcomMessage.h"
#include "RFXtrxHelpers.h"
#include <shared/plugin/yPluginApi/IYPluginApi.h>
#include <shared/DataContainer.h>

namespace yApi = shared::plugin::yPluginApi;

namespace rfxcomMessages
{
   //--------------------------------------------------------------
   /// \brief	The Cartelectronic subtype interface
   //--------------------------------------------------------------
   class ICartelectronicSubtype
   {
   public:
      //--------------------------------------------------------------
      /// \brief	Destructor
      //--------------------------------------------------------------
      virtual ~ICartelectronicSubtype() {}

      //--------------------------------------------------------------
      /// \brief	                        Declare the keyword
      /// \param[in] context              Yadoms APi context
      /// \param[in] command              The device name associated to keyword
      //--------------------------------------------------------------
      virtual void declare(boost::shared_ptr<yApi::IYPluginApi> context, const std::string& deviceName) const = 0;

      //--------------------------------------------------------------
      /// \brief	                        Enter a keyword in a list to historize
      /// \param[in] KeywordList          The keywordList
      //--------------------------------------------------------------
      virtual void historize(std::vector<boost::shared_ptr<yApi::historization::IHistorizable> > KeywordList) const = 0;

      //--------------------------------------------------------------
      /// \brief	                        Get ID from protocol data
      /// \param[in] buf                  buffer of the message
      //--------------------------------------------------------------
      virtual const std::string& idFromProtocol( const RBUF& rbuf ) = 0;

      //--------------------------------------------------------------
      /// \brief	                        Get battery Level from protocol data
      /// \param[in] buf                  buffer of the message
      //--------------------------------------------------------------
      virtual const char BatteryLevelFromProtocol( const RBUF& rbuf ) = 0;

      //--------------------------------------------------------------
      /// \brief	                        Get rssi from protocol data
      /// \param[in] buf                  buffer of the message
      //--------------------------------------------------------------
      virtual const char RssiFromProtocol( const RBUF& rbuf ) = 0;
   };

} // namespace rfxcomMessages
