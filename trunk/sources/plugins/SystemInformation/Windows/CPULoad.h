#pragma once

#include "../ILoad.h"
#include <shared/plugin/yPluginApi/IYPluginApi.h>
#include <pdh.h>


//--------------------------------------------------------------
/// \brief	CPU Load of the Windows System
/// \note   return the CPU load under Windows Operating System
//--------------------------------------------------------------
class CCPULoad : public ILoad
{
public:
   //--------------------------------------------------------------
   /// \brief	    Constructor
   /// \param[in] deviceId    The device
   //--------------------------------------------------------------
   CCPULoad(const std::string & device);

   //--------------------------------------------------------------
   /// \brief	    Destructor
   //--------------------------------------------------------------
   virtual ~CCPULoad();

   // ILoad Implementation
   virtual void declareKeywords(boost::shared_ptr<yApi::IYPluginApi> context);
   virtual void read();
   virtual void historizeData(boost::shared_ptr<yApi::IYPluginApi> context) const;
   // [END] ILoad Implementation

protected:
   //--------------------------------------------------------------
   /// \brief	    Initialization
   //--------------------------------------------------------------
   void Initialize();

private:
   //--------------------------------------------------------------
   /// \brief	    Device ID
   //--------------------------------------------------------------
   const std::string m_device;

   //--------------------------------------------------------------
   /// \brief	    The Query for the CPU Load
   //--------------------------------------------------------------
   PDH_HQUERY m_cpuQuery;

   //--------------------------------------------------------------
   /// \brief	    Counter TOTAL CPU Load
   //--------------------------------------------------------------
   PDH_HCOUNTER m_cpuTotal;

   //--------------------------------------------------------------
   /// \brief	    Keyword
   //--------------------------------------------------------------
   yApi::historization::CLoad m_keyword;

   //--------------------------------------------------------------
   /// \brief	    Initialization ok
   //--------------------------------------------------------------
   bool m_InitializeOk;
};