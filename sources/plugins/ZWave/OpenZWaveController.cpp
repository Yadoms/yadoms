#include "stdafx.h"
#include "OpenZWaveController.h"
#include "ZWave.h"
#include "OpenZWaveCommandClass.h"
#include <Options.h>
#include <Manager.h>
#include <Notification.h>
#include <platform/Log.h>
#include "KeywordContainer.h"
#include "OpenZWaveHelpers.h"
#include "ZWaveInternalState.h"
#include <shared/Log.h>

COpenZWaveController::COpenZWaveController()
   : m_homeId(0),
   m_initFailed(false),
   m_nodesQueried(false),
   m_lastSuccessfullySentCommand(OpenZWave::Driver::ControllerCommand_None),
   m_handler(nullptr),
   m_configuration(0)
{
}


COpenZWaveController::~COpenZWaveController()
{
}

void onGlobalNotification(OpenZWave::Notification const* _notification, void* _context)
{
   try
   {
      YADOMS_LOG_CONFIGURE("ZWave");
      YADOMS_LOG(trace) << "OpenZWave notification : " << _notification->GetAsString();

      auto pPlugin = static_cast<COpenZWaveController *>(_context);
      if (pPlugin != nullptr)
         pPlugin->onNotification(_notification, _context);
   }
   catch (OpenZWave::OZWException& ex)
   {
      YADOMS_LOG(error) << "OpenZWave exception (OnGlobalNotification) : " << ex.what();
   }
   catch (std::exception& ex)
   {
      YADOMS_LOG(error) << "OpenZWave std::exception (OnGlobalNotification) : " << ex.what();
   }
   catch (...)
   {
      YADOMS_LOG(error) << "OpenZWave unknown exception (OnGlobalNotification)";
   }
}


void COpenZWaveController::configure(CZWaveConfiguration* configuration, shared::event::CEventHandler* handler)
{
   BOOST_ASSERT(configuration != NULL);
   BOOST_ASSERT(handler != NULL);
   boost::lock_guard<boost::mutex> lock(m_treeMutex);
   m_configuration = configuration;
   m_handler = handler;
}

IZWaveController::E_StartResult COpenZWaveController::start(boost::function0<void> checkStopRequested)
{
   try
   {
      // Create the OpenZWave Manager.
      // The first argument is the path to the config files (where the manufacturer_specific.xml file is located
      // The second argument is the path for saved Z-Wave network state and the log file.  If you leave it NULL 
      // the log file will appear in the program's working directory.
      boost::filesystem::path full_path(m_configuration->getPath());
      auto folder = full_path;
      folder /= "config";

      boost::filesystem::path dataFolder(m_configuration->getDataPath());
      if (!boost::filesystem::exists(dataFolder))
      {
         boost::system::error_code returnedError;

         boost::filesystem::create_directories(dataFolder, returnedError);
         if (returnedError)
         {
            //did not successfully create directories
            YADOMS_LOG(error) << "Fail to create folder : " << dataFolder.string();
         }
      }

      OpenZWave::Options::Create(folder.string(), dataFolder.string(), "");
      OpenZWave::Options::Get()->AddOptionInt("SaveLogLevel", OpenZWave::LogLevel_Info);
      OpenZWave::Options::Get()->AddOptionInt("PollInterval", 30000); // 30 seconds (can easily poll 30 values in this time; ~120 values is the effective limit for 30 seconds)
      OpenZWave::Options::Get()->AddOptionBool("IntervalBetweenPolls", true);
      OpenZWave::Options::Get()->AddOptionBool("ValidateValueChanges", true);
      OpenZWave::Options::Get()->AddOptionBool("Associate", true); // Enable automatic association of the controller with group one of every device.

      OpenZWave::Options::Get()->AddOptionBool("Logging", true);
      OpenZWave::Options::Get()->AddOptionString("LogFileName", "OZW.log", false);
      OpenZWave::Options::Get()->AddOptionBool("AppendLogFile", false);
      OpenZWave::Options::Get()->AddOptionBool("ConsoleOutput", false); //disable console output
      

      OpenZWave::Options::Get()->AddOptionBool("SuppressValueRefresh", false);
      OpenZWave::Options::Get()->AddOptionBool("EnableSIS", true);

      OpenZWave::Options::Get()->AddOptionBool("SaveConfiguration", true); // Save the XML configuration upon driver close.

      OpenZWave::Options::Get()->Lock();


      OpenZWave::Manager::Create();

      // Add a callback handler to the manager.  The second argument is a context that
      // is passed to the OnNotification method.  If the OnNotification is a method of
      // a class, the context would usually be a pointer to that class object, to
      // avoid the need for the notification handler to be a static.
      OpenZWave::Manager::Get()->AddWatcher(onGlobalNotification, this);

      // Add a Z-Wave Driver

      //this part wait infinitely for serial port open success (configuration can be changed by another thread)
      auto serialPortOpened = false;
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
            //fail to open : then unlock mutex to allow configuration to be changed, don't retry, OPenZWave have already a retry mechanism
            YADOMS_LOG(error) << "Fail to open serial port : " << m_configuration->getSerialPort();
            return kSerialPortError;
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
      while (!m_nodesQueried && !m_initFailed)
      {
         checkStopRequested();
         boost::this_thread::sleep(boost::posix_time::milliseconds(200));
      }

      if (!m_initFailed)
      {
         RequestConfigurationParameters();
         ExploreNetwork();
         OpenZWave::Manager::Get()->WriteConfig(m_homeId);

         m_handler->postEvent<std::string>(CZWave::kInternalStateChange, EZWaveInteralState::kRunning);
         return kSuccess;
      }

      YADOMS_LOG(error) << "Fail to initialize controller";
      return kControllerError;
   }
   catch (OpenZWave::OZWException& ex)
   {
      YADOMS_LOG(error) << "Fail to start OpenZWave controller : OpenZWave exception : " << ex.what();
   }
   catch (std::exception& ex)
   {
      YADOMS_LOG(error) << "Fail to start OpenZWave controller : std::exception : " << ex.what();
   }
   catch (...)
   {
      YADOMS_LOG(error) << "Fail to start OpenZWave controller unknown exception";
   }

   return kUnknownError;
}

