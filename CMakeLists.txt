cmake_minimum_required(VERSION 4.0)
project(ezCam)

set(CMAKE_CXX_STANDARD 26)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_WIN32_EXECUTABLE TRUE)

set(CMAKE_PREFIX_PATH "C:\\Qt\\6.9.2\\mingw_64\\")


find_package(Qt6 COMPONENTS
        Core
        Gui
        Widgets
        Multimedia
        MultimediaWidgets
        REQUIRED)

add_executable(ezCam WIN32
        main.cpp
        MainWindow.h
        MainWindow.cpp)

target_link_libraries(ezCam
        Qt::Core
        Qt::Gui
        Qt::Widgets
        Qt6::Multimedia
        Qt6::MultimediaWidgets
)

if(WIN32)
    target_sources(ezCam PRIVATE ${CMAKE_SOURCE_DIR}/version.rc)
endif()


if (WIN32 AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    set(DEBUG_SUFFIX)
    if (MSVC AND CMAKE_BUILD_TYPE MATCHES "Debug")
        set(DEBUG_SUFFIX "d")
    endif ()
    set(QT_INSTALL_PATH "${CMAKE_PREFIX_PATH}")
    if (NOT EXISTS "${QT_INSTALL_PATH}/bin")
        set(QT_INSTALL_PATH "${QT_INSTALL_PATH}/..")
        if (NOT EXISTS "${QT_INSTALL_PATH}/bin")
            set(QT_INSTALL_PATH "${QT_INSTALL_PATH}/..")
        endif ()
    endif ()
    if (EXISTS "${QT_INSTALL_PATH}/plugins/platforms/qwindows${DEBUG_SUFFIX}.dll")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E make_directory
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/platforms/")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                "${QT_INSTALL_PATH}/plugins/platforms/qwindows${DEBUG_SUFFIX}.dll"
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/platforms/")
    endif ()
    foreach (QT_LIB Core Gui Widgets)
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                "${QT_INSTALL_PATH}/bin/Qt6${QT_LIB}${DEBUG_SUFFIX}.dll"
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>")
    endforeach (QT_LIB)
endif ()

if(WIN32)
    set(WINDEPLOYQT_EXECUTABLE "C:/Qt/6.9.2/mingw_64/bin/windeployqt.exe") # anpassen

    add_custom_command(TARGET ezCam POST_BUILD
            COMMAND "${WINDEPLOYQT_EXECUTABLE}" --release \"$<TARGET_FILE:ezCam>\"
            COMMENT "Running windeployqt on ezCam"
    )
endif()