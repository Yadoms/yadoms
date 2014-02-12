#include "stdafx.h"
#include "JsonUtility.h"
#include <shared/Log.h>

//----------------------------------------------
///\brief Log CJson to log
//----------------------------------------------
void CJsonUtility::Log(const CJson & jsonData)
{
   for (CJson::const_iterator it = jsonData.begin(); it != jsonData.end(); ++it) 
   {
      YADOMS_LOG(debug) << it->first << " : " << it->second.get_value<std::string>();
      Log(it->second);
   }
}
