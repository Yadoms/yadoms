#include "stdafx.h"
#include "yInterpreterApiImplementation.h"
#include <shared/Log.h>

namespace automation
{
   namespace interpreter
   {
      CYInterpreterApiImplementation::CYInterpreterApiImplementation(boost::shared_ptr<IRuleLogDispatcher> ruleLogDispatcher,
                                                                     boost::shared_ptr<const shared::script::yInterpreterApi::IInformation> interpreterInformations,
                                                                     boost::function2<void, int, const std::string&> onScriptStoppedFct)
         : m_ruleLogDispatcher(ruleLogDispatcher),
           m_informations(interpreterInformations),
           m_onScriptStoppedFct(onScriptStoppedFct)
      {
      }

      CYInterpreterApiImplementation::~CYInterpreterApiImplementation()
      {
      }

      void CYInterpreterApiImplementation::notifyScriptStopped(int scriptInstanceId,
                                                               const std::string error)
      {
         YADOMS_LOG(information) << "Rule #" << scriptInstanceId << " (interpreter " << m_informations->getName() << ") stopped";
         m_onScriptStoppedFct(scriptInstanceId,
                              error);
      }

      void CYInterpreterApiImplementation::onScriptLog(int scriptInstanceId,
                                                       bool error,
                                                       const std::string& logLine)
      {
         const auto& loggerName = shared::CLog::getCurrentThreadName() + ".Rule." + std::to_string(scriptInstanceId);
         auto& logger = Poco::Logger::get(loggerName);
         if (error)
            logger.error(logLine);
         else
            logger.information(logLine);

            //TODO virer avec dépendances
         //m_ruleLogDispatcher->log(scriptInstanceId,
         //                         error,
         //                         logLine);
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


