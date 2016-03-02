#pragma once


namespace database { 


   class IDatabaseEngine
   {
   public:
      //--------------------------------------------------------------
      /// \Brief		Initialize the database system
      /// \return EInitState	The initialization state
      /// \throws 		
      //--------------------------------------------------------------
      virtual void initialize() = 0;

      //--------------------------------------------------------------
      /// \Brief	Finalize the database system
      //--------------------------------------------------------------
      virtual void finalize() = 0;   
      
      //--------------------------------------------------------------
      /// \Brief	Virtual destructor
      //--------------------------------------------------------------   
      virtual ~IDatabaseEngine()
      {
      }
   };

 
} //namespace database 
