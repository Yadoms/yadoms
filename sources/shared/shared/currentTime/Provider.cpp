#include "stdafx.h"
#include "Provider.h"
#include "Local.h"

namespace shared
{
   namespace currentTime
   {
      CProvider::CProvider(boost::shared_ptr<ICurrentTime> timeInstance)
         :m_currentTimeInstance(timeInstance)
      {
      }
      
      void CProvider::setProvider(boost::shared_ptr<ICurrentTime> timeInstance)
      {
         m_currentTimeInstance = timeInstance;
      }

      boost::posix_time::ptime CProvider::now()
      {
         if (!m_currentTimeInstance)
            throw std::runtime_error("Current time provider not set");

         return m_currentTimeInstance->now();
      }

      static CProvider StaticProvider(boost::make_shared<Local>());
      CProvider& Provider()
      {
         return StaticProvider;
      };
   }
} // namespace shared::currentTime


