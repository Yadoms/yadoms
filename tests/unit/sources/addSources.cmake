
##################################################################################################
## Macro adding sources to the parent target
##################################################################################################
macro(ADD_SOURCES)
    file (RELATIVE_PATH _relPath "${CMAKE_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}")
    foreach (_src ${ARGN})
        if (_relPath)
            list (APPEND YADOMS_TESTS_SRC "${_relPath}/${_src}")
        else()
            list (APPEND YADOMS_TESTS_SRC "${_src}")
        endif()
    endforeach()
    if (_relPath)
        # propagate YADOMS_TESTS_SRC to parent directory
        set (YADOMS_TESTS_SRC ${YADOMS_TESTS_SRC} PARENT_SCOPE)
    endif()
endmacro()

##################################################################################################
## Macro adding yadoms sources to the parent target
##################################################################################################
macro(ADD_YADOMS_SOURCES)
    foreach (_src ${ARGN})
       list (APPEND YADOMS_TESTS_SRC ${YADOMS_PATH}/${_src})
    endforeach()
    # propagate YADOMS_TESTS_SRC to parent directory
    set (YADOMS_TESTS_SRC ${YADOMS_TESTS_SRC} PARENT_SCOPE)
endmacro()

##################################################################################################
## Macro adding yadoms include directories to the parent target
##################################################################################################
macro(ADD_YADOMS_INCL_DIR)
    foreach (_src ${ARGN})
       list (APPEND YADOMS_INCL_DIR ${_src})
    endforeach()
    # propagate YADOMS_INCL_DIR to parent directory
    set (YADOMS_INCL_DIR ${YADOMS_INCL_DIR} 
         CACHE INTERNAL "" FORCE
     #   TODO : Understand why with Linux PARENT_SCOPE doesn't work here !
    )
endmacro()
