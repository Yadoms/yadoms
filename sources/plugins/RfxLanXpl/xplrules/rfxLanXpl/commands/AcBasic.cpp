#include "stdafx.h"
#include "AcBasic.h"
#include <shared/exception/InvalidParameter.hpp>


namespace xplrules { namespace rfxLanXpl { namespace commands {

   CAcBasic::CAcBasic(const std::string& command)
      :Command("command"), Level("level")
   {
      shared::CDataContainer yadomsCommandTree(command);
      
      Command.read(yadomsCommandTree);

      if (!Command.isDefined())
         throw shared::exception::CException("ac.basic protocol needs a parameter command");

      if (!boost::iequals(Command(), "on") &&
         !boost::iequals(Command(), "off") &&
         !boost::iequals(Command(), "preset"))
      {
         throw shared::exception::CInvalidParameter("Invalid ac.basic command \"" + command + "\" : command out of range [on, off, preset]");
      }

      Level.read(yadomsCommandTree);
      
      if (Level.isDefined())
      {
         BOOST_ASSERT_MSG(Level() >= 0 && Level() <= 15, "Level value out of range");
         if (Level() < 0 || Level() > 15)
            throw shared::exception::CInvalidParameter("Invalid ac.basic command \"" + command + "\" : level out of range");
      }
   }

   CAcBasic::~CAcBasic()
   {
   }


} // namespace commands
} // namespace rfxLanXpl 
} // namespace xplrules
