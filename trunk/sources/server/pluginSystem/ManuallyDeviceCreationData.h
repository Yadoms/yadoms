#pragma once
#include <shared/plugin/yadomsApi/IManuallyDeviceCreationData.h>


namespace pluginSystem
{
   //-----------------------------------------------------
   ///\brief The IDeviceCommand implementation
   //-----------------------------------------------------
   class CManuallyDeviceCreationData : public shared::plugin::yadomsApi::IManuallyDeviceCreationData
   {
   public:
      //-----------------------------------------------------
      ///\brief                  Constructor
      ///\param[in] device       The device to create
      ///\param[in] keyword      The keyword to create
      ///\param[in] capacity     The capacity used for this device
      ///\param[in] parameters   The parameters of the device (JSON string)
      //-----------------------------------------------------
      CManuallyDeviceCreationData(const std::string& device, const std::string& keyword, const std::string& capacity, const std::string& parameters);

      //-----------------------------------------------------
      ///\brief               Destructor
      //-----------------------------------------------------
      virtual ~CManuallyDeviceCreationData();

      // IManuallyDeviceCreationData implementation
      virtual const std::string& getDevice() const;
      virtual const std::string& getKeyword() const;
      virtual const std::string& getCapcity() const;
      virtual const std::string& getParameters() const;
      virtual const std::string toString() const;
      // [END] IManuallyDeviceCreationData implementation

   private:
      //-----------------------------------------------------
      ///\brief               Device
      //-----------------------------------------------------
      std::string m_device;

      //-----------------------------------------------------
      ///\brief               Keyword
      //-----------------------------------------------------
      std::string m_keyword;

      //-----------------------------------------------------
      ///\brief               The capacity used for this device
      //-----------------------------------------------------
      std::string m_capacity;

      //-----------------------------------------------------
      ///\brief               Device parameters (JSON string)
      //-----------------------------------------------------
      std::string m_parameters;
   };  
} // namespace pluginSystem	
	
	