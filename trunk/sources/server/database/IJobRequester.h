#pragma once

#include "entities/Entities.h"


namespace database { 


   class IJobRequester
   {
   public:

      //--------------------------------------------------------------
      /// \brief           List all jobs
      /// \return          List of registered jobs
      //--------------------------------------------------------------
      virtual std::vector<boost::shared_ptr<entities::CJob> > getJobs() const = 0;

      //--------------------------------------------------------------
      /// \brief       Destructor
      //--------------------------------------------------------------
      virtual ~IJobRequester()
      {
      }
   };

 
} //namespace database 
   