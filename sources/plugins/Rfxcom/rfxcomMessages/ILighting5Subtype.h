#pragma once

#include "IRfxcomMessage.h"
#include "RFXtrxHelpers.h"
#include <shared/plugin/yPluginApi/IYPluginApi.h>
#include <shared/DataContainer.h>

namespace yApi = shared::plugin::yPluginApi;

namespace rfxcomMessages
{
   //--------------------------------------------------------------
   /// \brief	The Lightning5 keyword interface
   //--------------------------------------------------------------
   class ILighting5Subtype
   {
   public:
      //--------------------------------------------------------------
      /// \brief	Destructor
      //--------------------------------------------------------------
      virtual ~ILighting5Subtype() {}

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
      /// \brief	                        Set keyword state from Yadoms command
      /// \param[in] yadomsCommand        The command from Yadoms
      //--------------------------------------------------------------
      virtual void set(const std::string& yadomsCommand) = 0;

      //--------------------------------------------------------------
      /// \brief	                        Set keyword state from default value
      //--------------------------------------------------------------
      virtual void reset() = 0;

      //--------------------------------------------------------------
      /// \brief	                        Get the number of messages needed to perform command
      /// \return                         Number of messages
      //--------------------------------------------------------------
      virtual size_t getMessageNb() const = 0;

      //--------------------------------------------------------------
      /// \brief	                        Set keyword state from protocol data
      /// \param[in] cmdByte              The cmd byte
      /// \param[in] levelByte            The level byte
      //--------------------------------------------------------------
      virtual void setFromProtocolState(unsigned char cmdByte, unsigned char levelByte) = 0;

      //--------------------------------------------------------------
      /// \brief	                        Get protocol data from keyword state
      /// \param[in] idxMessage           Message index
      /// \param[out] cmdByte             The cmd byte
      /// \param[out] levelByte           The level byte
      //--------------------------------------------------------------
      virtual void toProtocolState(size_t idxMessage, unsigned char& cmdByte, unsigned char& levelByte) const = 0;
   };

} // namespace rfxcomMessages
