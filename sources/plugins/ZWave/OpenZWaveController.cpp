#include "stdafx.h"
#include "OpenZWaveController.h"
#include <shared/Log.h>
#include "ZWave.h"
#include "OpenZWaveCommandClass.h"
#include <shared/plugin/yPluginApi/StandardCapacities.h>
#include <shared/plugin/yPluginApi/historization/Dimmable.h>
#include <command_classes/SwitchBinary.h>
#include <Options.h>
#include <Manager.h>
#include <Notification.h>
#include <platform/Log.h>
#include "KeywordContainer.h"
#include "OpenZWaveNodeKeywordFactory.h"

COpenZWaveController::COpenZWaveController()
   :m_homeId(0), m_initFailed(false), m_nodesQueried(false), m_handler(NULL), m_configuration(NULL)
{

}


COpenZWaveController::~COpenZWaveController()
{

}


void OnGlobalNotification(OpenZWave::Notification const* _notification, void* _context)
{
   try
   {
      COpenZWaveController * pPlugin = static_cast<COpenZWaveController *>(_context);
      if (pPlugin != NULL)
         pPlugin->OnNotification(_notification, _context);
   }
   catch (OpenZWave::OZWException & ex)
   {
      YADOMS_LOG(fatal) << "OpenZWave exception (OnGlobalNotification) : " << ex.what();
   }
   catch (std::exception & ex)
   {
      YADOMS_LOG(fatal) << "OpenZWave std::exception (OnGlobalNotification) : " << ex.what();
   }
   catch (...)
   {
      YADOMS_LOG(fatal) << "OpenZWave unknown exception (OnGlobalNotification)";
   }
}


std::string COpenZWaveController::GenerateDeviceStringID(uint32 homeId, uint8 nodeId)
{
   std::stringstream sstr;
   sstr << homeId << "." << (int)nodeId;
   return sstr.str();
}

void COpenZWaveController::configure(CZWaveConfiguration * configuration, shared::event::CEventHandler * handler)
{
   BOOST_ASSERT(configuration != NULL);
   BOOST_ASSERT(handler != NULL);
   boost::lock_guard<boost::mutex> lock(m_treeMutex);
   m_configuration = configuration;
   m_handler = handler;
}

