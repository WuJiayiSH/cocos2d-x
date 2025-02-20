#/****************************************************************************
# Copyright (c) 2013 cocos2d-x.org
# Copyright (c) 2014 martell malone
# Copyright (c) 2015-2017 Chukong Technologies Inc.
#
# http://www.cocos2d-x.org
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:

# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.

# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.
# ****************************************************************************/

cmake_minimum_required(VERSION 3.6)

set(APP_NAME HelloJavascript)

project(${APP_NAME})

if(EMSCRIPTEN)
    set(CMAKE_CXX_FLAGS  "-s USE_PTHREADS -s USE_LIBJPEG=1 -s USE_LIBPNG=1 -s USE_ZLIB=1 -s USE_FREETYPE=1 -msse -msimd128 ")
endif()

set(RUNTIME_SRC_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/frameworks/runtime-src)
set(COCOS2DX_ROOT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/frameworks/cocos2d-x)
set(CMAKE_MODULE_PATH ${COCOS2DX_ROOT_PATH}/cmake/Modules/)

include(CocosBuildSet)
set(BUILD_JS_LIBS ON)
add_subdirectory(${COCOS2DX_ROOT_PATH}/cocos ${ENGINE_BINARY_PATH}/cocos/core)

# script and source files info, not need to compile
set(res_main_files
    "${CMAKE_CURRENT_SOURCE_DIR}/main.js"
    "${CMAKE_CURRENT_SOURCE_DIR}/project.json"
    )
set(res_res_folders
    "${CMAKE_CURRENT_SOURCE_DIR}/res"
    )
set(res_src_folders
    "${CMAKE_CURRENT_SOURCE_DIR}/src"
    )
set(res_script_folders
    "${COCOS2DX_ROOT_PATH}/cocos/scripting/js-bindings/script"
    )
if(APPLE OR VS)
    cocos_mark_multi_resources(res_main RES_TO "Resources" FILES ${res_main_files})
    cocos_mark_multi_resources(res_res RES_TO "Resources/res" FOLDERS ${res_res_folders})
    cocos_mark_multi_resources(res_src RES_TO "Resources/src" FOLDERS ${res_src_folders})
    cocos_mark_multi_resources(res_script RES_TO "Resources/script" FOLDERS ${res_script_folders})
    set(cc_common_res ${res_main} ${res_res} ${res_src} ${res_script})
endif()

# record sources, headers
set(GAME_SOURCE ${RUNTIME_SRC_ROOT}/Classes/AppDelegate.cpp)
set(GAME_HEADER ${RUNTIME_SRC_ROOT}/Classes/AppDelegate.h)

if(ANDROID)
    # change APP_NAME to the share library name for Android, it's value depend on AndroidManifest.xml
    set(APP_NAME cocos2djs)
    list(APPEND GAME_SOURCE ${RUNTIME_SRC_ROOT}/proj.android/app/jni/hellojavascript/main.cpp)
elseif(LINUX)
    list(APPEND GAME_SOURCE ${RUNTIME_SRC_ROOT}/proj.linux/main.cpp)
elseif(WINDOWS)
    set(WINDOWS_SRC ${RUNTIME_SRC_ROOT}/proj.win32/main.cpp)
    list(APPEND WINDOWS_SRC ${RUNTIME_SRC_ROOT}/proj.win32/game.rc)
    list(APPEND GAME_HEADER
         ${RUNTIME_SRC_ROOT}/proj.win32/main.h
         ${RUNTIME_SRC_ROOT}/proj.win32/resource.h
         )
    list(APPEND GAME_SOURCE ${WINDOWS_SRC} ${cc_common_res})
elseif(APPLE)
    if(IOS)
        list(APPEND GAME_HEADER
             ${RUNTIME_SRC_ROOT}/proj.ios_mac/ios/AppController.h
             ${RUNTIME_SRC_ROOT}/proj.ios_mac/ios/RootViewController.h
             )
        set(APP_UI_RES
            ${RUNTIME_SRC_ROOT}/proj.ios_mac/ios/LaunchScreen.storyboard
            ${RUNTIME_SRC_ROOT}/proj.ios_mac/ios/LaunchScreenBackground.png
            ${RUNTIME_SRC_ROOT}/proj.ios_mac/ios/Images.xcassets
            )
        list(APPEND GAME_SOURCE
             ${RUNTIME_SRC_ROOT}/proj.ios_mac/ios/main.m
             ${RUNTIME_SRC_ROOT}/proj.ios_mac/ios/AppController.mm
             ${RUNTIME_SRC_ROOT}/proj.ios_mac/ios/RootViewController.mm
             ${RUNTIME_SRC_ROOT}/proj.ios_mac/ios/Prefix.pch
             ${APP_UI_RES}
             )
    elseif(MACOSX)
        set(APP_UI_RES
            ${RUNTIME_SRC_ROOT}/proj.ios_mac/mac/Icon.icns
            ${RUNTIME_SRC_ROOT}/proj.ios_mac/mac/Info.plist
            )
        list(APPEND GAME_SOURCE
             ${RUNTIME_SRC_ROOT}/proj.ios_mac/mac/main.cpp
             ${RUNTIME_SRC_ROOT}/proj.ios_mac/mac/Prefix.pch
             ${APP_UI_RES}
             )
    endif()
    list(APPEND GAME_SOURCE ${cc_common_res})
