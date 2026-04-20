file(GLOB PROCESSOR_SOURCES "*.cc")
add_library(processors SHARED ${PROCESSOR_SOURCES})

target_include_directories(processors PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(processors 
    ${ROOT_LIBRARIES} XMLParser
    -L${ANAROOT_LIB_DIR} anacore anasamurai anacatana
)

install(TARGETS processors DESTINATION lib)
install(DIRECTORY ./ DESTINATION include FILES_MATCHING PATTERN "*.hh")
