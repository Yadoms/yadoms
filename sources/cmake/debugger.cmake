# Add VisualStudio NATVIS
#
# Natvis are files to add to VS project to allow an easiest debugging.
# It allows to custom display of some class (example for rapidjson, it displays the container as json tree, instead of unreadable bytes)
# These enable custom debug visualizers for project-specific types
#=============================================================================
MACRO(ADD_VS_NATVIS _targetName)

   if (MSVC)
      target_sources(${_targetName} PUBLIC ${CMAKE_SOURCE_DIR}/../ideTools/vs_natvis/rapidjson.natvis)
      target_sources(${_targetName} PUBLIC ${CMAKE_SOURCE_DIR}/../ideTools/vs_natvis/boost.natvis)
      target_sources(${_targetName} PUBLIC ${CMAKE_SOURCE_DIR}/../ideTools/vs_natvis/boost_Containers.natvis)
      target_sources(${_targetName} PUBLIC ${CMAKE_SOURCE_DIR}/../ideTools/vs_natvis/boost_DateTime.natvis)
      target_sources(${_targetName} PUBLIC ${CMAKE_SOURCE_DIR}/../ideTools/vs_natvis/boost_Geometry.natvis)
      target_sources(${_targetName} PUBLIC ${CMAKE_SOURCE_DIR}/../ideTools/vs_natvis/boost_Gil.natvis)
      target_sources(${_targetName} PUBLIC ${CMAKE_SOURCE_DIR}/../ideTools/vs_natvis/boost_IntrusiveContainers.natvis)
      target_sources(${_targetName} PUBLIC ${CMAKE_SOURCE_DIR}/../ideTools/vs_natvis/boost_MultiArray.natvis)
      target_sources(${_targetName} PUBLIC ${CMAKE_SOURCE_DIR}/../ideTools/vs_natvis/boost_MultiIndex.natvis)
      target_sources(${_targetName} PUBLIC ${CMAKE_SOURCE_DIR}/../ideTools/vs_natvis/boost_Multiprecision.natvis)
      target_sources(${_targetName} PUBLIC ${CMAKE_SOURCE_DIR}/../ideTools/vs_natvis/boost_PointerContainerLibrary.natvis)
      target_sources(${_targetName} PUBLIC ${CMAKE_SOURCE_DIR}/../ideTools/vs_natvis/boost_PropertyTree.natvis)
      target_sources(${_targetName} PUBLIC ${CMAKE_SOURCE_DIR}/../ideTools/vs_natvis/boost_Rational.natvis)
      target_sources(${_targetName} PUBLIC ${CMAKE_SOURCE_DIR}/../ideTools/vs_natvis/boost_Regex.natvis)
      target_sources(${_targetName} PUBLIC ${CMAKE_SOURCE_DIR}/../ideTools/vs_natvis/boost_SmartPointers.natvis)
      target_sources(${_targetName} PUBLIC ${CMAKE_SOURCE_DIR}/../ideTools/vs_natvis/boost_uBLAS.natvis)
      target_sources(${_targetName} PUBLIC ${CMAKE_SOURCE_DIR}/../ideTools/vs_natvis/boost_Unordered.natvis)
      target_sources(${_targetName} PUBLIC ${CMAKE_SOURCE_DIR}/../ideTools/vs_natvis/boost_Variant.natvis)
      
      source_group(natvis ${PROJECT_SOURCE_DIR}/../ideTools/vs_natvis/*.natvis)
   endif()

ENDMACRO()

