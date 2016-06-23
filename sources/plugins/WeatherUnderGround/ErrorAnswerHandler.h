#pragma once
#include <shared/plugin/yPluginApi/IYPluginApi.h>
#include <shared/DataContainer.h>
#include <shared/enumeration/EnumHelpers.hpp>

namespace yApi = shared::plugin::yPluginApi;

//-----------------------------------------------------
///\brief   Internal Response type name
//-----------------------------------------------------
DECLARE_ENUM_HEADER(EZWaveInteralState,
   ((Running))
   ((DriverReady))
   ((DriverFailed))
   ((Waiting))
   ((Completed))
)

//--------------------------------------------------------------
/// \brief	ErrorAnswerHandler Module
/// \note   This class consist of the traitment of error return by the website
//--------------------------------------------------------------
class ErrorAnswerHandler
{
public:
   //--------------------------------------------------------------
   /// \brief	  Constructor
   /// \param[in] api        pointer to the API
   /// \param[in] response   The response to analyse
   //--------------------------------------------------------------
   ErrorAnswerHandler(boost::shared_ptr<yApi::IYPluginApi> api, shared::CDataContainer response);

   //--------------------------------------------------------------
   /// \brief	  ContainError
   /// \return    if an error is return
   //--------------------------------------------------------------
   bool ContainError() const;
private:

   //--------------------------------------------------------------
   /// \brief	    Error State
   //--------------------------------------------------------------
   bool m_ErrorState;
};

