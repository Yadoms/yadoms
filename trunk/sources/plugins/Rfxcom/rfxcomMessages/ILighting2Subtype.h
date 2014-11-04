#pragma once

#include "IRfxcomMessage.h"
#include "RFXtrxHelpers.h"
#include <shared/plugin/yadomsApi/IYadomsApi.h>
#include <shared/DataContainer.h>

namespace yApi = shared::plugin::yadomsApi;

namespace rfxcomMessages
{
   //--------------------------------------------------------------
   /// \brief	The Lightning2 subtype interface
   //--------------------------------------------------------------
   class ILighting2Subtype
   {
   public:
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
      //--------------------------------------------------------------
      virtual void set(const shared::CDataContainer& yadomsCommand) = 0;

      //--------------------------------------------------------------
      /// \brief	                        Set keyword state from default value
      //--------------------------------------------------------------
      virtual void default() = 0;

      //--------------------------------------------------------------
      /// \brief	                        Get id and house code from protocol values
      /// \param[in] id1Byte              The id1 byte
      /// \param[in] id2Byte              The id2 byte
      /// \param[in] id3Byte              The id3 byte
      /// \param[in] id4Byte              The id4 byte
      /// \param[out] houseCode           The house code
      /// \param[out] id                  The id
      //--------------------------------------------------------------
      virtual void idFromProtocol(unsigned char id1Byte, unsigned char id2Byte, unsigned char id3Byte, unsigned char id4Byte, unsigned char& houseCode, unsigned int& id) const = 0;

      //--------------------------------------------------------------
      /// \brief	                        Set id and house code to protocol values
      /// \param[in] houseCode            The house code
      /// \param[in] id                   The id
      /// \param[out] id1Byte             The id1 byte
      /// \param[out] id2Byte             The id2 byte
      /// \param[out] id3Byte             The id3 byte
      /// \param[out] id4Byte             The id4 byte
      //--------------------------------------------------------------
      virtual void idToProtocol(unsigned char houseCode, unsigned int id, unsigned char& id1Byte, unsigned char& id2Byte, unsigned char& id3Byte, unsigned char& id4Byte) const = 0;

      //--------------------------------------------------------------
      /// \brief	                        Set keyword state from protocol data
      /// \param[in] cmdByte              The cmd byte
      /// \param[in] levelByte            The level byte
      //--------------------------------------------------------------
      virtual void setFromProtocolState(unsigned char cmdByte, unsigned char levelByte) = 0;

      //--------------------------------------------------------------
      /// \brief	                        Get protocol data from keyword state
      /// \param[out] cmdByte             The cmd byte
      /// \param[out] levelByte           The level byte
      //--------------------------------------------------------------
      virtual void toProtocolState(unsigned char& cmdByte, unsigned char& levelByte) const = 0;
   };

} // namespace rfxcomMessages
