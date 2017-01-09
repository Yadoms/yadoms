#pragma once
#include <shared/plugin/yPluginApi/IYPluginApi.h>
#include <shared/DataContainer.h>

namespace yApi = shared::plugin::yPluginApi;

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
   ErrorAnswerHandler(boost::shared_ptr<yApi::IYPluginApi> api,
                      shared::CDataContainer response);

   //--------------------------------------------------------------
   /// \brief	  ContainError
   /// \return    if an error is return
   //--------------------------------------------------------------
   bool ContainError() const;

   //--------------------------------------------------------------
   /// \brief	  Return the error type
   /// \return    the error name
   //--------------------------------------------------------------
   std::string getError() const;

private:

   //--------------------------------------------------------------
   /// \brief	    Error State
   //--------------------------------------------------------------
   bool m_errorState;

   //--------------------------------------------------------------
   /// \brief	    Error State plugin
   //--------------------------------------------------------------
   std::string m_errorStatePlugin;
};

