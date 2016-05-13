#pragma once

#include "../ILoad.h"
#include <sys/times.h>
#include <sys/vtimes.h>

//--------------------------------------------------------------
/// \brief	CPU Load of the Yadoms Process
/// \note   return the CPU load of the Yadoms Process under Linux Operating System
//--------------------------------------------------------------
class CYadomsCPULoad : public ILoad
{
public:
   //--------------------------------------------------------------
   /// \brief	    Constructor
   /// \param[in] deviceId    The device ID
   //--------------------------------------------------------------
   explicit CYadomsCPULoad(const std::string & device);

   //--------------------------------------------------------------
   /// \brief	    Destructor
   //--------------------------------------------------------------
   virtual ~CYadomsCPULoad();

   // ILoad Implementation
   virtual void declareKeywords(boost::shared_ptr<yApi::IYPluginApi> api, shared::CDataContainer details);
   virtual void read();
   virtual void historizeData(boost::shared_ptr<yApi::IYPluginApi> api) const;
   virtual boost::shared_ptr<yApi::historization::IHistorizable> GetHistorizable() const;
   // [END] ILoad Implementation
   
private:

   //--------------------------------------------------------------
   /// \brief	    Device ID
   //--------------------------------------------------------------
   const std::string m_device;

   //--------------------------------------------------------------
   /// \brief	    Keyword string
   //--------------------------------------------------------------
   boost::shared_ptr<yApi::historization::CLoad> m_keyword;

   //--------------------------------------------------------------
   /// \brief	    Number of total ticks, System ticks, User Ticks
   //--------------------------------------------------------------
   clock_t lastCPU, lastSysCPU, lastUserCPU;

   //--------------------------------------------------------------
   /// \brief	    Number of processors
   //--------------------------------------------------------------
   int numProcessors;
};

