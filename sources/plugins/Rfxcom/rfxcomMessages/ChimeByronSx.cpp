#include "stdafx.h"
#include "ChimeByronSx.h"
#include "RFXtrxHelpers.h"
#include <shared/Log.h>

namespace yApi = shared::plugin::yPluginApi;

namespace rfxcomMessages
{
   CChimeByronSx::CChimeByronSx()
      : m_event(boost::make_shared<shared::plugin::yPluginApi::historization::CEvent>("event")),
      m_sound(kDefaultSound),
      m_keywords({ m_event })
   {
   }

   std::string CChimeByronSx::getModel() const
   {
      return "Byron SX";
   }

   const std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> >& CChimeByronSx::keywords() const
   {
      return m_keywords;
   }

   void CChimeByronSx::set(const std::string& /*yadomsCommand*/, const shared::CDataContainerSharedPtr& deviceDetails)
   {
      auto sound = deviceDetails->get<std::string>("sound");
      if (sound == "Tubular 3 Notes") m_sound = kTubular3Notes;
      else if (sound == "Big Ben") m_sound = kBigBen;
      else if (sound == "Tubular 2 Notes") m_sound = kTubular2Notes;
      else if (sound == "Solo") m_sound = kSolo;
      else
      {
         YADOMS_LOG(information) << "Chime, unsupported sound \"" << sound << "\", use default" ;
         m_sound = kDefaultSound;
      }
   }

   void CChimeByronSx::reset()
   {
      m_sound = kDefaultSound;
   }

   unsigned int CChimeByronSx::idFromProtocol(unsigned char id1,
                                              unsigned char id2,
                                              unsigned char /*sound*/)
   {
      return id1 << 8 | id2;
   }

   void CChimeByronSx::idToProtocol(unsigned int id,
                                    unsigned char& id1,
                                    unsigned char& id2,
                                    unsigned char& sound) const
   {
      id1 = static_cast<unsigned char>(0xFF & (id >> 8));
      id2 = static_cast<unsigned char>(0xFF & id);
   }

   void CChimeByronSx::setFromProtocolState(unsigned char cmd)
   {
      switch (cmd)
      {
      case chime_sound0:
      case chime_sound4:
         m_sound = kTubular3Notes;
         break;
      case chime_sound1:
      case chime_sound5:
         m_sound = kBigBen;
         break;
      case chime_sound2:
      case chime_sound6:
         m_sound = kTubular2Notes;
         break;
      case chime_sound3:
      case chime_sound7:
         m_sound = kSolo;
         break;
      default:
         YADOMS_LOG(information) << "Chime, unsupported received sound \"" << cmd << "\", use default" ;
         m_sound = kDefaultSound;
         break;
      }
   }

   void CChimeByronSx::toProtocolState(unsigned char& sound) const
   {
      switch (m_sound)
      {
      case kTubular3Notes: sound = chime_sound0;
         break;
      case kBigBen: sound = chime_sound1;
         break;
      case kTubular2Notes: sound = chime_sound2;
         break;
      case kSolo: sound = chime_sound3;
         break;
      default:
         YADOMS_LOG(information) << "CChimeByronSx::toProtocolState : Unsupported sound value : " << m_sound ;
         sound = chime_sound0;
         break;
      }
   }
} // namespace rfxcomMessages


