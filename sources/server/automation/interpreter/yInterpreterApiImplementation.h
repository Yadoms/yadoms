#pragma once
#include <shared/script/yInterpreterApi/IYInterpreterApi.h>

namespace automation
{
   namespace interpreter
   {
      //-----------------------------------------------------
      ///\brief The IYPluginApi implementation
      //-----------------------------------------------------
      class CYInterpreterApiImplementation : public shared::script::yInterpreterApi::IYInterpreterApi
      {
      public:
         CYInterpreterApiImplementation(boost::shared_ptr<const shared::script::yInterpreterApi::IInformation> interpreterInformations,
                                        boost::function2<void, int, const std::string&> onScriptStoppedFct);
         virtual ~CYInterpreterApiImplementation();

         // IYInterpreterApi implementation
         void notifyScriptStopped(int scriptInstanceId,
                                  const std::string error) override;
         boost::shared_ptr<const shared::script::yInterpreterApi::IInformation> getInformation() const override;
         shared::event::CEventHandler& getEventHandler() override;
         // [END] IYInterpreterApi implementation

      private:
         boost::shared_ptr<const shared::script::yInterpreterApi::IInformation> m_informations;
         boost::function2<void, int, const std::string&> m_onScriptStoppedFct;
      };
   }
} // namespace automation::interpreter	


