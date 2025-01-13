cmake_minimum_required(VERSION 3.19)

# add a target to generate API documentation with Doxygen
find_package(Doxygen)
# option(BUILD_DOCUMENTATION "Create and install the HTML based API documentation (requires Doxygen)" ${DOXYGEN_FOUND})
# option(BUILD_DOCUMENTATION "Create and install the HTML based API documentation (requires Doxygen)" OFF)
cmake_dependent_option(BUILD_DOCUMENTATION "Create and install the HTML based API documentation (requires Doxygen)" OFF
                       "DOXYGEN_FOUND" OFF)

if(BUILD_DOCUMENTATION)
    if(NOT DOXYGEN_FOUND)
        message(FATAL_ERROR "Doxygen is needed to build the documentation.")
    endif()
    
    set(images_dir ${PROJECT_SOURCE_DIR}/doc/images)
    set(doxyfile_in ${PROJECT_SOURCE_DIR}/doc/Doxyfile.in)
    set(doxy_main_page ${PROJECT_SOURCE_DIR}/doc/mainpage.md)

    set(doxyfile ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)

    configure_file(${doxyfile_in} ${doxyfile} @ONLY)

    add_custom_target(${PROJECT_NAME}-documentation ALL
        COMMAND ${DOXYGEN_EXECUTABLE} ${doxyfile}
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Generating API documentation with Doxygen"
        VERBATIM
		SOURCES ${doxy_main_page} ${doxyfile_in})

    source_group("Resource Files" FILES ${doxy_main_page})
    source_group("Config Files" FILES ${doxyfile_in})

    install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/html/ DESTINATION doc/${PROJECT_NAME})
endif()
