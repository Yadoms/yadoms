#pragma once

#include "IInstance.h"

namespace task { 

   //------------------------------------------
   ///\brief   Finished task container
   //-----------------------------------------
   class CFinishedInstance : public IInstance
   {
   public:
      //------------------------------------------
      ///\brief   Constructor
      //------------------------------------------
      CFinishedInstance(const std::string & guid, const std::string & name, ETaskStatus status, boost::optional< float > progression, const std::string & message, const boost::posix_time::ptime & creationDate);

      //------------------------------------------
      ///\brief   Destructor
      //------------------------------------------
      virtual ~CFinishedInstance();

   public:
      //------------------------------------------
      ///\brief   Obtain Guid
      //------------------------------------------
      virtual std::string getGuid() const;
      
      //------------------------------------------
      ///\brief   Obtain Message
      //------------------------------------------
      virtual std::string getMessage() const;

      //------------------------------------------
      ///\brief   Obtain Progression
      //------------------------------------------
      virtual boost::optional< float > getProgression() const;

      //------------------------------------------
      ///\brief   Obtain Status
      //------------------------------------------
      virtual ETaskStatus getStatus() const;
      
      //---------------------------------
      ///\brief Get the current name of the task
      //---------------------------------
      virtual std::string getName() const;

      //---------------------------------
      ///\brief Get the creation date of the task
      //---------------------------------
      virtual boost::posix_time::ptime getCreationDate() const;

   private:

      //---------------------------------
      ///\brief The current name of the task
      //---------------------------------
      std::string m_name;

      //------------------------------------------
      ///\brief   The unique identifier of the task
      //------------------------------------------
      std::string m_guid;

      //--------------------------------------------------------------
      /// \brief			The current message given by the task
      //--------------------------------------------------------------
      std::string m_message;

      //--------------------------------------------------------------
      /// \brief			The current task instance progression
      //--------------------------------------------------------------
      boost::optional< float > m_progression;
      
      //------------------------------------------
      ///\brief   The status of the task
      //------------------------------------------
      ETaskStatus m_status;

      //------------------------------------------
      ///\brief   The creation date
      //------------------------------------------
      boost::posix_time::ptime m_creationDate;
   };

} //namespace task