void COpenZWaveController::stop()
{
   try
   {
      OpenZWave::Manager::Destroy();
   }
   catch (OpenZWave::OZWException& ex)
   {
      YADOMS_LOG(error) << "Fail to stop OpenZWave controller : OpenZWave exception : " << ex.what();
   }
   catch (std::exception& ex)
   {
      YADOMS_LOG(error) << "Fail to stop OpenZWave controller : std::exception : " << ex.what();
   }
   catch (...)
   {
      YADOMS_LOG(error) << "Fail to stop OpenZWave controller unknown exception";
   }
}


boost::shared_ptr<COpenZWaveNode> COpenZWaveController::getNode(OpenZWave::Notification const* _notification)
{
   auto const homeId = _notification->GetHomeId();
   auto const nodeId = _notification->GetNodeId();
   return getNode(homeId, nodeId);
}

boost::shared_ptr<COpenZWaveNode> COpenZWaveController::getNode(const uint32 homeId, const uint8 nodeId)
{
   for (auto it = m_nodes.begin(); it != m_nodes.end(); ++it)
   {
      if ((*it)->match(homeId, nodeId))
         return *it;
   }
   return boost::shared_ptr<COpenZWaveNode>();
}

void COpenZWaveController::RequestConfigurationParameters()
{
   for (auto it = m_nodes.begin(); it != m_nodes.end(); ++it)
   {
      OpenZWave::Manager::Get()->RequestAllConfigParams((*it)->getHomeId(), (*it)->getNodeId());
   }
}

void COpenZWaveController::ExploreNetwork()
{
   for (auto it = m_nodes.begin(); it != m_nodes.end(); ++it)
   {
      YADOMS_LOG(information) << "Exploring " << (*it)->getHomeId() << "." << (int)(*it)->getNodeId();

      unsigned char nGrp = OpenZWave::Manager::Get()->GetNumGroups((*it)->getHomeId(), (*it)->getNodeId());
      YADOMS_LOG(information) << "    Group count : " << nGrp;
      for (unsigned char i = 1; i <= nGrp; ++i)
      {
         std::string label = OpenZWave::Manager::Get()->GetGroupLabel((*it)->getHomeId(), (*it)->getNodeId(), i);
         YADOMS_LOG(information) << "        Group name : " << label;

         OpenZWave::InstanceAssociation* ia = NULL;
         int assocNb = OpenZWave::Manager::Get()->GetAssociations((*it)->getHomeId(), (*it)->getNodeId(), i, &ia);
         YADOMS_LOG(information) << "        Assoc count : " << assocNb;
         for (int k = 0; k < assocNb; k++)
         {
            YADOMS_LOG(information) << "          Association : " << (int)ia[k].m_nodeId << "#" << (int)ia[k].m_instance;
         }
      }
      
   }

}


