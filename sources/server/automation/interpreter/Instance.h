#pragma once
#include <shared/process/IProcess.h>
#include "IInstance.h"
#include "IIpcAdapter.h"

namespace automation
{
   namespace interpreter
   {
      //--------------------------------------------------------------
      /// \brief	this class is used to manage a interpreter instance. 
      //--------------------------------------------------------------
      class CInstance : public IInstance
      {
      public:
         CInstance(const boost::shared_ptr<const shared::script::yInterpreterApi::IInformation> interpreterInformation,
                   boost::shared_ptr<shared::process::IProcess> process,
                   boost::shared_ptr<IIpcAdapter> ipcAdapter);

         //--------------------------------------------------------------
         /// \brief	Destructor
         //--------------------------------------------------------------
         virtual ~CInstance();

         // IInstance Implementation
         void requestStop() override;
         void kill() override;
         boost::shared_ptr<const shared::script::yInterpreterApi::IInformation> aboutInterpreter() const override;
         void postAvalaibleRequest(boost::shared_ptr<shared::script::yInterpreterApi::IAvalaibleRequest> request) override;
         void postLoadScriptContentRequest(boost::shared_ptr<shared::script::yInterpreterApi::ILoadScriptContentRequest> request) override;
         void postSaveScriptContentRequest(boost::shared_ptr<shared::script::yInterpreterApi::ISaveScriptContentRequest> request) override;
         void postStartScriptRequest(boost::shared_ptr<shared::script::yInterpreterApi::IStartScriptRequest> request) override;
         void postStopScriptRequest(boost::shared_ptr<shared::script::yInterpreterApi::IStopScriptRequest> request) override;
         // [END] IInstance Implementation

      protected:
         void postInit(boost::shared_ptr<const shared::script::yInterpreterApi::IInformation> information) const;
         void postStopRequest() const;

      private:
         const boost::shared_ptr<const shared::script::yInterpreterApi::IInformation> m_interpreterInformation;

         //-----------------------------------------------------
         ///\brief               The interpreter process
         //-----------------------------------------------------
         boost::shared_ptr<shared::process::IProcess> m_process;

         //-----------------------------------------------------
         ///\brief               The api context accessor
         //-----------------------------------------------------
         boost::shared_ptr<IIpcAdapter> m_ipcAdapter;
      };
   }
} // namespace automation::interpreter


