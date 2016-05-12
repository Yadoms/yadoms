#pragma once

#include "IRfxcomMessage.h"
#include "RFXtrxHelpers.h"
#include <shared/plugin/yPluginApi/IYPluginApi.h>
#include <shared/DataContainer.h>

namespace yApi = shared::plugin::yPluginApi;

namespace rfxcomMessages
{
   //--------------------------------------------------------------
   /// \brief	The Lightning2 subtype interface
   //--------------------------------------------------------------
   class IRemoteSubtype
   {
   public:
      //--------------------------------------------------------------
      /// \brief	Destructor
      //--------------------------------------------------------------
      virtual ~IRemoteSubtype() {}

      //--------------------------------------------------------------
      /// \brief	                        Get device model
      /// \return                         The device model
      //--------------------------------------------------------------
      virtual const std::string& getModel() const = 0;

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
      /// \brief	                        Set keyword state from Yadoms command
      /// \param[in] yadomsCommand        The command from Yadoms
      //--------------------------------------------------------------
      virtual void set(const std::string& yadomsCommand) = 0;

      //--------------------------------------------------------------
      /// \brief	                        Set keyword state from protocol data
      /// \param[in] remoteRbuf           The buffer from RFXCom
      //--------------------------------------------------------------
      virtual void setFromProtocolState(const RBUF& remoteRbuf) = 0;

      //--------------------------------------------------------------
      /// \brief	                        Get protocol data from keyword state
      /// \param[out] remoteRbuf          The being prepared buffer for RFXCom
      //--------------------------------------------------------------
      virtual void toProtocolState(RBUF& remoteRbuf) const = 0;
   };

} // namespace rfxcomMessages