void COpenZWaveController::onNotification(OpenZWave::Notification const* _notification, void* _context)
{
   // Must do this inside a critical section to avoid conflicts with the main thread
   boost::lock_guard<boost::mutex> lock(m_treeMutex);

   //get all global informations (for all notifications)
   auto vID = _notification->GetValueID();
   ECommandClass commandClass(static_cast<int>(vID.GetCommandClassId()));

   switch (_notification->GetType())
   {
   case OpenZWave::Notification::Type_ValueAdded:
   {
      auto node = getNode(_notification);

      if (node)
         node->registerKeyword(vID, m_configuration->getIncludeSystemKeywords());
      break;
   }

   case OpenZWave::Notification::Type_ValueRemoved:
      break;

   case OpenZWave::Notification::Type_ValueChanged:
   {
      auto node = getNode(_notification);
      setupValue(node, vID);
      if (node)
      {
         auto kw = node->getKeyword(vID, m_configuration->getIncludeSystemKeywords());
         if (kw)
         {
            auto historizedData = node->updateKeywordValue(vID, m_configuration->getIncludeSystemKeywords());
            auto d(boost::make_shared<CKeywordContainer>(COpenZWaveHelpers::GenerateDeviceName(node->getHomeId(), node->getNodeId()), historizedData));
            std::string vLabel = OpenZWave::Manager::Get()->GetValueLabel(vID);
            YADOMS_LOG(debug) << "OpenZWave notification [Type_ValueChanged] : node.instance=" << static_cast<int>(vID.GetNodeId()) << "." << static_cast<int>(vID.GetInstance()) << " => " << vLabel << "=" << historizedData->formatValue();

            if (m_handler != nullptr)
            {
               if (vID.GetGenre() == OpenZWave::ValueID::ValueGenre_Config)
                  m_handler->postEvent(CZWave::kUpdateConfiguration, d);
               else
                  m_handler->postEvent(CZWave::kUpdateKeyword, d);
            }
         }
      }
      break;
   }

   case OpenZWave::Notification::Type_ValueRefreshed:
   {
      // One of the node values has changed
      auto node = getNode(_notification);
      if (node)
      {
         auto kw = node->getKeyword(vID, m_configuration->getIncludeSystemKeywords());
         if (kw)
         {
            auto historizedData = node->updateKeywordValue(vID, m_configuration->getIncludeSystemKeywords());
            auto d(boost::make_shared<CKeywordContainer>(COpenZWaveHelpers::GenerateDeviceName(node->getHomeId(), node->getNodeId()), historizedData));
            std::string vLabel = OpenZWave::Manager::Get()->GetValueLabel(vID);

            YADOMS_LOG(debug) << "OpenZWave notification [Type_ValueRefreshed] : node.instance=" << static_cast<int>(vID.GetNodeId()) << "." << static_cast<int>(vID.GetInstance()) << " => " << vLabel << "=" << historizedData->formatValue() ;

            if (m_handler != nullptr)
            {
               if (vID.GetGenre() == OpenZWave::ValueID::ValueGenre_Config)
                  m_handler->postEvent(CZWave::kUpdateConfiguration, d);
               else
                  m_handler->postEvent(CZWave::kUpdateKeyword, d);
            }
         }
      }
      break;
   }

   case OpenZWave::Notification::Type_Group:
   {
      // One of the node's association groups has changed
      unsigned char groupIdx = _notification->GetGroupIdx();
      auto groupLabel = OpenZWave::Manager::Get()->GetGroupLabel(_notification->GetHomeId(), _notification->GetNodeId(), groupIdx);
      YADOMS_LOG(debug) << "Group change : id= " << (int)groupIdx << " label=" << groupLabel;
      break;
   }

   case OpenZWave::Notification::Type_NodeNew:
   {
      //A new node has been found (not already stored in zwcfg*.xml file)
      YADOMS_LOG(debug) << "A new node has been found (not already stored in zwcfg*.xml file) " << _notification->GetHomeId() << "." << (int)_notification->GetNodeId();
      break;
   }

   case OpenZWave::Notification::Type_NodeAdded:
   {
      //A new node has been added to OpenZWave's list.  This may be due to a device being added to the Z-Wave network, or because the application is initializing itself.
      YADOMS_LOG(debug) << "A new node has been added to OpenZWave's list.  This may be due to a device being added to the Z-Wave network, or because the application is initializing itself " << _notification->GetHomeId() << "." << (int)_notification->GetNodeId();
      if (getNode(_notification->GetHomeId(), _notification->GetNodeId()).get() == nullptr)
         m_nodes.push_back(boost::make_shared<COpenZWaveNode>(_notification->GetHomeId(), _notification->GetNodeId()));
      break;
   }

   case OpenZWave::Notification::Type_NodeProtocolInfo:
   {
      //Basic node information has been received, such as whether the node is a listening device, a routing device and its baud rate and basic, generic and specific types.It is after this notification that you can call Manager::GetNodeType to obtain a label containing the device description.
      auto node = getNode(_notification);
      if (node)
      {
         bool isrouting = OpenZWave::Manager::Get()->IsNodeRoutingDevice(node->getHomeId(), node->getNodeId());
         bool islistening = OpenZWave::Manager::Get()->IsNodeListeningDevice(node->getHomeId(), node->getNodeId());
         bool islisteningfrequent = OpenZWave::Manager::Get()->IsNodeFrequentListeningDevice(node->getHomeId(), node->getNodeId());
         bool isawake = OpenZWave::Manager::Get()->IsNodeAwake(node->getHomeId(), node->getNodeId());
         bool isbeaming = OpenZWave::Manager::Get()->IsNodeBeamingDevice(node->getHomeId(), node->getNodeId());
         bool isfailed = OpenZWave::Manager::Get()->IsNodeFailed(node->getHomeId(), node->getNodeId());
         bool issecurity = OpenZWave::Manager::Get()->IsNodeSecurityDevice(node->getHomeId(), node->getNodeId());
         bool iszwplus = OpenZWave::Manager::Get()->IsNodeZWavePlus(node->getHomeId(), node->getNodeId());
         
         auto sNodeType = OpenZWave::Manager::Get()->GetNodeType(node->getHomeId(), node->getNodeId());
         YADOMS_LOG(information) << "ZWave : NodeProtocolInfo : " << sNodeType;
         YADOMS_LOG(information) << "ZWave : NodeProtocolInfo : Routing node " << isrouting;
         YADOMS_LOG(information) << "ZWave : NodeProtocolInfo : Listening node " << islistening;
         YADOMS_LOG(information) << "ZWave : NodeProtocolInfo : Listening frequent (FLiRS) " << islisteningfrequent;
         YADOMS_LOG(information) << "ZWave : NodeProtocolInfo : Beaming node " << isbeaming;
         YADOMS_LOG(information) << "ZWave : NodeProtocolInfo : Awake node " << isawake;
         YADOMS_LOG(information) << "ZWave : NodeProtocolInfo : Failed node " << isfailed;
         YADOMS_LOG(information) << "ZWave : NodeProtocolInfo : Security node " << issecurity;
         YADOMS_LOG(information) << "ZWave : NodeProtocolInfo : ZWave+ node " << iszwplus;
      }
      break;
   }

   case OpenZWave::Notification::Type_NodeNaming:
   {
      auto nodeInfo = getNode(_notification);
      if (nodeInfo)
      {
         shared::CDataContainer d = getNodeInfo(nodeInfo->getHomeId(), nodeInfo->getNodeId());
         if (m_handler != nullptr)
            m_handler->postEvent(CZWave::kDeclareDevice, d);
      }
      break;
   }


   case OpenZWave::Notification::Type_NodeRemoved:
   {
      auto const homeId = _notification->GetHomeId();
      auto const nodeId = _notification->GetNodeId();

      for (auto i = m_nodes.begin(); i != m_nodes.end(); ++i)
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
      if (m_handler != nullptr)
         m_handler->postEvent<std::string>(CZWave::kInternalStateChange, EZWaveInteralState::kDriverReady);

      m_homeId = _notification->GetHomeId();
      break;
   }

   case OpenZWave::Notification::Type_DriverFailed:
   {
      if (m_handler != nullptr)
         m_handler->postEvent<std::string>(CZWave::kInternalStateChange, EZWaveInteralState::kDriverFailed);

      m_initFailed = true;
      break;
   }

   case OpenZWave::Notification::Type_AwakeNodesQueried:
   case OpenZWave::Notification::Type_AllNodesQueried:
   case OpenZWave::Notification::Type_AllNodesQueriedSomeDead:
   {
      /*
      YADOMS_LOG(information) << "ZWave : Check if refreshing node info is required";
      for (auto i = m_nodes.begin(); i != m_nodes.end(); ++i)
      {
         if (!OpenZWave::Manager::Get()->IsNodeInfoReceived((*i)->getHomeId(), (*i)->getNodeId()))
         {
            YADOMS_LOG(information) << "ZWave : Refresh node info for "<< (*i)->getHomeId() << "." << (int)(*i)->getNodeId();
            OpenZWave::Manager::Get()->RefreshNodeInfo((*i)->getHomeId(), (*i)->getNodeId());
         }
      }*/
      m_nodesQueried = true;
      break;
   }

   case OpenZWave::Notification::Type_ControllerCommand:
      switch (_notification->GetEvent())
      {
      case OpenZWave::Driver::ControllerState_Normal:
         if (m_handler != nullptr)
            m_handler->postEvent<std::string>(CZWave::kInternalStateChange, EZWaveInteralState::kRunning);
         break;
      case OpenZWave::Driver::ControllerState_Starting:
         break;
      case OpenZWave::Driver::ControllerState_Cancel:
         m_lastSuccessfullySentCommand = OpenZWave::Driver::ControllerCommand_None;
         if (m_handler != nullptr)
            m_handler->postEvent<std::string>(CZWave::kInternalStateChange, EZWaveInteralState::kRunning);

         break;
      case OpenZWave::Driver::ControllerState_Error:
         switch (_notification->GetNotification())
         {
         case OpenZWave::Driver::ControllerError_None:
            break;
         case OpenZWave::Driver::ControllerError_ButtonNotFound:
            YADOMS_LOG(error) << "Command controller error : ButtonNotFound";
            break;
         case OpenZWave::Driver::ControllerError_NodeNotFound:
            YADOMS_LOG(error) << "Command controller error : NodeNotFound";
            break;
         case OpenZWave::Driver::ControllerError_NotBridge:
            YADOMS_LOG(error) << "Command controller error : NotBridge";
            break;
         case OpenZWave::Driver::ControllerError_NotSUC:
            YADOMS_LOG(error) << "Command controller error : NotSUC";
            break;
         case OpenZWave::Driver::ControllerError_NotSecondary:
            YADOMS_LOG(error) << "Command controller error : NotSecondary";
            break;
         case OpenZWave::Driver::ControllerError_IsPrimary:
            YADOMS_LOG(error) << "Command controller error : IsPrimary";
            break;
         case OpenZWave::Driver::ControllerError_NotFound:
            YADOMS_LOG(error) << "Command controller error : NotFound";
            break;
         case OpenZWave::Driver::ControllerError_Busy:
            YADOMS_LOG(error) << "Command controller error : Busy";
            break;
         case OpenZWave::Driver::ControllerError_Failed:
            YADOMS_LOG(error) << "Command controller error : Failed";
            break;
         case OpenZWave::Driver::ControllerError_Disabled:
            YADOMS_LOG(error) << "Command controller error : Disabled";
            break;
         case OpenZWave::Driver::ControllerError_Overflow:
            YADOMS_LOG(error) << "Command controller error : Overflow";
            break;
         }
         break;
      case OpenZWave::Driver::ControllerState_Waiting:
         if (m_handler != nullptr)
         {
            switch (m_lastSuccessfullySentCommand)
            {
            case OpenZWave::Driver::ControllerCommand_AddDevice:
               m_handler->postEvent<std::string>(CZWave::kInternalStateChange, EZWaveInteralState::kWaitingInclusion);
               break;
            case OpenZWave::Driver::ControllerCommand_RemoveDevice:
               m_handler->postEvent<std::string>(CZWave::kInternalStateChange, EZWaveInteralState::kWaitingExclusion);
               break;
            default:
               m_handler->postEvent<std::string>(CZWave::kInternalStateChange, EZWaveInteralState::kWaiting);
               break;
            }
         }

         break;
      case OpenZWave::Driver::ControllerState_Sleeping:
         break;
      case OpenZWave::Driver::ControllerState_InProgress:
         break;
      case OpenZWave::Driver::ControllerState_Completed:
         m_lastSuccessfullySentCommand = OpenZWave::Driver::ControllerCommand_None;
         if (m_handler != nullptr)
            m_handler->postEvent<std::string>(CZWave::kInternalStateChange, EZWaveInteralState::kCompleted);
         break;
      case OpenZWave::Driver::ControllerState_Failed:
         break;
      case OpenZWave::Driver::ControllerState_NodeOK:
         break;
      case OpenZWave::Driver::ControllerState_NodeFailed:
         break;
      }
      break;

   case OpenZWave::Notification::Type_DriverReset:
      break;

   case OpenZWave::Notification::Type_NodeQueriesComplete:
   {
      auto nodeInfo = getNode(_notification);
      if (nodeInfo)
      {
         YADOMS_LOG(information) << "NodeQueriesComplete : " << nodeInfo->getHomeId() << ":" << (int)nodeInfo->getNodeId();

         shared::CDataContainer d = getNodeInfo(nodeInfo->getHomeId(), nodeInfo->getNodeId());
         if (m_handler != nullptr)
            m_handler->postEvent(CZWave::kDeclareDevice, d);
      }
      break;
   }

   case OpenZWave::Notification::Type_Notification:
   {
      auto nodeInfo = getNode(_notification);
      if (nodeInfo)
      {
         
         
         std::string sNodeName = (boost::format("%1%.%2%") % nodeInfo->getHomeId() % (int)nodeInfo->getNodeId()).str();
         shared::CDataContainer d;
         d.set("name", sNodeName);

         switch (_notification->GetNotification())
         {
         case OpenZWave::Notification::Code_MsgComplete: //Completed messages
            break;
         case OpenZWave::Notification::Code_Timeout: //Messages that timeout will send a Notification with this code.
            break;
         case OpenZWave::Notification::Code_NoOperation: //Report on NoOperation message sent completion
            break;
         case OpenZWave::Notification::Code_Awake: //Report when a sleeping node wakes up
            d.set("state", shared::plugin::yPluginApi::historization::EDeviceState::kActive);
            break;
         case OpenZWave::Notification::Code_Sleep: //Report when a node goes to sleep
            d.set("state", shared::plugin::yPluginApi::historization::EDeviceState::kAsleep);
            break;
         case OpenZWave::Notification::Code_Dead: //Report when a node is presumed dead
            d.set("state", shared::plugin::yPluginApi::historization::EDeviceState::kDead);
            break;
         case OpenZWave::Notification::Code_Alive: //Report when a node is revived
            d.set("state", shared::plugin::yPluginApi::historization::EDeviceState::kActive);
            break;
         }
         if (m_handler != nullptr && d.containsValue("state"))
            m_handler->postEvent(CZWave::kUpdateDeviceState, d);

      }
      break;
   }


   break;

   default:
      break;
   }
}

