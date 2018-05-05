#pragma once

#include "XplActor.h"
#include "XplMessage.h"
#include <shared/event/EventHandler.hpp>
#include <Poco/TaskManager.h>
#include <Poco/ThreadPool.h>
#include <Poco/Net/NetworkInterface.h>
#include "XplServiceTask.h"

namespace xplcore
{
   class CXplService
   {
      // Non-copyable
      CXplService(CXplService&) = delete;
      CXplService& operator=(const CXplService&) = delete;

   public:

      //--------------------------------------------------------------
      /// \brief			Ctor using default network interface
      /// \param [in]   networkInterface        The network interface to use
      /// \param [in]   deviceId                The device Id delivered by Xpl Project
      /// \param [in]   instanceId              The instance Id
      /// \param [in]   externalIOService       a pointer to an external ioservice. if NULL a new ioservice is created.
      /// \note         Using this constructor implies that vendor Id will be CXplConstants::getYadomsVendorId
      //--------------------------------------------------------------
      CXplService(Poco::Net::NetworkInterface& networkInterface,
                  const std::string& deviceId,
                  const std::string& instanceId,
                  shared::event::CEventHandler* pHubFoundEventHandler = nullptr,
                  int hubFoundEventCode = 0);

      //--------------------------------------------------------------
      /// \brief			Dtor
      //--------------------------------------------------------------
      virtual ~CXplService();

      //--------------------------------------------------------------
      /// \brief			Stop the service
      //--------------------------------------------------------------
      void stop();

      //--------------------------------------------------------------
      /// \brief			Send the message to the remoteEndpoint
      /// \param [in]   message : The message to send
      //--------------------------------------------------------------
      void sendMessage(const CXplMessage& message) const;


      //--------------------------------------------------------------
      /// \brief		 Get the actor defined in the service
      /// \return		The current actor
      //--------------------------------------------------------------
      CXplActor getActor() const;


      //--------------------------------------------------------------
      /// \brief		   Subscribe to receive all messages
      /// \param [in]   pEventHandler        The event handler to notify when a message is received
      /// \param [in]   eventTypeIdentifier  The event id to use in notification
      /// \param [in]   continueChainingNotification  indicates if the notification queue should continue when a message match this filter
      //--------------------------------------------------------------
      void subscribeForAllMessages(shared::event::CEventHandler* pEventHandler,
                                   int eventTypeIdentifier,
                                   bool continueChainingNotification = true) const;

      //--------------------------------------------------------------
      /// \brief		   Subscribe to receive all messages which are targeted to this XplService instance 
      /// \param [in]   pEventHandler                 The event handler to notify when a message is received
      /// \param [in]   eventTypeIdentifier           The event id to use in notification
      /// \param [in]   continueChainingNotification  indicates if the notification queue should continue when a message match this filter
      //--------------------------------------------------------------
      void subscribeForAllMyMessages(shared::event::CEventHandler* pEventHandler,
                                     int eventTypeIdentifier,
                                     bool continueChainingNotification = true) const;

      //--------------------------------------------------------------
      /// \brief			Subscribe to receive all messages which match the filter defined by parameters
      /// \param [in]   msgtype     Type of message to receive (* allowed)
      /// \param [in]   vendor      Vendor from which to receive messages (* allowed)
      /// \param [in]   device      Device from which to receive messages (* allowed)
      /// \param [in]   instance    Instance from which to receive messages (* allowed)
      /// \param [in]   classId     Class Id from which to receive messages (* allowed)
      /// \param [in]   typeId      Type of message to receive  (* allowed)
      /// \param [in]   pEventHandler        The event handler to notify when a message is received
      /// \param [in]   eventTypeIdentifier  The event id to use in notification
      /// \param [in]   continueChainingNotification  indicates if the notification queue should continue when a message match this filter
      //--------------------------------------------------------------
      void subscribeForMessages(const std::string& msgtype,
                                const std::string& vendor,
                                const std::string& device,
                                const std::string& instance,
                                const std::string& classId,
                                const std::string& typeId,
                                shared::event::CEventHandler* pEventHandler,
                                int eventTypeIdentifier,
                                bool continueChainingNotification = true) const;

      //--------------------------------------------------------------
      /// \brief			Subscribe to receive all messages which match the filter defined by parameters
      /// \param [in]   filter to apply. It must be formed like : [msgtype].[vendor].[device].[instance].[class].[type] (* is allowed for each param)
      /// \param [in]   pEventHandler        The event handler to notify when a message is received
      /// \param [in]   eventTypeIdentifier  The event id to use in notification
      /// \param [in]   continueChainingNotification  indicates if the notification queue should continue when a message match this filter
      //--------------------------------------------------------------
      void subscribeForMessages(const std::string& filter,
                                shared::event::CEventHandler* pEventHandler,
                                int eventTypeIdentifier,
                                bool continueChainingNotification = true) const;


      //--------------------------------------------------------------
      /// \brief			Clear all subscription
      //--------------------------------------------------------------
      void clearAllSubscriptions() const;

   private:
      //--------------------------------------------------------------
      /// \brief			XplService thread pool (required for task manager)
      //--------------------------------------------------------------
      Poco::ThreadPool m_localThreadPool;

      //--------------------------------------------------------------
      /// \brief			XplService task manager
      //--------------------------------------------------------------
      Poco::TaskManager m_taskManager;

      //--------------------------------------------------------------
      /// \brief			XplService task pointer
      ///               use pointer because taskManager take the ownership and destroy it at end
      //--------------------------------------------------------------
      CXplServiceTask* m_xplTask;
   };
} // namespace xplcore


