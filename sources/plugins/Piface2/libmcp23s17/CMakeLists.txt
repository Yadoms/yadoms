#########################################
## The file allow building libmcp23s17
##
## Build tested with:
##		- Raspberry
##
#########################################

#########################################
## Main sources
#########################################
set(MCP23S17_SOURCES
		src/mcp23s17.h
		src/mcp23s17.c
)  

#########################################
## Define sourcegroup for IDE (msvc, xcode,...)
#########################################
source_group(src src/*.*)

#########################################
## Define project outputs
#########################################

add_library ( mcp23s17 STATIC ${MCP23S17_SOURCES} )
set_target_properties( mcp23s17 PROPERTIES OUTPUT_NAME "mcp23s17" )
set_target_properties( mcp23s17 PROPERTIES LIBRARY_OUTPUT_DIRECTORY static )
	
#########################################
## Define all include path
#########################################

set(MCP23S17_INCLUDE_DIRECTORY 
	${CMAKE_CURRENT_SOURCE_DIR}/src/
        PARENT_SCOPE)