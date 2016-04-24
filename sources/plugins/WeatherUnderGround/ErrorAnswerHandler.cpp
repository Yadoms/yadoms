#include "stdafx.h"
#include "ErrorAnswerHandler.h"
#include <shared/Log.h>
#include <shared/exception/Exception.hpp>

typedef std::list<std::string> EnumValuesTypes;

ErrorAnswerHandler::ErrorAnswerHandler(boost::shared_ptr<yApi::IYPluginApi> context, shared::CDataContainer response ):
                   m_ErrorState (false)
{
	//list of errors created into language packages

	static const EnumValuesTypes EEnumValuesTypes = boost::assign::list_of
		( "keynotfound" )
		( "querynotfound" )
        ;

	std::string error = response.getWithDefault<std::string>( "response.error.type","" );

	if (!error.empty())
	{
		m_ErrorState = true;

		std::string DescriptionError = response.getWithDefault<std::string>( "response.error.description","" );
		YADOMS_LOG(error) << "ERROR : " << DescriptionError  << std::endl;

		EnumValuesTypes::const_iterator it = find( EEnumValuesTypes.begin(), EEnumValuesTypes.end(), error );
	    if (it != EEnumValuesTypes.end())
	    {
			//We use the same tag name that must be defined into the language package
			context->setPluginState(yApi::historization::EPluginState::kCustom, (const std::string &) *it );
		}
	}
}

bool ErrorAnswerHandler::ContainError ( void )
{
	return m_ErrorState;
}