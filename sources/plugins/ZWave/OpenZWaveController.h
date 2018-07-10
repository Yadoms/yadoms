#pragma once

#include "IZWaveController.h"
#include <command_classes/CommandClasses.h>
#include <command_classes/CommandClass.h>
#include "OpenZWaveNode.h"
#include "OpenZWaveControllerCache.h"

class COpenZWaveController : public IZWaveController
{
public:
   //--------------------------------------------------------------
   /// \brief	    Constructor
   //--------------------------------------------------------------
   COpenZWaveController();

   //--------------------------------------------------------------
   /// \brief	    Destructor
   //--------------------------------------------------------------
   virtual ~COpenZWaveController();

   // IZWaveController implementation
   void configure(CZWaveConfiguration* configuration, shared::event::CEventHandler* handler) override;
   E_StartResult start(boost::function0<void> checkStoprequested) override;
   void stop() override;
   void sendCommand(const std::string& device, const std::string& keyword, const std::string& value) override;
   void startInclusionMode() override;
   void startExclusionMode() override;
   void hardResetController() override;
   void softResetController() override;
   void testNetwork(int count) override;
   void cancelCurrentCommand() override;
   void healNetwork() override;
   shared::CDataContainer getNodeConfigurationSchema(const std::string & device) override;
   void setNodeConfiguration(const std::string & device, const shared::CDataContainer &configuration) override;
   void updateNodeConfiguration(const std::string & device, const std::string& keyword, const std::string& value, shared::CDataContainer & configuration) override;
   NodeListType & getNodeList() override;
   // [END] IZWaveController implementation


   //-----------------------------------------------------------------------------
   /// \brief	Callback that is triggered when a value, group or node changes
   /// \param [in]    _notification    The openzwave notification
   /// \param [in]    content          The context
   //-----------------------------------------------------------------------------
   void onNotification(OpenZWave::Notification const* _notification, void* _context);

private:
   //-----------------------------------------------------------------------------
   /// \brief	Return the NodeInfo object associated with this notification
   //-----------------------------------------------------------------------------   
   boost::shared_ptr<COpenZWaveNode> getNode(OpenZWave::Notification const* _notification);

   //-----------------------------------------------------------------------------
   /// \brief	Return the NodeInfo object matching homeId and nodeId
   //-----------------------------------------------------------------------------   
   boost::shared_ptr<COpenZWaveNode> getNode(const uint32 homeId, const uint8 nodeId);

   //-----------------------------------------------------------------------------
   /// \brief	Return the Node information
   //-----------------------------------------------------------------------------   
   shared::CDataContainer getNodeInfo(const uint32 homeId, const uint8 nodeId);

   //-----------------------------------------------------------------------------
   /// \brief	Configure the value refresh mode depending on the device listening state
   //-----------------------------------------------------------------------------   
   void setupValue(boost::shared_ptr<COpenZWaveNode> node, OpenZWave::ValueID & vid);


   void manageDeviceValue(const std::string & deviceName, shared::CDataContainer &container);
   void manageDeviceState(const std::string & deviceName, shared::plugin::yPluginApi::historization::EDeviceState &container);
   void manageKeywordValue(const std::string & deviceName, boost::shared_ptr<CKeywordContainer> &container);

   //-----------------------------------------------------------------------------
   /// \brief	Pop data from cache and declare device and keyword on yadoms side (for a single device only)
   //-----------------------------------------------------------------------------   
   void cachePop(const std::string & deviceName);

   //-----------------------------------------------------------------------------
   /// \brief	Pop data from cache and declare device and keyword on yadoms side (for a all remaining devices)
   //-----------------------------------------------------------------------------   
   void cachePopAll();

   //-----------------------------------------------------------------------------
   /// \brief	Ask configuration parameters to each found node
   //-----------------------------------------------------------------------------   
   void RequestConfigurationParameters();

   void ExploreNetwork();

   //--------------------------------------------------------------
   /// \brief	   Mutex protecting the configuration content
   //--------------------------------------------------------------
   mutable boost::mutex m_treeMutex;


   //--------------------------------------------------------------
   /// \brief	   The current HomeId
   //--------------------------------------------------------------
   uint32 m_homeId;

   //--------------------------------------------------------------
   /// \brief	   Tells if initialization failed
   //--------------------------------------------------------------
   bool m_initFailed;

   //--------------------------------------------------------------
   /// \brief	   Tells id all nodes have been queried
   //--------------------------------------------------------------
   bool m_nodesQueried;

   //--------------------------------------------------------------
   /// \brief	   Contains the last successfully sent command to controller
   //--------------------------------------------------------------
   OpenZWave::Driver::ControllerCommand m_lastSuccessfullySentCommand;

   //--------------------------------------------------------------
   /// \brief	   The zwave node list
   //--------------------------------------------------------------
   NodeListType m_nodes;

   //--------------------------------------------------------------
   /// \brief	   The EventHandler of ZWave plugin
   //--------------------------------------------------------------
   shared::event::CEventHandler* m_handler;

   //--------------------------------------------------------------
   /// \brief	   The plugin configuration
   //--------------------------------------------------------------
   CZWaveConfiguration* m_configuration;

   //--------------------------------------------------------------
   /// \brief	   The cache
   //--------------------------------------------------------------
   COpenZWaveControllerCache m_cache;
};

