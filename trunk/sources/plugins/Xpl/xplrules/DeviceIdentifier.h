#pragma once

namespace xplrules {

   //------------------------------------
   ///\brief Class container for a device identifier.
   /// It could provide also the commercial name
   //------------------------------------
   class CDeviceIdentifier
   {
   public:
      //------------------------------------
      ///\brief Constructor
      ///\param [in] id The device id
      //------------------------------------
      CDeviceIdentifier(const std::string & id);

      //------------------------------------
      ///\brief Constructor
      ///\param [in] id The device id
      ///\param [in] commercialName The device commercial name
      //------------------------------------
      CDeviceIdentifier(const std::string & id, const std::string & commercialName);

      //------------------------------------
      ///\brief Destructor
      //------------------------------------
      virtual ~CDeviceIdentifier();

      //------------------------------------
      ///\brief Get the device identifier
      ///\return the device identifier
      //------------------------------------
      const std::string & getId();

      //------------------------------------
      ///\brief Get the device commercial name
      /// If the commercial name is not defined, it retuens the device identifier
      ///\return the commercial name, or device identifier if commercial one is not defined
      //------------------------------------
      const std::string & getCommercialName();

   private:
      //------------------------------------
      ///\brief The device identifier
      //------------------------------------
      std::string m_id;

      //------------------------------------
      ///\brief The commercial name
      //------------------------------------
      std::string m_commercialName;
   };


} //namespace xplrules


