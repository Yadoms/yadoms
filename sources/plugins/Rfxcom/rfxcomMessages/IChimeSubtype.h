#pragma once

#include "IRfxcomMessage.h"
#include "RFXtrxHelpers.h"
#include <shared/plugin/yadomsApi/IYadomsApi.h>
#include <shared/DataContainer.h>

namespace yApi = shared::plugin::yadomsApi;

namespace rfxcomMessages
{
   //--------------------------------------------------------------
   /// \brief	The Thermostat3 subtype interface
   //--------------------------------------------------------------
   class IChimeSubtype
   {
   public:
      //--------------------------------------------------------------
      /// \brief	Destructor
      //--------------------------------------------------------------
      virtual ~IChimeSubtype() {}

      //--------------------------------------------------------------
      /// \brief	                        Get device model
      /// \return                         The device model
      //--------------------------------------------------------------
      virtual std::string getModel() const = 0;

      //--------------------------------------------------------------
      /// \brief	                        Declare the keyword
      /// \param[in] context              Yadoms APi context
      /// \param[in] command              The device name associated to keyword
      //--------------------------------------------------------------
      virtual void declare(boost::shared_ptr<yApi::IYadomsApi> context, const std::string& deviceName) const = 0;

      //--------------------------------------------------------------
      /// \brief	                        Historize data
      /// \param[in] context              Yadoms APi context
      /// \param[in] command              The device name associated to keyword
      //--------------------------------------------------------------
      virtual void historize(boost::shared_ptr<yApi::IYadomsApi> context, const std::string& deviceName) const = 0;

      //--------------------------------------------------------------
      /// \brief	                        Set keyword state from Yadoms command
      /// \param[in] yadomsCommand        The command from Yadoms
      /// \param[in] deviceDetails        Device details
      //--------------------------------------------------------------
      virtual void set(const shared::CDataContainer& yadomsCommand, const shared::CDataContainer& deviceDetails) = 0;

      //--------------------------------------------------------------
      /// \brief	                        Set keyword state from default value
      //--------------------------------------------------------------
      virtual void reset() = 0;

      //--------------------------------------------------------------
      /// \brief	                        Get ID from protocol data
      /// \param[in] id1                  ID1 byte
      /// \param[in] id2                  ID2 byte
      /// \param[in] sound                Sound byte, also used as ID3 in some cases
      //--------------------------------------------------------------
      virtual unsigned int idFromProtocol(unsigned char id1, unsigned char id2, unsigned char sound) = 0;

      //--------------------------------------------------------------
      /// \brief	                        Get protocol data from keyword state
      /// \param[in] id                   ID
      /// \param[out] id1                 ID1 byte
      /// \param[out] id2                 ID2 byte
      /// \param[out] sound               Sound byte, also used as ID3 in some cases
      //--------------------------------------------------------------
      virtual void idToProtocol(unsigned int id, unsigned char& id1, unsigned char& id2, unsigned char& sound) const = 0;

      //--------------------------------------------------------------
      /// \brief	                        Set keyword state from protocol data
      /// \param[in] cmd                  Command
      //--------------------------------------------------------------
      virtual void setFromProtocolState(unsigned char cmd) = 0;

      //--------------------------------------------------------------
      /// \brief	                        Get protocol data from keyword state
      /// \param[out] sound               Sound byte, if not used as ID3
      //--------------------------------------------------------------
      virtual void toProtocolState(unsigned char& sound) const = 0;
   };

} // namespace rfxcomMessages
