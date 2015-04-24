//
// DummyQualifier.h
//
// Yadoms-plugin dummy-qualifier (do nothing)
//
#pragma once

#include "IQualifier.h"
#include "database/IPluginEventLoggerRequester.h"
#include "IdentityForQualifier.h"

namespace pluginSystem
{

   //--------------------------------------------------------------
   /// \brief	this class simulate plugin qualifier, but do nothing
   //--------------------------------------------------------------
   class CDummyQualifier : public IQualifier
   {
   public:
      //--------------------------------------------------------------
      /// \brief	Constructor
      //--------------------------------------------------------------
      CDummyQualifier();

      //--------------------------------------------------------------
      /// \brief	Destructor
      //--------------------------------------------------------------
      virtual ~CDummyQualifier();

      // IQualifier implementation
      virtual void signalLoad(const boost::shared_ptr<const shared::plugin::information::IInformation> pluginInformation);
      virtual void signalUnload(const boost::shared_ptr<const shared::plugin::information::IInformation> pluginInformation);
      virtual void signalCrash(const boost::shared_ptr<const shared::plugin::information::IInformation> pluginInformation, const std::string& reason);
      virtual bool isSafe(const boost::shared_ptr<const shared::plugin::information::IInformation> pluginInformation);
      virtual int getQualityLevel(const boost::shared_ptr<const shared::plugin::information::IInformation> pluginInformation);
      // [END] IQualifier implementation
   };

} // namespace pluginSystem
