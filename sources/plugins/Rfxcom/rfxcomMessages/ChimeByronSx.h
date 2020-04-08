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
      std::string getModel() const override;
      const std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> >& keywords() const override;
      void set(const std::string& yadomsCommand, const shared::CDataContainerSharedPtr& deviceDetails) override;
      void reset() override;
      unsigned int idFromProtocol(unsigned char id1,
                                  unsigned char id2,
                                  unsigned char sound) override;
      void idToProtocol(unsigned int id,
                        unsigned char& id1,
                        unsigned char& id2,
                        unsigned char& sound) const override;
      void setFromProtocolState(unsigned char cmd) override;
      void toProtocolState(unsigned char& sound) const override;
      // [END] ILighting2Subtype implementation

   private:
      //--------------------------------------------------------------
      /// \brief	                        The keywords
      //--------------------------------------------------------------
      boost::shared_ptr<yApi::historization::CEvent> m_event;

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

      //--------------------------------------------------------------
      /// \brief	The keywords list to historize in one step for better performances
      //--------------------------------------------------------------
      std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > m_keywords;
   };
} // namespace rfxcomMessages


