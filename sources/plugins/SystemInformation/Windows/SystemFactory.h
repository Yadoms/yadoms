#pragma once

#include <shared/plugin/yPluginApi/IYPluginApi.h>
#include "MemoryLoad.h"
#include "CPULoad.h"
#include "YadomsCPULoad.h"
#include "DiskUsage.h"
#include "DisksList.h"
#include "RAMProcessMemory.h"
#include "VirtualProcessMemory.h"
#include "../ISIConfiguration.h"

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

//--------------------------------------------------------------
/// \brief	Class where all Keywords are created and updated
/// \note   
//--------------------------------------------------------------
class CSystemFactory
{
public:
   //--------------------------------------------------------------
   /// \brief	    Constructor
   /// \param[in] context             yPluginApi API
   /// \param[in] device              The device name
   /// \param[in] ISIConfiguration    The plugin configuration
   /// \param[in] details             Details information for keyword
   //--------------------------------------------------------------
   CSystemFactory(boost::shared_ptr<yApi::IYPluginApi> context, 
	              const std::string & device,
				  const ISIConfiguration& configuration, 
				  shared::CDataContainer details);

   //--------------------------------------------------------------
   /// \brief	    Destructor
   //--------------------------------------------------------------
   virtual ~CSystemFactory();

   //--------------------------------------------------------------
   /// \brief	    OnSpeedUpdate
   /// \param[in] context             yPluginApi API
   //--------------------------------------------------------------
   void OnSpeedUpdate ( boost::shared_ptr<yApi::IYPluginApi> context );

   //--------------------------------------------------------------
   /// \brief	    OnSlowUpdate
   /// \param[in] context             yPluginApi API
   /// \param[in] ISIConfiguration    The configuration state
   //--------------------------------------------------------------
   void OnSlowUpdate  ( boost::shared_ptr<yApi::IYPluginApi> context, 
	                    const ISIConfiguration& configuration );

   //--------------------------------------------------------------
   /// \brief	    OnConfigurationUpdate
   /// \param[in] context             yPluginApi API
   /// \param[in] ISIConfiguration    The new configuration
   /// \param[in] details             Details information for keyword
   //--------------------------------------------------------------
   void OnConfigurationUpdate ( boost::shared_ptr<yApi::IYPluginApi> context, 
	                            const ISIConfiguration& configuration, shared::CDataContainer details );

private:
   //--------------------------------------------------------------
   /// \brief	    Plugin name
   //--------------------------------------------------------------
   std::string m_PluginName;

   //--------------------------------------------------------------
   /// \brief	    Keyword
   //--------------------------------------------------------------
   CMemoryLoad           m_MemoryLoad;
   CCPULoad              m_CPULoad;
   CYadomsCPULoad        m_YadomsCPULoad;
   CRAMProcessMemory     m_RAMProcessMemory;
   CVirtualProcessMemory m_VirtualProcessMemory;

   //--------------------------------------------------------------
   /// \brief	    List of keywords for disks
   //--------------------------------------------------------------
   std::vector<CDiskUsage> DiskUsageList;
};

