cmake_minimum_required(VERSION 3.5)

project(
  IntelliNote
  VERSION 0.1
  LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(BUILD_TEST ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(
  QT NAMES Qt6
  COMPONENTS Widgets WebChannel WebEngineWidgets Network LinguistTools
  REQUIRED)
find_package(Qt6 REQUIRED COMPONENTS Widgets WebChannel WebEngineWidgets
                                     Network LinguistTools)

set(TS_FILES IntelliNote_zh_CN.ts)

qt_add_resources(QRC_FILES "res.qrc")
source_group("Resource Files" FILES "res.qrc")

add_subdirectory(qmarkdowntextedit)

set(PROJECT_SOURCES
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    GPTSession.h
    GPTSession.cpp
    NoteManager.h
    NoteManager.cpp
    Note.h
    Note.cpp
    TransSession.h
    TransSession.cpp
    MDViewer.h
    MDViewer.cpp
#    NoteTreeModel.h
#    NoteTreeModel.cpp
    Dialogs/PromptGenerateDialog.h
    Dialogs/PromptGenerateDialog.cpp
    Dialogs/PromptGenerateDialog.ui
    ${QRC_FILES}
    ${TS_FILES})

if(BUILD_TEST)
  enable_testing()

  qt_add_executable(TEST_GPTSession MANUAL_FINALIZATION GPTSession.cpp
                    test/GPTSessiontest.cpp)
  target_link_libraries(
    TEST_GPTSession
    PRIVATE Qt${QT_VERSION_MAJOR}::Widgets
            Qt${QT_VERSION_MAJOR}::WebEngineWidgets
            Qt${QT_VERSION_MAJOR}::WebChannel
            Qt${QT_VERSION_MAJOR}::Network
            qmarkdowntextedit)

  qt_add_executable(TEST_TransSession MANUAL_FINALIZATION TransSession.cpp
          test/TransSessiontest.cpp)
  target_link_libraries(
          TEST_TransSession
          PRIVATE Qt${QT_VERSION_MAJOR}::Widgets
          Qt${QT_VERSION_MAJOR}::WebEngineWidgets
          Qt${QT_VERSION_MAJOR}::WebChannel
          Qt${QT_VERSION_MAJOR}::Network
          qmarkdowntextedit)

  add_test(NAME GPTSession_Test COMMAND $<TARGET_FILE:TEST_GPTSession>)
  add_test(NAME TransSession_Test COMMAND $<TARGET_FILE:TEST_TransSession>)
endif()

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
  qt_add_executable(IntelliNote MANUAL_FINALIZATION ${PROJECT_SOURCES})
  # Define target properties for Android with Qt 6 as: set_property(TARGET
  # IntelliNote APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
  # ${CMAKE_CURRENT_SOURCE_DIR}/android) For more information, see
  # https://doc.qt.io/qt-6/qt-add-executable.html#target-creation

  qt_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
else()
  if(ANDROID)
    add_library(IntelliNote SHARED ${PROJECT_SOURCES})
    # Define properties for Android with Qt 5 after find_package() calls as:
    # set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
  else()
    add_executable(IntelliNote ${PROJECT_SOURCES})
  endif()

  qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
endif()

target_link_libraries(
  IntelliNote
  PRIVATE Qt${QT_VERSION_MAJOR}::Widgets
          Qt${QT_VERSION_MAJOR}::WebEngineWidgets
          Qt${QT_VERSION_MAJOR}::WebChannel
          Qt${QT_VERSION_MAJOR}::Network
          qmarkdowntextedit)

set_target_properties(
  IntelliNote
  PROPERTIES MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
             MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
             MACOSX_BUNDLE_SHORT_VERSION_STRING
             ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
             MACOSX_BUNDLE TRUE
             WIN32_EXECUTABLE TRUE)

if(QT_VERSION_MAJOR EQUAL 6)
  qt_finalize_executable(IntelliNote)
endif()
