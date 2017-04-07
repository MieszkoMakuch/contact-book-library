//
// Created by Mieszko Makuch on 16.03.2017.
//

#ifndef ZAD1ZAD2_READ_DATA_H
#define ZAD1ZAD2_READ_DATA_H

#include "contact_list.h"
#include "contact_BST.h"

void add_contacts_from_csv(struct ContactList *contact_list,
                           struct ContactBST *contactBST,
                           char *file_name);

#endif //ZAD1ZAD2_READ_DATA_H
