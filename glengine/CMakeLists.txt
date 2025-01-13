project(glengine)
cmake_minimum_required(VERSION 3.5)

set(SRC_DIR "${PROJECT_SOURCE_DIR}/src")
set(INC_DIR "${PROJECT_SOURCE_DIR}/include/")

#Configure config.hpp.in
configure_file(
	${PROJECT_SOURCE_DIR}/include/${PROJECT_NAME}/config.hpp.in
  ${CMAKE_CURRENT_BINARY_DIR}/include/${PROJECT_NAME}/config.hpp @ONLY
)

include_directories(${CMAKE_CURRENT_BINARY_DIR}/include BEFORE)

set(SRC
  ${SRC_DIR}/orbitalCamera.cpp
  ${SRC_DIR}/glengine.cpp
  ${SRC_DIR}/loader.cpp
  ${SRC_DIR}/computeNormal.cpp
)

set(HEADER
  # ${INC_DIR}/${PROJECT_NAME}/config.hpp.in
  ${PROJECT_SOURCE_DIR}/include/${PROJECT_NAME}/config.hpp.in
  ${INC_DIR}/${PROJECT_NAME}/orbitalCamera.hpp
  ${INC_DIR}/${PROJECT_NAME}/glengine.hpp
  ${INC_DIR}/${PROJECT_NAME}/loader.hpp
  ${INC_DIR}/${PROJECT_NAME}/computeNormal.hpp
)

add_library(${PROJECT_NAME} ${SRC} ${HEADER})
target_include_directories (${PROJECT_NAME}
  PUBLIC ${INC_DIR}
)

install(
  TARGETS ${PROJECT_NAME}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

# add_subdirectory(doc)