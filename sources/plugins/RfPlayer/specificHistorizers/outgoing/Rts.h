#pragma once

#include <shared/plugin/yPluginApi/IYPluginApi.h>
#include <shared/enumeration/EnumHelpers.hpp>
#include <shared/plugin/yPluginApi/historization/SingleHistorizableData.hpp>

namespace yApi = shared::plugin::yPluginApi;

namespace specificHistorizers {
namespace outgoing {
     
   DECLARE_ENUM_HEADER(ERts,
      ((Off)(0))
      ((On)(1))
      ((Dim)(2))
      ((AllOff)(4))
      ((AllOn)(5))
      ((Assoc)(6))
   )

   class CRtsKeyword : public shared::plugin::yPluginApi::historization::CCurtain
   {
   public:
      //-----------------------------------------------------
      ///\brief                     Constructor
      ///\param[in] keywordName     Yadoms keyword name
      //-----------------------------------------------------
      explicit CRtsKeyword(const std::string& keywordName);

      //-----------------------------------------------------
      ///\brief                     Destructor
      //-----------------------------------------------------
      virtual ~CRtsKeyword();

	  //-----------------------------------------------------
	  ///\brief                     Set only keyword is supported
	  //-----------------------------------------------------
	  void set(ERts value);
   };


} //namespace outgoin
} //namespace specificHistorizers