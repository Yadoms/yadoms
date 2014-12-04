#pragma once

#include "ILoad.h"
#include "sys/sysinfo.h"

//--------------------------------------------------------------
/// \brief	Memory Load of the Linux System
/// \note   return the memory load for Linux Operating System
//--------------------------------------------------------------
class CRaspBerryPiSystemMemoryLoad : public ILoad
{
public:
   //--------------------------------------------------------------
   /// \brief	    Constructor
   /// \param[in] deviceId    The device ID
   //--------------------------------------------------------------
   CRaspBerryPiSystemMemoryLoad(const std::string & device);

   //--------------------------------------------------------------
   /// \brief	    Destructor
   //--------------------------------------------------------------
   virtual ~CRaspBerryPiSystemMemoryLoad();

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
   /// \brief	    Memory Information Structure
   //--------------------------------------------------------------
   struct sysinfo memInfo;

   //--------------------------------------------------------------
   /// \brief	    Keyword
   //--------------------------------------------------------------
   yApi::historization::CLoad m_keyword;

};

