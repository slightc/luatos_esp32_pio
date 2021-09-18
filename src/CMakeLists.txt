FILE(GLOB_RECURSE app_sources ${CMAKE_SOURCE_DIR}/src/*.*)

idf_component_register(SRCS ${app_sources})

list(APPEND EXTRA_COMPONENT_DIRS components/lua)
list(APPEND EXTRA_COMPONENT_DIRS components/luat)
list(APPEND EXTRA_COMPONENT_DIRS components/net)
