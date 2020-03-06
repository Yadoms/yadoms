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
      CFinishedInstance(const std::string & guid, const std::string & name, ETaskStatus status, boost::optional< float > progression, const std::string & message, const std::string & messageException, shared::CDataContainerSharedPtr data, const boost::posix_time::ptime & creationDate);

      //------------------------------------------
      ///\brief   Destructor
      //------------------------------------------
      virtual ~CFinishedInstance();

   public:
      // IInstance implementation
      std::string getGuid() const override;
      boost::optional<float> getProgression() const override;
      std::string getMessage() const override;
      std::string getExceptionMessage() const override;
      ETaskStatus getStatus() const override;
      shared::CDataContainerSharedPtr getTaskData() const override;
      std::string getName() const override;
      boost::posix_time::ptime getCreationDate() const override;
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
      
      //--------------------------------------------------------------
      /// \brief			The current message exception given by the task
      //--------------------------------------------------------------
      std::string m_messageException;

      //------------------------------------------
      ///\brief   The task data
      //------------------------------------------
      shared::CDataContainerSharedPtr m_data;

      //------------------------------------------
      ///\brief   The creation date
      //------------------------------------------
      boost::posix_time::ptime m_creationDate;


   };

} //namespace task
