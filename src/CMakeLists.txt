add_executable(ejp-s main.c repo.c repo.service.c problem.c problem.service.c)
target_link_libraries( ejp-s parse sha256 http info cJSON common test)
target_include_directories(ejp-s PRIVATE ${CMAKE_SOURCE_DIR}/include)