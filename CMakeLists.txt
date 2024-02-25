if(ESP_PLATFORM)
    set(srcs
        "st7735.c")

    set(includes 
        ".")

    idf_component_register(SRCS "${srcs}"
                           INCLUDE_DIRS ${includes}
                           REQUIRES mcu_port fonts)
endif()
