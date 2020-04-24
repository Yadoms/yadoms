#pragma once

class CCommand
{
public:
	CCommand(bool cmd, const std::string& data);
	virtual ~CCommand();
	bool isControl();
	const std::string& get();
private:
	bool m_control;
	std::string m_content;
};
