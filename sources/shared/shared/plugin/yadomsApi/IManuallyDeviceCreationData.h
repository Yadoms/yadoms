#pragma once


namespace shared { namespace plugin { namespace yadomsApi
{
   //-----------------------------------------------------
   ///\brief The API used for manually created devices
   //-----------------------------------------------------
   class IManuallyDeviceCreationData
   {
   public:
      //-----------------------------------------------------
      ///\brief               Destructor
      //-----------------------------------------------------
      virtual ~IManuallyDeviceCreationData() {}

      //-----------------------------------------------------
      ///\brief               Get the device
      ///\return              The device to create
      //-----------------------------------------------------
      virtual const std::string& getDevice() const = 0;

      //-----------------------------------------------------
      ///\brief               Get the keyword
      ///\return              The keyword to create
      //-----------------------------------------------------
      virtual const std::string& getKeyword() const = 0;

      //-----------------------------------------------------
      ///\brief               Get the capacity
      ///\return              The capacity used for this device
      //-----------------------------------------------------
      virtual const std::string& getCapcity() const = 0;

      //-----------------------------------------------------
      ///\brief               Get the device parameters
      ///\return              The device parameters, as JSON string
      //-----------------------------------------------------
      virtual const std::string& getParameters() const = 0;

      //-----------------------------------------------------
      ///\brief Describe the command as string
      ///\return Human-readable string representing the command
      //-----------------------------------------------------
      virtual const std::string toString() const = 0;//TODO utile ?

};
	
} } } // namespace shared::plugin::yadomsApi	
	
	