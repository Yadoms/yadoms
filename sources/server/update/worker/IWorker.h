#pragma once

namespace update {
   namespace worker {

      //---------------------------------------------
      ///\brief   Interface for update workers
      //---------------------------------------------
      class IWorker
      {
      public:
         //---------------------------------
         ///\brief Define a function prototype for updating the worker progress
         //---------------------------------
         typedef boost::function3<void, bool, boost::optional<float>, std::string > WorkerProgressFunc;

         //---------------------------------------------
         ///\brief   Destructor
         //---------------------------------------------
         virtual ~IWorker() {}

         //---------------------------------------------
         ///\brief   Start a check for update work
         //---------------------------------------------
         virtual void checkForUpdateAsync(WorkerProgressFunc callback) = 0;

         //---------------------------------------------
         ///\brief   Start an update work
         //---------------------------------------------
         virtual void updateAsync(WorkerProgressFunc callback) = 0;
         
      };

   } // namespace worker
} // namespace update
