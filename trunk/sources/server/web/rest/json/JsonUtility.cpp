#include "stdafx.h"
#include "JsonUtility.h"
#include <shared/Log.h>

namespace web { namespace rest { namespace json {


   //----------------------------------------------
   ///\brief Log CJson to log
   //----------------------------------------------
   void CJsonUtility::Log(const CJson & jsonData)
   {
      YADOMS_LOG(debug) << jsonData.serialize();
   }


} //namespace json
} //namespace rest
} //namespace web 

