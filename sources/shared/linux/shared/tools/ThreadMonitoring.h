//
// Platforms-spcific thread monitoring system
//
#pragma once

namespace shared { namespace tools {

   //--------------------------------------------------------------
   /// \brief	this class is used to manage platforms-specific threads functions
   //--------------------------------------------------------------
   class CThreadMonitoring
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
      void start();
      
      //--------------------------------------------------------------
      /// \brief			Stop the monitoring thread 
      //--------------------------------------------------------------      
      void stop();
   };

}} // namespace shared::tools