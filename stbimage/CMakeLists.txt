project(stbimage)
cmake_minimum_required(VERSION 3.5)

set(SRC_DIR "${PROJECT_SOURCE_DIR}/src")
set(INC_DIR "${PROJECT_SOURCE_DIR}/include/")

include_directories(${CMAKE_CURRENT_BINARY_DIR}/include BEFORE)

set(SRC
  ${SRC_DIR}/stb_image.cpp
)

set(HEADER
  ${INC_DIR}/${PROJECT_NAME}/stb_image.h
  ${INC_DIR}/${PROJECT_NAME}/stb_image_resize2.h
  ${INC_DIR}/${PROJECT_NAME}/stb_image_write.h
)

set(RESOURCE_FILES
#	${PROJECT_SOURCE_DIR}/resources/truc.txt
)

add_library(${PROJECT_NAME} ${SRC} ${HEADER} ${RESOURCE_FILES})
target_include_directories (${PROJECT_NAME}
  PUBLIC ${INC_DIR}
)

install(
	TARGETS ${PROJECT_NAME}
	RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

