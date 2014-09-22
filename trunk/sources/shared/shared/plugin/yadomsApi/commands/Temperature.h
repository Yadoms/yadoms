#pragma once
#include <shared/Export.h>
#include <shared/DataContainer.h>
#include <shared/enumeration/EnumHelpers.hpp>
#include <shared/Field.hpp>
#include "IHistorizable.h"

namespace shared { namespace plugin { namespace yadomsApi { namespace commands
{   
   //-----------------------------------------------------
   ///\brief A temperature historizable object
   //-----------------------------------------------------
   class YADOMS_SHARED_EXPORT CTemperature : public IHistorizable
   {
   public:
      //-----------------------------------------------------
      ///\brief                     Constructor
      ///\param[in] keywordName     Yadoms keyword name
      //-----------------------------------------------------
      CTemperature(const std::string& keywordName);

      //-----------------------------------------------------
      ///\brief                     Destructor
      //-----------------------------------------------------
      virtual ~CTemperature();

      // IHistorizable implementation
      virtual const std::string& getKeyword() const;
      virtual const CStandardCapacity& getCapacity() const;
      virtual const std::string formatValue() const;
      // [END] IHistorizable implementation

      //-----------------------------------------------------
      ///\brief                     Set value from Yadoms command
      ///\param[in] yadomsCommand   Yadoms command container
      ///\throw                     shared::exception::CInvalidParameter or COutOfRange if fail to parse command
      //-----------------------------------------------------
      void set(const shared::CDataContainer& yadomsCommand);

      //-----------------------------------------------------
      ///\brief                     Set value from on/off state
      ///\param[in] temperature     temperature value (degres)
      //-----------------------------------------------------
      void set(double temperature);

      //-----------------------------------------------------
      ///\brief                     Get the temperature value
      ///\return                    The temperature value (degres)
      //-----------------------------------------------------
      double temperature() const;

   private:
      //-----------------------------------------------------
      ///\brief                     The keyword name
      //-----------------------------------------------------
      const std::string m_keywordName;

      //-----------------------------------------------------
      ///\brief                     The temperature value (degres)
      //-----------------------------------------------------
      double m_temperature;
   };



} } } } // namespace shared::plugin::yadomsApi::commands

