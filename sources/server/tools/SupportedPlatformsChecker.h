#pragma once
#include <shared/DataContainer.h>


namespace tools
{

   //---------------------------------------------
   ///\brief This class helps to check if Yadoms element (plugins, scriptInterpreters...) is
   // compatible to current platform (from the "supportedPlatforms" node of the element configuration)
   //---------------------------------------------
   class CSupportedPlatformsChecker
   {
   public:
      //---------------------------------------------
      ///\brief Contructor
      //---------------------------------------------
      CSupportedPlatformsChecker();

      //---------------------------------------------
      ///\brief Destructor
      //---------------------------------------------
      virtual ~CSupportedPlatformsChecker();

      //---------------------------------------------
      ///\brief Check if element is supported by current platform
      ///\param[in] supportedPlatformsNode The "supportedPlatforms" node in the element package.json configuration file
      ///\return true is element is supported by current platform, false if not
      ///\throw CInvalidParameter if fail to parse node
      //---------------------------------------------
      static bool isSupported(const shared::CDataContainer& supportedPlatformsNode);

   protected:
      //---------------------------------------------
      ///\brief	    Parse the "supportedPlatforms" from/to versions sub-node to check if platform version is compatible
      ///\param[in] supportedPlatformsVersionNode The sub-node of "supportedPlatforms" node, describing version support
      ///\return     true if supported, false if not
      ///\throw CInvalidParameter if fail to parse node
      //---------------------------------------------
      static bool isVersionSupported(const shared::CDataContainer& supportedPlatformsVersionNode);
   };

} // namespace tools