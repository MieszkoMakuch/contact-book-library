//
// Created by Mieszko Makuch on 15.03.2017.
//

/**
 * @file contact_BST.h
 *
 * @brief Contact book implemented using binary search tree.
 *
 * A contact contains information about contacts name, surname, birth date, email, phone and address.
 * It may be rebuild/sorted by one of the following keys: NAME, SURNAME, BIRTHDATE, EMAIL, PHONE
 *
 * To create a contact book, use @ref create_contactBST. To destroy a
 * contact bok, use @ref delete_whole_contactBST.
 */

#ifndef CONTACT_BST_H
#define CONTACT_BST_H

#include "contact.h"

struct ContactBSTNode {
    struct Contact *contact;
    struct ContactBSTNode *left;
    struct ContactBSTNode *right;
};

struct ContactBST {
    struct ContactBSTNode *root;
    enum contact_sorting_attributes sorted_by_key;
    comparator cmp;
};

struct ContactBST *create_contactBST(enum contact_sorting_attributes sorted_by);

void delete_whole_contactBST(struct ContactBST **contactBST);

void rebuild_contactBST_with_new_key(struct ContactBST *contactBST, enum contact_sorting_attributes new_key);

struct ContactBSTNode *create_contactBSTNode(struct Contact *contact);

struct ContactBSTNode *contactBST_find_min(struct ContactBST *contactBST);

struct ContactBSTNode *contactBST_find_max(struct ContactBST *contactBST);

struct ContactBST *contactBST_insert(struct ContactBST *contactBST, struct ContactBSTNode *new_node);

void delete_contactBSTNode(struct ContactBSTNode **contactBST_node);

struct ContactBST *contactBST_delete_contact(struct ContactBST *contactBST, struct Contact *contact_to_delete);

struct ContactBSTNode *contactBST_find_by_attributes(struct ContactBST *contactBST,
                                                     char *name, char *surname,
                                                     struct Date birth_date,
                                                     char *email);

void contactBST_print_inorder(struct ContactBST *contactBST);

void contactBST_print_preorder(struct ContactBST *contactBST);

void contactBST_print_postorder(struct ContactBST *contactBST);

#endif //CONTACT_BST_H
