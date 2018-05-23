cmake_minimum_required(VERSION 2.8)

project(WinApi)

add_definitions(-DUNICODE -D_UNICODE)

add_library(
    winApiUtils
    STATIC
    Source/Dialog.cpp
    Source/DialogMsgMatchers.cpp
    Source/MessageDialog.cpp
    Source/ListBoxDialog.cpp
    Source/GridDialog.cpp
    Source/TreeDialog.cpp
    Source/QueryDialog.cpp
    Source/SelectPathsDialog.cpp
    Source/SelectDirPathDialog.cpp
    Source/SelectFilePathDialog.cpp
    Source/Control.cpp
    Source/ControlGrid.cpp
    Source/ControlEdit.cpp
    Source/ControlCheckBox.cpp
    Source/ControlTree.cpp
    Source/ControlText.cpp
    Source/WinApiDirPathGetter.cpp
    Source/WinApiFilePathGetter.cpp
    Source/WinApiLastErrorException.cpp
    Source/Clipboard.cpp
)

target_include_directories(
    winApiUtils
    PUBLIC
    Include
    D:/boost_1_61_0
)

add_executable(
    winApiTests
    Source/WinApiTS.cpp
    Resource/QueryDialog.rc
    Resource/ListBoxDialog.rc
    Resource/GridDialog.rc
    Resource/TreeDialog.rc
    Resource/SelectPathsDialog.rc
)

target_include_directories(
    winApiTests
    PUBLIC
    Include
    D:/boost_1_61_0
)

add_dependencies(
    winApiTests
    winApiUtils
)

target_link_libraries(
    winApiTests
    winApiUtils
    comctl32
)

if( MSVC ) # VS2012 doesn't support correctly the tuples yet
    add_definitions( /D _VARIADIC_MAX=10 )
endif()
