#pragma once


namespace automation
{
   namespace script
   {
      //--------------------------------------------------------------
      /// \brief	yScriptApi IPC adapter interface, used by scripts to dial with Yadoms
      //--------------------------------------------------------------
      class IIpcAdapter
      {
      public:
         virtual ~IIpcAdapter()
         {
         }

         virtual std::string id() const = 0;
      };
   }
} // namespace automation::script


