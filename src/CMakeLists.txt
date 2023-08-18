add_executable(ejs-s main.c repo.c problem.c testcase.c)
target_link_libraries( ejs-s parse sha256 http info cJSON common test)
target_include_directories(ejs-s PRIVATE ${CMAKE_SOURCE_DIR}/include)