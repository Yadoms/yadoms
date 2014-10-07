#pragma once
#include <shared/Export.h>
#include <shared/DataContainer.h>
#include <shared/enumeration/EnumHelpers.hpp>
#include "IHistorizable.h"

namespace shared { namespace plugin { namespace yadomsApi { namespace commands
{   
   //-----------------------------------------------------
   ///\brief A power factor historizable object
   //-----------------------------------------------------
   class YADOMS_SHARED_EXPORT CPowerFactor : public IHistorizable
   {
   public:
      //-----------------------------------------------------
      ///\brief                     Constructor
      ///\param[in] keywordName     Yadoms keyword name
      //-----------------------------------------------------
      CPowerFactor(const std::string& keywordName);

      //-----------------------------------------------------
      ///\brief                     Destructor
      //-----------------------------------------------------
      virtual ~CPowerFactor();

      // IHistorizable implementation
      virtual const std::string& getKeyword() const;
      virtual const CStandardCapacity& getCapacity() const;
      virtual const std::string formatValue(const std::string& currentValue = CStringExtension::EmptyString) const;
      virtual EMeasureType getMeasureType() const;
      // [END] IHistorizable implementation

      //-----------------------------------------------------
      ///\brief                     Set value from Yadoms command
      ///\param[in] yadomsCommand   Yadoms command container
      ///\throw                     shared::exception::CInvalidParameter or COutOfRange if fail to parse command
      //-----------------------------------------------------
      void set(const shared::CDataContainer& yadomsCommand);

      //-----------------------------------------------------
      ///\brief                     Set value
      ///\param[in] powerFactor     Power factor (-1 to 1)
      //-----------------------------------------------------
      void set(double powerFactor);

      //-----------------------------------------------------
      ///\brief                     Get value
      ///\return                    The power factor (-1 to 1)
      //-----------------------------------------------------
      double powerFactor() const;

   protected:
      //-----------------------------------------------------
      ///\brief                     Normalize the value
      ///\param[in] value           Raw value
      ///\return                    The normalized value (-1 to 1)
      //-----------------------------------------------------
      static double Normalize(double value);

   private:
      //-----------------------------------------------------
      ///\brief                     The keyword name
      //-----------------------------------------------------
      const std::string m_keywordName;

      //-----------------------------------------------------
      ///\brief                     The power factor (-1 to 1)
      //-----------------------------------------------------
      double m_powerFactor;
   };



} } } } // namespace shared::plugin::yadomsApi::commands

