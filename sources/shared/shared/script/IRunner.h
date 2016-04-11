#pragma once
//TODO à virer
namespace shared { namespace script {

//-----------------------------------------------------
///\brief The process runner interface
//-----------------------------------------------------
class IRunner
{
public:
   //-----------------------------------------------------
   ///\brief               Destructor
   //-----------------------------------------------------
   virtual ~IRunner() {}

   //-----------------------------------------------------
   ///\brief               Kill the process
   //-----------------------------------------------------
   virtual void kill() = 0;
};

} } // namespace shared::script
	
	