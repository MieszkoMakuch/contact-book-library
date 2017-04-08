cmake_minimum_required(VERSION 3.7)
project(contact-book-library)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        contact.c
        contact.h
        contact_book_library.h
        contact_BST.c
        contact_BST.h
        contact_list.c
        contact_list.h
        error.c
        error.h
        read_data.c
        read_data.h testFiles/tests.c)

add_library(contact-book-library ${SOURCE_FILES})