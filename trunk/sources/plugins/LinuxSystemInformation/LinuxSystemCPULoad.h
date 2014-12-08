#pragma once

#include "ILoad.h"
#include <string>

//--------------------------------------------------------------
/// \brief	CPU Load of the Linux System
/// \note   return the CPU load under Linux Operating System
//--------------------------------------------------------------
class CLinuxSystemCPULoad : public ILoad
{
public:
   //--------------------------------------------------------------
   /// \brief	    Constructor
   /// \param[in] deviceId    The device
   //--------------------------------------------------------------
   CLinuxSystemCPULoad(const std::string & device);

   //--------------------------------------------------------------
   /// \brief	    Destructor
   //--------------------------------------------------------------
   virtual ~CLinuxSystemCPULoad();

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
   /// \brief	    Device
   //--------------------------------------------------------------
   const std::string m_device;

   //--------------------------------------------------------------
   /// \brief	    Temp variables
   //--------------------------------------------------------------
   unsigned long long lastTotalUser, lastTotalUserLow, lastTotalSys, lastTotalIdle;

   //--------------------------------------------------------------
   /// \brief	    Keyword
   //--------------------------------------------------------------
   yApi::historization::CLoad m_keyword;
};
