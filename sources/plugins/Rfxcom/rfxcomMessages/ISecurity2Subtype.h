#pragma once

#include "IRfxcomMessage.h"
#include "RFXtrxHelpers.h"
#include <shared/plugin/yPluginApi/IYPluginApi.h>
#include <shared/DataContainer.h>

namespace yApi = shared::plugin::yPluginApi;

namespace rfxcomMessages
{
   //--------------------------------------------------------------
   /// \brief	The Security2 subtype interface
   //--------------------------------------------------------------
   class ISecurity2Subtype
   {
   public:
      //--------------------------------------------------------------
      /// \brief	Destructor
      //--------------------------------------------------------------
      virtual ~ISecurity2Subtype() {}

      //--------------------------------------------------------------
      /// \brief	                        Get device model
      /// \return                         The device model
      //--------------------------------------------------------------
      virtual std::string getModel() const = 0;

      //--------------------------------------------------------------
      /// \brief	                        Declare the keyword
      /// \param[in] api                  Yadoms APi context
      /// \param[in] command              The device name associated to keyword
      //--------------------------------------------------------------
      virtual void declare(boost::shared_ptr<yApi::IYPluginApi> api, const std::string& deviceName) const = 0;

      //--------------------------------------------------------------
      /// \brief	                        Historize data
      /// \param[in] api                  Yadoms APi context
      /// \param[in] command              The device name associated to keyword
      //--------------------------------------------------------------
      virtual void historize(boost::shared_ptr<yApi::IYPluginApi> api, const std::string& deviceName) const = 0;

      //--------------------------------------------------------------
      /// \brief	                        Set the ID of the keyword
      /// \param[in] id                   ID to set
      //--------------------------------------------------------------
      virtual void setId(unsigned int id) = 0;

      //--------------------------------------------------------------
      /// \brief	                        Get the ID of the keyword
      /// \return                         Keyword ID
      //--------------------------------------------------------------
      virtual unsigned int getId() const = 0;

      //--------------------------------------------------------------
      /// \brief	                        Set keyword state from Yadoms command
      /// \param[in] keyword              Keyword concerned by the command
      /// \param[in] yadomsCommand        The command from Yadoms
      //--------------------------------------------------------------
      virtual void set(const std::string& keyword, const std::string& yadomsCommand) = 0;

      //--------------------------------------------------------------
      /// \brief	                        Set keyword state from default value
      //--------------------------------------------------------------
      virtual void resetState() = 0;

      //--------------------------------------------------------------
      /// \brief	                        Set keyword state from protocol data
      /// \param[in] Security2            The message useful part
      //--------------------------------------------------------------
      virtual void setFromProtocolState(const RBUF& Security2) = 0;

      //--------------------------------------------------------------
      /// \brief	                        Get protocol data from keyword state
      /// \param[out] Security2           The message useful part
      //--------------------------------------------------------------
      virtual void toProtocolState(RBUF& Security2) const = 0;

   };

} // namespace rfxcomMessages
