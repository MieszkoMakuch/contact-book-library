//
// Created by Mieszko Makuch on 14.03.2017.
//

#include<stdio.h>
#include<stdlib.h>
#include "contact_BST.h"
#include "error.h"

struct ContactBST *create_contactBST(enum contact_sorting_attributes sorted_by) {
    struct ContactBST *contactBST = malloc(sizeof(struct ContactBST));
    contactBST->root = NULL;
    contactBST->sorted_by_key = sorted_by;

    comparator cmp = get_comparator_form_atribute(sorted_by);
    contactBST->cmp = cmp;
    return contactBST;
}

void delete_all_contactBSTNodes(struct ContactBSTNode **node) {
    if (*node == NULL) {
        return;
    }
    delete_all_contactBSTNodes(&(*node)->left);
    delete_all_contactBSTNodes(&(*node)->right);
    delete_contactBSTNode(node);
}

void delete_whole_contactBST(struct ContactBST **contactBST) {
    delete_all_contactBSTNodes(&(*contactBST)->root);
    free(*contactBST);
    *contactBST = NULL;
}

struct ContactBSTNode *contactBSTNode_insert(struct ContactBSTNode *node,
                                             struct ContactBSTNode *new_node,
                                             comparator cmp) {
    if (node == NULL) { return new_node; }

    if (cmp(*new_node->contact, *node->contact) > 0) {
        node->right = contactBSTNode_insert(node->right, new_node, cmp);
    } else if (cmp(*new_node->contact, *node->contact) < 0) {
        node->left = contactBSTNode_insert(node->left, new_node, cmp);
    }
    return node;
}

struct ContactBSTNode *rebuild_contactBSTNode(struct ContactBSTNode *node,
                                              struct ContactBSTNode *new_tree, comparator cmp){
    if(node == NULL) return new_tree;

    new_tree = rebuild_contactBSTNode(node->right, new_tree, cmp);
    new_tree = rebuild_contactBSTNode(node->left, new_tree, cmp);

    node->left = node->right = NULL;
    return contactBSTNode_insert(new_tree, node, cmp);
}

void rebuild_contactBST_with_new_key(struct ContactBST *contactBST, enum contact_sorting_attributes new_key) {
    if (contactBST->sorted_by_key == new_key) { return; }

    contactBST->cmp = get_comparator_form_atribute(new_key);
    contactBST->root = rebuild_contactBSTNode(contactBST->root, NULL, contactBST->cmp);
}

struct ContactBSTNode *create_contactBSTNode(struct Contact *contact) {
    struct ContactBSTNode *new_node = malloc(sizeof(struct ContactBSTNode));
    if (new_node == NULL) { cannot_allocate_memory_error(); }

    new_node->contact = contact;
    new_node->left = new_node->right = NULL;
    return new_node;
}

struct ContactBSTNode *contactBSTNode_find_min(struct ContactBSTNode *node) {
    if (node == NULL) {
        return NULL;
    }
    if (node->left)
        return contactBSTNode_find_min(node->left);
    else
        return node;
}

struct ContactBSTNode *contactBST_find_min(struct ContactBST *contactBST) {
    return contactBSTNode_find_min(contactBST->root);
}

struct ContactBSTNode *contactBSTNode_find_max(struct ContactBSTNode *node) {
    if (node == NULL)
        return NULL;
    if (node->right)
        return contactBSTNode_find_max(node->right);
    else
        return node;
}

struct ContactBSTNode *contactBST_find_max(struct ContactBST *contactBST) {
    return contactBSTNode_find_max(contactBST->root);
}

struct ContactBST *contactBST_insert(struct ContactBST *contactBST, struct ContactBSTNode *new_node) {
    contactBST->root = contactBSTNode_insert(contactBST->root, new_node, contactBST->cmp);
    return contactBST;
}

void delete_contactBSTNode(struct ContactBSTNode **contactBST_node) {
    if ((*contactBST_node) == NULL) return;

    delete_contact(&((*contactBST_node)->contact));
    free(*contactBST_node);
    *contactBST_node = NULL;
}

