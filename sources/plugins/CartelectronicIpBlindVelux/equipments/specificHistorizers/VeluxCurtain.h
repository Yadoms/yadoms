#pragma once
//#include <shared/enumeration/EnumHelpers.hpp>
#include <shared/plugin/yPluginApi/historization/SingleHistorizableData.hpp>

namespace specificHistorizers
{
	//-----------------------------------------------------
	///\brief               The curtain command values
	//-----------------------------------------------------   
	DECLARE_ENUM_HEADER(EVeluxCurtainCommand,
		    ((Open)(1))
			((Stop)(2))
		    ((Close)(3))
	);


	//-----------------------------------------------------
	///\brief A specific curtain historizable object
	//-----------------------------------------------------
	class CVeluxCurtain : public shared::plugin::yPluginApi::historization::CSingleHistorizableData<EVeluxCurtainCommand>
	{
	public:
		//-----------------------------------------------------
		///\brief                     Constructor
		///\param[in] keywordName     Yadoms keyword name
		//-----------------------------------------------------
		explicit CVeluxCurtain(const std::string& keywordName);

		//-----------------------------------------------------
		///\brief                     Destructor
		//-----------------------------------------------------
		virtual ~CVeluxCurtain();
	};
} // namespace specificHistorizers
