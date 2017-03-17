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
      virtual const std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> >& keywords() const = 0;

      //--------------------------------------------------------------
      /// \brief	                        Get ID from protocol data
      /// \param[in] buf                  buffer of the message
      /// \return                         The id
      //--------------------------------------------------------------
      virtual std::string idFromProtocol(const RBUF& rbuf) const = 0;

      //--------------------------------------------------------------
      /// \brief	                        Get battery Level from protocol data
      /// \param[in] buf                  buffer of the message
      /// \return                         The battery level
      //--------------------------------------------------------------
      virtual char BatteryLevelFromProtocol(const RBUF& rbuf) = 0;

      //--------------------------------------------------------------
      /// \brief	                        Get signalStrength from protocol data
      /// \param[in] buf                  buffer of the message
      /// \return                         The signalStrength level
      //--------------------------------------------------------------
      virtual char signalStrengthFromProtocol(const RBUF& rbuf) = 0;

      //--------------------------------------------------------------
      /// \brief	                        Get device model
      /// \return                         The device model
      //--------------------------------------------------------------
      virtual std::string getModel() const = 0;
   };
} // namespace rfxcomMessages