bool COpenZWaveController::start()
{

   try
   {
      // Create the OpenZWave Manager.
      // The first argument is the path to the config files (where the manufacturer_specific.xml file is located
      // The second argument is the path for saved Z-Wave network state and the log file.  If you leave it NULL 
      // the log file will appear in the program's working directory.
      boost::filesystem::path full_path(m_configuration->getPath());
      boost::filesystem::path folder = full_path.parent_path();
      folder /= "config";

      boost::filesystem::path dataFolder = full_path.parent_path();
      dataFolder /= "data";
      if (!boost::filesystem::exists(dataFolder)) 
      {
         boost::system::error_code returnedError;

         boost::filesystem::create_directories(dataFolder, returnedError);
         if (returnedError)
         {
            //did not successfully create directories
            YADOMS_LOG(error) << "Fali to create folder : " << dataFolder.string();
         }
      }

      OpenZWave::Options::Create(folder.string(), dataFolder.string(), "");
      OpenZWave::Options::Get()->AddOptionInt("SaveLogLevel", OpenZWave::LogLevel_Error);
      OpenZWave::Options::Get()->AddOptionInt("QueueLogLevel", OpenZWave::LogLevel_Error);
      OpenZWave::Options::Get()->AddOptionInt("DumpTrigger", OpenZWave::LogLevel_Error);
      OpenZWave::Options::Get()->AddOptionInt("PollInterval", 500);
      OpenZWave::Options::Get()->AddOptionBool("IntervalBetweenPolls", true);
      OpenZWave::Options::Get()->AddOptionBool("ValidateValueChanges", true);
      OpenZWave::Options::Get()->Lock();


      OpenZWave::Manager::Create();

      // Add a callback handler to the manager.  The second argument is a context that
      // is passed to the OnNotification method.  If the OnNotification is a method of
      // a class, the context would usually be a pointer to that class object, to
      // avoid the need for the notification handler to be a static.
      OpenZWave::Manager::Get()->AddWatcher(OnGlobalNotification, this);

      // Add a Z-Wave Driver
      
      //this part wait infinitely for serial port open success (configuration can be changed by another thread)
      bool serialPortOpened = false;
      while (!serialPortOpened)
      {
         //lock access to configuration
         boost::lock_guard<boost::mutex> lock(m_treeMutex);

         //open the port
         if (OpenZWave::Manager::Get()->AddDriver(m_configuration->getSerialPort()))
         {
            //ok
            serialPortOpened = true;
         }
         else
         {
            //fail to open : then unlock mutex to allow configuration to be changed, then wait 1 sec
            YADOMS_LOG(warning) << "Fail to open serial port : " << m_configuration->getSerialPort() << " port address : " << m_configuration->getSerialPort();
            m_treeMutex.unlock();
            boost::this_thread::sleep(boost::posix_time::milliseconds(1000));
         }
      }
         


      // Now we just wait for either the AwakeNodesQueried or AllNodesQueried notification,
      // then write out the config file.
      // In a normal app, we would be handling notifications and building a UI for the user.

      // Since the configuration file contains command class information that is only 
      // known after the nodes on the network are queried, wait until all of the nodes 
      // on the network have been queried (at least the "listening" ones) before
      // writing the configuration file.  (Maybe write again after sleeping nodes have
      // been queried as well.)
      while (!m_nodesQueried)
      {
         boost::this_thread::sleep(boost::posix_time::milliseconds(200));
      }

      if (!m_initFailed)
      {

         OpenZWave::Manager::Get()->WriteConfig(m_homeId);

         boost::lock_guard<boost::mutex> lock(m_treeMutex);
         NodeListType::iterator i;

         for (i = m_nodes.begin(); i != m_nodes.end(); ++i)
         {
            boost::shared_ptr<COpenZWaveNode> nodeInfo = *i;
            std::string sNodeName = OpenZWave::Manager::Get()->GetNodeName(nodeInfo->getHomeId(), nodeInfo->getNodeId());
            std::string sNodeManufacturer = OpenZWave::Manager::Get()->GetNodeManufacturerName(nodeInfo->getHomeId(), nodeInfo->getNodeId());
            std::string sNodeProductName = OpenZWave::Manager::Get()->GetNodeProductName(nodeInfo->getHomeId(), nodeInfo->getNodeId());
            std::string sNodeProductType = OpenZWave::Manager::Get()->GetNodeProductType(nodeInfo->getHomeId(), nodeInfo->getNodeId());
            std::string sNodeProductId = OpenZWave::Manager::Get()->GetNodeProductId(nodeInfo->getHomeId(), nodeInfo->getNodeId());
            std::string sNodeLocation = OpenZWave::Manager::Get()->GetNodeLocation(nodeInfo->getHomeId(), nodeInfo->getNodeId());
            std::string sNodeType = OpenZWave::Manager::Get()->GetNodeType(nodeInfo->getHomeId(), nodeInfo->getNodeId());

            std::string id = GenerateDeviceStringID(nodeInfo->getHomeId(), nodeInfo->getNodeId());

            shared::CDataContainer d;
            d.set("name", id);

            if (sNodeName.empty())
               d.set("friendlyName", sNodeProductName);
            else
               d.set("friendlyName", sNodeName);

            shared::CDataContainer details;
            details.set("Manufacturer", sNodeManufacturer);
            details.set("Product", sNodeProductName);
            details.set("ProductId", sNodeProductId);
            details.set("ProductType", sNodeProductType);
            details.set("Location", sNodeLocation);
            details.set("Type", sNodeType);
            d.set("details", details);

            if (m_handler != NULL)
               m_handler->postEvent(CZWave::kDeclareDevice, d);
         }

      }

      return true;
   }
   catch (OpenZWave::OZWException & ex)
   {
      YADOMS_LOG(fatal) << "Fail to start OpenZWave controller : OpenZWave exception : " << ex.what();
   }
   catch (std::exception & ex)
   {
      YADOMS_LOG(fatal) << "Fail to start OpenZWave controller : std::exception : " << ex.what();
   }
   catch (...)
   {
      YADOMS_LOG(fatal) << "Fail to start OpenZWave controller unknown exception";
   }

   return false;
}

void COpenZWaveController::stop()
{
   try
   {
      OpenZWave::Manager::Destroy();
   }
   catch (OpenZWave::OZWException & ex)
   {
      YADOMS_LOG(fatal) << "Fail to stop OpenZWave controller : OpenZWave exception : " << ex.what();
   }
   catch (std::exception & ex)
   {
      YADOMS_LOG(fatal) << "Fail to stop OpenZWave controller : std::exception : " << ex.what();
   }
   catch (...)
   {
      YADOMS_LOG(fatal) << "Fail to stop OpenZWave controller unknown exception";
   }
}


