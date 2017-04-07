//
// Created by Mieszko Makuch on 16.03.2017.
//

/**
 * @file read_data.h
 *
 * @brief Reads data form generated file
 *
 * To read contacts form file and automatically add them to chosen contact book
 * use @ref add_contacts_from_csv
 */

#ifndef READ_DATA_H
#define READ_DATA_H

#include "contact_list.h"
#include "contact_BST.h"

void add_contacts_from_csv(struct ContactList *contact_list,
                           struct ContactBST *contactBST,
                           char *file_name);

#endif //READ_DATA_H
