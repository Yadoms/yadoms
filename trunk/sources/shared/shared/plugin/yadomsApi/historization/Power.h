#pragma once
#include <shared/Export.h>
#include <shared/DataContainer.h>
#include <shared/enumeration/EnumHelpers.hpp>
#include <shared/Field.hpp>
#include "IHistorizable.h"

namespace shared { namespace plugin { namespace yadomsApi { namespace historization
{   
   //-----------------------------------------------------
   ///\brief A power historizable object
   //-----------------------------------------------------
   class YADOMS_SHARED_EXPORT CPower : public IHistorizable
   {
   public:
      //-----------------------------------------------------
      ///\brief                     Constructor
      ///\param[in] keywordName     Yadoms keyword name
      ///\param[in] measureType     The measure type (normally kAbsolute)
      //-----------------------------------------------------
      CPower(const std::string& keywordName, const EMeasureType& measureType = EMeasureType::kAbsolute);

      //-----------------------------------------------------
      ///\brief                     Destructor
      //-----------------------------------------------------
      virtual ~CPower();

      // IHistorizable implementation
      virtual const std::string& getKeyword() const;
      virtual const CStandardCapacity& getCapacity() const;
      virtual const std::string formatValue() const;
      virtual const EMeasureType& getMeasureType() const;
      // [END] IHistorizable implementation

      //-----------------------------------------------------
      ///\brief                     Set value from Yadoms command
      ///\param[in] yadomsCommand   Yadoms command container
      ///\throw                     shared::exception::CInvalidParameter or COutOfRange if fail to parse command
      //-----------------------------------------------------
      void set(const shared::CDataContainer& yadomsCommand);

      //-----------------------------------------------------
      ///\brief                     Set value
      ///\param[in] humidity        power value (W)
      //-----------------------------------------------------
      void set(double power);

      //-----------------------------------------------------
      ///\brief                     Get value
      ///\return                    The power value (W)
      //-----------------------------------------------------
      double power() const;

   private:
      //-----------------------------------------------------
      ///\brief                     The keyword name
      //-----------------------------------------------------
      const std::string m_keywordName;

      //-----------------------------------------------------
      ///\brief                     The power value (W)
      //-----------------------------------------------------
      double m_power;

      //-----------------------------------------------------
      ///\brief                     The measure type
      //-----------------------------------------------------
      const EMeasureType m_measureType;
   };



} } } } // namespace shared::plugin::yadomsApi::historization

