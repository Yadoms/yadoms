#pragma once

#include <interpreter_IPC/interpreterInit.pb.h>


namespace automation
{
   namespace interpreter
   {
      namespace serializers
      {
         //--------------------------------------------------------------
         /// \brief		Serializer of IInformation
         //--------------------------------------------------------------
         class IInformation
         {
         public:
            virtual ~IInformation()
            {
            }

            virtual void toPb(interpreter_IPC::toInterpreter::Information* pb) const = 0;
         };
      }
   }
} // namespace automation::interpreter::serializers


