add_library(presentation STATIC
    views/table_view.cpp
)

target_include_directories(presentation PUBLIC ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(presentation PUBLIC domain SDL3::SDL3)