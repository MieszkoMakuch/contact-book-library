//
// Created by Mieszko Makuch on 11.03.2017.
//

#include "contact.h"

#ifndef ZEWTAW1_CLIST_H
#define ZEWTAW1_CLIST_H

struct ContactNode {
    struct Contact *contact;
    int id;
    struct ContactNode *next;
    struct ContactNode *prev;
};

struct ContactList {
    char *name;
    struct ContactNode *first;
    struct ContactNode *last;
};

struct ContactNode* create_contact_node(struct Contact *contact);

void delete_contact_node(struct ContactNode **contact_node);

struct ContactList* create_contact_list (char *name);

void delete_contact_list (struct ContactList **contactList);

void add_contact(struct ContactList *contact_list, struct ContactNode *new_node);

struct ContactNode *find_contact_node(struct ContactList contact_list,
                                      char *name, char *surname, struct Date birth_date);

bool delete_contact_from_list (struct ContactList *contact_list,
                               char *name, char *surname, struct Date birth_date);

void print_contact_node(struct ContactNode contact_node);

void print_contact_list(struct ContactList contact_list);

void sort_contact_list(struct ContactList *list, enum contact_sorting_attributes sort_by);

struct ContactList *create_contact_list_from_file(char *file_name);

#endif //ZEWTAW1_CLIST_H