shared::CDataContainer COpenZWaveController::getNodeInfo(const uint32 homeId, const uint8 nodeId)
{
   std::string sNodeName = OpenZWave::Manager::Get()->GetNodeName(homeId, nodeId);
   std::string sNodeManufacturerId = OpenZWave::Manager::Get()->GetNodeManufacturerId(homeId, nodeId);
   std::string sNodeManufacturer = OpenZWave::Manager::Get()->GetNodeManufacturerName(homeId, nodeId);
   std::string sNodeProductName = OpenZWave::Manager::Get()->GetNodeProductName(homeId, nodeId);
   std::string sNodeProductType = OpenZWave::Manager::Get()->GetNodeProductType(homeId, nodeId);
   std::string sNodeProductId = OpenZWave::Manager::Get()->GetNodeProductId(homeId, nodeId);
   std::string sNodeLocation = OpenZWave::Manager::Get()->GetNodeLocation(homeId, nodeId);
   std::string sNodeType = OpenZWave::Manager::Get()->GetNodeType(homeId, nodeId);
   uint8 sNodeVersion = OpenZWave::Manager::Get()->GetNodeVersion(homeId, nodeId);
   auto id = COpenZWaveHelpers::GenerateDeviceName(homeId, nodeId);

   bool isrouting = OpenZWave::Manager::Get()->IsNodeRoutingDevice(homeId, nodeId);
   bool islistening = OpenZWave::Manager::Get()->IsNodeListeningDevice(homeId, nodeId);
   bool islisteningfrequent = OpenZWave::Manager::Get()->IsNodeFrequentListeningDevice(homeId, nodeId);
   bool isawake = OpenZWave::Manager::Get()->IsNodeAwake(homeId, nodeId);
   bool isbeaming = OpenZWave::Manager::Get()->IsNodeBeamingDevice(homeId, nodeId);
   bool isfailed = OpenZWave::Manager::Get()->IsNodeFailed(homeId, nodeId);
   bool issecurity = OpenZWave::Manager::Get()->IsNodeSecurityDevice(homeId, nodeId);
   bool iszwplus = OpenZWave::Manager::Get()->IsNodeZWavePlus(homeId, nodeId);

   uint8 sNodePlusType = iszwplus ?(OpenZWave::Manager::Get()->GetNodePlusType(homeId, nodeId)):0;

   std::string sNodeDeviceTypeString = OpenZWave::Manager::Get()->GetNodeDeviceTypeString(homeId, nodeId);
   std::string sNodeRole = OpenZWave::Manager::Get()->GetNodeRoleString(homeId, nodeId);

   YADOMS_LOG(information) << "ZWave : NodeInfo : id = " << id;
   YADOMS_LOG(information) << "ZWave : NodeInfo : name = " << sNodeName;
   YADOMS_LOG(information) << "ZWave : NodeInfo : manufacturerId = " << sNodeManufacturerId;
   YADOMS_LOG(information) << "ZWave : NodeInfo : manufacturer = " << sNodeManufacturer;
   YADOMS_LOG(information) << "ZWave : NodeInfo : productName = " << sNodeProductName;
   YADOMS_LOG(information) << "ZWave : NodeInfo : productType = " << sNodeProductType;
   YADOMS_LOG(information) << "ZWave : NodeInfo : productId = " << sNodeProductId;
   YADOMS_LOG(information) << "ZWave : NodeInfo : version = " << static_cast<int>(sNodeVersion);
   YADOMS_LOG(information) << "ZWave : NodeInfo : ZWave+ = " << (iszwplus ? "true" : "false");
   if(iszwplus)
      YADOMS_LOG(information) << "ZWave : NodeInfo : ZWave+type = " << sNodePlusType;
   YADOMS_LOG(information) << "ZWave : NodeInfo : Device type = " << sNodeDeviceTypeString;
   YADOMS_LOG(information) << "ZWave : NodeInfo : Role = " << sNodeRole;
   YADOMS_LOG(information) << "ZWave : NodeInfo : " << sNodeType;
   YADOMS_LOG(information) << "ZWave : NodeInfo : Routing node " << isrouting;
   YADOMS_LOG(information) << "ZWave : NodeInfo : Listening node " << islistening;
   YADOMS_LOG(information) << "ZWave : NodeInfo : Listening frequent (FLiRS) " << islisteningfrequent;
   YADOMS_LOG(information) << "ZWave : NodeInfo : Beaming node " << isbeaming;
   YADOMS_LOG(information) << "ZWave : NodeInfo : Awake node " << isawake;
   YADOMS_LOG(information) << "ZWave : NodeInfo : Failed node " << isfailed;
   YADOMS_LOG(information) << "ZWave : NodeInfo : Security node " << issecurity;
   YADOMS_LOG(information) << "ZWave : NodeInfo : ZWave+ node " << iszwplus;


   shared::CDataContainer d;
   d.set("name", id);

   if (sNodeName.empty())
      d.set("friendlyName", sNodeProductName);
   else
      d.set("friendlyName", sNodeName);

   shared::CDataContainer details;
   details.set("Manufacturer", sNodeManufacturer);
   details.set("ManufacturerId", sNodeManufacturerId);
   details.set("Product", sNodeProductName);
   details.set("ProductId", sNodeProductId);
   details.set("ProductType", sNodeProductType);
   details.set("Location", sNodeLocation);
   details.set("Type", sNodeType);
   details.set("Version", sNodeVersion);
   details.set("ZWave+Type", sNodePlusType);
   details.set("DeviceType", sNodeDeviceTypeString);
   details.set("Role", sNodeRole);

   details.set("IsRouting", isrouting);
   details.set("IsListening", isrouting);
   details.set("IsFLiRS", islisteningfrequent);
   details.set("IsBeaming", isbeaming);
   details.set("IsAwake", isawake);
   details.set("IsFailed", isfailed);
   details.set("IsSecurity", issecurity);
   details.set("IsZWave+", iszwplus);

   d.set("details", details);
   return d;
}

