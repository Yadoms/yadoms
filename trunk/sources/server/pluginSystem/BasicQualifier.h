//
// Qualifier.h
//
// Yadoms-plugin qualifier
//
#pragma once

#include "IQualifier.h"
#include "database/IPluginEventLoggerRequester.h"
#include "../dataAccessLayer/IEventLogger.h"
#include "IdentityForQualifier.h"

namespace pluginSystem
{

   //--------------------------------------------------------------
   /// \brief	This basic qualifier only record plugin events in database
   //--------------------------------------------------------------
   class CBasicQualifier : public IQualifier
   {
   public:
      //--------------------------------------------------------------
      /// \brief	Constructor
      /// \param[in] pluginLogger   Plugin logger database requester
      /// \param[in] mainLogger     Main logger
      //--------------------------------------------------------------
      CBasicQualifier(boost::shared_ptr<database::IPluginEventLoggerRequester> pluginLogger,
         boost::shared_ptr<dataAccessLayer::IEventLogger> mainLogger);

      //--------------------------------------------------------------
      /// \brief	Destructor
      //--------------------------------------------------------------
      virtual ~CBasicQualifier();

      // IQualifier implementation
      virtual void signalLoad(const boost::shared_ptr<const shared::plugin::information::IInformation> pluginInformation);
      virtual void signalUnload(const boost::shared_ptr<const shared::plugin::information::IInformation> pluginInformation);
      virtual void signalCrash(const boost::shared_ptr<const shared::plugin::information::IInformation> pluginInformation, const std::string& reason);
      virtual bool isSafe(const boost::shared_ptr<const shared::plugin::information::IInformation> pluginInformation);
      virtual int getQualityLevel(const boost::shared_ptr<const shared::plugin::information::IInformation> pluginInformation);
      // [END] IQualifier implementation

   private:
      //--------------------------------------------------------------
      /// \brief	               Add an event to database
      /// \param[in]  pluginInformation     Plugin information (name, version...)
      /// \param[in]  eventType  Event type (load, unload, crash...)
      /// \param[in]  reason     Crash cause (exception...)
      //--------------------------------------------------------------
      void AddEventToDatabase(const boost::shared_ptr<const shared::plugin::information::IInformation> pluginInformation,
         database::entities::EEventType eventType, const std::string& reason = shared::CStringExtension::EmptyString);

      //--------------------------------------------------------------
      /// \brief	Plugin logger access
      //--------------------------------------------------------------
      boost::shared_ptr<database::IPluginEventLoggerRequester> m_pluginLogger;

      //--------------------------------------------------------------
      /// \brief	Main logger access
      //--------------------------------------------------------------
      boost::shared_ptr<dataAccessLayer::IEventLogger> m_mainLogger;
   };

} // namespace pluginSystem
