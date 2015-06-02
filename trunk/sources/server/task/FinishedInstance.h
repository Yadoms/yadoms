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
      CFinishedInstance(const std::string & guid, const std::string & name, ETaskStatus status, boost::optional< float > progression, const std::string & message, const shared::CDataContainer data, const boost::posix_time::ptime & creationDate);

      //------------------------------------------
      ///\brief   Destructor
      //------------------------------------------
      virtual ~CFinishedInstance();

   public:
      // IInstance implementation
      virtual std::string getGuid() const;
      virtual boost::optional<float> getProgression() const;
      virtual std::string getMessage() const;
      virtual ETaskStatus getStatus() const;
      virtual shared::CDataContainer getTaskData() const;
      virtual std::string getName() const;
      virtual boost::posix_time::ptime getCreationDate() const;
      // [END] - IInstance implementation

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
      /// \brief			The current task instance progression
      //--------------------------------------------------------------
      boost::optional< float > m_progression;
      
      //------------------------------------------
      ///\brief   The status of the task
      //------------------------------------------
      ETaskStatus m_status;

      //--------------------------------------------------------------
      /// \brief			The current message given by the task
      //--------------------------------------------------------------
      std::string m_message;

      //------------------------------------------
      ///\brief   The task data
      //------------------------------------------
      shared::CDataContainer m_data;

      //------------------------------------------
      ///\brief   The creation date
      //------------------------------------------
      boost::posix_time::ptime m_creationDate;


   };

} //namespace task
