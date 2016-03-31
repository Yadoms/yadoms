//
// Instance.h
//
// yadoms-plugin instance handler
//
#pragma once

#include "ILoader.h"

namespace pluginSystem
{

   //--------------------------------------------------------------
   /// \brief	this class is used to load a native executable plugin (compiled plugin)
   //--------------------------------------------------------------
   class CNativeExecutableLoader : public ILoader
   {
   public:
      //--------------------------------------------------------------
      /// \brief	Constructor
      //--------------------------------------------------------------
      CNativeExecutableLoader();

      //--------------------------------------------------------------
      /// \brief	Destructor
      //--------------------------------------------------------------
      virtual ~CNativeExecutableLoader();


      // ILoader Implementation
      boost::shared_ptr<shared::process::IRunner> createRunner(const boost::shared_ptr<const shared::plugin::information::IInformation> pluginInformation,
                                                               boost::shared_ptr<shared::process::ILogger> logger,
                                                               const std::string& apiContextId,
                                                               boost::shared_ptr<IInstanceStateHandler> stopNotifier) const override;
      // [END] ILoader Implementation

} // namespace pluginSystem
