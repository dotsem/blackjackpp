add_library(presentation STATIC
    views/table_view.cpp
    assets/texture_manager.cpp
)

target_include_directories(presentation PUBLIC ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(presentation PUBLIC domain SDL3::SDL3)