void COpenZWaveController::setupValue(boost::shared_ptr<COpenZWaveNode> node, OpenZWave::ValueID & vID)
{
   if (OpenZWave::Manager::Get()->IsNodeListeningDevice(node->getHomeId(), node->getNodeId()) ||
      OpenZWave::Manager::Get()->IsNodeFrequentListeningDevice(node->getHomeId(), node->getNodeId()))
   {
      //this is a connected device (or often wakeup)
      //make all values to be verified
      if (!OpenZWave::Manager::Get()->GetChangeVerified(vID))
      {
         OpenZWave::Manager::Get()->SetChangeVerified(vID, true);
      }
   }
   else
   {
      //this is an sleeping node (probably for battery life)
      //just get values
      if (OpenZWave::Manager::Get()->GetChangeVerified(vID))
      {
         OpenZWave::Manager::Get()->SetChangeVerified(vID, false);
      }
   }
}


void COpenZWaveController::sendCommand(const std::string& device, const std::string& keyword, const std::string& value)
{
   boost::lock_guard<boost::mutex> lock(m_treeMutex);

   uint32 homeId;
   uint8 nodeId;
   uint8 instance;
   ECommandClass keywordClass;

   COpenZWaveHelpers::RetreiveOpenZWaveIds(device, keyword, homeId, nodeId, instance);

   auto node = getNode(homeId, nodeId);
   if (node)
   {
      if (!node->sendCommand(keyword, value))
      {
         throw shared::exception::CException((boost::format("Fail to send command keyword[%1%] = %2% ") % keyword % value).str());
      }
   }
}

