file(GLOB_RECURSE app_sources ${CMAKE_CURRENT_LIST_DIR}/*.c)

idf_component_register(
    SRCS ${app_sources}
    INCLUDE_DIRS "."
    REQUIRES esp_wifi esp_netif nvs_flash esp_event esp_timer driver
)