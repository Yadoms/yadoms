#pragma once
#include <shared/script/yInterpreterApi/IAvalaibleRequest.h>
#include <shared/script/yInterpreterApi/ILoadScriptContentRequest.h>
#include <shared/script/yInterpreterApi/ISaveScriptContentRequest.h>
#include <shared/script/yInterpreterApi/IStartScriptRequest.h>
#include <shared/script/yInterpreterApi/IStopScriptRequest.h>
#include <shared/script/yInterpreterApi/IInformation.h>


namespace automation
{
   namespace interpreter
   {
      //--------------------------------------------------------------
      /// \brief	yInterpreterApi IPC adapter interface, used by interpreter to dial with Yadoms
      //--------------------------------------------------------------
      class IIpcAdapter
      {
      public:
         virtual ~IIpcAdapter()
         {
         }

         virtual std::string id() const = 0;

         virtual void postStopRequest() = 0;
         virtual void postInit(boost::shared_ptr<const shared::script::yInterpreterApi::IInformation> information) = 0;

         virtual void postAvalaibleRequest(boost::shared_ptr<shared::script::yInterpreterApi::IAvalaibleRequest> request) = 0;
         virtual void postLoadScriptContentRequest(boost::shared_ptr<shared::script::yInterpreterApi::ILoadScriptContentRequest> request) = 0;
         virtual void postSaveScriptContentRequest(boost::shared_ptr<shared::script::yInterpreterApi::ISaveScriptContentRequest> request) = 0;
         virtual void postStartScriptRequest(boost::shared_ptr<shared::script::yInterpreterApi::IStartScriptRequest> request) = 0;
         virtual void postStopScriptRequest(boost::shared_ptr<shared::script::yInterpreterApi::IStopScriptRequest> request) = 0;
      };
   }
} // namespace automation::interpreter


