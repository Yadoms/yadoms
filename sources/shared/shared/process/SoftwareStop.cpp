#include "stdafx.h"
#include "SoftwareStop.h"
#include <shared/Log.h>

namespace shared
{
   namespace process
   {
      boost::function<bool()> CSoftwareStop::m_onStopRequestedFct;

      void CSoftwareStop::setOnStopRequestedHandler(boost::function<bool()> onStopRequestedFct)
      {
         m_onStopRequestedFct = onStopRequestedFct;
      }

      void CSoftwareStop::stop()
      {
         if (m_onStopRequestedFct.empty())
            return;

         // Signal stop request and wait for application fully stops
         if (!m_onStopRequestedFct())
            YADOMS_LOG(error) << "CSoftwareStop : Fail to wait the app end event";
      }
   }
} // namespace shared::process


