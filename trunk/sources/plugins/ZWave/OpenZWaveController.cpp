#include "stdafx.h"
#include "OpenZWaveController.h"
#include <shared/Log.h>
#include "FatalErrorException.h"
#include "ZWave.h"
#include "OpenZWaveCommandClass.h"
#include <shared/plugin/yadomsApi/StandardCapacities.h>
#include <shared/plugin/yadomsApi/commands/Switch.h>
#include "command_classes/SwitchBinary.h"

COpenZWaveController::COpenZWaveController()
   :m_homeId(0), m_initFailed(false), m_nodesQueried(false), m_configuration(NULL), m_handler(NULL)
{

}


COpenZWaveController::~COpenZWaveController()
{

}


void OnGlobalNotification(OpenZWave::Notification const* _notification, void* _context)
{
   COpenZWaveController * pPlugin = static_cast<COpenZWaveController *>(_context);
   if (pPlugin != NULL)
      pPlugin->OnNotification(_notification, _context);
}


std::string COpenZWaveController::GenerateDeviceStringID(uint32 homeId, uint8 nodeId)
{
   std::stringstream sstr;
   sstr << homeId << "." << (int)nodeId;
   return sstr.str();
}

std::string COpenZWaveController::GenerateKeywordStringID(std::string label, uint8 instance)
{
   std::stringstream sstr;
   sstr << label << "." << (int)instance;
   return sstr.str();
}


