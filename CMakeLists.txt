project(ffmpeg_test)

set(src
    src/main.cpp
    thirdparty/tiny-process-library/process.cpp)

if(WIN32)
    list(APPEND src
        thirdparty/tiny-process-library/process_win.cpp)
else()
    list(APPEND src
        thirdparty/tiny-process-library/process_unix.cpp)
endif()

add_executable(
    ffmpeg_test
    ${src}
)

target_compile_features(ffmpeg_test PUBLIC cxx_std_14)