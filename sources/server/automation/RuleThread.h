#pragma once
#include <shared/shared/ThreadBase.h>
#include <server/automation/IRuleThreadHandler.h>

namespace automation
{
   //-----------------------------------------------------
   ///\brief A rule thread
   //-----------------------------------------------------
   class CRuleThread : public shared::CThreadBase
   {
   public:
      //-----------------------------------------------------
      ///\brief               Constructor
      /// \param [in] ruleName The rule name, used to name the thread
      //-----------------------------------------------------
      CRuleThread(const std::string & ruleName, IRuleThreadHandler& handler);

      //-----------------------------------------------------
      ///\brief               Destructor
      //-----------------------------------------------------
      virtual ~CRuleThread();

      // CThreadBase implementation
      virtual void doWork();
      // [END] CThreadBase implementation

   private:
      IRuleThreadHandler& m_handler;
   };
	
} // namespace automation	
	
	