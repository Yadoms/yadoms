#pragma once
#include <shared/plugin/yPluginApi/IYPluginApi.h>
#include <shared/DataContainer.h>

namespace yApi = shared::plugin::yPluginApi;

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
      virtual ~IChimeSubtype()
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
      /// \param[in] deviceDetails        Device details
      //--------------------------------------------------------------
      virtual void set(const std::string& yadomsCommand, const shared::CDataContainerSharedPtr& deviceDetails) = 0;

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
      virtual unsigned int idFromProtocol(unsigned char id1,
                                          unsigned char id2,
                                          unsigned char sound) = 0;

      //--------------------------------------------------------------
      /// \brief	                        Get protocol data from keyword state
      /// \param[in] id                   ID
      /// \param[out] id1                 ID1 byte
      /// \param[out] id2                 ID2 byte
      /// \param[out] sound               Sound byte, also used as ID3 in some cases
      //--------------------------------------------------------------
      virtual void idToProtocol(unsigned int id,
                                unsigned char& id1,
                                unsigned char& id2,
                                unsigned char& sound) const = 0;

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