void COpenZWaveController::configure(CZWaveConfiguration * configuration, shared::event::CEventHandler * handler)
{
   BOOST_ASSERT(configuration != NULL);
   BOOST_ASSERT(handler != NULL);
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

      OpenZWave::Options::Create(folder.string(), "", "");
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
      // Modify this line to set the correct serial port for your PC interface.
      std::string realSerialPort = (boost::format("\\\\.\\%1%") % m_configuration->getSerialPort()).str();
      if (!OpenZWave::Manager::Get()->AddDriver(realSerialPort))
         throw shared::exception::CException("Fail to open serial port");


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
         std::vector<COpenZWaveController::NodeInfo*>::iterator i;

         for (i = m_nodes.begin(); i != m_nodes.end(); ++i)
         {
            COpenZWaveController::NodeInfo* nodeInfo = *i;
            std::string sNodeName = OpenZWave::Manager::Get()->GetNodeName(nodeInfo->m_homeId, nodeInfo->m_nodeId);
            std::string sNodeManufacturer = OpenZWave::Manager::Get()->GetNodeManufacturerName(nodeInfo->m_homeId, nodeInfo->m_nodeId);
            std::string sNodeProductName = OpenZWave::Manager::Get()->GetNodeProductName(nodeInfo->m_homeId, nodeInfo->m_nodeId);
            std::string sNodeProductType = OpenZWave::Manager::Get()->GetNodeProductType(nodeInfo->m_homeId, nodeInfo->m_nodeId);
            std::string sNodeProductId = OpenZWave::Manager::Get()->GetNodeProductId(nodeInfo->m_homeId, nodeInfo->m_nodeId);
            std::string sNodeLocation = OpenZWave::Manager::Get()->GetNodeLocation(nodeInfo->m_homeId, nodeInfo->m_nodeId);
            std::string sNodeType = OpenZWave::Manager::Get()->GetNodeType(nodeInfo->m_homeId, nodeInfo->m_nodeId);

            std::string id = GenerateDeviceStringID(nodeInfo->m_homeId, nodeInfo->m_nodeId);

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
   catch (OpenZWave::CFatalErrorException & ex)
   {
      YADOMS_LOG(fatal) << "Fail to start OpenZWave controller : " << ex.what();
   }
   return false;
}

void COpenZWaveController::stop()
{
   OpenZWave::Manager::Destroy();
}





time_t to_time_t(boost::posix_time::ptime t)
{
   boost::posix_time::ptime epoch(boost::gregorian::date(1970, 1, 1));
   boost::posix_time::time_duration::sec_type x = (t - epoch).total_seconds();
   return time_t(x);
}




//-----------------------------------------------------------------------------
// <GetNodeInfo>
// Return the NodeInfo object associated with this notification
//-----------------------------------------------------------------------------
COpenZWaveController::NodeInfo* COpenZWaveController::GetNodeInfo(OpenZWave::Notification const* _notification)
{
   uint32 const homeId = _notification->GetHomeId();
   uint8 const nodeId = _notification->GetNodeId();
   return GetNodeInfo(homeId, nodeId);
}


COpenZWaveController::NodeInfo* COpenZWaveController::GetNodeInfo(const int homeId, const int nodeId)
{
   for (std::vector<NodeInfo*>::iterator it = m_nodes.begin(); it != m_nodes.end(); ++it)
   {
      NodeInfo* nodeInfo = *it;
      if ((nodeInfo->m_homeId == homeId) && (nodeInfo->m_nodeId == nodeId))
      {
         return nodeInfo;
      }
   }

   return NULL;
}

boost::shared_ptr<COpenZWaveController::CZWNode> COpenZWaveController::GetNode(OpenZWave::Notification const* _notification)
{
   uint32 const homeId = _notification->GetHomeId();
   uint8 const nodeId = _notification->GetNodeId();
   return GetNode(homeId, nodeId);
}

boost::shared_ptr<COpenZWaveController::CZWNode> COpenZWaveController::GetNode(const int homeId, const int nodeId)
{
   for (NodeListType::iterator it = m_allNodes.begin(); it != m_allNodes.end(); ++it)
   {
      if ((*it)->match(homeId, nodeId))
         return *it;
   }
   return boost::shared_ptr<COpenZWaveController::CZWNode>();
}






void COpenZWaveController::OnNotification(OpenZWave::Notification const* _notification, void* _context)
{
   // Must do this inside a critical section to avoid conflicts with the main thread
   boost::lock_guard<boost::mutex> lock(m_treeMutex);


   //get all glocbal informations (for all notifications)
   time_t updateTime = to_time_t(boost::posix_time::second_clock::universal_time());

   OpenZWave::ValueID vID = _notification->GetValueID();
   ECommandClass commandClass((int)vID.GetCommandClassId());
   unsigned char instance;
   switch (commandClass())
   {
   case  ECommandClass::kMultiInstance:
   case  ECommandClass::kSensorMultilevel:
   case  ECommandClass::kThermostatSetpoint:
   case  ECommandClass::kSensorBinary:
      instance = vID.GetIndex();
      break;
   default:
      instance = vID.GetInstance();
      break;
   }






   switch (_notification->GetType())
   {
   case OpenZWave::Notification::Type_ValueAdded:
   {

      boost::shared_ptr<COpenZWaveController::CZWNode> node = GetNode(_notification);
      if (node)
      {
         node->registerCapacity(commandClass);
      }

      //if (NodeInfo* nodeInfo = GetNodeInfo(_notification))
      //{
      //   // Add the new value to our list
      //   nodeInfo->Instances[instance][commandClass].Values.push_back(vID);
      //   nodeInfo->m_LastSeen = updateTime;
      //   nodeInfo->Instances[instance][commandClass].m_LastSeen = updateTime;
      //
      //   
      //}
      break;
   }

   case OpenZWave::Notification::Type_ValueRemoved:
   {
      //if (NodeInfo* nodeInfo = GetNodeInfo(_notification))
      //{
      //   // Remove the value from out list
      //   for (std::list<OpenZWave::ValueID>::iterator it = nodeInfo->Instances[instance][commandClass].Values.begin(); it != nodeInfo->Instances[instance][commandClass].Values.end(); ++it)
      //   {
      //      if ((*it) == vID)
      //      {
      //         nodeInfo->Instances[instance][commandClass()].Values.erase(it);
      //         nodeInfo->Instances[instance][commandClass()].m_LastSeen = updateTime;
      //         nodeInfo->m_LastSeen = updateTime;
      //         break;
      //      }
      //   }
      //}
      break;
   }

   case OpenZWave::Notification::Type_ValueChanged:
   {
      boost::shared_ptr<COpenZWaveController::CZWNode> node = GetNode(_notification);
      if (node)
      {
         OpenZWave::ValueID::ValueType vType = vID.GetType();
         OpenZWave::ValueID::ValueGenre vGenre = vID.GetGenre();

         if (vGenre == OpenZWave::ValueID::ValueGenre_User)
         {
            std::string id = GenerateDeviceStringID(node->getHomeId(), node->getNodeId());
            std::string vLabel = OpenZWave::Manager::Get()->GetValueLabel(vID);
            std::string vUnits = OpenZWave::Manager::Get()->GetValueUnits(vID);
            std::string keywordId = GenerateKeywordStringID(vLabel, instance);

            node->registerKeyword(commandClass, keywordId, vID);

            std::string stringvalue;
            OpenZWave::Manager::Get()->GetValueAsString(vID, &stringvalue);

            shared::CDataContainer d;
            d.set("device", id);
            d.set("keyword", keywordId);
            d.set("units", vUnits);

            switch (vType)
            {
            case OpenZWave::ValueID::ValueType_Bool:
               d.set("type", shared::plugin::yadomsApi::EKeywordType::kBool);
               break;

            case OpenZWave::ValueID::ValueType_Byte:
            case OpenZWave::ValueID::ValueType_Decimal:
            case OpenZWave::ValueID::ValueType_Int:
            case OpenZWave::ValueID::ValueType_Short:
               d.set("type", shared::plugin::yadomsApi::EKeywordType::kNumeric);
               break;

            default:
               d.set("type", shared::plugin::yadomsApi::EKeywordType::kString);
               break;
            }

            d.set("value", stringvalue);


            switch (commandClass())
            {
            case ECommandClass::kSwitchAll:
            case ECommandClass::kSwitchBinary:
            case ECommandClass::kSwitchMultilevel:
            case ECommandClass::kSwitchToggleBinary:
            case ECommandClass::kSwitchToggleMultilevel:
               d.set("capacity", yApi::CStandardCapacities::Switch.getName());
               d.set("access", shared::plugin::yadomsApi::EKeywordAccessMode::kGetSet);
               break;

            case ECommandClass::kSensorBinary:
               d.set("capacity", yApi::CStandardCapacities::Switch.getName());
               d.set("access", shared::plugin::yadomsApi::EKeywordAccessMode::kGet);
               break;

            case ECommandClass::kBattery:
               d.set("capacity", yApi::CStandardCapacities::BatteryLevel.getName());
               d.set("access", shared::plugin::yadomsApi::EKeywordAccessMode::kGet);
               break;

            default:
               d.set("capacity", commandClass.getAsString());
               d.set("access", shared::plugin::yadomsApi::EKeywordAccessMode::kGet);
               break;
            }


            if (m_handler != NULL)
               m_handler->postEvent(CZWave::kUpdateKeyword, d);
         }
      }


      // One of the node values has changed
     // if (m_nodesQueried)
         if (NodeInfo* nodeInfo = GetNodeInfo(_notification))
         {


         

         }
      break;
   }

   case OpenZWave::Notification::Type_ValueRefreshed:
      // One of the node values has changed
      break;

   case OpenZWave::Notification::Type_Group:
   {
      // One of the node's association groups has changed
      break;
   }

   case OpenZWave::Notification::Type_NodeAdded:
   {
      m_allNodes.push_back(boost::shared_ptr<CZWNode>(new CZWNode(_notification->GetHomeId(), _notification->GetNodeId())));

      // Add the new node to our list
      //NodeInfo* nodeInfo = new NodeInfo();
      //nodeInfo->m_homeId = _notification->GetHomeId();
      //nodeInfo->m_nodeId = _notification->GetNodeId();
      //nodeInfo->m_polled = false;
      //m_nodes.push_back(nodeInfo);
      break;
   }

   case OpenZWave::Notification::Type_NodeRemoved:
   {
      uint32 const homeId = _notification->GetHomeId();
      uint8 const nodeId = _notification->GetNodeId();
      for (NodeListType::iterator i = m_allNodes.begin(); i != m_allNodes.end(); ++i)
      {
         if ((*i)->match(homeId, nodeId))
         {
            m_allNodes.erase(i);
            break;
         }
      }


      // Remove the node from our list
      //uint32 const homeId = _notification->GetHomeId();
      //uint8 const nodeId = _notification->GetNodeId();
      //for (std::vector<NodeInfo*>::iterator it = m_nodes.begin(); it != m_nodes.end(); ++it)
      //{
      //   NodeInfo* nodeInfo = *it;
      //   if ((nodeInfo->m_homeId == homeId) && (nodeInfo->m_nodeId == nodeId))
      //   {
      //      m_nodes.erase(it);
      //      delete nodeInfo;
      //      break;
      //   }
      //}
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
   case OpenZWave::Notification::Type_NodeNaming:
   case OpenZWave::Notification::Type_NodeProtocolInfo:
   case OpenZWave::Notification::Type_NodeQueriesComplete:
   default:
   {
   }
   }

}

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


void COpenZWaveController::SendCommand(const std::string & device, const std::string & keyword, const std::string & value)
{
   // Must do this inside a critical section to avoid conflicts with the main thread
   boost::lock_guard<boost::mutex> lock(m_treeMutex);

   std::vector<std::string> splittedDevice;
   boost::split(splittedDevice, device, boost::is_any_of("."), boost::token_compress_on);
   if (splittedDevice.size() != 2)
   {
      throw shared::exception::CException("The device id is invalid : not matching pattern : <homeId>-<nodeId> ");
   }
   int homeId = boost::lexical_cast<int>(splittedDevice[0]);
   uint8 nodeId = atoi(splittedDevice[1].c_str()); //dont use lexical cast for uint8, becuase it realize a string to char conversion: "2" is transform in '2' = 0x32

   std::vector<std::string> splittedKeyword;
   boost::split(splittedKeyword, keyword, boost::is_any_of("."), boost::token_compress_on);
   if (splittedKeyword.size() != 2)
   {
      throw shared::exception::CException("The keyword id is invalid : not matching pattern : <label>-<instance> ");
   }
   std::string label = splittedKeyword[0];
   uint8 instance = atoi(splittedKeyword[1].c_str()); //dont use lexical cast for uint8, becuase it realize a string to char conversion: "2" is transform in '2' = 0x32

   //TODO : gerer les autres type
   boost::shared_ptr<COpenZWaveController::CZWNode> node = GetNode(homeId, nodeId);
   if (node)
   {
      //objectif
      //node->SendCommand(keyword, value);


      OpenZWave::ValueID vID = node->getValueId(ECommandClass::kSwitchBinary, keyword);

      shared::plugin::yadomsApi::commands::CSwitch commandDetails(value); 
      OpenZWave::Manager::Get()->SetValue(vID, commandDetails.isOn());

      //boost::shared_ptr<OpenZWave::SwitchBinary> sb = boost::dynamic_pointer_cast<OpenZWave::SwitchBinary>((*node)[ECommandClass::kSwitchBinary]->get());

      
      YADOMS_LOG(info) << "ZWave peripheral command : " << (commandDetails.isOn() ? "On" : "Off");
      
      //OpenZWave::ValueBool vb;
      //vb.Set(commandDetails.isOn());

      
      //sb->SetValue(vb);
   }


   //OpenZWave::ValueID vID(0, 0, OpenZWave::ValueID::ValueGenre_Basic, 0, 0, 0, OpenZWave::ValueID::ValueType_Bool);
   //GetValueByCommandClass(homeId, nodeId, instance, (uint8)ECommandClass::kSwitchBinary, vID);
   //
   ////read command details (may throw exception if something is wrong)
   //shared::plugin::yadomsApi::commands::CSwitch commandDetails(value);
   //YADOMS_LOG(info) << "ZWave peripheral command : " << (commandDetails.isOn()?"On":"Off");
   //OpenZWave::Manager::Get()->SetValue(vID, commandDetails.isOn());
   //
   //
   //OpenZWave::Manager::Get()->RequestAllConfigParams(homeId, nodeId);
   /*
   OpenZWave::Manager::Get()->RefreshValue(vID);

   std::string stringvalue;
   OpenZWave::Manager::Get()->GetValueAsString(vID, &stringvalue);
   YADOMS_LOG(info) << "ZWave peripheral value : " << stringvalue;*/
}


