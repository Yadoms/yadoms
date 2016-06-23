#pragma once
#include <shared/plugin/yPluginApi/IYPluginApi.h>

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
      virtual ~ILighting5Subtype()
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
      virtual boost::shared_ptr<const yApi::historization::IHistorizable> keyword() const = 0;

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
      virtual void setFromProtocolState(unsigned char cmdByte,
                                        unsigned char levelByte) = 0;

      //--------------------------------------------------------------
      /// \brief	                        Get protocol data from keyword state
      /// \param[in] idxMessage           Message index
      /// \param[out] cmdByte             The cmd byte
      /// \param[out] levelByte           The level byte
      //--------------------------------------------------------------
      virtual void toProtocolState(size_t idxMessage,
                                   unsigned char& cmdByte,
                                   unsigned char& levelByte) const = 0;
   };
} // namespace rfxcomMessages


