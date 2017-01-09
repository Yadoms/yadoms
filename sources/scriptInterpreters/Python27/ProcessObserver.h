#pragma once
#include <shared/process/IProcessObserver.h>


//-----------------------------------------------------
///\brief The process observer
//-----------------------------------------------------
class CProcessObserver : public shared::process::IProcessObserver
{
public:
   CProcessObserver(int scriptInstanceId,
                    boost::function2<void, bool, int> onInstanceStateChangedFct);

   //-----------------------------------------------------
   ///\brief               Destructor
   //-----------------------------------------------------
   virtual ~CProcessObserver();

protected:
   // IProcessObserver Implementation
   void onStart() override;
   void onFinish(int returnCode,
                 const std::string& error) override;
   // [END] IProcessObserver Implementation

private:
   int m_scriptInstanceId;
   boost::function2<void, bool, int> m_onInstanceStateChangedFct;
};

