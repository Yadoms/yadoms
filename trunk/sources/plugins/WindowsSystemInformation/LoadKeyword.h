#pragma once
#include <shared/plugin/yadomsApi/StandardCapacity.h>
#include <shared/plugin/yadomsApi/historization/IHistorizable.h>

// Shortcut to yadomsApi namespace
namespace yApi = shared::plugin::yadomsApi;

//-----------------------------------------------------
///\brief A temperature historizable object
//-----------------------------------------------------
class CLoadKeyword : public yApi::historization::IHistorizable
{
public:
   //-----------------------------------------------------
   ///\brief                     Constructor
   ///\param[in] keywordName     Yadoms keyword name
   //-----------------------------------------------------
   CLoadKeyword(const std::string& keywordName);

   //-----------------------------------------------------
   ///\brief                     Destructor
   //-----------------------------------------------------
   virtual ~CLoadKeyword();

   // IHistorizable implementation
   virtual const std::string& getKeyword() const;
   virtual const yApi::CStandardCapacity& getCapacity() const;
   virtual const std::string formatValue() const;
   virtual const yApi::historization::EMeasureType& getMeasureType() const;
   // [END] IHistorizable implementation

   //-----------------------------------------------------
   ///\brief                     Set the load
   ///\param[in] load            Load (%)
   //-----------------------------------------------------
   void set(float load);

   //-----------------------------------------------------
   ///\brief                     Get the load
   ///\return                    The load (%)
   //-----------------------------------------------------
   float load() const;

private:
   //-----------------------------------------------------
   ///\brief                     The keyword name
   //-----------------------------------------------------
   const std::string m_keywordName;

   //-----------------------------------------------------
   ///\brief                     The load (%)
   //-----------------------------------------------------
   float m_load;
};

