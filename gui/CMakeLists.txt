cmake_minimum_required (VERSION 2.8.9)

set(CMAKE_AUTOMOC ON)

SET (EXE_NAME medici-gui)
add_executable(${EXE_NAME}
    main.cpp
    main_form.cpp

    selectors/gui_card_selector.cpp
    selectors/gui_deck_selector.cpp

    calculator_window.cpp
    progress_widget.cpp

    cards_translations.cpp

    deck_preference.cpp
)

target_link_libraries(${EXE_NAME} Qt5::Widgets source-library )
