//
// Created by Mieszko Makuch on 12.03.2017.
//

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "contact_list.h"
#include "error.h"
#include "read_data.h"

int node_id = 0;

struct ContactNode *create_contact_node(struct Contact *contact) {
    struct ContactNode *new_node = malloc(sizeof(struct ContactNode));
    if (new_node == NULL) { cannot_allocate_memory_error(); }

    new_node->contact = contact;
    new_node->id = node_id++;
    new_node->next = NULL;
    new_node->prev = NULL;
    return new_node;
}

void delete_contact_node(struct ContactNode **contact_node) {
    if ((*contact_node) == NULL) return;

    delete_contact(&((*contact_node)->contact));
    free(*contact_node);

    *contact_node = NULL;
}

struct ContactList *create_contact_list(char *name) {
    struct ContactList *new_list = malloc(sizeof(struct ContactList));
    if (new_list == NULL) { cannot_allocate_memory_error(); }

    new_list->first = new_list->last = NULL;
    new_list->name = name;
    return new_list;
}

// Allocates list name dynamically so that it can be deleted
// using delete_contact_list function without errors
struct ContactList *create_tmp_contact_list(char *name) {
    return create_contact_list(strdup(name));
}

void delete_contact_list(struct ContactList **contactList) {
    if (*contactList == NULL) return;
    free((*contactList)->name);

    struct ContactNode *tmp;
    tmp = (*contactList)->first;

    while (tmp != NULL) {
        struct ContactNode *to_delete = tmp;
        tmp = tmp->next;
        delete_contact_node(&to_delete);
    }

    free(*contactList);
    (*contactList) = NULL;
}

void add_contact(struct ContactList *contact_list, struct ContactNode *new_node) {
    if (contact_list->first == NULL) {
        contact_list->first = contact_list->last = new_node;
    } else {
        struct ContactNode *last = contact_list->last;
        last->next = new_node;
        new_node->prev = last;
        contact_list->last = new_node;
    }
}

struct ContactNode *find_contact_node(struct ContactList contact_list,
                                      char *name, char *surname, struct Date birth_date) {
    struct ContactNode *tmp_node;
    for (tmp_node = contact_list.last; tmp_node != NULL; tmp_node = tmp_node->prev) {
        if (contact_equals(*tmp_node->contact, name, surname, birth_date)) {
            return tmp_node;
        }
    }
    return NULL;
}

bool delete_contact_from_list(struct ContactList *contact_list,
                              char *name, char *surname, struct Date birth_date) {
    if (contact_list == NULL) return false;
    struct ContactNode *contact_node = find_contact_node(*contact_list, name, surname, birth_date);
    if (contact_node == NULL) return false;

    if (contact_node == contact_list->first && contact_node == contact_list->last) {
        // contact_node is first and last -> contact list has only one element
        contact_list->first = contact_list->last = NULL;
    } else if (contact_node == contact_list->first) {
        // contact_node is first, and isn't last -> there is at least one next contact
        contact_list->first = contact_node->next;
        contact_list->first->prev = NULL;
    } else if (contact_node == contact_list->last) {
        // contact_node is last, and isn't first -> there is at least one prev contact
        contact_list->last = contact_node->prev;
        contact_list->last->next = NULL;
    } else {
        // contact_node is somewhere in the middle
        contact_node->prev->next = contact_node->next;
        contact_node->next->prev = contact_node->prev;
    }

    delete_contact_node(&contact_node);
    return true;
}

void print_contact_node(struct ContactNode contact_node) {
    printf("Node ID: %03d, ", contact_node.id);
    print_contact(*contact_node.contact);
}

void print_contact_list(struct ContactList contact_list) {
    struct ContactNode *tmp_node;
    printf("Printing list named: %s\n", contact_list.name);
    for (tmp_node = contact_list.first; tmp_node != NULL; tmp_node = tmp_node->next) {
        print_contact_node(*tmp_node);
    }
}

struct ContactNode *findMiddle(struct ContactList *list) {
    struct ContactNode *slow = list->first;
    if (slow->next == NULL) {
        return slow;
    }

    struct ContactNode *fast = slow->next;
    while (fast != NULL && fast->next != NULL) {
        slow = slow->next;
        fast = fast->next->next;
    }

    return slow;
}

struct ContactList *merge(struct ContactList *cl1, struct ContactList *cl2, comparator cmp) {

    if (cl1->first == cl2->first && cl1->last == cl2->last) {
        free(cl2);
        return cl1;
    }

    struct ContactNode *contact_node1 = cl1->first;
    struct ContactNode *contact_node2 = cl2->first;
    struct ContactNode *n;

    if (cmp(*contact_node1->contact, *contact_node2->contact) < 0) {
        n = contact_node1;
        contact_node1 = contact_node1->next;
    } else {
        n = contact_node2;
        contact_node2 = contact_node2->next;
    }

    cl1->first = n;

    while (contact_node1 != NULL && contact_node2 != NULL) {
        if (cmp(*contact_node1->contact, *contact_node2->contact) < 0) {
            n->next = contact_node1;
            contact_node1->prev = n;
            n = contact_node1;
            contact_node1 = contact_node1->next;
        } else {
            n->next = contact_node2;
            contact_node2->prev = n;
            n = contact_node2;
            contact_node2 = contact_node2->next;
        }
    }

    if (contact_node1 != NULL) {
        n->next = contact_node1;
        contact_node1->prev = n;
    } else {
        n->next = contact_node2;
        contact_node2->prev = n;
    }

    while (n->next != NULL) {
        n = n->next;
    }

    cl1->last = n;
    free(cl2);

    return cl1;
}

struct ContactList *mergeSort(struct ContactList *cl, comparator cmp) {

    if (cl->first == NULL || cl->first == cl->last) {
        return cl;
    }

    struct ContactNode *middle = findMiddle(cl);
    struct ContactNode *afterMiddle = middle->next;
    if (middle->next != NULL) {
        middle->next->prev = NULL;
    }
    middle->next = NULL;

    struct ContactList *left = create_tmp_contact_list("tmp_left");
    struct ContactList *right = create_tmp_contact_list("tmp_right");

    left->first = cl->first;
    left->last = middle;
    right->first = afterMiddle;
    right->last = afterMiddle == NULL ? NULL : cl->last;
    left = mergeSort(left, cmp);
    right = mergeSort(right, cmp);

    return merge(left, right, cmp);
}

void sort_contact_list(struct ContactList *list, enum contact_sorting_attributes sort_by) {
    if (list == NULL || list->first == list->last) {
        return;
    }

    comparator cmp = get_comparator_form_atribute(sort_by);

    struct ContactList *tmp = mergeSort(list, cmp);
    list->first = tmp->first;
    list->last = tmp->last;
    free(tmp);
}

struct ContactList *create_contact_list_from_file(char *file_name) {
    char *list_name_heap = strdup("Sample");
    struct ContactList *contact_list = create_contact_list(list_name_heap);
    add_contacts_from_csv(contact_list, NULL, file_name);
    return contact_list;
}