#pragma once
#include <shared/Export.h>
#include <shared/DataContainer.h>
#include <shared/enumeration/EnumHelpers.hpp>
#include <shared/Field.hpp>
#include "IHistorizable.h"

namespace shared { namespace plugin { namespace yadomsApi { namespace commands
{
   //-----------------------------------------------------
   ///\brief A rssi (signal strength) historizable object
   ///\note Rssi value is read-only
   //-----------------------------------------------------
   class YADOMS_SHARED_EXPORT CRssi : public IHistorizable
   {
   public:
      //-----------------------------------------------------
      ///\brief                     Constructor
      ///\param[in] keywordName     Yadoms keyword name
      //-----------------------------------------------------
      CRssi(const std::string& keywordName);

      //-----------------------------------------------------
      ///\brief                     Destructor
      //-----------------------------------------------------
      virtual ~CRssi();

      // IHistorizable implementation
      virtual const std::string& getKeyword() const;
      virtual const CStandardCapacity& getCapacity() const;
      virtual const std::string formatValue(const std::string& currentValue = CStringExtension::EmptyString) const;
      virtual EMeasureType getMeasureType() const;
      // [END] IHistorizable implementation

      //-----------------------------------------------------
      ///\brief                     Set value
      ///\param[in] rssi            Rssi (0-100)
      //-----------------------------------------------------
      void set(int rssi);

      //-----------------------------------------------------
      ///\brief                     Get rssi
      ///\return                    The rssi (0-100)
      //-----------------------------------------------------
      int rssi() const;

   protected:
      //-----------------------------------------------------
      ///\brief                     Normalize the value
      ///\param[in] level           Raw value
      ///\return                    The normalized value (0-100)
      //-----------------------------------------------------
      static int Normalize(int value);

   private:
      //-----------------------------------------------------
      ///\brief                     The keyword name
      //-----------------------------------------------------
      const std::string m_keywordName;

      //-----------------------------------------------------
      ///\brief                     The rssi value (0-100)
      //-----------------------------------------------------
      int m_rssi;
   };



} } } } // namespace shared::plugin::yadomsApi::commands

