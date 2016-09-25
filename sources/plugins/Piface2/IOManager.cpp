#include "stdafx.h"
#include "IOManager.h"
#include "InitializationException.hpp"
#include "pifacedigital.h"
#include <mcp23s17.h>
#include "eventDefinitions.h"

CIOManager::CIOManager(const std::string& device, shared::event::CEventHandler& interruptEventHandler)
   : m_InterruptEventHandler(interruptEventHandler), 
   m_deviceName (device)
{}

void CIOManager::Initialize(std::map<std::string, boost::shared_ptr<CIO> > IOlist)
{
   m_mapKeywordsName = IOlist;

   // Initial reading of DIO
   for (int counter = 0; counter<NB_INPUTS; ++counter)
   {
      std::string name = "DI" + boost::lexical_cast<std::string>(counter);
      m_mapKeywordsName[name]->get();
   }

   // Creation of the reception thread
   //m_interruptReceiverThread = boost::thread(&CIOManager::interruptReceiverThreaded, this, m_deviceName);

   //if (!pifacedigital_enable_interrupts())
   //   throw CInitializationException("interrupt initialization error");
}

void CIOManager::onCommand(boost::shared_ptr<yApi::IYPluginApi> api,
                                boost::shared_ptr<const yApi::IDeviceCommand> command,
                                bool fromInput)
{
   std::cout << "Command received :" << yApi::IDeviceCommand::toString(command) << std::endl;

   auto search = m_mapKeywordsName.find(command->getKeyword());

   if (search != m_mapKeywordsName.end())
      search->second->set(boost::lexical_cast<bool>(command->getBody()));
   else
      std::cerr << "Cannot find keyword " << command->getKeyword();

   // Historize here ! check the list and historize.
}

void CIOManager::interruptReceiverThreaded(const std::string& keywordName) const
{
   try
   {
      while (true)
      {
         unsigned char inputs=0;

         //int value = pifacedigital_digital_read(portUsed);
         if (pifacedigital_wait_for_input(&inputs, -1, 0) > 0)
            //OK
         //else
            // throw sometimes.

         // TODO : Set the value directly read each IO ? or from the input variable
         // estimate where values have changed and send the message for the historization

         m_InterruptEventHandler.postEvent<const CIOState>(kEvtIOStateReceived, { keywordName, inputs });
      }
   }
   catch (boost::thread_interrupted&)
   {
   }
}

CIOManager::~CIOManager()
{
   // TODO : A voir si cela doit se trouver ici !
   //m_interruptReceiverThread.interrupt();
   //m_interruptReceiverThread.timed_join(boost::posix_time::seconds(20));

   std::cout << "close the connection" << std::endl;

   // Close de connection
   pifacedigital_close(0);
}