#pragma once

#include <shared/plugin/yPluginApi/IYPluginApi.h>

namespace yApi = shared::plugin::yPluginApi;

namespace rfxcomMessages
{
   //--------------------------------------------------------------
   /// \brief	The Lightning2 subtype interface
   //--------------------------------------------------------------
   class ILighting2Subtype
   {
   public:
      enum EDeviceType
      {
         kUnknown = 0,
         kOnOff,
         kDimmable
      };

      //--------------------------------------------------------------
      /// \brief	Destructor
      //--------------------------------------------------------------
      virtual ~ILighting2Subtype()
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
      virtual const std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>>& keywords() const = 0;

      //--------------------------------------------------------------
      /// \brief	                        Set keyword state from Yadoms command
      /// \param[in] yadomsCommand        The command from Yadoms
      //--------------------------------------------------------------
      virtual void set(boost::shared_ptr<const yApi::IDeviceCommand> yadomsCommand) = 0;

      //--------------------------------------------------------------
      /// \brief	                        Set keyword state from default value
      //--------------------------------------------------------------
      virtual void reset() = 0;

      //--------------------------------------------------------------
      /// \brief	                        Get id and house code from protocol values
      /// \param[in] id1Byte              The id1 byte
      /// \param[in] id2Byte              The id2 byte
      /// \param[in] id3Byte              The id3 byte
      /// \param[in] id4Byte              The id4 byte
      /// \param[out] houseCode           The house code
      /// \param[out] id                  The id
      //--------------------------------------------------------------
      virtual void idFromProtocol(unsigned char id1Byte,
                                  unsigned char id2Byte,
                                  unsigned char id3Byte,
                                  unsigned char id4Byte,
                                  unsigned char& houseCode,
                                  unsigned int& id) const = 0;

      //--------------------------------------------------------------
      /// \brief	                        Set id and house code to protocol values
      /// \param[in] houseCode            The house code
      /// \param[in] id                   The id
      /// \param[out] id1Byte             The id1 byte
      /// \param[out] id2Byte             The id2 byte
      /// \param[out] id3Byte             The id3 byte
      /// \param[out] id4Byte             The id4 byte
      //--------------------------------------------------------------
      virtual void idToProtocol(unsigned char houseCode,
                                unsigned int id,
                                unsigned char& id1Byte,
                                unsigned char& id2Byte,
                                unsigned char& id3Byte,
                                unsigned char& id4Byte) const = 0;

      //--------------------------------------------------------------
      /// \brief	                        Set keyword state from protocol data
      /// \param[in] cmdByte              The cmd byte
      /// \param[in] levelByte            The level byte
      //--------------------------------------------------------------
      virtual void setFromProtocolState(unsigned char cmdByte,
                                        unsigned char levelByte) = 0;

      //--------------------------------------------------------------
      /// \brief	                        Get protocol data from keyword state
      /// \param[out] cmdByte             The cmd byte
      /// \param[out] levelByte           The level byte
      //--------------------------------------------------------------
      virtual void toProtocolState(unsigned char& cmdByte,
                                   unsigned char& levelByte) const = 0;
   };
} // namespace rfxcomMessages