boost::shared_ptr<COpenZWaveNode> COpenZWaveController::GetNode(OpenZWave::Notification const* _notification)
{
   uint32 const homeId = _notification->GetHomeId();
   uint8 const nodeId = _notification->GetNodeId();
   return GetNode(homeId, nodeId);
}

boost::shared_ptr<COpenZWaveNode> COpenZWaveController::GetNode(const int homeId, const uint8 nodeId)
{
   for (NodeListType::iterator it = m_nodes.begin(); it != m_nodes.end(); ++it)
   {
      if ((*it)->match(homeId, nodeId))
         return *it;
   }
   return boost::shared_ptr<COpenZWaveNode>();
}






void COpenZWaveController::OnNotification(OpenZWave::Notification const* _notification, void* _context)
{
   // Must do this inside a critical section to avoid conflicts with the main thread
   boost::lock_guard<boost::mutex> lock(m_treeMutex);


   //get all glocbal informations (for all notifications)
   OpenZWave::ValueID vID = _notification->GetValueID();
   ECommandClass commandClass((int)vID.GetCommandClassId());


      
   //YADOMS_LOG(information) << "ZWave Notification : " << _notification->GetAsString();

   switch (_notification->GetType())
   {
   case OpenZWave::Notification::Type_ValueAdded:
   {
      COpenZWaveNodeKeywordFactory::generateHistoriser(_notification);

      boost::shared_ptr<COpenZWaveNode> node = GetNode(_notification);
      if (node)
         node->registerCapacity(commandClass);
      break;
   }

   case OpenZWave::Notification::Type_ValueRemoved:
      break;

   case OpenZWave::Notification::Type_ValueChanged:
   {
      COpenZWaveNodeKeywordFactory::generateHistoriser(_notification);

      boost::shared_ptr<COpenZWaveNode> node = GetNode(_notification);
      if (node)
      {
         OpenZWave::ValueID::ValueGenre vGenre = vID.GetGenre();
         
         if (vGenre == OpenZWave::ValueID::ValueGenre_User)
         {
            std::string id = GenerateDeviceStringID(node->getHomeId(), node->getNodeId());
            std::string vLabel = OpenZWave::Manager::Get()->GetValueLabel(vID);
           
            node->registerKeyword(commandClass, vLabel, vID);

            boost::shared_ptr<CKeywordContainer> d(new CKeywordContainer(id, node->getLastKeywordValue(commandClass, vLabel)));
            if (m_handler != NULL)
               m_handler->postEvent(CZWave::kUpdateKeyword, d);
         }
      }
      break;
   }

   case OpenZWave::Notification::Type_ValueRefreshed:
      // One of the node values has changed
      COpenZWaveNodeKeywordFactory::generateHistoriser(_notification);
      break;

   case OpenZWave::Notification::Type_Group:
   {
      // One of the node's association groups has changed
      break;
   }

   case OpenZWave::Notification::Type_NodeAdded:
   {
      
      m_nodes.push_back( boost::shared_ptr<COpenZWaveNode>( new COpenZWaveNode(_notification->GetHomeId(), _notification->GetNodeId())));
      break;
   }



   case OpenZWave::Notification::Type_NodeProtocolInfo:
   {
      boost::shared_ptr<COpenZWaveNode> node = GetNode(_notification);
      if (node)
      {
         std::string sNodeType = OpenZWave::Manager::Get()->GetNodeType(node->getHomeId(), node->getNodeId());
         YADOMS_LOG(information) << "ZWave : NodeProtocolInfo : " << sNodeType;
         
      }
      break;
   }

   case OpenZWave::Notification::Type_NodeNaming:
   {
      boost::shared_ptr<COpenZWaveNode> nodeInfo = GetNode(_notification);
      if (nodeInfo)
      {
         std::string sNodeName = OpenZWave::Manager::Get()->GetNodeName(nodeInfo->getHomeId(), nodeInfo->getNodeId());
         std::string sNodeManufacturer = OpenZWave::Manager::Get()->GetNodeManufacturerName(nodeInfo->getHomeId(), nodeInfo->getNodeId());
         std::string sNodeProductName = OpenZWave::Manager::Get()->GetNodeProductName(nodeInfo->getHomeId(), nodeInfo->getNodeId());
         std::string sNodeProductType = OpenZWave::Manager::Get()->GetNodeProductType(nodeInfo->getHomeId(), nodeInfo->getNodeId());
         std::string sNodeProductId = OpenZWave::Manager::Get()->GetNodeProductId(nodeInfo->getHomeId(), nodeInfo->getNodeId());
      
         YADOMS_LOG(information) << "ZWave : NodeNaming : name = " << sNodeName;
         YADOMS_LOG(information) << "ZWave : NodeNaming : manufacturer = " << sNodeManufacturer;
         YADOMS_LOG(information) << "ZWave : NodeNaming : productName = " << sNodeProductName;
         YADOMS_LOG(information) << "ZWave : NodeNaming : productType = " << sNodeProductType;
         YADOMS_LOG(information) << "ZWave : NodeNaming : productId = " << sNodeProductId;
      
      }
      break;
   }


   case OpenZWave::Notification::Type_NodeRemoved:
   {
      uint32 const homeId = _notification->GetHomeId();
      uint8 const nodeId = _notification->GetNodeId();
      for (NodeListType::iterator i = m_nodes.begin(); i != m_nodes.end(); ++i)
      {
         if ((*i)->match(homeId, nodeId))
         {
            m_nodes.erase(i);
            break;
         }
      }
      break;
   }

   case OpenZWave::Notification::Type_NodeEvent:
   {
      // We have received an event from the node, caused by a
      // basic_set or hail message.
      break;
   }

   case OpenZWave::Notification::Type_PollingDisabled:
   {
      break;
   }

   case OpenZWave::Notification::Type_PollingEnabled:
   {
      break;
   }

   case OpenZWave::Notification::Type_DriverReady:
   {
      m_homeId = _notification->GetHomeId();
      break;
   }

   case OpenZWave::Notification::Type_DriverFailed:
   {
      m_initFailed = true;
      break;
   }

   case OpenZWave::Notification::Type_AwakeNodesQueried:
   case OpenZWave::Notification::Type_AllNodesQueried:
   case OpenZWave::Notification::Type_AllNodesQueriedSomeDead:
   {
      m_nodesQueried = true;
      break;
   }

   case OpenZWave::Notification::Type_DriverReset:
   case OpenZWave::Notification::Type_NodeQueriesComplete:
   default:
      break;

   }

}
   /*
bool COpenZWaveController::GetValueByCommandClass(const int homeId, const int nodeID, const int instanceID, const int commandClass, OpenZWave::ValueID &nValue)
{

   COpenZWaveController::NodeInfo *pNode = GetNodeInfo(homeId, nodeID);
   if (!pNode)
      return false;

   for (std::list<OpenZWave::ValueID>::iterator itt = pNode->Instances[instanceID][commandClass].Values.begin(); itt != pNode->Instances[instanceID][commandClass].Values.end(); ++itt)
   {
      unsigned char cmdClass = itt->GetCommandClassId();
      if (cmdClass == commandClass)
      {
         nValue = *itt;
         return true;
      }
   }
   return false;
}
*/

