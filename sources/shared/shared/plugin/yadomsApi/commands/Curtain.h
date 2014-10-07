#pragma once
#include <shared/Export.h>
#include <shared/DataContainer.h>
#include <shared/enumeration/EnumHelpers.hpp>
#include <shared/Field.hpp>
#include "IHistorizable.h"
#include <boost/foreach.hpp>

namespace shared { namespace plugin { namespace yadomsApi { namespace commands
{
   //-----------------------------------------------------
   ///\brief A curtain historizable object
   //-----------------------------------------------------
   class YADOMS_SHARED_EXPORT CCurtain : public IHistorizable
   {
   public:
      //-----------------------------------------------------
      ///\brief               The curtain command values
      //-----------------------------------------------------   
      DECLARE_ENUM_HEADER_SHARED(ECommand, YADOMS_SHARED_EXPORT,
         ((Stop)(0))
         ((Open)(1))
         ((Close)(2))
      );

   public:
      //-----------------------------------------------------
      ///\brief                     Constructor
      ///\param[in] keywordName     Yadoms keyword name
      //-----------------------------------------------------
      CCurtain(const std::string& keywordName);

      //-----------------------------------------------------
      ///\brief                     Destructor
      //-----------------------------------------------------
      virtual ~CCurtain();

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
      ///\param[in] command         Curtain command
      //-----------------------------------------------------
      void set(ECommand command);

      //-----------------------------------------------------
      ///\brief                     Get the command value
      ///\return                    The command value
      //-----------------------------------------------------
      const ECommand command() const;

   private:
      //-----------------------------------------------------
      ///\brief                     The keyword name
      //-----------------------------------------------------
      const std::string m_keywordName;

      //-----------------------------------------------------
      ///\brief               The command value
      //-----------------------------------------------------
      CField<ECommand> m_command;
   };



} } } } // namespace shared::plugin::yadomsApi::commands

