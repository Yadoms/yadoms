#pragma once
#include <shared/Export.h>
#include <shared/DataContainer.h>
#include <shared/enumeration/EnumHelpers.hpp>
#include <shared/Field.hpp>
#include "IHistorizable.h"

//TODO renommer les commandes (en historizableObjects)
namespace shared { namespace plugin { namespace yadomsApi { namespace commands
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
      ///\param[in] isIncrement     true if data is increment (must be added to current database value) or totalizer
      //-----------------------------------------------------
      CCounter(const std::string& keywordName, bool isIncrement = false);

      //-----------------------------------------------------
      ///\brief                     Destructor
      //-----------------------------------------------------
      virtual ~CCounter();

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
      EMeasureType m_measureType;
   };



} } } } // namespace shared::plugin::yadomsApi::commands

