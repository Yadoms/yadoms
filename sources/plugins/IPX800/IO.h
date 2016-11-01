#pragma once

#include "IIO.h"
#include <shared/plugin/yPluginApi/IYPluginApi.h>
#include <shared/Export.h>
#include <shared/enumeration/EnumHelpers.hpp>
#include "IPX800Configuration.h"

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

//--------------------------------------------------------------
/// \brief	All IOs operations
//--------------------------------------------------------------
class CIO : public IIO
{
public:
   //--------------------------------------------------------------
   /// \brief	  Constructor
   /// \param[in] keywordName           The keyword name
   /// \param[in] Type of the IO        DIO / Relay
   /// \param[in] accessMode            access R/W of the IO
   //--------------------------------------------------------------
   CIO(const std::string& keywordName,
       const yApi::EKeywordAccessMode& accessMode);

   //--------------------------------------------------------------
   /// \brief	    Destructor
   //--------------------------------------------------------------
   virtual ~CIO();

   // ILoad Implementation
   boost::shared_ptr<const yApi::historization::IHistorizable> historizable() const override;
   void set(bool state) override;
   // [END] ILoad Implementation

   //--------------------------------------------------------------
   /// \brief	    get
   /// \note       read the state of the IO
   /// \return     state of the IO
   //--------------------------------------------------------------
   bool get(void);

protected:

private:

   //--------------------------------------------------------------
   /// \brief	    Keyword
   //--------------------------------------------------------------
   boost::shared_ptr<yApi::historization::CSwitch> m_value;
};