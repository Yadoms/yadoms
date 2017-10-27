#pragma once
#include "IFactory.h"
#include <shared/process/IProcess.h>
#include <shared/process/ICommandLine.h>
#include <IPathProvider.h>
#include "IIpcAdapter.h"
#include "yInterpreterApiImplementation.h"
#include "InstanceStateHandler.h"
#include <shared/process/IExternalProcessLogger.h>

namespace automation
{
   namespace interpreter
   {
      //--------------------------------------------------------------
      /// \brief	IFactory implementation
      //--------------------------------------------------------------
      class CFactory : public IFactory
      {
      public:
         //--------------------------------------------------------------
         /// \brief	Constructor
         /// \param[in] pathProvider      The path provider
         //--------------------------------------------------------------
         explicit CFactory(boost::shared_ptr<const IPathProvider> pathProvider);

         //--------------------------------------------------------------
         /// \brief	Destructor
         //--------------------------------------------------------------
         virtual ~CFactory();

         // IFactory Implementation
         boost::shared_ptr<IInstance> createInterpreterInstance(const std::string& interpreterFileName,
                                                                boost::function2<void, bool, const std::string&> onInstanceStateChangedFct,
                                                                boost::function2<void, int, const std::string&> onScriptStoppedFct) const override;
         boost::filesystem::path interpreterLogFile(const std::string& interpreterFileName) const override;
         // [END] IFactory Implementation

      protected:
         boost::shared_ptr<const shared::script::yInterpreterApi::IInformation> createInterpreterInformation(const std::string& interpreterFileName) const;
         static boost::shared_ptr<shared::process::IExternalProcessLogger> createInterpreterLogger(const std::string& interpreterFileName);
         boost::shared_ptr<IIpcAdapter> createInterpreterRunningContext(boost::shared_ptr<const shared::script::yInterpreterApi::IInformation> interpreterInformation,
                                                                        boost::function2<void, int, const std::string&> onScriptStoppedFct) const;
         boost::shared_ptr<CYInterpreterApiImplementation> createInterpreterApiImplementation(boost::shared_ptr<const shared::script::yInterpreterApi::IInformation> interpreterInformation,
                                                                                              boost::function2<void, int, const std::string&> onScriptStoppedFct) const;
         boost::shared_ptr<shared::process::ICommandLine> createCommandLine(const boost::shared_ptr<const shared::script::yInterpreterApi::IInformation> interpreterInformation,
                                                                            const std::string& messageQueueId) const;
         boost::shared_ptr<CInstanceStateHandler> createInstanceStateHandler(boost::shared_ptr<const shared::script::yInterpreterApi::IInformation> interpreterInformation,
                                                                             boost::function2<void, bool, const std::string&> onInstanceStateChangedFct) const;
         boost::shared_ptr<shared::process::IProcess> createInstanceProcess(boost::shared_ptr<shared::process::ICommandLine> commandLine,
                                                                            boost::shared_ptr<shared::process::IExternalProcessLogger> logger,
                                                                            boost::shared_ptr<interpreter::CInstanceStateHandler> instanceStateHandler) const;
      private:
         boost::shared_ptr<const IPathProvider> m_pathProvider;
      };
   }
} // namespace automation::interpreter


