#include "stdafx.h"
#include "yInterpreterApiImplementation.h"
#include <shared/Log.h>

namespace automation
{
   namespace interpreter
   {
      CYInterpreterApiImplementation::CYInterpreterApiImplementation(boost::shared_ptr<const shared::script::yInterpreterApi::IInformation> interpreterInformations,
                                                                     boost::function2<void, int, const std::string&> onScriptStoppedFct)
         : m_informations(interpreterInformations),
           m_onScriptStoppedFct(onScriptStoppedFct)
      {
      }

      CYInterpreterApiImplementation::~CYInterpreterApiImplementation()
      {
      }

      void CYInterpreterApiImplementation::notifyScriptStopped(int scriptInstanceId,
                                                               const std::string& error)
      {
         YADOMS_LOG(information) << "Rule #" << scriptInstanceId << " (interpreter " << m_informations->getName() << ") stopped";
         m_onScriptStoppedFct(scriptInstanceId,
                              error);
      }

      boost::shared_ptr<const shared::script::yInterpreterApi::IInformation> CYInterpreterApiImplementation::getInformation() const
      {
         return m_informations;
      }

      shared::event::CEventHandler& CYInterpreterApiImplementation::getEventHandler()
      {
         BOOST_ASSERT(false); // No event handler required here
         static shared::event::CEventHandler evtHandler;
         return evtHandler;
      }
   }
} // namespace automation::interpreter


