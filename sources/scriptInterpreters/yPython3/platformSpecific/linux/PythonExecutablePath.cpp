#include "stdafx.h"
#include "PythonExecutablePath.h"

boost::shared_ptr<std::vector<boost::filesystem::path>> CPythonExecutablePath::getCommonPythonPaths()
{
   return boost::make_shared<std::vector<boost::filesystem::path>>();
}

const std::string& CPythonExecutablePath::getExecutableName()
{
   static const std::string ExecutableName("python3");
   return ExecutableName;
}


