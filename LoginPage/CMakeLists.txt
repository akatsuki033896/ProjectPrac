cmake_minimum_required(VERSION 3.19)
project(LoginPage LANGUAGES CXX)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets Sql)

qt_standard_project_setup()

qt_add_executable(LoginPage
    WIN32 MACOSX_BUNDLE
    main.cpp
    mainwindow.cpp
    mainwindow.h
)

qt_add_resources(LoginPage "app_resources"
    PREFIX "/"
    FILES
        style.qss
)

target_link_libraries(LoginPage
    PRIVATE
        Qt::Core
        Qt::Widgets
        Qt::Sql

)

install(TARGETS LoginPage
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET LoginPage
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
