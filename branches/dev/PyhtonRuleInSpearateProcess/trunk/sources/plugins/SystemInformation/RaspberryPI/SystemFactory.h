#pragma once

#include <shared/plugin/yPluginApi/IYPluginApi.h>
#include <MemoryLoad.h>
#include <CPULoad.h>
#include <YadomsCPULoad.h>
#include <DiskUsage.h>
#include <DisksList.h>
#include "TemperatureSensor.h"
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
   /// \param[in] device    The device
   //--------------------------------------------------------------
   CSystemFactory(boost::shared_ptr<yApi::IYPluginApi> context, const std::string & device, const ISIConfiguration& configuration);

   //--------------------------------------------------------------
   /// \brief	    Destructor
   //--------------------------------------------------------------
   virtual ~CSystemFactory();

   void OnSpeedUpdate ( boost::shared_ptr<yApi::IYPluginApi> context , const ISIConfiguration& configuration);
   void OnSlowUpdate  ( boost::shared_ptr<yApi::IYPluginApi> context , const ISIConfiguration& configuration);
   void OnConfigurationUpdate ( boost::shared_ptr<yApi::IYPluginApi> context, const ISIConfiguration& configuration );

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
   CTemperatureSensor    m_TemperatureSensor;
   CRAMProcessMemory     m_RAMProcessMemory;
   CVirtualProcessMemory m_VirtualProcessMemory;

   std::vector<boost::shared_ptr<CDiskUsage> > m_DiskUsageList;
};

