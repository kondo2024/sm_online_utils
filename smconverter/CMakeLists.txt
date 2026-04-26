include_directories(include)

file(GLOB LIB_SOURCES "src/*.cc")
add_library(smconverter SHARED ${LIB_SOURCES})

target_link_libraries(smconverter
    ${ROOT_LIBRARIES}
    XMLParser
    -L${ANAROOT_LIB_DIR} anacore anasamurai anacatana
)

file(GLOB APP_SOURCES "apps/*.cc")

foreach(source_file ${APP_SOURCES})
    get_filename_component(exe_name ${source_file} NAME_WE)
    add_executable(${exe_name} ${source_file})
    target_link_libraries(${exe_name} smconverter)
    install(TARGETS ${exe_name} DESTINATION bin)
endforeach()

install(TARGETS smconverter DESTINATION lib)
install(DIRECTORY include/ DESTINATION include/smconverter)
