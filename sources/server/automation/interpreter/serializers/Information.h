#pragma once
#include "IInformation.h"
#include <shared/script/yInterpreterApi/IInformation.h>

namespace automation
{
   namespace interpreter
   {
      namespace serializers
      {
         class CInformation :public IInformation
         {
         public:
            explicit CInformation(boost::shared_ptr<const shared::script::yInterpreterApi::IInformation> information);

            virtual ~CInformation();

            // IInformationSerializer implementation
            void toPb(interpreter_IPC::toInterpreter::Information* pb) const override;
            // [END] IInformationSerializer implementation

         private:
            boost::shared_ptr<const shared::script::yInterpreterApi::IInformation> m_information;
         };
      }
   }
} // namespace automation::interpreter::serializers


