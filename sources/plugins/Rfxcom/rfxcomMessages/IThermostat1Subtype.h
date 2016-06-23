#pragma once
#include "RFXtrxHelpers.h"
#include <shared/plugin/yPluginApi/IYPluginApi.h>

namespace yApi = shared::plugin::yPluginApi;

namespace rfxcomMessages
{
   //--------------------------------------------------------------
   /// \brief	The Thermostat1 subtype interface
   //--------------------------------------------------------------
   class IThermostat1Subtype
   {
   public:
      //--------------------------------------------------------------
      /// \brief	Destructor
      //--------------------------------------------------------------
      virtual ~IThermostat1Subtype()
      {
      }

      //--------------------------------------------------------------
      /// \brief	                        Get device model
      /// \return                         The device model
      //--------------------------------------------------------------
      virtual std::string getModel() const = 0;

      //--------------------------------------------------------------
      /// \brief	                        Get the keywords
      /// \return                         The keyword list
      //--------------------------------------------------------------
      virtual const std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> >& keywords() const = 0;

      //--------------------------------------------------------------
      /// \brief	                        Set keyword state from Yadoms command
      /// \param[in] yadomsCommand        The command from Yadoms
      //--------------------------------------------------------------
      virtual void set(const std::string& yadomsCommand) = 0;

      //--------------------------------------------------------------
      /// \brief	                        Set keyword state from default value
      //--------------------------------------------------------------
      virtual void reset() = 0;

      //--------------------------------------------------------------
      /// \brief	                        Set keyword state from protocol data
      /// \param[in] thermostat1Rbuf      The buffer from RFXCom
      //--------------------------------------------------------------
      virtual void setFromProtocolState(const RBUF& thermostat1Rbuf) = 0;

      //--------------------------------------------------------------
      /// \brief	                        Get protocol data from keyword state
      /// \param[out] thermostat1Rbuf     The being prepared buffer for RFXCom
      //--------------------------------------------------------------
      virtual void toProtocolState(RBUF& thermostat1Rbuf) const = 0;
   };
} // namespace rfxcomMessages


