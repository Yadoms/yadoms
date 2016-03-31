#pragma once
#include "IInstanceStateHandler.h"
#include <shared/process/IRunner.h>
#include <shared/process/ILogger.h>
#include <shared/plugin/information/IInformation.h>

namespace pluginSystem
{
   //-----------------------------------------------------
   ///\brief A plugin instance
   //-----------------------------------------------------
   class ILoader
   {
   public:
      //-----------------------------------------------------
      ///\brief               Destructor
      //-----------------------------------------------------
      virtual ~ILoader() {}

      //--------------------------------------------------------------
      /// \brief			            Create the appropiated runner depending on plugin type (executable, python, etc...)
      /// \param  pluginInformation General plugin information
      /// \param  logger            Logger
      /// \param  apiContextId      Api context unique ID
      /// \param  stopNotifier      Stop notifier object
      /// \return created runner
      /// \throw CPluginException if fail to create runner
      //--------------------------------------------------------------
      virtual boost::shared_ptr<shared::process::IRunner> createRunner(const boost::shared_ptr<const shared::plugin::information::IInformation> pluginInformation,
                                                                       boost::shared_ptr<shared::process::ILogger> logger,
                                                                       const std::string& apiContextId,
                                                                       boost::shared_ptr<IInstanceStateHandler> stopNotifier) const = 0;
   };
	
} // namespace pluginSystem	
	
	