void COpenZWaveController::startInclusionMode()
{
   if (OpenZWave::Manager::Get()->AddNode(m_homeId))
   {
      m_lastSuccessfullySentCommand = OpenZWave::Driver::ControllerCommand_AddDevice;
   }
   else
   {
      YADOMS_LOG(error) << "Fail to start inclusion mode";
   }
}

void COpenZWaveController::startExclusionMode()
{
   if (OpenZWave::Manager::Get()->RemoveNode(m_homeId))
   {
      m_lastSuccessfullySentCommand = OpenZWave::Driver::ControllerCommand_RemoveDevice;
   }
   else
   {
      YADOMS_LOG(error) << "Fail to start exclusion mode";
   }
}

void COpenZWaveController::hardResetController()
{
   OpenZWave::Manager::Get()->ResetController(m_homeId);
}

void COpenZWaveController::softResetController()
{
   OpenZWave::Manager::Get()->SoftReset(m_homeId);
}

void COpenZWaveController::testNetwork(int count)
{
   OpenZWave::Manager::Get()->TestNetwork(m_homeId, count); //send count frames to each node
}

void COpenZWaveController::cancelCurrentCommand()
{
   if (OpenZWave::Manager::Get()->CancelControllerCommand(m_homeId))
   {
      m_lastSuccessfullySentCommand = OpenZWave::Driver::ControllerCommand_None;
   }
   else
   {
      YADOMS_LOG(error) << "Fail to cancel last command";
   }
}

