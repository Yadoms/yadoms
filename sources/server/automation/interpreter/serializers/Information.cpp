#include "stdafx.h"
#include "Information.h"

namespace automation
{
   namespace interpreter
   {
      namespace serializers
      {
         CInformation::CInformation(boost::shared_ptr<const shared::script::yInterpreterApi::IInformation> information)
            : m_information(information)
         {
         }

         CInformation::~CInformation()
         {
         }

         void CInformation::toPb(interpreter_IPC::toInterpreter::Information* pb) const
         {
            pb->set_type(m_information->getType());
            pb->set_name(m_information->getName());
            pb->set_description(m_information->getDescription());
            pb->set_version(m_information->getVersion().toString());
            pb->set_author(m_information->getAuthor());
            pb->set_url(m_information->getUrl());
            pb->set_credits(m_information->getCredits());
            pb->set_supportedonthisplatform(m_information->isSupportedOnThisPlatform());
            pb->set_packagefilecontent(m_information->getPackage()->serialize());
            pb->set_path(m_information->getPath().string());

            if (!pb->IsInitialized())
               throw std::runtime_error("unable to serialize interpreter information");
         }
      }
   }
} // namespace automation::interpreter::serializers


