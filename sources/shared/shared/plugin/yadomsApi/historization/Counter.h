#pragma once
#include <shared/Export.h>
#include <shared/DataContainer.h>
#include <shared/enumeration/EnumHelpers.hpp>
#include <shared/Field.hpp>
#include "IHistorizable.h"

namespace shared { namespace plugin { namespace yadomsApi { namespace historization
{   
   //-----------------------------------------------------
   ///\brief A temperature historizable object
   //-----------------------------------------------------
   class YADOMS_SHARED_EXPORT CCounter : public IHistorizable
   {
   public:
      //-----------------------------------------------------
      ///\brief                     Constructor
      ///\param[in] keywordName     Yadoms keyword name
      ///\param[in] measureType     To be used as increment counter (values will be added to current database value) or totalizer
      //-----------------------------------------------------
      CCounter(const std::string& keywordName, const EMeasureType& measureType = EMeasureType::kCumulative);

      //-----------------------------------------------------
      ///\brief                     Destructor
      //-----------------------------------------------------
      virtual ~CCounter();

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
      ///\brief                     Set value from on/off state
      ///\param[in] count           Count value
      //-----------------------------------------------------
      void set(unsigned int count);

      //-----------------------------------------------------
      ///\brief                     Get the temperature value
      ///\return                    The count value
      //-----------------------------------------------------
      unsigned int count() const;

   private:
      //-----------------------------------------------------
      ///\brief                     The keyword name
      //-----------------------------------------------------
      const std::string m_keywordName;

      //-----------------------------------------------------
      ///\brief                     The count value
      //-----------------------------------------------------
      unsigned int m_count;

      //-----------------------------------------------------
      ///\brief                     The measure type
      //-----------------------------------------------------
      const EMeasureType m_measureType;
   };



} } } } // namespace shared::plugin::yadomsApi::historization

