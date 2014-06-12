#pragma once

//--------------------------------------------------------------
/// \brief	Interface to the list of devices
//--------------------------------------------------------------
class IDeviceList
{  
public:
   //--------------------------------------------------------------
   /// \brief	                  Destructor
   //--------------------------------------------------------------
   virtual ~IDeviceList() {}

   //--------------------------------------------------------------
   /// \brief	                  Declare the device if not exists (do nothing if device already exists)
   /// \param[in] device         The device name
   /// \param[in] model          The device model
   /// \param[in] device         String (JSON) containing device parameters
   //--------------------------------------------------------------
   virtual void declareDevice(const std::string& device, const std::string& model, const std::string& parameters) = 0;

   //--------------------------------------------------------------
   /// \brief	                  Get the parameters of known device
   /// \param[in] device         The device name
   /// \return                   String (JSON) containing device parameters
   /// \throw                    shared::exception::InvalidParameter if device not found in cahe list and database 
   /// \note                     If device is not found in the cache list and found in database, cache list is updated
   //--------------------------------------------------------------
   virtual const std::string& getDeviceParameters(const std::string& device) = 0;
};



