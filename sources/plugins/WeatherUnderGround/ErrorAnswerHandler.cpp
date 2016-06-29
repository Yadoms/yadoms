#include "stdafx.h"
#include "ErrorAnswerHandler.h"

typedef std::list<std::string> EnumValuesTypes;

ErrorAnswerHandler::ErrorAnswerHandler(boost::shared_ptr<yApi::IYPluginApi> api,
                                       shared::CDataContainer response)
   : m_errorState(false)
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
      std::cerr << "ERROR : " << DescriptionError << std::endl;

      auto it = find(EEnumValuesTypes.begin(), EEnumValuesTypes.end(), error);
      if (it != EEnumValuesTypes.end())
      {
         //We use the same tag name that must be defined into the language package
         api->setPluginState(yApi::historization::EPluginState::kCustom, *it);
      }
      else
         std::cout << "This error tag is not handled yet by YADOMS" << error << std::endl;
   }
}

bool ErrorAnswerHandler::ContainError() const
{
   return m_errorState;
}

