#include "stdafx.h"
#include "Factory.h"
#include "interpreter/Instance.h"
#include "interpreter/Information.h"
#include <shared/process/Logger.h>
#include <shared/Executable.h>
#include <shared/process/Process.h>
#include "interpreter/yInterpreterApiImplementation.h"
#include "interpreter/IpcAdapter.h"
#include <shared/process/NativeExecutableCommandLine.h>
#include "interpreter/InstanceStateHandler.h"


namespace automation
{
   CFactory::CFactory(const IPathProvider& pathProvider)
      : m_pathProvider(pathProvider)
   {
   }

   CFactory::~CFactory()
   {
   }

   boost::shared_ptr<interpreter::IInstance> CFactory::createInterpreterInstance(const std::string& interpreterFileName,
                                                                                 boost::function2<void, bool, const std::string&> onInstanceStateChangedFct) const
   {
      auto interpreterInformation = createInterpreterInformation(interpreterFileName);

      auto logger = createProcessLogger(interpreterFileName);

      auto yInterpreterIpcAdapter = createInterpreterRunningContext(interpreterInformation);

      auto commandLine = createCommandLine(interpreterInformation,
                                           yInterpreterIpcAdapter->id());

      auto instanceStateHandler = createInstanceStateHandler(interpreterInformation,
                                                             onInstanceStateChangedFct);

      auto process = createInstanceProcess(commandLine,
                                           logger,
                                           instanceStateHandler);

      return boost::make_shared<interpreter::CInstance>(interpreterInformation,
                                                        process,
                                                        yInterpreterIpcAdapter);
   }

   boost::filesystem::path CFactory::interpreterLogFile(const std::string& interpreterFileName) const
   {
      return m_pathProvider.scriptInterpretersPath() / (interpreterFileName + ".log");
   }

   boost::shared_ptr<const shared::script::yInterpreterApi::IInformation> CFactory::createInterpreterInformation(const std::string& interpreterFileName) const
   {
      return boost::make_shared<interpreter::CInformation>(m_pathProvider.scriptInterpretersPath() / interpreterFileName);
   }

   boost::shared_ptr<shared::process::ILogger> CFactory::createProcessLogger(const std::string& interpreterFileName) const
   {
      return boost::make_shared<shared::process::CLogger>("interpreter/" + interpreterFileName,
                                                          interpreterLogFile(interpreterFileName));
   }

   boost::shared_ptr<interpreter::IIpcAdapter> CFactory::createInterpreterRunningContext(boost::shared_ptr<const shared::script::yInterpreterApi::IInformation> interpreterInformation) const
   {
      auto apiImplementation = createInterpreterApiImplementation(interpreterInformation);

      return boost::make_shared<interpreter::CIpcAdapter>(interpreterInformation->getName());
   }

   boost::shared_ptr<interpreter::CYInterpreterApiImplementation> CFactory::createInterpreterApiImplementation(boost::shared_ptr<const shared::script::yInterpreterApi::IInformation> interpreterInformation) const
   {
      return boost::make_shared<interpreter::CYInterpreterApiImplementation>(interpreterInformation);
   }

   boost::shared_ptr<shared::process::ICommandLine> CFactory::createCommandLine(const boost::shared_ptr<const shared::script::yInterpreterApi::IInformation> interpreterInformation,
                                                                                const std::string& messageQueueId) const
   {
      std::vector<std::string> args;
      args.push_back(messageQueueId);

      return boost::make_shared<shared::process::CNativeExecutableCommandLine>(interpreterInformation->getPath() / shared::CExecutable::ToFileName(interpreterInformation->getType()),
                                                                               ".",
                                                                               args);
   }

   boost::shared_ptr<interpreter::CInstanceStateHandler> CFactory::createInstanceStateHandler(boost::shared_ptr<const shared::script::yInterpreterApi::IInformation> interpreterInformation,
                                                                                              boost::function2<void, bool, const std::string&> onInstanceStateChangedFct) const
   {
      return boost::make_shared<interpreter::CInstanceStateHandler>(interpreterInformation,
                                                                    onInstanceStateChangedFct);
   }

   boost::shared_ptr<shared::process::IProcess> CFactory::createInstanceProcess(boost::shared_ptr<shared::process::ICommandLine> commandLine,
                                                                                boost::shared_ptr<shared::process::ILogger> logger,
                                                                                boost::shared_ptr<interpreter::CInstanceStateHandler> instanceStateHandler) const
   {
      return boost::make_shared<shared::process::CProcess>(commandLine,
                                                           instanceStateHandler,
                                                           logger);
   }
} // namespace automation


