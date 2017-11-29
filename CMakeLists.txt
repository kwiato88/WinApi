cmake_minimum_required(VERSION 2.8)

project(WinApi)

add_definitions(-DUNICODE -D_UNICODE)

add_library(
    winApiUtils
    STATIC
    Source/Dialog.cpp
    Source/DialogMsgMatchers.cpp
    Source/ListBoxDialog.cpp
    Source/GridDialog.cpp
    Source/TreeDialog.cpp
    Source/SelectPathsDialog.cpp
    Source/SelectDirPathDialog.cpp
    Source/SelectFilePathDialog.cpp
    Source/Control.cpp
    Source/ControlGrid.cpp
    Source/ControlEdit.cpp
    Source/ControlCheckBox.cpp
    Source/ControlTree.cpp
    Source/WinApiDirPathGetter.cpp
    Source/WinApiFilePathGetter.cpp
    Source/WinApiLastErrorException.cpp
)

target_include_directories(
    winApiUtils
    PUBLIC
    Include
    D:/boost_1_61_0
)

if( MSVC ) # VS2012 doesn't support correctly the tuples yet
    add_definitions( /D _VARIADIC_MAX=10 )
endif()
