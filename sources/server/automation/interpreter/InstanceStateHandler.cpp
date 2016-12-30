#include "stdafx.h"
#include <shared/Log.h>
#include "InstanceStateHandler.h"

namespace automation
{
   namespace interpreter
   {
      CInstanceStateHandler::CInstanceStateHandler(boost::shared_ptr<const shared::script::yInterpreterApi::IInformation> interpreterInformation,
                                                   boost::function2<void, bool, const std::string&> onInstanceStateChangedFct)
         : m_interpreterInformation(interpreterInformation),
           m_onInstanceStateChangedFct(onInstanceStateChangedFct)
      {
      }

      CInstanceStateHandler::~CInstanceStateHandler()
      {
      }

      void CInstanceStateHandler::onStart()
      {
         YADOMS_LOG(information) << "Interpreter " << m_interpreterInformation->getName() << " is running";
         m_onInstanceStateChangedFct(true,
                                     m_interpreterInformation->getType());
      }

      void CInstanceStateHandler::onFinish(int returnCode,
                                           const std::string& error)
      {
         if (error.empty())
            YADOMS_LOG(information) << "Interpreter " << m_interpreterInformation->getName() << " is stopped";
         else
            YADOMS_LOG(information) << "Interpreter " << m_interpreterInformation->getName() << " is stopped with error (" << returnCode << ") : " << error;

         m_onInstanceStateChangedFct(false,
                                     m_interpreterInformation->getType());
      }
   }
} // namespace automation::interpreter


