#pragma once
#include <shared/Export.h>
#include "IHistorizable.h"

namespace shared { namespace plugin { namespace yPluginApi { namespace historization
{
   //-----------------------------------------------------
   ///\brief Plugin state
   //-----------------------------------------------------
   DECLARE_ENUM_HEADER_SHARED(EPluginState, YADOMS_SHARED_EXPORT,
      ((Unknown))
      ((Error))
      ((Stopped))
      ((Running))
      ((Custom))
   );


   //-----------------------------------------------------
   ///\brief A message historizable object
   //-----------------------------------------------------
   class YADOMS_SHARED_EXPORT CPluginState : public IHistorizable
   {
   public:
      //-----------------------------------------------------
      ///\brief                     Constructor
      //-----------------------------------------------------
      CPluginState();

      //-----------------------------------------------------
      ///\brief                     Destructor
      //-----------------------------------------------------
      virtual ~CPluginState();

      // IHistorizable implementation
      virtual const std::string& getKeyword() const;
      virtual const CStandardCapacity& getCapacity() const;
      virtual const EKeywordAccessMode& getAccessMode() const;
      virtual std::string formatValue() const;
      virtual const EMeasureType& getMeasureType() const;
      virtual CDataContainer getTypeInfo() const;
      // [END] IHistorizable implementation;

      //-----------------------------------------------------
      ///\brief                     Set value from on/off state
      ///\param[in] state           Plugin state
      ///\param[in] messageId       Message id associated with state
      //-----------------------------------------------------
      void set(const EPluginState& state, const std::string& messageId = std::string());

      //-----------------------------------------------------
      ///\brief               Get the plugin state
      ///\return              The plugin state
      //-----------------------------------------------------
      EPluginState state() const;

      //-----------------------------------------------------
      ///\brief               Get the message id associated to state
      ///\return              The message id
      //-----------------------------------------------------
      const std::string& messageId() const;

   private:
      //-----------------------------------------------------
      ///\brief                     The plugin state
      //-----------------------------------------------------
      EPluginState m_state;

      //-----------------------------------------------------
      ///\brief                     The message id associated to state
      //-----------------------------------------------------
      std::string m_messageId;
   };



} } } } // namespace shared::plugin::yPluginApi::historization

