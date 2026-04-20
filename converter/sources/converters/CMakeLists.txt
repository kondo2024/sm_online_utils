file(GLOB CONVERTER_SOURCES "*.cc")

foreach(source_file ${CONVERTER_SOURCES})
    get_filename_component(exe_name ${source_file} NAME_WE)
    add_executable(${exe_name} ${source_file})
    target_link_libraries(${exe_name} 
        processors
        ${ROOT_LIBRARIES}
    )

    install(TARGETS ${exe_name} DESTINATION bin)
    
    message(STATUS "Found converter target: ${exe_name}")
endforeach()
