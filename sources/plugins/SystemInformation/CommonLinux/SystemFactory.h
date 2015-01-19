#pragma once

#include <shared/plugin/yPluginApi/IYPluginApi.h>
#include "MemoryLoad.h"
#include "CPULoad.h"
#include "YadomsCPULoad.h"
#include "DiskUsage.h"
#include "DisksList.h"


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
   CSystemFactory(boost::shared_ptr<yApi::IYPluginApi> context, const std::string & device);

   //--------------------------------------------------------------
   /// \brief	    Destructor
   //--------------------------------------------------------------
   virtual ~CSystemFactory();

   void OnSpeedUpdate ( boost::shared_ptr<yApi::IYPluginApi> context );
   void OnSlowUpdate  ( boost::shared_ptr<yApi::IYPluginApi> context );

private:
   //--------------------------------------------------------------
   /// \brief	    Device name
   //--------------------------------------------------------------
   //const std::string m_device;

   //--------------------------------------------------------------
   /// \brief	    Keyword
   //--------------------------------------------------------------
   CMemoryLoad    m_MemoryLoad;
   CCPULoad       m_CPULoad;
   CYadomsCPULoad m_YadomsCPULoad;

   std::vector<boost::shared_ptr<CDiskUsage> > m_DiskUsageList;
};

