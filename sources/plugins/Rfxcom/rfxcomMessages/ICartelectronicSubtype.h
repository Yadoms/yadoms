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
      /// \brief	                        Get the keywords
      /// \return                         The keyword list
      //--------------------------------------------------------------
      virtual const std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>>& keywords(
         const shared::CDataContainerSharedPtr& deviceConfiguration) = 0;

      //--------------------------------------------------------------
      /// \brief	                        Get ID from protocol data
      /// \param[in] rbuf                 buffer of the message
      /// \return                         The id
      //--------------------------------------------------------------
      virtual std::string idFromProtocol(const RBUF& rbuf) const = 0;

      //--------------------------------------------------------------
      /// \brief	                        Get battery Level from protocol data
      /// \param[in] rbuf                 buffer of the message
      /// \return                         The battery level
      //--------------------------------------------------------------
      virtual char batteryLevelFromProtocol(const RBUF& rbuf) = 0;

      //--------------------------------------------------------------
      /// \brief	                        Get signalPower from protocol data
      /// \param[in] rbuf                 buffer of the message
      /// \return                         The signalPower level
      //--------------------------------------------------------------
      virtual char signalPowerFromProtocol(const RBUF& rbuf) = 0;

      //--------------------------------------------------------------
      /// \brief	                        Get device model
      /// \return                         The device model
      //--------------------------------------------------------------
      virtual std::string getModel() const = 0;
   };
} // namespace rfxcomMessages
