#pragma once
#include <shared/ThreadBase.h>

class CMosquittoServer : public shared::CThreadBase
{
public:
   //--------------------------------------------------------------
   /// \brief	Constructor
   //--------------------------------------------------------------
   CMosquittoServer();

   //--------------------------------------------------------------
   /// \brief	Destructor
   //--------------------------------------------------------------
   virtual ~CMosquittoServer();

   // CThreadBase override
   void doWork() override;
   void stop() override;
   // {END] CThreadBase override

};