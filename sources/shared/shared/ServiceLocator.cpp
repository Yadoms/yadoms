#include "stdafx.h"
#include "ServiceLocator.h"


namespace shared
{
   //declare static instance
   CServiceLocator CServiceLocator::m_instance;

   CServiceLocator& CServiceLocator::instance()
   {
      return m_instance;
   }
} //namespace shared
