#include "stdafx.h"
#include "Curtain.h"
#include "../StandardValues.h"
#include <shared/exception/InvalidParameter.hpp>


namespace shared { namespace plugin { namespace yadomsApi { namespace commands
{
DECLARE_ENUM_IMPLEMENTATION_NESTED(CCurtain::ECommand, ECommand,
   (Stop)
   (Open)
   (Close)
);

CCurtain::CCurtain(const std::string& keywordName)
   :m_keywordName(keywordName), m_command(ECommand::kStop)
{
}

CCurtain::~CCurtain()
{
}

const std::string& CCurtain::getKeyword() const
{
   return m_keywordName;
}

const CStandardCapacity& CCurtain::getCapacity() const
{
   return CStandardCapacities::Curtain;
}

void CCurtain::set(const shared::CDataContainer& yadomsCommand)
{
   m_command = yadomsCommand.get<ECommand>("cmd");
}

void CCurtain::set(ECommand command)
{
   m_command = command;
}

const CCurtain::ECommand CCurtain::command() const
{
   return m_command();
}

const std::string CCurtain::formatValue() const
{
   return boost::lexical_cast<std::string>(m_command());
}

} } } } // namespace shared::plugin::yadomsApi::commands

