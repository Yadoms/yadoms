#pragma once

#include "../ILoad.h"
#include <string>

//--------------------------------------------------------------
/// \brief	CPU Load of the Linux System
/// \note   return the CPU load under Linux Operating System
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
   /// \brief	    Read all needed information
   //--------------------------------------------------------------
   void ReadFromFile(unsigned long long *dtotalUser,
                     unsigned long long *dtotalUserLow,
                     unsigned long long *dtotalSys,
                     unsigned long long *dtotalIdle);

   //--------------------------------------------------------------
   /// \brief	    Device
   //--------------------------------------------------------------
   const std::string m_device;

   //--------------------------------------------------------------
   /// \brief	    Temp variables
   //--------------------------------------------------------------
   unsigned long long m_lastTotalUser; 
   unsigned long long m_lastTotalUserLow; 
   unsigned long long m_lastTotalSys; 
   unsigned long long m_lastTotalIdle;

   //--------------------------------------------------------------
   /// \brief	    Keyword
   //--------------------------------------------------------------
   yApi::historization::CLoad m_keyword;
};
