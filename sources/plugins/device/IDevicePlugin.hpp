#ifndef _PLUGIN_HPP_
#define _PLUGIN_HPP_

#include <string>

#ifdef WIN32
#define EXPORT_MAIN_FUNCTIONS __declspec(dllexport)
#else
#define EXPORT_MAIN_FUNCTIONS 
#endif

namespace plugins
{
  class Plugin 
  {  
  public:
    virtual std::string toString() = 0;
  };
}

#endif