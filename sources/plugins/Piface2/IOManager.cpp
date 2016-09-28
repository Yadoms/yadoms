#include "stdafx.h"
#include "IOManager.h"
#include "InitializationException.hpp"
#include "pifacedigital.h"
#include <mcp23s17.h>
#include "eventDefinitions.h"

CIOManager::CIOManager(const std::string& device)
   :m_deviceName (device),
   m_inputValue(0)
{
   // Open the connection
   if (pifacedigital_open(0) == -1)
      throw CInitializationException("Initialization error - Configuration of the SPI in raspi-config ?");
}

void CIOManager::Initialize(boost::shared_ptr<yApi::IYPluginApi> api, 
                            std::map<std::string, boost::shared_ptr<CIO> > IOlist,
                            boost::shared_ptr<shared::event::CEventHandler> interruptEventHandler)
{
   m_InterruptEventHandler = interruptEventHandler;
   m_mapKeywordsName = IOlist;
   m_keywordsToDeclare.clear();

   // Initial reading of DI
   for (int counter = 0; counter<NB_INPUTS; ++counter)
   {
      std::string name = "DI" + boost::lexical_cast<std::string>(counter);
      m_inputValue |= (m_mapKeywordsName[name]->get() << counter);
      m_keywordsToDeclare.push_back(m_mapKeywordsName[name]->historizable());
   }

   //Historize initial values
   api->historizeData(m_deviceName, m_keywordsToDeclare);

   if (pifacedigital_enable_interrupts() != 0)
      throw CInitializationException("interrupt initialization error");

   // Creation of the reception thread
   m_interruptReceiverThread = boost::thread(&CIOManager::interruptReceiverThreaded, this, m_deviceName);
}

void CIOManager::onCommand(boost::shared_ptr<yApi::IYPluginApi> api,
                           boost::shared_ptr<const yApi::IDeviceCommand> command)
{
   std::cout << "Command received :" << yApi::IDeviceCommand::toString(command) << std::endl;

   auto search = m_mapKeywordsName.find(command->getKeyword());

   if (search != m_mapKeywordsName.end())
   {
      // Set the value of the corresponding IO and historize 
      search->second->set(boost::lexical_cast<bool>(command->getBody()));
      api->historizeData(m_deviceName, search->second->historizable());
   }
   else
      std::cerr << "Cannot find keyword " << command->getKeyword();
}

void CIOManager::onCommand(boost::shared_ptr<yApi::IYPluginApi> api,
                           int receivedValue)
{
   m_keywordsToDeclare.clear();

   std::cout << "Value received :" << std::hex << receivedValue << std::endl;

   for (int counter = 0; counter<NB_INPUTS; ++counter)
   {
      std::string name = "DI" + boost::lexical_cast<std::string>(counter);

      // If a value changed
      if ((m_inputValue & (1 << counter)) != (receivedValue & (1 << counter)))
      {
         m_mapKeywordsName[name]->set((bool)(m_inputValue & (1 << counter))>>counter);
         m_keywordsToDeclare.push_back(m_mapKeywordsName[name]->historizable());
      }
   }

   api->historizeData(m_deviceName, m_keywordsToDeclare);
}

void CIOManager::interruptReceiverThreaded(const std::string& keywordName) const
{
   try
   {
      std::cout << "### Start interruptReceiverThread ###" << std::endl;

      while (true) // /*!boost::this_thread::interruption_requested()*/
      {
         unsigned char inputs=0;

         if (pifacedigital_wait_for_input(&inputs, -1, 0) > 0)
         {
            std::cout << "pifacedigital_wait_for_input :" << std::hex << (int)inputs << std::endl;
            m_InterruptEventHandler->postEvent<const CIOState>(kEvtIOStateReceived, { keywordName, inputs });
         }
         else
         {
            std::cout << "pifacedigital_wait_for_input <=0" << std::endl;
         }
      }
   }
   catch (boost::thread_interrupted&)
   {
      std::cout << "### Stop interruptReceiverThread ###" << std::endl;
   }
   catch (...)
   {
      std::cout << "### Unknown error ###" << std::endl;
   }
}

CIOManager::~CIOManager()
{
   m_interruptReceiverThread.interrupt();

   //TODO : return value to be interpreted !
   m_interruptReceiverThread.timed_join(boost::posix_time::seconds(20));

   std::cout << "close Piface2 connection" << std::endl;

   // Close de connection
   pifacedigital_close(0);
}