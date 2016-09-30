#########################################
## The file allow building libpifacedigital
##
## Build tested with:
##		- Raspberry
##
#########################################

#########################################
## Main sources
#########################################
set(PIFACEDIGITAL_SOURCES
		src/pifacedigital.h
		src/pifacedigital.c
)

#########################################
## Define sourcegroup for IDE (msvc, xcode,...)
#########################################
source_group(src src/*.*)

#MESSAGE("PIFACEDIGITAL_SOURCES:" ${PIFACEDIGITAL_SOURCES})
#MESSAGE("MCP23S17_INCLUDE_DIRECTORY:" ${MCP23S17_INCLUDE_DIRECTORY})

#########################################
## Define project outputs
#########################################

add_library ( pifacedigital STATIC ${PIFACEDIGITAL_SOURCES})
set_property( TARGET pifacedigital PROPERTY INCLUDE_DIRECTORIES ${MCP23S17_INCLUDE_DIRECTORY})
set_target_properties( pifacedigital PROPERTIES OUTPUT_NAME "pifacedigital" )
set_target_properties( pifacedigital PROPERTIES LIBRARY_OUTPUT_DIRECTORY static )
target_link_libraries( pifacedigital mcp23s17 )
	
#########################################
## Define all include path
#########################################

set(PIFACE2DIGITAL_INCLUDE_DIRECTORY 
	${CMAKE_CURRENT_SOURCE_DIR}/src/
        PARENT_SCOPE)