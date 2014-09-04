#pragma once

#include "ILoad.h"

// Shortcut to yadomsApi namespace
namespace yApi = shared::plugin::yadomsApi;

//--------------------------------------------------------------
/// \brief	Memory Load of the Windows System
/// \note   return the memory load for Windows Operating System
//--------------------------------------------------------------
class CWindowsSystemMemoryLoad : public ILoad
{
public:
   //--------------------------------------------------------------
   /// \brief	    Constructor
   /// \param[in] deviceId    The device ID
   //--------------------------------------------------------------
   CWindowsSystemMemoryLoad(const std::string & deviceId);

   //--------------------------------------------------------------
   /// \brief	    Initialize the configuration of variable
   //--------------------------------------------------------------
   virtual void Initialize();

   //--------------------------------------------------------------
   /// \brief	    Destructor
   //--------------------------------------------------------------
   virtual ~CWindowsSystemMemoryLoad();

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
   /// \brief	    Returns read (computed) memory load
   /// \return     Temperature in �C
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
   double m_memoryLoad;

   //--------------------------------------------------------------
   /// \brief	    Capacity string
   //--------------------------------------------------------------
   const std::string m_Capacity;

   //--------------------------------------------------------------
   /// \brief	    Keyword string
   //--------------------------------------------------------------
   const std::string m_Keyword;
};

