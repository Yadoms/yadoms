#pragma once

#include "ILoad.h"
#include "sys/sysinfo.h"

//--------------------------------------------------------------
/// \brief	Memory Load of the Linux System
/// \note   return the memory load for Linux Operating System
//--------------------------------------------------------------
class CLinuxSystemMemoryLoad : public ILoad
{
public:
   //--------------------------------------------------------------
   /// \brief	    Constructor
   /// \param[in] deviceId    The device ID
   //--------------------------------------------------------------
   CLinuxSystemMemoryLoad(const std::string & device);

   //--------------------------------------------------------------
   /// \brief	    Destructor
   //--------------------------------------------------------------
   virtual ~CLinuxSystemMemoryLoad();

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

