#pragma once

namespace task { 

   //------------------------------------------
   ///\brief   Finished task container
   //-----------------------------------------
   class CTaskEvent
   {
   public:

      //------------------------------------------
      ///\brief   Constructor
      //------------------------------------------
      CTaskEvent(const std::string & guid);

      //------------------------------------------
      ///\brief   Destructor
      //------------------------------------------
      virtual ~CTaskEvent();

   public:
      //------------------------------------------
      ///\brief   Obtain Guid
      //------------------------------------------
      const std::string & getGuid();
      
   private:
      
      std::string m_guid;
   };

} //namespace task
