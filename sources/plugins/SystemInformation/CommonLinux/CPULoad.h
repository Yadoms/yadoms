#pragma once

#include "../ILoad.h"
#include <shared/plugin/yPluginApi/IYPluginApi.h>

//--------------------------------------------------------------
/// \brief	CPU Load of the Linux System
/// \note   return the CPU load under Linux Operating System
//--------------------------------------------------------------
class CCPULoad : public ILoad
{
public:
   //--------------------------------------------------------------
   /// \brief	    Constructor
   /// \param[in] keywordName The keyword name
   //--------------------------------------------------------------
   explicit CCPULoad(const std::string& keywordName);

   //--------------------------------------------------------------
   /// \brief	    Destructor
   //--------------------------------------------------------------
   virtual ~CCPULoad();

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
   /// \brief	    Read all needed information
   //--------------------------------------------------------------
   void ReadFromFile(unsigned long long *dtotalUser,
                     unsigned long long *dtotalUserLow,
                     unsigned long long *dtotalSys,
                     unsigned long long *dtotalIdle,
                     unsigned long long *dtotalIowait,
                     unsigned long long *dtotalIrq,
                     unsigned long long *dtotalSoftIrq);

   //--------------------------------------------------------------
   /// \brief	    Temp variables
   //--------------------------------------------------------------
   unsigned long long m_lastTotalUser; 
   unsigned long long m_lastTotalUserLow; 
   unsigned long long m_lastTotalSys; 
   unsigned long long m_lastTotalIdle;
   unsigned long long m_lastTotalIowait; 
   unsigned long long m_lastTotalIrq;   
   unsigned long long m_lastTotalSoftIrq;

   //--------------------------------------------------------------
   /// \brief	    Keyword
   //--------------------------------------------------------------
   boost::shared_ptr<yApi::historization::CLoad> m_keyword;
};
