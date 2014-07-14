#pragma once

#include "ILoad.h"

// Shortcut to yadomsApi namespace
namespace yApi = shared::plugin::yadomsApi;

//--------------------------------------------------------------
/// \brief	CPU Load of the Yadoms Process
/// \note   return the CPU load of the Yadoms Process under Windows Operating System
//--------------------------------------------------------------
class CWindowsSystemYadomsCPULoad : public ILoad
{
public:
   //--------------------------------------------------------------
   /// \brief	    Constructor
   /// \param[in] deviceId    The device ID
   //--------------------------------------------------------------
   CWindowsSystemYadomsCPULoad(const std::string & deviceId);

   //--------------------------------------------------------------
   /// \brief	    Initialize the configuration of variable
   //--------------------------------------------------------------
   virtual void Initialize();

   //--------------------------------------------------------------
   /// \brief	    Destructor
   //--------------------------------------------------------------
   virtual ~CWindowsSystemYadomsCPULoad();

   //--------------------------------------------------------------
   /// \brief	    Returns the sensor device ID
   /// \return     Device ID
   //--------------------------------------------------------------
   virtual const std::string& getDeviceId() const;

   //--------------------------------------------------------------
   /// \brief	            Declare the device and its associated keywords
   /// \param[in] context  YadomsApi context to which declare the device
   //--------------------------------------------------------------
   void declareDevice(boost::shared_ptr<yApi::IYadomsApi> context);

   //--------------------------------------------------------------
   /// \brief	            Send all sensor data to Yadoms
   /// \param[in] context  YadomsApi context to which historize data
   //--------------------------------------------------------------
   void historizeData(boost::shared_ptr<yApi::IYadomsApi> context) const;

   //--------------------------------------------------------------
   /// \brief	    Returns read (computed) CPU load of the Yadoms Process
   /// \return     CPU Load in %
   //--------------------------------------------------------------
   virtual double getValue();

   //--------------------------------------------------------------
   /// \brief	    Returns the Capacity String
   /// \return     Device ID
   //--------------------------------------------------------------
   const std::string& getCapacity() const;

   //--------------------------------------------------------------
   /// \brief	    Returns the Keyword String
   /// \return     Device ID
   //--------------------------------------------------------------
   const std::string& getKeyword() const;

private:

   //--------------------------------------------------------------
   /// \brief	    Device ID
   //--------------------------------------------------------------
   const std::string m_deviceId;

   //--------------------------------------------------------------
   /// \brief	    Memory Load in %
   //--------------------------------------------------------------
   double m_CPULoad;

   //--------------------------------------------------------------
   /// \brief	    Number of total ticks, System ticks, User Ticks
   //--------------------------------------------------------------
   unsigned __int64 lastCPU, lastSysCPU, lastUserCPU;

   //--------------------------------------------------------------
   /// \brief	    Number of processors
   //--------------------------------------------------------------
   int numProcessors;

   //--------------------------------------------------------------
   /// \brief	    Handle of Yadoms
   //--------------------------------------------------------------
   HANDLE self;

   //--------------------------------------------------------------
   /// \brief	    Capacity string
   //--------------------------------------------------------------
   const std::string m_Capacity;

   //--------------------------------------------------------------
   /// \brief	    Keyword string
   //--------------------------------------------------------------
   const std::string m_Keyword;

   //--------------------------------------------------------------
   /// \brief	    Initialization ok
   //--------------------------------------------------------------
   bool m_InitializeOk;
};

