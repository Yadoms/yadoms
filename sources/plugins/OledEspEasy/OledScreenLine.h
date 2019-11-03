#pragma once

#include <shared/plugin/yPluginApi/IYPluginApi.h>
#include <shared/plugin/yPluginApi/historization/Text.h>

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;


//--------------------------------------------------------------
/// \brief	Configuration of the empty plugin
//--------------------------------------------------------------
class COledScreenLine
{
public:
	COledScreenLine(int line);
	virtual ~COledScreenLine();

	int getLine() const;
	const std::string & getKeywordName() const;
	boost::shared_ptr< yApi::historization::CText> & getHistorizer();
private:
	std::string m_keywordName;
	int m_line;
	boost::shared_ptr< yApi::historization::CText> m_historizer;
};

