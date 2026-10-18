add_library(presentation STATIC
    views/table_view/table_view.cpp
    views/table_view/table_background.cpp
    views/table_view/hand_view.cpp
    views/table_view/betting_hud.cpp
    views/table_view/round_overlay.cpp
    assets/texture_manager.cpp
    components/text/text.cpp
)

target_include_directories(presentation PUBLIC ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(presentation PUBLIC domain SDL3::SDL3 SDL3_ttf::SDL3_ttf)