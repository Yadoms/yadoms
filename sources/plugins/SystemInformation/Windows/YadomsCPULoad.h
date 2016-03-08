#pragma once

#include "../ILoad.h"
#include <shared/plugin/yPluginApi/IYPluginApi.h>


// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

//--------------------------------------------------------------
/// \brief	CPU Load of the Yadoms Process
/// \note   return the CPU load of the Yadoms Process under Windows Operating System
//--------------------------------------------------------------
class CYadomsCPULoad : public ILoad
{
public:
   //--------------------------------------------------------------
   /// \brief	    Constructor
   /// \param[in] device    The device
   //--------------------------------------------------------------
   explicit CYadomsCPULoad(const std::string & device);

   //--------------------------------------------------------------
   /// \brief	    Destructor
   //--------------------------------------------------------------
   virtual ~CYadomsCPULoad();

   // ILoad Implementation
   virtual void declareKeywords(boost::shared_ptr<yApi::IYPluginApi> context);
   virtual void read();
   virtual void historizeData(boost::shared_ptr<yApi::IYPluginApi> context) const;
   virtual boost::shared_ptr<yApi::historization::IHistorizable> GetHistorizable() const;
   // [END] ILoad Implementation

protected:
   //--------------------------------------------------------------
   /// \brief	    Initialization
   //--------------------------------------------------------------
   void Initialize();

private:
   //--------------------------------------------------------------
   /// \brief	    Device
   //--------------------------------------------------------------
   const std::string m_device;

   //--------------------------------------------------------------
   /// \brief	    Number of total ticks, System ticks, User Ticks
   //--------------------------------------------------------------
   unsigned __int64 m_lastCPU, m_lastSysCPU, m_lastUserCPU;

   //--------------------------------------------------------------
   /// \brief	    Number of processors
   //--------------------------------------------------------------
   int m_numProcessors;

   //--------------------------------------------------------------
   /// \brief	    Handle of Yadoms
   //--------------------------------------------------------------
   HANDLE m_self;

   //--------------------------------------------------------------
   /// \brief	    Keyword string
   //--------------------------------------------------------------
   boost::shared_ptr<yApi::historization::CLoad> m_keyword;

   //--------------------------------------------------------------
   /// \brief	    Initialization ok
   //--------------------------------------------------------------
   bool m_InitializeOk;
};

