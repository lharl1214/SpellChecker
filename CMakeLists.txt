cmake_minimum_required(VERSION 4.0)
project(SpellChecker C)

set(CMAKE_C_STANDARD 11)

add_executable(SpellChecker
        Distance.c
        Distance.h
        SpellChecker.c)
add_custom_command(TARGET SpellChecker POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_SOURCE_DIR}/aspell-english-common.txt
        $<TARGET_FILE_DIR:SpellChecker>/aspell-english-common.txt)
