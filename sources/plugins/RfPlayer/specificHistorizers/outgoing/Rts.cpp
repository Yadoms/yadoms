#include "stdafx.h"
#include "Rts.h"
#include <shared/plugin/yPluginApi/StandardCapacities.h>
#include <shared/plugin/yPluginApi/StandardUnits.h>

namespace specificHistorizers {
namespace outgoing {

	DECLARE_ENUM_IMPLEMENTATION(ERts,
		((Off)("OFF"))
		((On)("ON"))
		((Dim)("DIM"))
		((AllOff)("ALL_OFF"))
		((AllOn)("ALL_ON"))
		((Assoc)("ASSOC"))
	)

   CRtsKeyword::CRtsKeyword(const std::string& keywordName)
      : shared::plugin::yPluginApi::historization::CCurtain(keywordName)
   {
   }

   CRtsKeyword::~CRtsKeyword()
   {
   }

   void CRtsKeyword::set(ERts value)
   {
	   switch (value())
	   {
	   case ERts::kOnValue:
		   shared::plugin::yPluginApi::historization::CCurtain::set(shared::plugin::yPluginApi::historization::ECurtainCommand::kOpen);
		   break;
	   case ERts::kOffValue:
		   shared::plugin::yPluginApi::historization::CCurtain::set(shared::plugin::yPluginApi::historization::ECurtainCommand::kClose);
		   break;
	   default:
		   shared::plugin::yPluginApi::historization::CCurtain::set(shared::plugin::yPluginApi::historization::ECurtainCommand::kStop);
		   break;
	   }
   }

} //namespace outgoing
} //namespace specificHistorizers