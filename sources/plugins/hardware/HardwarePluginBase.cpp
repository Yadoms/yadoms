# include "HardwarePluginBase.h"

namespace plugins
{
   CHardwarePluginBase::CHardwarePluginBase(const std::string & name)
	   :CThreadBase(name)
   {
   }

   CHardwarePluginBase::~CHardwarePluginBase()
   {
   }

   const std::string CHardwarePluginBase::toString()
   {
	   return getName();
   }
};
