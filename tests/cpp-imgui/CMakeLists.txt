cmake_minimum_required(VERSION 3.14)

set(APP_NAME cpp-imgui)

project(${APP_NAME})

if(EMSCRIPTEN)
    set(CMAKE_C_FLAGS  "-s FORCE_FILESYSTEM=1 -s FETCH=1 -s USE_GLFW=3 -s VERBOSE=1 -s USE_LIBJPEG=1 -s USE_LIBPNG=1 -s USE_ZLIB=1 -s USE_FREETYPE=1 -msse -msimd128 ")
    set(CMAKE_CXX_FLAGS  "-s FORCE_FILESYSTEM=1 -s FETCH=1 -s USE_GLFW=3 -s VERBOSE=1 -s USE_LIBJPEG=1 -s USE_LIBPNG=1 -s USE_ZLIB=1 -s USE_FREETYPE=1 -msse -msimd128 ")
endif()

if(NOT DEFINED BUILD_ENGINE_DONE)
    set(COCOS2DX_ROOT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../..)
    set(CMAKE_MODULE_PATH ${COCOS2DX_ROOT_PATH}/cmake/Modules/)

    include(CocosBuildSet)
    add_subdirectory(${COCOS2DX_ROOT_PATH}/cocos ${ENGINE_BINARY_PATH}/cocos/core)
endif()

# record sources, headers, resources...
set(GAME_SOURCE)
set(GAME_HEADER)

set(GAME_RES_FOLDER
    "${CMAKE_CURRENT_SOURCE_DIR}/Resources"
    )
if(APPLE OR VS)
    cocos_mark_multi_resources(cc_common_res RES_TO "Resources" FOLDERS ${GAME_RES_FOLDER})
endif()

list(APPEND GAME_HEADER
     Classes/HelloImGuiScene.h
     Classes/AppDelegate.h
     )
list(APPEND GAME_SOURCE
     Classes/AppDelegate.cpp
     Classes/HelloImGuiScene.cpp
     )

if(ANDROID)
    # change APP_NAME to the share library name for Android, it's value depend on AndroidManifest.xml
    set(APP_NAME cpp_empty_test)
    list(APPEND GAME_SOURCE
         proj.android/app/jni/main.cpp
         )
elseif(LINUX)
    list(APPEND GAME_SOURCE
         proj.linux/main.cpp
         )
elseif(WINDOWS)
    list(APPEND GAME_HEADER
         proj.win32/main.h
         )
    list(APPEND GAME_SOURCE
         proj.win32/main.cpp
         ${cc_common_res}
         )
elseif(APPLE)
    if(IOS)
        list(APPEND GAME_HEADER
             proj.ios/AppController.h
             proj.ios/RootViewController.h
             )
        set(APP_UI_RES
            proj.ios/LaunchScreen.storyboard
            proj.ios/LaunchScreenBackground.png
            proj.ios/Images.xcassets
            )
        list(APPEND GAME_SOURCE
             proj.ios/main.m
             proj.ios/AppController.mm
             proj.ios/RootViewController.mm
             ${APP_UI_RES}
             )
    elseif(MACOSX)
        set(APP_UI_RES
            proj.mac/Icon.icns
            proj.mac/Info.plist
            proj.mac/en.lproj/InfoPlist.strings
            )
        list(APPEND GAME_SOURCE
             proj.mac/main.cpp
             ${APP_UI_RES}
             )
    endif()
    list(APPEND GAME_SOURCE ${cc_common_res})
elseif(EMSCRIPTEN)
    list(APPEND GAME_SOURCE
         proj.emscripten/main.cpp
         )
endif()

# fetch imgui-widgets
include(FetchContent)
FetchContent_Declare(
  imgui_widgets
  GIT_REPOSITORY https://github.com/wujiayish/cocos2d-x-imgui-widgets
  GIT_TAG        d5de356e1138f2bc50c1280a762f654d41d39c7a 
)
FetchContent_MakeAvailable(imgui_widgets)
include_directories(${imgui_widgets_SOURCE_DIR})
include(${imgui_widgets_SOURCE_DIR}/CMakeLists.txt)
set(GAME_HEADER ${GAME_HEADER} ${CCIMGUI_WIDGETS_HEADER})
set(GAME_SOURCE ${GAME_SOURCE} ${CCIMGUI_WIDGETS_SRC})

