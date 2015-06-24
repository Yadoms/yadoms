#pragma once

namespace automation { namespace script
{
   //-----------------------------------------------------
   ///\brief The general information getter interface
   //-----------------------------------------------------
   class IGeneralInfo
   {
   public:
      //-----------------------------------------------------
      ///\brief               Destructor
      //-----------------------------------------------------
      virtual ~IGeneralInfo() {}

      //-----------------------------------------------------
      ///\brief Get an information by key
      ///\param[in] key Information key
      ///\return Information as string (empty string if not found)
      //-----------------------------------------------------
      virtual std::string get(const std::string& key) const = 0;
   };

} } // namespace automation::script

