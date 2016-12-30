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
         explicit CYInterpreterApiImplementation(boost::shared_ptr<const shared::script::yInterpreterApi::IInformation> interpreterInformations);
         virtual ~CYInterpreterApiImplementation();

         // IYInterpreterApi implementation
         boost::shared_ptr<const shared::script::yInterpreterApi::IInformation> getInformation() const override;
         shared::event::CEventHandler& getEventHandler() override;
         // [END] IYInterpreterApi implementation

      private:
         boost::shared_ptr<const shared::script::yInterpreterApi::IInformation> m_informations;
      };
   }
} // namespace automation::interpreter	


