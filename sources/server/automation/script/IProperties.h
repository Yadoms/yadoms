#pragma once
#include <shared/DataContainer.h>

namespace automation
{
   namespace script
   {
      //-----------------------------------------------------
      ///\brief The script properties interface
      //-----------------------------------------------------
      class IProperties
      {
      public:
         //-----------------------------------------------------
         ///\brief               Destructor
         //-----------------------------------------------------
         virtual ~IProperties()
         {
         }

         //-----------------------------------------------------
         ///\brief               Get the name of the associated interperter
         ///\return              The interpreter name
         //-----------------------------------------------------
         virtual std::string interpreterName() const = 0;

         //-----------------------------------------------------
         ///\brief               Check if the rule is model-based, or localy defined
         ///\return              true if model-based
         //-----------------------------------------------------
         virtual bool isModelBased() const = 0;

         //-----------------------------------------------------
         ///\brief               Get the root script path
         ///\return              The root script path
         //-----------------------------------------------------
         virtual boost::filesystem::path scriptPath() const = 0;

         //-----------------------------------------------------
         ///\brief               Get the path of the log file associated with this script
         ///\return              The log file path
         //-----------------------------------------------------
         virtual boost::filesystem::path logPath() const = 0;

         //-----------------------------------------------------
         ///\brief               Get the rule configuration, if any (only able for model-based rules
         ///\return              The configuration (can be empty)
         //-----------------------------------------------------
         virtual const shared::CDataContainer& configuration() const = 0;
      };
   }
} // namespace automation::script


