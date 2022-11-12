add_executable(heart_beat run.cc app.cc render_window.cc heart.cc)

set(SDL2_DIR_PATH ${CMAKE_SOURCE_DIR}/deps/sdl2)
set(SDL2_TTF_DIR_PATH ${CMAKE_SOURCE_DIR}/deps/sdl2_ttf)

target_include_directories(heart_beat PUBLIC
	${SDL2_DIR_PATH}/include
	${CMAKE_SOURCE_DIR}/include)

target_link_libraries(heart_beat PUBLIC
	${SDL2_DIR_PATH}/lib/x64/SDL2.lib
	${SDL2_DIR_PATH}/lib/x64/SDL2main.lib)

add_executable(heart_beat_c heart.c)

target_include_directories(heart_beat_c PUBLIC
	${SDL2_DIR_PATH}/include
	${SDL2_TTF_DIR_PATH}/include
	${CMAKE_SOURCE_DIR}/include)

target_link_libraries(heart_beat_c PUBLIC
	${SDL2_DIR_PATH}/lib/x64/SDL2.lib
	${SDL2_DIR_PATH}/lib/x64/SDL2main.lib
	${SDL2_TTF_DIR_PATH}/lib/x64/SDL2_ttf.lib)