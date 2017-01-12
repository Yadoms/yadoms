#include "stdafx.h"
#include "ErrorAnswerHandler.h"
#include "webSiteErrorException.hpp"
#include <shared/Log.h>

typedef std::list<std::string> EnumValuesTypes;

ErrorAnswerHandler::ErrorAnswerHandler(boost::shared_ptr<yApi::IYPluginApi> api,
                                       shared::CDataContainer response):
   m_errorState(false),
   m_errorStatePlugin("")
{
   //list of errors created into language packages

   static const EnumValuesTypes EEnumValuesTypes = boost::assign::list_of
      ("keynotfound")
      ("querynotfound");

   auto error = response.getWithDefault<std::string>("response.error.type", "");

   if (!error.empty())
   {
      m_errorState = true;

      auto DescriptionError = response.getWithDefault<std::string>("response.error.description", "");
      YADOMS_LOG(error) << "ERROR : " << DescriptionError ;

      auto it = find(EEnumValuesTypes.begin(), EEnumValuesTypes.end(), error);
      if (it != EEnumValuesTypes.end())
         m_errorStatePlugin = *it;
      else
         YADOMS_LOG(information) << "This error tag is not handled yet by YADOMS" << error ;
   }
}

bool ErrorAnswerHandler::ContainError() const
{
   return m_errorState;
}

std::string ErrorAnswerHandler::getError() const
{
   return m_errorStatePlugin;
}