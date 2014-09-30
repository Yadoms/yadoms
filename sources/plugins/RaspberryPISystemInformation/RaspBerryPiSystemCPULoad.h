#pragma once

#include "ILoad.h"
#include "LoadKeyword.h"
#include <string>

//--------------------------------------------------------------
/// \brief	CPU Load of the Raspberry Pi System
/// \note   return the CPU load under Raspberry Pi Operating System
//--------------------------------------------------------------
class CRaspBerryPiSystemCPULoad : public ILoad
{
public:
   //--------------------------------------------------------------
   /// \brief	    Constructor
   /// \param[in] deviceId    The device
   //--------------------------------------------------------------
   CRaspBerryPiSystemCPULoad(const std::string & device);

   //--------------------------------------------------------------
   /// \brief	    Destructor
   //--------------------------------------------------------------
   virtual ~CRaspBerryPiSystemCPULoad();

   // ILoad Implementation
   virtual void declareKeywords(boost::shared_ptr<yApi::IYadomsApi> context);
   virtual void read();
   virtual void historizeData(boost::shared_ptr<yApi::IYadomsApi> context) const;
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
   CLoadKeyword m_keyword;   
};
