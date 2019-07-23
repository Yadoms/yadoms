#include "stdafx.h"
#include "OledScreenLine.h"

COledScreenLine::COledScreenLine(int line)
	: m_line(line), 
	m_keywordName((boost::format("line_%1%") % line).str()), 
	m_historizer(boost::make_shared<yApi::historization::CText>(m_keywordName, yApi::EKeywordAccessMode::kGetSet, shared::plugin::yPluginApi::EMeasureType::kAbsolute, shared::plugin::yPluginApi::typeInfo::CStringTypeInfo::Empty, shared::plugin::yPluginApi::EHistoryDepth::kNoHistory))
{
	
}

COledScreenLine::~COledScreenLine()
{
	
}

const int COledScreenLine::getLine() const
{
	return m_line;
}

const std::string& COledScreenLine::getKeywordName() const
{
	return m_keywordName;
}

boost::shared_ptr< yApi::historization::CText>& COledScreenLine::getHistorizer()
{
	return m_historizer;
}