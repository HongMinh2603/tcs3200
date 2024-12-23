set(app_src tcs3200.c)
set(pre_req driver freertos )
idf_component_register(SRCS ${app_src}
                    INCLUDE_DIRS "."
                    REQUIRES ${pre_req})