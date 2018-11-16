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
      void signalLoad(const boost::shared_ptr<const shared::plugin::information::IInformation> pluginInformation) override;
      void signalUnload(const boost::shared_ptr<const shared::plugin::information::IInformation> pluginInformation) override;
      void signalCrash(const boost::shared_ptr<const shared::plugin::information::IInformation> pluginInformation, const std::string& reason) override;
      bool isSafe(const boost::shared_ptr<const shared::plugin::information::IInformation> pluginInformation) override;
      int getQualityLevel(const boost::shared_ptr<const shared::plugin::information::IInformation> pluginInformation) override;
      // [END] IQualifier implementation
   };

} // namespace pluginSystem