set(all_code_files
    ${GAME_HEADER}
    ${GAME_SOURCE}
    )

# mark app complie info
if(NOT ANDROID)
    add_executable(${APP_NAME} ${all_code_files})
else()
    add_library(${APP_NAME} SHARED ${all_code_files})
    add_subdirectory(${COCOS2DX_ROOT_PATH}/cocos/platform/android ${ENGINE_BINARY_PATH}/cocos/platform)
    target_link_libraries(${APP_NAME} -Wl,--whole-archive cpp_android_spec -Wl,--no-whole-archive)
endif()

target_link_libraries(${APP_NAME} cocos2d)

target_include_directories(${APP_NAME} PRIVATE Classes)

# fetch imgui
include(FetchContent)
FetchContent_Declare(
  imgui
  GIT_REPOSITORY https://github.com/wujiayish/cocos2d-x-imgui
  GIT_TAG        2b0321d42cead1783cdfefc36109802cd0243f4f 
)
FetchContent_MakeAvailable(imgui)
target_link_libraries(${APP_NAME} cc_imgui)
target_include_directories(${APP_NAME} PRIVATE ${imgui_SOURCE_DIR})

# mark app resources
setup_cocos_app_config(${APP_NAME})
if(APPLE)
    set_target_properties(${APP_NAME} PROPERTIES RESOURCE "${APP_UI_RES}")
    if(MACOSX)
        set_target_properties(${APP_NAME} PROPERTIES
                              MACOSX_BUNDLE_INFO_PLIST "${CMAKE_CURRENT_SOURCE_DIR}/proj.mac/Info.plist"
                              )
    elseif(IOS)
        cocos_pak_xcode(${APP_NAME} INFO_PLIST "iOSBundleInfo.plist.in")
        set_xcode_property(${APP_NAME} ASSETCATALOG_COMPILER_APPICON_NAME "AppIcon-${APP_NAME}")
        set_xcode_property(${APP_NAME} CODE_SIGN_IDENTITY "iPhone Developer")
        set_xcode_property(${APP_NAME} DEVELOPMENT_TEAM "")
        set_xcode_property(${APP_NAME} CODE_SIGN_IDENTITY "iPhone Developer")
    endif()
elseif(WINDOWS)
    cocos_copy_target_dll(${APP_NAME})
endif()

if(LINUX OR WINDOWS)
    set(APP_RES_DIR "$<TARGET_FILE_DIR:${APP_NAME}>/Resources")
    cocos_copy_target_res(${APP_NAME} COPY_TO ${APP_RES_DIR} FOLDERS ${GAME_RES_FOLDER})
endif()

if(EMSCRIPTEN)
    set(CMAKE_EXECUTABLE_SUFFIX ".html")
    target_link_options(${APP_NAME} PRIVATE
                        "-sEXPORTED_FUNCTIONS=[_cocos_ccdirector_pause,_cocos_ccdirector_resume,_cocos_ccdirector_step,_main]"
                        "-sEXPORTED_RUNTIME_METHODS=[ccall,cwrap]"
                        )
    set(EMSCRIPTEN_LINK_FLAGS "-lidbfs.js -s MIN_WEBGL_VERSION=2 -s MAX_WEBGL_VERSION=2 -s INITIAL_MEMORY=512MB --shell-file ${CMAKE_CURRENT_SOURCE_DIR}/index.html --use-preload-cache")
    # Disable wasm, generate js build
    # string(APPEND EMSCRIPTEN_LINK_FLAGS " -s WASM=0")

    # No resource in this example
    # foreach(FOLDER IN LISTS GAME_RES_FOLDER)
    #     string(APPEND EMSCRIPTEN_LINK_FLAGS " --preload-file ${FOLDER}/@/")
    # endforeach()
    
    set_target_properties(${APP_NAME} PROPERTIES LINK_FLAGS "${EMSCRIPTEN_LINK_FLAGS}")
endif()
