#pragma once

#include <shared/plugin/yPluginApi/IYPluginApi.h>
#include <shared/DataContainer.h>
#include "IChimeSubtype.h"

namespace yApi = shared::plugin::yPluginApi;

namespace rfxcomMessages
{
   //--------------------------------------------------------------
   /// \brief	The Chime Byron SX keyword
   //--------------------------------------------------------------
   class CChimeByronSx : public IChimeSubtype
   {
   public:
      //--------------------------------------------------------------
      /// \brief	                        Constructor
      //--------------------------------------------------------------
      CChimeByronSx();

      // ILighting2Subtype implementation
      virtual std::string getModel() const;
      virtual void declare(boost::shared_ptr<yApi::IYPluginApi> context, const std::string& deviceName) const;
      virtual void historize(boost::shared_ptr<yApi::IYPluginApi> context, const std::string& deviceName) const;
      virtual void set(const std::string& yadomsCommand, const shared::CDataContainer& deviceDetails);
      virtual void reset();
      virtual unsigned int idFromProtocol(unsigned char id1, unsigned char id2, unsigned char sound);
      virtual void idToProtocol(unsigned int id, unsigned char& id1, unsigned char& id2, unsigned char& sound) const;
      virtual void setFromProtocolState(unsigned char cmd);
      virtual void toProtocolState(unsigned char& sound) const;
      // [END] ILighting2Subtype implementation

   private:
      //--------------------------------------------------------------
      /// \brief	                        The keywords
      //--------------------------------------------------------------
      yApi::historization::CEvent m_event;
      
      //--------------------------------------------------------------
      /// \brief	The sound to play (non-historizable)
      //--------------------------------------------------------------
      enum
      {
         kTubular3Notes,
         kBigBen,
         kTubular2Notes,
         kSolo,
         kDefaultSound = kTubular3Notes
      } m_sound;
   };

} // namespace rfxcomMessages
