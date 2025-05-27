file(GLOB SRC_FILES "*.cc")
file(GLOB INC_FILES "*.hh")
message(STATUS ${use_fsdbsd})

##### add this line if you add detector with a name starting from T
if (NOT ${use_fsdbsd})
  list(FILTER INC_FILES EXCLUDE REGEX ".*FSDBSD.*hh$")
  list(FILTER SRC_FILES EXCLUDE REGEX ".*FSDBSD.*cc$")
endif()
#list(FILTER TSRC_FILES EXCLUDE REGEX "PFAD*.cc$")






###  ---------------------------
### Make smconverter_linkdef.hh
#### make smconverter_linkdef.hh
message(STATUS "Making dictionary smconverter for ROOT")
file(WRITE smconverter_linkdef.hh "#if defined(__CINT__) || defined(__CLING__)\n")
file(APPEND smconverter_linkdef.hh "#pragma link off all globals;\n")
file(APPEND smconverter_linkdef.hh "#pragma link off all classes;\n")
file(APPEND smconverter_linkdef.hh "#pragma link off all functions;\n")

list(APPEND CMAKE_MESSAGE_INDENT "  ")
foreach(loop_var IN LISTS TINC_FILES)
  message(STATUS "${loop_var}")
  get_filename_component(loop_var_filename "${loop_var}" NAME_WE)
  file(APPEND smconverter_linkdef.hh "#pragma link C++ class ${loop_var_filename}+;\n")
endforeach()
list(POP_BACK CMAKE_MESSAGE_INDENT)
file(APPEND smconverter_linkdef.hh "#endif\n")

ROOT_GENERATE_DICTIONARY(G__smconverter
  ${TINC_FILES}
  LINKDEF smconverter_linkdef.hh
)
###  ---------------------------

add_library(smconverter SHARED ${SRC_FILES} G__smconverter.cxx)

install(FILES ${INC_FILES} DESTINATION include)
install(TARGETS smconverter
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

install(
      FILES
      ${CMAKE_CURRENT_BINARY_DIR}/libsmconverter_rdict.pcm
      ${CMAKE_CURRENT_BINARY_DIR}/libsmconverter.rootmap
      DESTINATION lib/)



  #
#
#include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include ${CMAKE_CURRENT_SOURCE_DIR} )
#
#
#ROOT_GENERATE_DICTIONARY(G__anatogaxsi
#  ${HEADERS}
#  LINKDEF anatogaxsi_linkdef.hh
#  )
#
#
#add_library(anatogaxsi SHARED
#                    ${SRCS}
#        G__anatogaxsi.cxx
#        )
#
##target_link_libraries(anatogaxsi ${ROOT_LIBRARIES} anacore)
#target_link_libraries(anatogaxsi ${ROOT_LIBRARIES})
#
#
#
#install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/
#                  DESTINATION include)
#
#install(TARGETS anatogaxsi
#  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
#
#install(
#      FILES
#      ${CMAKE_CURRENT_BINARY_DIR}/libanatogaxsi_rdict.pcm
#      ${CMAKE_CURRENT_BINARY_DIR}/libanatogaxsi.rootmap
#      DESTINATION lib/)
#
