#pragma once
#include <shared/Export.h>
#include <shared/DataContainer.h>
#include <shared/enumeration/EnumHelpers.hpp>
#include "IHistorizable.h"

namespace shared { namespace plugin { namespace yadomsApi { namespace commands
{   
   //-----------------------------------------------------
   ///\brief A temperature historizable object
   //-----------------------------------------------------
   class YADOMS_SHARED_EXPORT CBatteryLevel : public IHistorizable
   {
   public:
      //-----------------------------------------------------
      ///\brief                     Constructor
      ///\param[in] keywordName     Yadoms keyword name
      ///\param[in] measureType     The measure type (normally kAbsolute)
      //-----------------------------------------------------
      CBatteryLevel(const std::string& keywordName, EMeasureType measureType = kAbsolute);

      //-----------------------------------------------------
      ///\brief                     Destructor
      //-----------------------------------------------------
      virtual ~CBatteryLevel();

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
      ///\brief                     Set the battery level
      ///\param[in] batteryLevel    Battery level (0-100)
      //-----------------------------------------------------
      void set(int batteryLevel);

      //-----------------------------------------------------
      ///\brief                     Get the battery level
      ///\return                    The battery level (0-100)
      //-----------------------------------------------------
      int batteryLevel() const;

   protected:
      //-----------------------------------------------------
      ///\brief                     Normalize the humidity value
      ///\param[in] value           Raw value
      ///\return                    The normalized humidity (0-100)
      //-----------------------------------------------------
      static int Normalize(int value);

   private:
      //-----------------------------------------------------
      ///\brief                     The keyword name
      //-----------------------------------------------------
      const std::string m_keywordName;

      //-----------------------------------------------------
      ///\brief                     The battery level (0-100)
      //-----------------------------------------------------
      int m_batteryLevel;

      //-----------------------------------------------------
      ///\brief                     The measure type
      //-----------------------------------------------------
      EMeasureType m_measureType;
   };



} } } } // namespace shared::plugin::yadomsApi::commands

