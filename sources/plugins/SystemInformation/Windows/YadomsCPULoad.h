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
   /// \param[in] keywordName The keyword name
   //--------------------------------------------------------------
   explicit CYadomsCPULoad(const std::string& keywordName);

   //--------------------------------------------------------------
   /// \brief	    Destructor
   //--------------------------------------------------------------
   virtual ~CYadomsCPULoad();

   // ILoad Implementation
   void read() override;

   boost::shared_ptr<const yApi::historization::IHistorizable> historizable() const override
   {
      return m_keyword;
   }

   // [END] ILoad Implementation

protected:
   //--------------------------------------------------------------
   /// \brief	    Initialization
   //--------------------------------------------------------------
   void Initialize();

private:
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

