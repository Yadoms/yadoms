#pragma once
#include <Poco/Notification.h>
#include "database/entities/Entities.h"

namespace notifications {

   //-----------------------------
   ///\brief Notification for a configuration setting updated
   //-----------------------------
   class CConfigurationUpdatedNotification : public Poco::Notification
   {
   public:

      //-----------------------------
      ///\brief Constructor
      ///\param [in]  section   The configuration section
      ///\param [in]  name       The configuration name
      CConfigurationUpdatedNotification(const std::string & section, const std::string & name)
         :m_section(section), m_name(name)
      {
      }

      //-----------------------------
      ///\brief Destructor
      //-----------------------------
      virtual ~CConfigurationUpdatedNotification()
      {
      }

      //-----------------------------
      ///\brief Get the configuration section
      ///\return  the configuration section
      //-----------------------------
      const std::string & getSection() const { return m_section; }

      //-----------------------------
      ///\brief Get the configuration name
      ///\return  the configuration name
      //-----------------------------
      const std::string & getName() const { return m_name; }

   private:
      //-----------------------------
      ///\brief The updated configuration section
      //-----------------------------
      std::string m_section;

      //-----------------------------
      ///\brief The updated configuration name
      //-----------------------------
      std::string m_name;
   };

} //namespace notifications 
