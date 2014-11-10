#include "stdafx.h"
#include "ChimeByronSx.h"
#include "RFXtrxHelpers.h"
#include <shared/Log.h>
#include <shared/plugin/yadomsApi/StandardCapacities.h>
#include <shared/exception/InvalidParameter.hpp>

namespace yApi = shared::plugin::yadomsApi;

namespace rfxcomMessages
{

CChimeByronSx::CChimeByronSx()
   :m_event("event"), m_sound(kDefaultSound)
{
}

std::string CChimeByronSx::getModel() const
{
   return "Byron SX";
}

void CChimeByronSx::declare(boost::shared_ptr<yApi::IYadomsApi> context, const std::string& deviceName) const
{
   context->declareKeyword(deviceName, m_event);
}

void CChimeByronSx::historize(boost::shared_ptr<yApi::IYadomsApi> context, const std::string& deviceName) const
{
   context->historizeData(deviceName, m_event);
}

void CChimeByronSx::set(const shared::CDataContainer& /*yadomsCommand*/, const shared::CDataContainer& deviceDetails)
{
   std::string sound = deviceDetails.get<std::string>("sound");
   if     (sound == "Tubular 3 Notes") m_sound = kTubular3Notes;
   else if(sound == "Big Ben")         m_sound = kBigBen;
   else if(sound == "Tubular 2 Notes") m_sound = kTubular2Notes;
   else if(sound == "Solo")            m_sound = kSolo;
   else
   {
      YADOMS_LOG(warning) << "Chime, unsupported sound \"" << sound << "\", use default";
      m_sound = kDefaultSound;
   }
}

void CChimeByronSx::reset()
{
   m_sound = kDefaultSound;
}

unsigned int CChimeByronSx::idFromProtocol(unsigned char id1, unsigned char id2, unsigned char /*sound*/)
{
   return id1 << 8 | id2;
}

void CChimeByronSx::idToProtocol(unsigned int id, unsigned char& id1, unsigned char& id2, unsigned char& sound) const
{
   id1 = (unsigned char) (0xFF & (id >> 8));
   id2 = (unsigned char) (0xFF & id);
}

void CChimeByronSx::setFromProtocolState(unsigned char cmd)
{
   switch(cmd)
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
      YADOMS_LOG(warning) << "Chime, unsupported received sound \"" << cmd << "\", use default";
      m_sound = kDefaultSound;
      break;
   }
}

void CChimeByronSx::toProtocolState(unsigned char& sound) const
{
   switch(m_sound)
   {
   case kTubular3Notes: sound = chime_sound0; break;
   case kBigBen       : sound = chime_sound1; break;
   case kTubular2Notes: sound = chime_sound2; break;
   case kSolo         : sound = chime_sound3; break;
   default:
      BOOST_ASSERT_MSG(false, "Unsupported sound value");
      sound = chime_sound0; break;
   }
}

} // namespace rfxcomMessages