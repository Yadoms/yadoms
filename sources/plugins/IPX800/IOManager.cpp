#include "stdafx.h"
#include "IOManager.h"
#include "urlManager.h"
#include <boost/regex.hpp> 
#include <shared/Log.h>

CIOManager::CIOManager(const std::string& device, const Poco::Net::SocketAddress& socket, bool passwordActivated, const std::string& password):
     m_deviceName (device),
     m_socketAddress(socket),
     m_isPasswordActivated(passwordActivated),
     m_password (password)
{}

void CIOManager::Initialize(std::vector<boost::shared_ptr<equipments::IEquipment> >& extensionList)
{
   m_devicesList = extensionList;
}

void CIOManager::removeDevice(boost::shared_ptr<yApi::IYPluginApi> api, const std::string& deviceRemoved)
{
   for (unsigned char counter = 0; counter < m_devicesList.size(); ++counter)
   {
      // Deletion from the list of the device
      if (m_devicesList[counter]->getDeviceName() == deviceRemoved)
      {
         // If it's an extension, we delete the extension.
         // If it's the IPX800, we delete all elements
         if (m_devicesList[counter]->getDeviceType() != "IPX800")
            m_devicesList.erase(m_devicesList.begin()+counter);
         else
            m_devicesList.clear();
      }
   }
}

void CIOManager::readAllIOFromDevice(boost::shared_ptr<yApi::IYPluginApi> api, bool forceHistorization)
{
   // Initial Reading of relays
   readIOFromDevice(api, "R", forceHistorization);

   // Initial Reading of DIs
   readIOFromDevice(api, "D", forceHistorization);

   // Initial Reading of Analog Input
   readIOFromDevice(api, "A", forceHistorization);

   // Initial Reading of Counters
   readIOFromDevice(api, "C", forceHistorization);
}

void CIOManager::onCommand(boost::shared_ptr<yApi::IYPluginApi> api,
                           boost::shared_ptr<const yApi::IDeviceCommand> command)
{
   boost::shared_ptr<shared::CDataContainer> parameters = shared::CDataContainer::make();

   YADOMS_LOG(information) << "Command received :" << yApi::IDeviceCommand::toString(command) ;

   const auto& deviceDetails = api->getDeviceDetails(command->getDevice());
   const auto deviceType = deviceDetails->get<std::string>("type");

   for (std::vector<boost::shared_ptr<equipments::IEquipment> >::const_iterator iteratorExtension = m_devicesList.begin();
      iteratorExtension != m_devicesList.end();
      ++iteratorExtension)
   {
      if (deviceType == (*iteratorExtension)->getDeviceType())
      {
         if (m_isPasswordActivated)
            parameters->set("key", m_password);

         auto results = urlManager::sendCommand(m_socketAddress, (*iteratorExtension)->buildMessageToDevice(api, parameters, command));

         // If the answer is a success, we historize the data
         if (results->containsValue("Success"))
            (*iteratorExtension)->historizePendingCommand(api, command);
         else
         {
            YADOMS_LOG(error) << "Command is not executed by the IPX800" ;
            
            // if an error is return, we reset the pending operation
            (*iteratorExtension)->resetPendingCommand();
         }
      }
   }
}

void CIOManager::readIOFromDevice(boost::shared_ptr<yApi::IYPluginApi> api, 
                                  const std::string& type,
                                  bool forceHistorization)
{
   boost::shared_ptr<shared::CDataContainer> parameters = shared::CDataContainer::make();

   // add the password if activated
   if (m_isPasswordActivated)
      parameters->set("key", m_password);

   parameters->set("Get", type);

   auto results = urlManager::sendCommand( m_socketAddress, parameters);

   for (std::vector<boost::shared_ptr<equipments::IEquipment> >::const_iterator iteratorExtension = m_devicesList.begin();
      iteratorExtension != m_devicesList.end();
      ++iteratorExtension)
      (*iteratorExtension)->updateFromDevice(type, api, results, forceHistorization);
}

void CIOManager::OnConfigurationUpdate(boost::shared_ptr<yApi::IYPluginApi> api,
                                           const IIPX800Configuration& configuration)
{
   m_socketAddress = configuration.getIPAddressWithSocket();
   m_isPasswordActivated = configuration.isPasswordActivated();
   m_password = configuration.getPassword();
}

void CIOManager::OnDeviceConfigurationUpdate(boost::shared_ptr<yApi::IYPluginApi> api,
                                             const boost::shared_ptr<const yApi::ISetDeviceConfiguration> newConfiguration)
{
   /* TODO : To be checked
   std::vector<boost::shared_ptr<equipments::IEquipment> >::const_iterator iteratorExtension;

   for (iteratorExtension = m_devicesList.begin(); iteratorExtension != m_devicesList.end(); ++iteratorExtension)
   {
      if ((*iteratorExtension)->getDeviceName() == newConfiguration->name())
      {
         (*iteratorExtension)->setNewConfiguration(newConfiguration->configuration());
      }
   }*/
}

CIOManager::~CIOManager()
{}