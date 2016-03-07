#pragma once
#include <shared/plugin/yPluginApi/historization/SingleHistorizableData.hpp>

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

//--------------------------------------------------------------
/// \brief	SMS Keyword handler
//--------------------------------------------------------------
class CSmsKeyword : public yApi::historization::CSingleHistorizableData < std::string >
{
public:
   //--------------------------------------------------------------
   /// \brief Constructor
   /// \param [in] keywordName   The keyword name
   //--------------------------------------------------------------
   explicit CSmsKeyword(const std::string & keywordName);

   //--------------------------------------------------------------
   /// \brief Destructor
   //--------------------------------------------------------------
   virtual ~CSmsKeyword();
};
