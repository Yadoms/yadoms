#pragma once
#include <shared/Export.h>
#include <shared/DataContainer.h>
#include <shared/enumeration/EnumHelpers.hpp>
#include <shared/Field.hpp>
#include "IHistorizable.h"

namespace shared { namespace plugin { namespace yadomsApi { namespace commands
{   
   //-----------------------------------------------------
   ///\brief A energy historizable object
   //-----------------------------------------------------
   class YADOMS_SHARED_EXPORT CEnergy : public IHistorizable
   {
   public:
      //-----------------------------------------------------
      ///\brief                     Constructor
      ///\param[in] keywordName     Yadoms keyword name
      ///\param[in] measureType     To be used as increment counter (values will be added to current database value) or totalizer
      //-----------------------------------------------------
      CEnergy(const std::string& keywordName, EMeasureType measureType = kCumulative);

      //-----------------------------------------------------
      ///\brief                     Destructor
      //-----------------------------------------------------
      virtual ~CEnergy();

      // IHistorizable implementation
      virtual const std::string& getKeyword() const;
      virtual const CStandardCapacity& getCapacity() const;
      virtual const std::string formatValue() const;
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
      ///\param[in] energy          Energy value (Wh)
      //-----------------------------------------------------
      void set(double energy);

      //-----------------------------------------------------
      ///\brief                     Get value
      ///\return                    The energy value (Wh)
      //-----------------------------------------------------
      double energy() const;

   private:
      //-----------------------------------------------------
      ///\brief                     The keyword name
      //-----------------------------------------------------
      const std::string m_keywordName;

      //-----------------------------------------------------
      ///\brief                     The energy value (Wh)
      //-----------------------------------------------------
      double m_energy;

      //-----------------------------------------------------
      ///\brief                     The measure type
      //-----------------------------------------------------
      EMeasureType m_measureType;
   };



} } } } // namespace shared::plugin::yadomsApi::commands

