#pragma once
#include "RFXtrxHelpers.h"
#include <shared/plugin/yPluginApi/IYPluginApi.h>

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
      virtual ~ICartelectronicSubtype()
      {
      }

      //--------------------------------------------------------------
      /// \brief	                        Declare the keyword
      /// \param[in] api                  Yadoms APi context
      /// \param[in] command              The device name associated to keyword
      //--------------------------------------------------------------
      virtual void declare(boost::shared_ptr<yApi::IYPluginApi> api,
                           const std::string& deviceName) const = 0;

      //--------------------------------------------------------------
      /// \brief	                        Enter a keyword in a list to historize
      /// \param[in] KeywordList          The keywordList
      //--------------------------------------------------------------
      virtual void historize(std::vector<boost::shared_ptr<yApi::historization::IHistorizable>>& KeywordList) const = 0;

      //--------------------------------------------------------------
      /// \brief	                        Get ID from protocol data
      /// \param[in] buf                  buffer of the message
      //--------------------------------------------------------------
      virtual std::string idFromProtocol(const RBUF& rbuf) const = 0;

      //--------------------------------------------------------------
      /// \brief	                        Get battery Level from protocol data
      /// \param[in] buf                  buffer of the message
      //--------------------------------------------------------------
      virtual char BatteryLevelFromProtocol(const RBUF& rbuf) = 0;

      //--------------------------------------------------------------
      /// \brief	                        Get rssi from protocol data
      /// \param[in] buf                  buffer of the message
      //--------------------------------------------------------------
      virtual char RssiFromProtocol(const RBUF& rbuf) = 0;

      //--------------------------------------------------------------
      /// \brief	                        Get device model
      /// \return                         The device model
      //--------------------------------------------------------------
      virtual std::string getModel() const = 0;
   };
} // namespace rfxcomMessages


