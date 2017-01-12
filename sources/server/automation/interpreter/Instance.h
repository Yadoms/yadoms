#pragma once
#include <shared/process/IProcess.h>
#include "IInstance.h"
#include "IIpcAdapter.h"
#include "IRuleLogDispatcher.h"

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
                   boost::shared_ptr<IIpcAdapter> ipcAdapter,
                   boost::shared_ptr<IRuleLogDispatcher> ruleLogDispatcher);

         //--------------------------------------------------------------
         /// \brief	Destructor
         //--------------------------------------------------------------
         virtual ~CInstance();

         // IInstance Implementation
         boost::shared_ptr<const shared::script::yInterpreterApi::IInformation> aboutInterpreter() const override;
         void requestToStop() override;
         bool isAvalaible() override;
         std::string loadScriptContent(const std::string& scriptPath) const override;
         void saveScriptContent(const std::string& scriptPath,
                                const std::string& scriptContent) const override;
         void startScript(int scriptInstanceId,
                          const std::string& scriptPath,
                          const std::string& yScriptApiId) const override;
         void stopScript(int scriptInstanceId) const override;
         boost::shared_ptr<IRuleLogDispatcher> getRuleLogDispatcher() const override;
         // [END] IInstance Implementation

      protected:
         bool getAvalaibility() const;

      private:
         const boost::shared_ptr<const shared::script::yInterpreterApi::IInformation> m_interpreterInformation;

         boost::shared_ptr<shared::process::IProcess> m_process;
         boost::shared_ptr<IIpcAdapter> m_ipcAdapter;
         boost::shared_ptr<IRuleLogDispatcher> m_ruleLogDispatcher;

         bool m_avalaible;
      };
   }
} // namespace automation::interpreter
