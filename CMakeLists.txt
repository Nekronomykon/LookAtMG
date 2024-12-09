cmake_minimum_required(VERSION 3.12)

project(LookAtMG VERSION 0.25 LANGUAGES CXX)

find_package(VTK 9 COMPONENTS
  CommonColor
  CommonCore
  DomainsChemistry
  FiltersSources
  IOChemistry
  IOGeometry
  IOImage
  ImagingCore
  ImagingSources
  InteractionStyle
  RenderingCore
  RenderingLabel
  RenderingFreeType
  RenderingGL2PSOpenGL2
  RenderingLOD
  RenderingOpenGL2
  GUISupportQt
  RenderingQt
  REQUIRED QUIET
  )

  include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR})
  set(CMAKE_INCLUDE_CURRENT_DIR ON)
  
  set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 #Qt5
REQUIRED COMPONENTS 
Core Gui Widgets #PrintSupport #OpenGL
)
#find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Gui Widgets PrintSupport OpenGL)

file(GLOB UI_FILES *.ui)
file(GLOB HDR_FILES *.h)
file(GLOB CXX_FILES *.cxx)
file(GLOB CPP_FILES *.cpp)
file(GLOB RC_FILES *.qrc)

qt_wrap_ui(UISources ${UI_FILES} )

set(PROJECT_SOURCES 
${CPP_FILES} ${HDR_FILES} ${UI_FILES} ${RC_FILES} 
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(LookAtMG
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET LookAtMG APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(LookAtMG SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(LookAtMG
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(LookAtMG PRIVATE 
Qt${QT_VERSION_MAJOR}::Core
Qt${QT_VERSION_MAJOR}::Gui
Qt${QT_VERSION_MAJOR}::Widgets
#Qt${QT_VERSION_MAJOR}::OpenGL
#Qt${QT_VERSION_MAJOR}::PrintSupport
${VTK_LIBRARIES}
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.LookAtMG)
endif()
set_target_properties(LookAtMG PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS LookAtMG
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(LookAtMG)
endif()

# vtk_module_autoinit is needed
vtk_module_autoinit(
  TARGETS LookAtMG
  MODULES ${VTK_LIBRARIES}
)