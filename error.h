//
// Created by Mieszko Makuch on 12.03.2017.
//

#ifndef ZEWTAW1_ERROR_H
#define ZEWTAW1_ERROR_H

/**
 * Print error and exit(1), if the memory cannot be allocated
 */

void throw_error(const char message[]);

void cannot_allocate_memory_error();

void invalid_sort_by_attribute_error();

void contact_does_not_exists_error();

#endif //ZEWTAW1_ERROR_H\

