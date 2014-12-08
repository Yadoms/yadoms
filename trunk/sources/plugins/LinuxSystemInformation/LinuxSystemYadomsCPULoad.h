#pragma once

#include "ILoad.h"
#include "sys/times.h"
#include "sys/vtimes.h"

//--------------------------------------------------------------
/// \brief	CPU Load of the Yadoms Process
/// \note   return the CPU load of the Yadoms Process under Linux Operating System
//--------------------------------------------------------------
class CLinuxSystemYadomsCPULoad : public ILoad
{
public:
   //--------------------------------------------------------------
   /// \brief	    Constructor
   /// \param[in] deviceId    The device ID
   //--------------------------------------------------------------
   CLinuxSystemYadomsCPULoad(const std::string & device);

   //--------------------------------------------------------------
   /// \brief	    Destructor
   //--------------------------------------------------------------
   virtual ~CLinuxSystemYadomsCPULoad();

   // ILoad Implementation
   virtual void declareKeywords(boost::shared_ptr<yApi::IYPluginApi> context);
   virtual void read();
   virtual void historizeData(boost::shared_ptr<yApi::IYPluginApi> context) const;
   // [END] ILoad Implementation
   
private:

   //--------------------------------------------------------------
   /// \brief	    Device ID
   //--------------------------------------------------------------
   const std::string m_device;

   //--------------------------------------------------------------
   /// \brief	    Keyword string
   //--------------------------------------------------------------
   yApi::historization::CLoad m_keyword;

   //--------------------------------------------------------------
   /// \brief	    Number of total ticks, System ticks, User Ticks
   //--------------------------------------------------------------
   clock_t lastCPU, lastSysCPU, lastUserCPU;

   //--------------------------------------------------------------
   /// \brief	    Number of processors
   //--------------------------------------------------------------
   int numProcessors;
};

