cmake_minimum_required(VERSION 3.7...3.18)

if(${CMAKE_VERSION} VERSION_LESS 3.12)
	cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
endif()

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

find_package(Qt5 REQUIRED COMPONENTS Core Widgets Gui)
project(wimblewizz VERSION 1.0.0
		DESCRIPTION "Project wizard for my niche specific c++ workflow."
		LANGUAGES CXX)

set(uiFiles
	ProjectWizardWindow.ui)

set(headerFiles
	ProjectWizardWindow.h)

set(cppFiles
	ProjectWizardWindow.cpp
	Main.cpp)

qt5_wrap_ui(wrappedUI ${uiFiles})
qt5_wrap_cpp(wrappedHeaders ${headerFiles})

add_executable(${PROJECT_NAME} WIN32
	${uiFiles}
	${headerFiles}
	${cppFiles}
	${wrappedUi}
	${wrappedHeaders})

target_link_libraries(${PROJECT_NAME}
	PUBLIC
	Qt5::Core
	Qt5::Widgets
	Qt5::Gui)