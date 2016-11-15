#include "stdafx.h"
#include "IOManager.h"
#include "urlManager.h"
#include <boost/regex.hpp> 

CIOManager::CIOManager(const std::string& device, Poco::Net::IPAddress IPAddress, bool passwordActivated, std::string password):
     m_deviceName (device),
     m_IPAddress (IPAddress),
     m_isPasswordActivated(passwordActivated),
     m_password (password)
{}

void CIOManager::Initialize(boost::shared_ptr<yApi::IYPluginApi> api,
                            std::vector<boost::shared_ptr<equipments::IEquipment> >& extensionList)
{
   m_devicesList = extensionList;
   m_keywordsToDeclare.clear();
}

void CIOManager::removeDevice(boost::shared_ptr<yApi::IYPluginApi> api, std::string deviceRemoved)
{
   std::vector<boost::shared_ptr<equipments::IEquipment> >::const_iterator iteratorExtension;

   for (iteratorExtension = m_devicesList.begin(); iteratorExtension != m_devicesList.end(); ++iteratorExtension)
   {
      // Deletion from the list of the device
      if ((*iteratorExtension)->getDeviceName() == deviceRemoved)
      {
         // If it's an extension, we delete the extension.
         // If it's the IPX800, we delete all elements
         if ((*iteratorExtension)->getDeviceType() != "IPX800")
            m_devicesList.erase(iteratorExtension);
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
   shared::CDataContainer parameters;
   std::string keywordName = command->getKeyword();
   std::string commandSelected;

   std::cout << "Command received :" << yApi::IDeviceCommand::toString(command) << std::endl;

   const auto& deviceDetails = api->getDeviceDetails(command->getDevice());
   auto deviceType = deviceDetails.get<std::string>("type");

   std::vector<boost::shared_ptr<equipments::IEquipment> >::const_iterator iteratorExtension;

   for (iteratorExtension = m_devicesList.begin(); iteratorExtension != m_devicesList.end(); ++iteratorExtension)
   {
      if (deviceType == (*iteratorExtension)->getDeviceType())
      {
         shared::CDataContainer results;

         parameters.set("key", m_password);

         results = urlManager::sendCommand(m_IPAddress, (*iteratorExtension)->buildMessageToDevice(api, parameters, command));

         // If the answer is a success, we historize the data
         if (results.containsValue("Success"))
            (*iteratorExtension)->historizePendingCommand(api, command);
         else
            std::cerr << "Error return by the IPX800" << std::endl;
      }
   }
}

void CIOManager::readIOFromDevice(boost::shared_ptr<yApi::IYPluginApi> api, 
                                  const std::string& type,
                                  bool forceHistorization)
{
   shared::CDataContainer parameters;
   shared::CDataContainer results;

   // add the password if activated
   if (m_isPasswordActivated)
      parameters.set("key", m_password);

   parameters.set("Get", type);

   results = urlManager::sendCommand(m_IPAddress, parameters);

   std::vector<boost::shared_ptr<equipments::IEquipment> >::const_iterator iteratorExtension;

   for (iteratorExtension = m_devicesList.begin(); iteratorExtension != m_devicesList.end(); ++iteratorExtension)
      (*iteratorExtension)->updateFromDevice(type, api, results, forceHistorization);
}

CIOManager::~CIOManager()
{}