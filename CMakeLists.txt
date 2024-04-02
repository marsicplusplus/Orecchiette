cmake_minimum_required( VERSION 3.1 )

project( Orecchiette )

set( GLFW_BUILD_DOCS OFF CACHE BOOL "GLFW lib only" FORCE)
set( GLFW_BUILD_EXAMPLES OFF CACHE BOOL "GLFW lib only" FORCE)
set( GLFW_BUILD_TESTS OFF CACHE BOOL "GLFW lib only" FORCE)
set( GLFW_INSTALL OFF CACHE BOOL "GLFW lib only" FORCE)

add_subdirectory( external/glfw )
add_subdirectory( external/plog )

include_directories( 
	external/glfw/include 
	external/glad/include
	external/glm
	external/plog/include
	src/
	include/
	)

if( MSVC )
    SET( CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /ENTRY:mainCRTStartup" )
endif()

set( SRC
	 external/glad/src/glad.c
	 src/core/renderer.cpp
	 src/core/framebuffer.cpp
	 src/core/OrthonormalBasis.cpp
	 src/core/ray.cpp
	 src/core/transform.cpp
	 src/core/options.cpp
	 src/core/scene.cpp
	 src/core/thread_pool.cpp
	 src/core/primitive.cpp
	 src/emitters/area.cpp
	 src/emitters/point_light.cpp
	 src/materials/diffuse.cpp
	 src/materials/emissive.cpp
	 src/shapes/sphere.cpp
	 src/shapes/xyrect.cpp
	 src/shapes/triangle.cpp
	 src/cameras/perspective.cpp
	 src/samplers/xorshift.cpp
	 src/orecchiette.cpp
	 src/managers/input_manager.cpp
	)

add_executable( Orecchiette ${SRC})
target_link_libraries( Orecchiette 
						glfw 
					)
target_compile_options( Orecchiette PRIVATE )
set_property(TARGET Orecchiette PROPERTY CXX_STANDARD 17)