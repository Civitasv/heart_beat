add_executable(heart_beat run.cc app.cc render_window.cc heart.cc)

set(SDL2_DIR_PATH ${CMAKE_SOURCE_DIR}/deps/sdl2)

target_include_directories(heart_beat PUBLIC
	${SDL2_DIR_PATH}/include
	${CMAKE_SOURCE_DIR}/include)

target_link_libraries(heart_beat PUBLIC
	${SDL2_DIR_PATH}/lib/x64/SDL2.lib
	${SDL2_DIR_PATH}/lib/x64/SDL2main.lib)