struct ContactBSTNode *contactBSTNode_delete_contact(struct ContactBSTNode *node,
                                                     struct Contact *contact_to_delete,
                                                     comparator cmp) {
    struct ContactBSTNode *temp;
    if (node == NULL) {

        printf("ERROR: Element Not Found\n");
    } else if (cmp(*contact_to_delete, *node->contact) < 0) {
        node->left = contactBSTNode_delete_contact(node->left, contact_to_delete, cmp);
    } else if (cmp(*contact_to_delete, *node->contact) > 0) {
        node->right = contactBSTNode_delete_contact(node->right, contact_to_delete, cmp);
    } else { //Contact found
        // Replace element with minimum element from tht right sub tree or with
        // maximum element from the left sub tree. Delete the element.
        if (node->right && node->left) {
            temp = contactBSTNode_find_min(node->right);
            node->contact = temp->contact;
            node->right = contactBSTNode_delete_contact(node->right, contact_to_delete, cmp);
        } else {
            // If there is only one or zero children directly remove element
            // from the tree and connect its parent to its child
            temp = node;
            if (node->left == NULL)
                node = node->right;
            else if (node->right == NULL)
                node = node->left;
            delete_contactBSTNode(&temp);
        }
    }

    return node;
}

struct ContactBST *contactBST_delete_contact(struct ContactBST *contactBST, struct Contact *contact_to_delete) {
    contactBST->root = contactBSTNode_delete_contact(contactBST->root, contact_to_delete, contactBST->cmp);
    return contactBST;
}

struct ContactBSTNode *contactBSTNode_find(struct ContactBSTNode *node,
                                           struct Contact contact,
                                           comparator cmp) {
    if (node == NULL) { return NULL; }

    if (cmp(contact, *node->contact) > 0) {
        return contactBSTNode_find(node->right, contact, cmp);
    } else if (cmp(contact, *node->contact) < 0) {
        return contactBSTNode_find(node->left, contact, cmp);
    }
    return node;
}

struct ContactBSTNode *contactBST_find(struct ContactBST *contactBST, struct Contact contact) {
    return contactBSTNode_find(contactBST->root, contact, contactBST->cmp);
}

struct ContactBSTNode *contactBST_find_by_attributes(struct ContactBST *contactBST,
                                                     char *name, char *surname,
                                                     struct Date birth_date,
                                                     char *email) {
    struct Contact *wanted_contact = create_mock_contact(name, surname, birth_date, email, "SomeNumber", "SomeAddress");
    struct ContactBSTNode *foundBSTNode = contactBSTNode_find(contactBST->root, *wanted_contact, contactBST->cmp);
    delete_contact(&wanted_contact);

    if (!foundBSTNode) { contact_does_not_exists_error(); }
    return foundBSTNode;
}

void contactBSTNode_print_inorder(struct ContactBSTNode *node) {
    if (node == NULL) {
        return;
    }
    contactBSTNode_print_inorder(node->left);
    print_contact(*node->contact);
    contactBSTNode_print_inorder(node->right);
}

void contactBST_print_inorder(struct ContactBST *contactBST) {
    printf("Printing ContactBST in order: \n");
    contactBSTNode_print_inorder(contactBST->root);
}

void contactBSTNode_print_preorder(struct ContactBSTNode *node) {
    if (node == NULL) {
        return;
    }
    print_contact(*node->contact);
    contactBSTNode_print_preorder(node->left);
    contactBSTNode_print_preorder(node->right);
}

void contactBST_print_preorder(struct ContactBST *contactBST) {
    printf("Printing ContactBST pre order: \n");
    contactBSTNode_print_preorder(contactBST->root);
}

void contactBSTNode_print_postorder(struct ContactBSTNode *node) {
    if (node == NULL) {
        return;
    }
    contactBSTNode_print_postorder(node->left);
    contactBSTNode_print_postorder(node->right);
    print_contact(*node->contact);
}

void contactBST_print_postorder(struct ContactBST *contactBST) {
    printf("Printing ContactBST post order: \n");
    contactBSTNode_print_postorder(contactBST->root);
}