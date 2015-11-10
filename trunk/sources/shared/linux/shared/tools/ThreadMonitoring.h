//
// Platforms-spcific thread monitoring system
//
#pragma once

#include <Poco/Runnable.h>

namespace shared { namespace tools {

   //--------------------------------------------------------------
   /// \brief	this class is used to manage platforms-specific threads functions
   //--------------------------------------------------------------
   class CThreadMonitoring : public Poco::Runnable
   {
   public:
      //--------------------------------------------------------------
      /// \brief			Constructor
      //--------------------------------------------------------------
      CThreadMonitoring();

      //--------------------------------------------------------------
      /// \brief			Destructor
      //--------------------------------------------------------------
      virtual ~CThreadMonitoring();

      //--------------------------------------------------------------
      /// \brief			Start the monitoring thread 
      //--------------------------------------------------------------      
      void run();
      
      void addThreadToMonitor(int threadId);

   private:
      std::vector<int> m_threadIdToMonitor;
   };

}} // namespace shared::tools