#include "stdafx.h"
#include "PythonExecutablePath.h"

void CPythonExecutablePath::getCommonPythonPaths(std::vector<boost::filesystem::path> & paths)
{
}

const std::string& CPythonExecutablePath::getExecutableName()
{
   static const std::string ExecutableName("python3");
   return ExecutableName;
}


