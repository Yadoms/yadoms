#include "stdafx.h"
#include "OpenZWaveController.h"
#include <shared/Log.h>
#include "FatalErrorException.h"


COpenZWaveController::COpenZWaveController()
   :m_homeId(0), m_initFailed(false), m_nodesQueried(false)
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

bool COpenZWaveController::start(CZWaveConfiguration & configuration, shared::event::CEventHandler & handler)
{

   try
   {
      // Create the OpenZWave Manager.
      // The first argument is the path to the config files (where the manufacturer_specific.xml file is located
      // The second argument is the path for saved Z-Wave network state and the log file.  If you leave it NULL 
      // the log file will appear in the program's working directory.
      boost::filesystem::path full_path(configuration.getPath());
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
      std::string realSerialPort = (boost::format("\\\\.\\%1%") % configuration.getSerialPort()).str();
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

            YADOMS_LOG(info) << "Found new zwave node";
            YADOMS_LOG(info) << "    Name : " << sNodeName;
            YADOMS_LOG(info) << "    Manufacturer : " << sNodeManufacturer;
            YADOMS_LOG(info) << "    Product : " << sNodeProductName;
            YADOMS_LOG(info) << "    ProductId : " << sNodeProductId;
            YADOMS_LOG(info) << "    ProductType : " << sNodeProductType;
            YADOMS_LOG(info) << "    Location : " << sNodeLocation;
            YADOMS_LOG(info) << "    Type : " << sNodeType;
         }

         /*
         OpenZWave::Driver::DriverData data;
         OpenZWave::Manager::Get()->GetDriverStatistics(m_homeId, &data);
         printf("SOF: %d ACK Waiting: %d Read Aborts: %d Bad Checksums: %d\n", data.m_SOFCnt, data.m_ACKWaiting, data.m_readAborts, data.m_badChecksum);
         printf("Reads: %d Writes: %d CAN: %d NAK: %d ACK: %d Out of Frame: %d\n", data.m_readCnt, data.m_writeCnt, data.m_CANCnt, data.m_NAKCnt, data.m_ACKCnt, data.m_OOFCnt);
         printf("Dropped: %d Retries: %d\n", data.m_dropped, data.m_retries);
         */
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
   OpenZWave::Options::Destroy();
}










//-----------------------------------------------------------------------------
// <GetNodeInfo>
// Return the NodeInfo object associated with this notification
//-----------------------------------------------------------------------------
COpenZWaveController::NodeInfo* COpenZWaveController::GetNodeInfo(OpenZWave::Notification const* _notification)
{
   uint32 const homeId = _notification->GetHomeId();
   uint8 const nodeId = _notification->GetNodeId();
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


void COpenZWaveController::OnNotification(OpenZWave::Notification const* _notification, void* _context)
{
   // Must do this inside a critical section to avoid conflicts with the main thread
   boost::lock_guard<boost::mutex> lock(m_treeMutex);

   switch (_notification->GetType())
   {
   case OpenZWave::Notification::Type_ValueAdded:
   {
      if (NodeInfo* nodeInfo = GetNodeInfo(_notification))
      {
         // Add the new value to our list
         nodeInfo->m_values.push_back(_notification->GetValueID());
      }
      break;
   }

   case OpenZWave::Notification::Type_ValueRemoved:
   {
      if (NodeInfo* nodeInfo = GetNodeInfo(_notification))
      {
         // Remove the value from out list
         for (std::vector<OpenZWave::ValueID>::iterator it = nodeInfo->m_values.begin(); it != nodeInfo->m_values.end(); ++it)
         {
            if ((*it) == _notification->GetValueID())
            {
               nodeInfo->m_values.erase(it);
               break;
            }
         }
      }
      break;
   }

   case OpenZWave::Notification::Type_ValueChanged:
   {
      // One of the node values has changed
      if (NodeInfo* nodeInfo = GetNodeInfo(_notification))
      {
         nodeInfo = nodeInfo;		// placeholder for real action
      }
      break;
   }

   case OpenZWave::Notification::Type_Group:
   {
      // One of the node's association groups has changed
      if (NodeInfo* nodeInfo = GetNodeInfo(_notification))
      {
         nodeInfo = nodeInfo;		// placeholder for real action
      }
      break;
   }

   case OpenZWave::Notification::Type_NodeAdded:
   {
      // Add the new node to our list
      NodeInfo* nodeInfo = new NodeInfo();
      nodeInfo->m_homeId = _notification->GetHomeId();
      nodeInfo->m_nodeId = _notification->GetNodeId();
      nodeInfo->m_polled = false;
      m_nodes.push_back(nodeInfo);
      break;
   }

   case OpenZWave::Notification::Type_NodeRemoved:
   {
      // Remove the node from our list
      uint32 const homeId = _notification->GetHomeId();
      uint8 const nodeId = _notification->GetNodeId();
      for (std::vector<NodeInfo*>::iterator it = m_nodes.begin(); it != m_nodes.end(); ++it)
      {
         NodeInfo* nodeInfo = *it;
         if ((nodeInfo->m_homeId == homeId) && (nodeInfo->m_nodeId == nodeId))
         {
            m_nodes.erase(it);
            delete nodeInfo;
            break;
         }
      }
      break;
   }

   case OpenZWave::Notification::Type_NodeEvent:
   {
      // We have received an event from the node, caused by a
      // basic_set or hail message.
      if (NodeInfo* nodeInfo = GetNodeInfo(_notification))
      {
         nodeInfo = nodeInfo;		// placeholder for real action
      }
      break;
   }

   case OpenZWave::Notification::Type_PollingDisabled:
   {
      if (NodeInfo* nodeInfo = GetNodeInfo(_notification))
      {
         nodeInfo->m_polled = false;
      }
      break;
   }

   case OpenZWave::Notification::Type_PollingEnabled:
   {
      if (NodeInfo* nodeInfo = GetNodeInfo(_notification))
      {
         nodeInfo->m_polled = true;
      }
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



