cmake_minimum_required(VERSION 3.10)

project(file-to-header)

function(download_file url filename)

if(NOT EXISTS ${filename})
  file(DOWNLOAD ${url} ${filename}
       SHOW_PROGRESS
       TIMEOUT 60  # seconds
       TLS_VERIFY ON)
endif()

endfunction(download_file)

download_file("https://www.khronos.org/registry/OpenGL/api/GL/glcorearb.h" "${CMAKE_CURRENT_SOURCE_DIR}/GL/glcorearb.h")
download_file("https://www.khronos.org/registry/EGL/api/KHR/khrplatform.h" "${CMAKE_CURRENT_SOURCE_DIR}/KHR/khrplatform.h")
download_file("https://raw.githubusercontent.com/kcat/openal-soft/master/include/AL/al.h" "${CMAKE_CURRENT_SOURCE_DIR}/AL/al.h")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")
set(APP_NAME f2h)

set(HEADERS
    targa.h
    wave.h
    wavefront.h
)

set(SOURCES
    main.c
    targa.c
    wave.c
    wavefront.c
)

add_executable(${APP_NAME} ${HEADERS} ${SOURCES})

target_compile_features(${APP_NAME} PUBLIC c_std_11)

target_compile_options(${APP_NAME} PUBLIC -pedantic -Wall)

target_include_directories(${APP_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
