#pragma once
#include <shared/process/IProcessObserver.h>
#include <shared/script/yInterpreterApi/IInformation.h>

namespace automation
{
   namespace interpreter
   {
      //-----------------------------------------------------
      ///\brief The instance error handler
      //-----------------------------------------------------
      class CInstanceStateHandler : public shared::process::IProcessObserver
      {
      public:
         CInstanceStateHandler(boost::shared_ptr<const shared::script::yInterpreterApi::IInformation> interpreterInformation,
                               boost::function2<void, bool, const std::string&> onInstanceStateChangedFct);

         //-----------------------------------------------------
         ///\brief               Destructor
         //-----------------------------------------------------
         virtual ~CInstanceStateHandler();

      protected:
         // IProcessObserver Implementation
         void onStart() override;
         void onFinish(int returnCode,
                       const std::string& error) override;
         // [END] IProcessObserver Implementation

      private:
         boost::shared_ptr<const shared::script::yInterpreterApi::IInformation> m_interpreterInformation;
         boost::function2<void, bool, const std::string&> m_onInstanceStateChangedFct;
      };
   }
} // namespace automation::interpreter


