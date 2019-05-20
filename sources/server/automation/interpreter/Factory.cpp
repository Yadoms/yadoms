#include "stdafx.h"
#include "Factory.h"
#include "Instance.h"
#include "Information.h"
#include <shared/Executable.h>
#include <shared/process/Process.h>
#include <shared/process/NativeExecutableCommandLine.h>
#include "yInterpreterApiImplementation.h"
#include "IpcAdapter.h"
#include "InstanceStateHandler.h"
#include "logging/YadomsSubModuleProcessLogger.h"


namespace automation
{
   namespace interpreter
   {
      CFactory::CFactory(boost::shared_ptr<const IPathProvider> pathProvider)
         : m_pathProvider(pathProvider)
      {
      }

      CFactory::~CFactory()
      {
      }

      boost::shared_ptr<IInstance> CFactory::createInterpreterInstance(const std::string& interpreterFileName,
                                                                       boost::function2<void, bool, const std::string&> onInstanceStateChangedFct,
                                                                       boost::function2<void, int, const std::string&> onScriptStoppedFct) const
      {
         auto interpreterInformation = createInterpreterInformation(interpreterFileName);

         const auto logger = createInterpreterLogger(interpreterFileName);

         auto yInterpreterIpcAdapter = createInterpreterRunningContext(interpreterInformation,
                                                                       onScriptStoppedFct);

         const auto commandLine = createCommandLine(interpreterInformation,
                                                    yInterpreterIpcAdapter->id());

         const auto instanceStateHandler = createInstanceStateHandler(interpreterInformation,
                                                                      onInstanceStateChangedFct);

         auto process = createInstanceProcess(commandLine,
                                              logger,
                                              instanceStateHandler);

         return boost::make_shared<CInstance>(interpreterInformation,
                                              interpreterLogFile(interpreterFileName),
                                              process,
                                              yInterpreterIpcAdapter);
      }

      boost::filesystem::path CFactory::interpreterLogFile(const std::string& interpreterFileName) const
      {
         return m_pathProvider->scriptInterpretersLogPath() / interpreterFileName / "interpreter.log";
      }

      boost::shared_ptr<const shared::script::yInterpreterApi::IInformation> CFactory::createInterpreterInformation(
         const std::string& interpreterFileName) const
      {
         return boost::make_shared<CInformation>(m_pathProvider->scriptInterpretersPath() / interpreterFileName);
      }

      boost::shared_ptr<shared::process::IExternalProcessLogger> CFactory::createInterpreterLogger(const std::string& interpreterFileName)
      {
         return boost::make_shared<logging::CYadomsSubModuleProcessLogger>("interpreter." + interpreterFileName);
      }

      boost::shared_ptr<IIpcAdapter> CFactory::createInterpreterRunningContext(
         boost::shared_ptr<const shared::script::yInterpreterApi::IInformation> interpreterInformation,
         boost::function2<void, int, const std::string&> onScriptStoppedFct) const
      {
         auto apiImplementation = createInterpreterApiImplementation(interpreterInformation,
                                                                     onScriptStoppedFct);

         return boost::make_shared<CIpcAdapter>(interpreterInformation->getName(),
                                                apiImplementation);
      }

      boost::shared_ptr<CYInterpreterApiImplementation> CFactory::createInterpreterApiImplementation(
         boost::shared_ptr<const shared::script::yInterpreterApi::IInformation> interpreterInformation,
         boost::function2<void, int, const std::string&> onScriptStoppedFct) const
      {
         return boost::make_shared<CYInterpreterApiImplementation>(interpreterInformation,
                                                                   onScriptStoppedFct);
      }

      boost::shared_ptr<shared::process::ICommandLine> CFactory::createCommandLine(
         const boost::shared_ptr<const shared::script::yInterpreterApi::IInformation> interpreterInformation,
         const std::string& messageQueueId) const
      {
         std::vector<std::string> args;
         args.push_back(messageQueueId);

         return boost::make_shared<shared::process::CNativeExecutableCommandLine>(
            interpreterInformation->getPath() / shared::CExecutable::ToFileName(interpreterInformation->getType()),
            ".",
            args);
      }

      boost::shared_ptr<CInstanceStateHandler> CFactory::createInstanceStateHandler(
         boost::shared_ptr<const shared::script::yInterpreterApi::IInformation> interpreterInformation,
         boost::function2<void, bool, const std::string&> onInstanceStateChangedFct) const
      {
         return boost::make_shared<CInstanceStateHandler>(interpreterInformation,
                                                          onInstanceStateChangedFct);
      }

      boost::shared_ptr<shared::process::IProcess> CFactory::createInstanceProcess(boost::shared_ptr<shared::process::ICommandLine> commandLine,
                                                                                   boost::shared_ptr<shared::process::IExternalProcessLogger> logger,
                                                                                   boost::shared_ptr<CInstanceStateHandler> instanceStateHandler)
      const
      {
         return boost::make_shared<shared::process::CProcess>(commandLine,
                                                              instanceStateHandler,
                                                              logger);
      }
   }
} // namespace automation::interpreter
