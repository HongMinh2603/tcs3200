set(app_src i2c-lcd.c)
set(pre_req driver freertos)
idf_component_register(SRCS ${app_src}
                    INCLUDE_DIRS "."
                    REQUIRES ${pre_req})