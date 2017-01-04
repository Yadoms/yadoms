#include "stdafx.h"
#include "ScriptLogger.h"


CScriptLogger::CScriptLogger(int scriptInstanceId)
   :m_logPrefix("(" + std::to_string(scriptInstanceId) + ") ")
{
}

CScriptLogger::~CScriptLogger()
{
}

void CScriptLogger::init()
{
}

void CScriptLogger::information(const std::string& line)
{
   std::cout << m_logPrefix + line;
}

void CScriptLogger::error(const std::string& line)
{
   std::cerr << m_logPrefix + line;
}