void COpenZWaveController::healNetwork()
{
   OpenZWave::Manager::Get()->HealNetwork(m_homeId, true);
}

shared::CDataContainer COpenZWaveController::getNodeConfigurationSchema(const std::string & device)
{
   boost::lock_guard<boost::mutex> lock(m_treeMutex);

   uint32 homeId;
   uint8 nodeId;
   uint8 instance;
   ECommandClass keywordClass;

   COpenZWaveHelpers::RetreiveOpenZWaveIds(device, "", homeId, nodeId, instance);

   auto node = getNode(homeId, nodeId);
   if (node)
   {
      return node->getConfigurationSchema();
   }
   throw shared::exception::CException((boost::format("Fail to ask configuration for device %2% ") % device).str());
}

void COpenZWaveController::setNodeConfiguration(const std::string & device, const shared::CDataContainer &configuration)
{
   boost::lock_guard<boost::mutex> lock(m_treeMutex);

   uint32 homeId;
   uint8 nodeId;
   uint8 instance;
   ECommandClass keywordClass;

   COpenZWaveHelpers::RetreiveOpenZWaveIds(device, "", homeId, nodeId, instance);

   auto node = getNode(homeId, nodeId);
   if (node)
   {
      return node->setConfigurationValues(configuration);
   }
   throw shared::exception::CException((boost::format("Fail to ask configuration for device %2% ") % device).str());
}

void COpenZWaveController::updateNodeConfiguration(const std::string & device, const std::string& keyword, const std::string& value, shared::CDataContainer & configuration)
{
   boost::lock_guard<boost::mutex> lock(m_treeMutex);

   uint32 homeId;
   uint8 nodeId;
   uint8 instance;
   ECommandClass keywordClass;

   COpenZWaveHelpers::RetreiveOpenZWaveIds(device, "", homeId, nodeId, instance);

   auto node = getNode(homeId, nodeId);
   if (node)
   {
      return node->updateNodeConfiguration(keyword, value, configuration);
   }
   throw shared::exception::CException((boost::format("Fail to update configuration for device %2% ") % device).str());
}

IZWaveController::NodeListType & COpenZWaveController::getNodeList()
{
   return m_nodes;
}