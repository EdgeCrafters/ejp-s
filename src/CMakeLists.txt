cmake_minimum_required(VERSION 3.11)

include_directories(../lib)

add_executable(ejs-s main.c repo.c workbook.c problem.c testcase.c)

target_link_libraries( ejs-s PRIVATE parse sha256 diff http info cJSON common test)
target_compile_options( ejs-s PUBLIC -w)