elseif(EMSCRIPTEN)
    list(APPEND GAME_SOURCE ${RUNTIME_SRC_ROOT}/proj.emscripten/main.cpp)
endif()

set(APP_SRC ${GAME_HEADER} ${GAME_SOURCE})

# mark app complie info and libs info
if(NOT ANDROID)
    add_executable(${APP_NAME} ${APP_SRC})
else()
    add_library(${APP_NAME} SHARED ${APP_SRC})
    add_subdirectory(${COCOS2DX_ROOT_PATH}/cocos/platform/android ${ENGINE_BINARY_PATH}/cocos/platform)
    target_link_libraries(${APP_NAME} -Wl,--whole-archive cpp_android_spec -Wl,--no-whole-archive)
    add_subdirectory(${COCOS2DX_ROOT_PATH}/cocos/scripting/js-bindings/proj.android ${ENGINE_BINARY_PATH}/cocos/js-android)
    target_link_libraries(${APP_NAME} -Wl,--whole-archive js_android_spec -Wl,--no-whole-archive)
endif()

target_link_libraries(${APP_NAME} jscocos2d)
target_include_directories(${APP_NAME} PRIVATE ${RUNTIME_SRC_ROOT}/Classes)

# mark app resources, resource will be copy auto after mark
setup_cocos_app_config(${APP_NAME})
if(APPLE)
    set_target_properties(${APP_NAME} PROPERTIES RESOURCE "${APP_UI_RES}")
    if(MACOSX)
        set_target_properties(${APP_NAME} PROPERTIES
                              MACOSX_BUNDLE_INFO_PLIST "${RUNTIME_SRC_ROOT}/proj.ios_mac/mac/Info.plist"
                              )
    elseif(IOS)
        cocos_pak_xcode(${APP_NAME} INFO_PLIST "iOSBundleInfo.plist.in")
        set_xcode_property(${APP_NAME} ASSETCATALOG_COMPILER_APPICON_NAME "AppIcon")
        set_xcode_property(${APP_NAME} DEVELOPMENT_TEAM "")
        set_xcode_property(${APP_NAME} CODE_SIGN_IDENTITY "iPhone Developer")
    endif()
elseif(WINDOWS)
    cocos_copy_target_dll(${APP_NAME})
endif()
# copy resource on linux or WINDOWS
if(LINUX OR WINDOWS)
    set(APP_RES_DIR "$<TARGET_FILE_DIR:${APP_NAME}>/Resources")
    cocos_copy_target_res(${APP_NAME} COPY_TO ${APP_RES_DIR} FILES ${res_main_files})
    cocos_copy_target_res(${APP_NAME} COPY_TO ${APP_RES_DIR}/res FOLDERS ${res_res_folders})
    cocos_copy_target_res(${APP_NAME} COPY_TO ${APP_RES_DIR}/src FOLDERS ${res_src_folders})
    cocos_copy_target_res(${APP_NAME} COPY_TO ${APP_RES_DIR}/script FOLDERS ${res_script_folders})
endif()

if(EMSCRIPTEN)
    set(CMAKE_EXECUTABLE_SUFFIX ".html")
    target_link_options(${APP_NAME} PRIVATE
                        "-sEXPORTED_FUNCTIONS=[_cocos_ccdirector_pause,_cocos_ccdirector_resume,_cocos_ccdirector_step,_main]"
                        "-sEXPORTED_RUNTIME_METHODS=[ccall,cwrap]"
                        )
    set(EMSCRIPTEN_LINK_FLAGS "-s PTHREAD_POOL_SIZE=8 -s FETCH=1 -s USE_GLFW=3 -s FORCE_FILESYSTEM=1 -s VERBOSE=1 -s ERROR_ON_UNDEFINED_SYMBOLS=0 -lidbfs.js -s MIN_WEBGL_VERSION=2 -s MAX_WEBGL_VERSION=2 -s INITIAL_MEMORY=512MB --shell-file ${CMAKE_CURRENT_SOURCE_DIR}/index.html --use-preload-cache")

    foreach(res_main_file IN LISTS res_main_files)
        get_filename_component(res_main_filename ${res_main_file} NAME)
        string(APPEND EMSCRIPTEN_LINK_FLAGS " --preload-file ${res_main_file}@/${res_main_filename}")
    endforeach()

    foreach(res_res_folder IN LISTS res_res_folders)
        string(APPEND EMSCRIPTEN_LINK_FLAGS " --preload-file ${res_res_folder}/@/res")
    endforeach()

    foreach(res_src_folder IN LISTS res_src_folders)
        string(APPEND EMSCRIPTEN_LINK_FLAGS " --preload-file ${res_src_folder}/@/src")
    endforeach()

    foreach(res_script_folder IN LISTS res_script_folders)
        string(APPEND EMSCRIPTEN_LINK_FLAGS " --preload-file ${res_script_folder}/@/script")
    endforeach()
    
    set_target_properties(${APP_NAME} PROPERTIES LINK_FLAGS "${EMSCRIPTEN_LINK_FLAGS}")
endif()