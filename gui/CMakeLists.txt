cmake_minimum_required (VERSION 2.8.9)

set(CMAKE_AUTOMOC ON)

include_directories(".")

SET (EXE_NAME medici-gui)
add_executable(${EXE_NAME}
    main.cpp

    windows/main_form.cpp
    windows/calculator_window.cpp
    widgets/progress_widget.cpp
    widgets/deck_preference.cpp
    widgets/selectors/gui_card_selector.cpp
    widgets/selectors/gui_deck_selector.cpp

    helpers/cards_translations.cpp

)

target_link_libraries(${EXE_NAME} Qt5::Widgets source-library )
