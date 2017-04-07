//
// Created by Mieszko Makuch on 12.03.2017.
//

/**
 * @file contact.h
 *
 * @brief Single contact node
 *
 * A contact contains information about name, surname, birth date, email, phone and address.
 *
 * To create a contact, use @ref create_date and @ref create_contact. To destroy a
 * contact, use @ref delete_contact.
 */

#ifndef CONTACT_H
#define CONTACT_H

#include <stdbool.h>

struct Date {
    int day;
    int month;
    int year;
};

struct Contact {
    int id;
    char *name;
    char *surname;
    struct Date birth_date;
    char *email;
    char *phone;
    char *address;
};

typedef int (*comparator)(struct Contact, struct Contact);

enum contact_sorting_attributes {
    NAME, SURNAME, BIRTHDATE, EMAIL, PHONE
};

struct Contact *create_contact(char *name,
                               char *surname,
                               struct Date birth_date,
                               char *email,
                               char *phone,
                               char *address);

char *string_from_stack_to_heap(char *stack_sting);

struct Contact *create_mock_contact(char *name,
                                    char *surname,
                                    struct Date birth_date,
                                    char *email,
                                    char *phone,
                                    char *address);

void delete_contact(struct Contact **contact);

struct Date create_date(int day, int month, int year);

void print_contact(struct Contact contact);

bool contact_equals(struct Contact contact,
                    char *name, char *surname, struct Date birth_date);

int compare_contacts_by_names(struct Contact contact1, struct Contact contact2);

int compare_contacts_by_surnames(struct Contact contact1, struct Contact contact2);

int compare_contacts_by_birth_dates(struct Contact contact1, struct Contact contact2);

int compare_contacts_by_emails(struct Contact contact1, struct Contact contact2);

int compare_contacts_by_phone(struct Contact contact1, struct Contact contact2);

comparator get_comparator_form_atribute(enum contact_sorting_attributes attribute);

#endif //CONTACT_H