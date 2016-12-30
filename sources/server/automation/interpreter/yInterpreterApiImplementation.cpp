#include "stdafx.h"
#include "yInterpreterApiImplementation.h"

namespace automation
{
   namespace interpreter
   {
      CYInterpreterApiImplementation::CYInterpreterApiImplementation(boost::shared_ptr<const shared::script::yInterpreterApi::IInformation> interpreterInformations)
         : m_informations(interpreterInformations)
      {
      }

      CYInterpreterApiImplementation::~CYInterpreterApiImplementation()
      {
      }

      boost::shared_ptr<const shared::script::yInterpreterApi::IInformation> CYInterpreterApiImplementation::getInformation() const
      {
         return m_informations;
      }

      shared::event::CEventHandler& CYInterpreterApiImplementation::getEventHandler()
      {
         BOOST_ASSERT(false); // No event handler required here
         static shared::event::CEventHandler evtHandler;
         return evtHandler;
      }
   }
} // namespace automation::interpreter


