#pragma once
#include <shared/process/IProcessObserver.h>


//-----------------------------------------------------
///\brief The process observer
//-----------------------------------------------------
class CProcessObserver : public shared::process::IProcessObserver
{
public:
   CProcessObserver(int scriptInstanceId,
                    boost::function3<void, bool, int, const std::string&> onInstanceStateChangedFct);

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
   boost::function3<void, bool, int, const std::string&> m_onInstanceStateChangedFct;
};

