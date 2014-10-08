#pragma once
#include <shared/Export.h>
#include <shared/DataContainer.h>
#include <shared/enumeration/EnumHelpers.hpp>
#include "IHistorizable.h"

namespace shared { namespace plugin { namespace yadomsApi { namespace commands
{   
   //-----------------------------------------------------
   ///\brief A weight historizable object
   //-----------------------------------------------------
   class YADOMS_SHARED_EXPORT CWeight : public IHistorizable
   {
   public:
      //-----------------------------------------------------
      ///\brief                     Constructor
      ///\param[in] keywordName     Yadoms keyword name
      ///\param[in] measureType     The measure type (normally kAbsolute)
      //-----------------------------------------------------
      CWeight(const std::string& keywordName, EMeasureType measureType = kAbsolute);

      //-----------------------------------------------------
      ///\brief                     Destructor
      //-----------------------------------------------------
      virtual ~CWeight();

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
      ///\param[in] frequency       Weight value (Kg)
      //-----------------------------------------------------
      void set(double weight);

      //-----------------------------------------------------
      ///\brief                     Get value
      ///\return                    The weight value (Kg)
      //-----------------------------------------------------
      double weight() const;

   private:
      //-----------------------------------------------------
      ///\brief                     The keyword name
      //-----------------------------------------------------
      const std::string m_keywordName;

      //-----------------------------------------------------
      ///\brief                     The weight value (Kg)
      //-----------------------------------------------------
      double m_weight;

      //-----------------------------------------------------
      ///\brief                     The measure type
      //-----------------------------------------------------
      EMeasureType m_measureType;
   };



} } } } // namespace shared::plugin::yadomsApi::commands

