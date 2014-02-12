#pragma once

#include "Json.h"


//----------------------------------------------
///\brief Class providing utilities about CJson
//----------------------------------------------
class CJsonUtility
{
public:

   //----------------------------------------------
   ///\brief Log CJson to log
   ///\param [in]  jsonData : the CJson data to log
   //----------------------------------------------
   static void Log(const CJson & jsonData);

};
