cmake_minimum_required(VERSION 3.22.1)

project(stb LANGUAGES C)

set(SRC
	include/stb/stb_image.h
	include/stb/stb_image_write.h

	detail/stb.c
)

add_library(${PROJECT_NAME} STATIC ${SRC})

target_compile_definitions(${PROJECT_NAME} PRIVATE STB_IMAGE_WRITE_IMPLEMENTATION)
target_compile_definitions(${PROJECT_NAME} PRIVATE STB_IMAGE_IMPLEMENTATION)

target_include_directories(${PROJECT_NAME} PUBLIC "include")
