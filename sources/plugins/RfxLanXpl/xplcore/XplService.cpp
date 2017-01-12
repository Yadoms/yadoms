#include "stdafx.h"
#include "XplService.h"
#include "XplMessage.h"
#include "XplActor.h"
#include "XplConstants.h"
#include "XplServiceTask.h"
#include <shared/Log.h>


// A client send its data as broadcast on the XPL port,
// and listen on a certain port, given in its XPL frame.

// A hub is listening the XPL port, repeat all received data to the connected clients on their respective ports.

namespace xplcore
{
   CXplService::CXplService(Poco::Net::NetworkInterface& networkInterface,
                            const std::string& deviceId,
                            const std::string& instanceId,
                            shared::event::CEventHandler* pHubFoundEventHandler,
                            int hubFoundEventCode)
      : m_localThreadPool("XplService ThreaPool"),
        m_taskManager(m_localThreadPool) //make sure task manager use its own threadpool, otherwise it could not be stopped properly
   {
      m_xplTask = new CXplServiceTask(networkInterface,
                                      CXplConstants::getYadomsVendorId(),
                                      deviceId,
                                      instanceId,
                                      pHubFoundEventHandler,
                                      hubFoundEventCode);

      //task manager take the ownership
      m_taskManager.start(m_xplTask);
   }

   CXplService::~CXplService()
   {
      stop();
   }


   void CXplService::sendMessage(const CXplMessage& message) const
   {
      m_xplTask->sendMessage(message);
   }

   void CXplService::stop()
   {
      YADOMS_LOG(information) << "Ask for serviceTask to end." ;
      //cancel xpl task
      if (m_xplTask)
         m_xplTask->cancel();

      //cancel all other ones
      m_taskManager.cancelAll();

      YADOMS_LOG(information) << "Wait for all tasks to end." ;
      m_taskManager.joinAll();
      YADOMS_LOG(information) << "CXplService stopped." ;
      //do not delete m_xplTasks, before taskmanager has already done it
   }

   CXplActor CXplService::getActor() const
   {
      return m_xplTask->getActor();
   }


   void CXplService::subscribeForAllMessages(shared::event::CEventHandler* pEventHandler,
                                             int eventTypeIdentifier,
                                             bool continueChainingNotification /*= true*/) const
   {
      m_xplTask->subscribeForAllMessages(pEventHandler,
                                         eventTypeIdentifier
                                         , continueChainingNotification);
   }

   void CXplService::subscribeForAllMyMessages(shared::event::CEventHandler* pEventHandler,
                                               int eventTypeIdentifier
                                               , bool continueChainingNotification /*= true*/) const
   {
      m_xplTask->subscribeForAllMyMessages(pEventHandler,
                                           eventTypeIdentifier,
                                           continueChainingNotification);
   }

   void CXplService::subscribeForMessages(const std::string& msgtype,
                                          const std::string& vendor,
                                          const std::string& device,
                                          const std::string& instance,
                                          const std::string& classId,
                                          const std::string& typeId,
                                          shared::event::CEventHandler* pEventHandler,
                                          int eventTypeIdentifier,
                                          bool continueChainingNotification /*= true*/) const
   {
      m_xplTask->subscribeForMessages(msgtype,
                                      vendor,
                                      device,
                                      instance,
                                      classId,
                                      typeId,
                                      pEventHandler,
                                      eventTypeIdentifier,
                                      continueChainingNotification);
   }

   void CXplService::subscribeForMessages(const std::string& filter,
                                          shared::event::CEventHandler* pEventHandler,
                                          int eventTypeIdentifier,
                                          bool continueChainingNotification /*= true*/) const
   {
      m_xplTask->subscribeForMessages(filter,
                                      pEventHandler,
                                      eventTypeIdentifier,
                                      continueChainingNotification);
   }

   void CXplService::clearAllSubscriptions() const
   {
      m_xplTask->clearAllSubscriptions();
   }
} // namespace xplcore


