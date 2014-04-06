#pragma once
#include "tools/Xpl/XplHub.h"
#include "tools/ThreadBase.h"

class CHub : public CThreadBase
{
public:
   CHub(const std::string & localIPOfTheInterfaceToUse);
   virtual ~CHub(void);

   void doWork();
private:
   CXplHub m_hub;
};