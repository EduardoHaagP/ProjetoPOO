cmake_minimum_required(VERSION 3.5)

project(ProjetoPOO VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
        veiculos.h
        veiculos.cpp
        clientes.h
        clientes.cpp
        gerenciadores.h
        gerenciadores.cpp
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(ProjetoPOO
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        telalogin.h telalogin.cpp telalogin.ui
        telaprincipal.h telaprincipal.cpp telaprincipal.ui
        telacadastrocliente.h telacadastrocliente.cpp telacadastrocliente.ui
        telalistagemvendas.h telalistagemvendas.cpp telalistagemvendas.ui
        telaconsultaestoque.h telaconsultaestoque.cpp telaconsultaestoque.ui
        telaresgistrovendas.h telaresgistrovendas.cpp telaresgistrovendas.ui
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET ProjetoPOO APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(ProjetoPOO SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(ProjetoPOO
            ${PROJECT_SOURCES}
        )
    endif()
endif()

# Adiciona uma definição ao compilador para que o código C++ saiba o caminho do projeto
target_compile_definitions(ProjetoPOO PRIVATE "PROJECT_SOURCE_DIR=\"${CMAKE_SOURCE_DIR}\"")

target_link_libraries(ProjetoPOO PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

set_target_properties(ProjetoPOO PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

install(TARGETS ProjetoPOO
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(ProjetoPOO)
endif()