void COpenZWaveController::RetreiveOpenZWaveIds(const std::string & device, const std::string & keyword,
   int & homeId, uint8 & nodeId, std::string & keywordName, ECommandClass & keywordClass)
{
   std::vector<std::string> splittedDevice;
   boost::split(splittedDevice, device, boost::is_any_of("."), boost::token_compress_on);
   if (splittedDevice.size() != 2)
   {
      throw shared::exception::CException("The device id is invalid : not matching pattern : <homeId>-<nodeId> ");
   }
   homeId = boost::lexical_cast<int>(splittedDevice[0]);
   nodeId = static_cast<uint8>(atoi(splittedDevice[1].c_str())); //dont use lexical cast for uint8, because it realize a string to char conversion: "2" is transform in '2' = 0x32

   std::vector<std::string> splittedKeyword;
   boost::split(splittedKeyword, keyword, boost::is_any_of("."), boost::token_compress_on);
   if (splittedKeyword.size() != 2)
   {
      throw shared::exception::CException("The keyword id is invalid : not matching pattern : <label>-<class> ");
   }
   keywordName = splittedKeyword[0];
   keywordClass.fromString(splittedKeyword[1]);

}

void COpenZWaveController::SendCommand(const std::string & device, const std::string & keyword, const std::string & value)
{
   boost::lock_guard<boost::mutex> lock(m_treeMutex);

   int homeId;
   uint8 nodeId;
   std::string keywordName;
   ECommandClass keywordClass;

   RetreiveOpenZWaveIds(device, keyword, homeId, nodeId, keywordName, keywordClass);

   //TODO : gerer les autres type
   boost::shared_ptr<COpenZWaveNode> node = GetNode(homeId, nodeId);
   if (node)
   {
      node->sendCommand(keywordClass, keywordName, value);
   }
}


