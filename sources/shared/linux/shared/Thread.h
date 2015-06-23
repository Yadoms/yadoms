//
// Threads.h
//
// Platforms-spcific thread management
//
#pragma once

namespace shared
{

   //--------------------------------------------------------------
   /// \brief	this class is used to manage platforms-specific threads functions
   //--------------------------------------------------------------
   class CThreads
   {
   public:
      //--------------------------------------------------------------
      /// \brief			Constructor
      //--------------------------------------------------------------
      CThreads();

      //--------------------------------------------------------------
      /// \brief			Destructor
      //--------------------------------------------------------------
      virtual ~CThreads();

      //--------------------------------------------------------------
      /// \brief			Kill a thread
      /// \param[in] hThread Native thread handle
      /// \note killThread is a dangerous function that should only be used in the most extreme cases.
      //--------------------------------------------------------------
      static void killThread(pthread_t hThread);
   };

} // namespace shared