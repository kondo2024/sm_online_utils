file(GLOB SRC_FILES "*.cc")
file(GLOB INC_FILES "*.hh")

add_library(smconverter SHARED ${SRC_FILES})

install(FILES ${INC_FILES} DESTINATION include)
install(TARGETS smconverter
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

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
