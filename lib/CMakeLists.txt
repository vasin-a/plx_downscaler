cmake_minimum_required(VERSION 3.20)

project(lib LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(SRC
	include/downscaler/glm.hpp
	include/downscaler/Pixmap.hpp
	include/downscaler/PixmapConverter.hpp
	include/downscaler/ScalingAlgorithm.hpp
	include/downscaler/Transform.hpp

	src/BoxBlurMipMap.cpp
	src/GaussianDownsample.cpp
	src/MipMapChain.cpp
	src/MipMapChain.hpp
	src/pch.hpp
	src/Sampler.hpp
	src/ScalingAlgorithm.cpp
	src/Transform.cpp
)

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${SRC})

add_library(${PROJECT_NAME} STATIC ${SRC})

target_precompile_headers(${PROJECT_NAME} PUBLIC src/pch.hpp)
target_include_directories(${PROJECT_NAME} PUBLIC include)
target_link_libraries(${PROJECT_NAME} PUBLIC glm stb)
target_link_libraries(${PROJECT_NAME} PRIVATE shared)
