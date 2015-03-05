cmake_minimum_required (VERSION 2.8.9)

set(CMAKE_AUTOMOC ON)
find_package(Qt5Widgets REQUIRED)

SET (EXE_NAME medici-gui)
add_executable(${EXE_NAME}
        main.cpp
        main_form.cpp

	card_selector.cpp
	deck_selector.cpp

	calculation_thread.cpp
	calculation_controller.cpp
)

target_link_libraries(${EXE_NAME} Qt5::Widgets source-library )
