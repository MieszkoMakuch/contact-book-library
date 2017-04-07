//
// Created by Mieszko Makuch on 12.03.2017.
//

#include <stdlib.h>
#include <errno.h>
#include <memory.h>
#include <stdio.h>
#include "error.h"

void throw_error(const char message[]) {
    perror(message);
    exit(EXIT_FAILURE);
}

void cannot_allocate_memory_error() {
    throw_error("Cannot allocate memory");
}

void invalid_sort_by_attribute_error(){
    throw_error("Invalid sort_by attribute. Contacts cannot be sorted.");
}

void contact_does_not_exists_error(){
    throw_error("Cannot find contact with matching attributes.");
}