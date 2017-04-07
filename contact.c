//
// Created by Mieszko Makuch on 12.03.2017.
//

#include <stdlib.h>
#include <stdio.h>
#include <memory.h>
#include "string.h"
#include "contact.h"
#include "error.h"

int CONTACT_ID = 0;

struct Contact* create_contact(char *name,
                               char *surname,
                               struct Date birth_date,
                               char *email,
                               char *phone,
                               char *address) {
    struct Contact *new_contact = malloc(sizeof(struct Contact));
    if (new_contact == NULL) {cannot_allocate_memory_error();}

    new_contact->id = CONTACT_ID++;
    new_contact->name = name;
    new_contact->surname = surname;
    new_contact->birth_date = birth_date;
    new_contact->email = email;
    new_contact->phone = phone;
    new_contact->address = address;

    return new_contact;
}

// Allocates contact attributes dynamically so that it can be deleted
// using delete_contact function without errors
struct Contact* create_mock_contact(char *name,
                                    char *surname,
                                    struct Date birth_date,
                                    char *email,
                                    char *phone,
                                    char *address) {
    return create_contact(strdup(name),
                          strdup(surname),
                          birth_date,
                          strdup(email),
                          strdup(phone),
                          strdup(address));
}

void delete_contact (struct Contact **contact){
    if((*contact) == NULL) return;

    free((*contact)->name);
    free((*contact)->surname);
    free((*contact)->email);
    free((*contact)->phone);
    free((*contact)->address);
    free((*contact));

    (*contact) = NULL;
}

struct Date create_date (int day, int month, int year){
    struct Date date;
    date.day = day;
    date.month = month;
    date.year = year;
    return date;
}

bool date_equals(struct Date date1, struct Date date2) {
    return  date1.day == date2.day &&
            date1.month == date2.month &&
            date1.year == date2.year;
}

int compare_dates(struct Date date1, struct Date date2) {
    int year_cmp = date1.year - date2.year;
    if(year_cmp){return year_cmp;}

    int month_cmp = date1.month - date2.month;
    if(month_cmp){return month_cmp;}

    return date1.day - date2.day;
}

void print_contact (struct Contact contact) {
    printf("Contact ID: %03d, Name: %s, surname: %s, birth date: %i.%i.%i, email: %s, phone: %s, address: %s\n",
           contact.id,
           contact.name, contact.surname,
           contact.birth_date.day,
           contact.birth_date.month,
           contact.birth_date.year,
           contact.email, contact.phone, contact.address);
}

bool contact_equals(struct Contact contact,
                    char *name, char *surname, struct Date birth_date){
    return  strcmp(contact.name, name) == 0 &&
            strcmp(contact.surname, surname) == 0 &&
            date_equals(contact.birth_date, birth_date);
}

int compare_contacts_by_names(struct Contact contact1, struct Contact contact2) {
    int name_cmp = strcmp(contact1.name, contact2.name);
    if(name_cmp){ return name_cmp;}

    int surname_cmp = strcmp(contact1.surname, contact2.surname);
    if(surname_cmp){ return surname_cmp;}

    int birth_date_cmp = compare_dates(contact1.birth_date, contact2.birth_date);
    if(birth_date_cmp){ return birth_date_cmp;}

    // If booth contacts has the same name, surname, birth_date and email they are
    // considered to be the same contacts
    return strcmp(contact1.email, contact2.email);
}

int compare_contacts_by_surnames(struct Contact contact1, struct Contact contact2) {
    int surname_cmp = strcmp(contact1.surname, contact2.surname);
    if(surname_cmp){ return surname_cmp;}

    int name_cmp = strcmp(contact1.name, contact2.name);
    if(name_cmp){ return name_cmp;}

    int birth_date_cmp = compare_dates(contact1.birth_date, contact2.birth_date);
    if(birth_date_cmp){ return birth_date_cmp;}

    // If booth contacts has the same name, surname, birth_date and email they are
    // considered to be the same contacts
    return strcmp(contact1.email, contact2.email);
}

int compare_contacts_by_birth_dates(struct Contact contact1, struct Contact contact2) {
    int birth_date_cmp = compare_dates(contact1.birth_date, contact2.birth_date);
    if(birth_date_cmp){ return birth_date_cmp;}

    int name_cmp = strcmp(contact1.name, contact2.name);
    if(name_cmp){ return name_cmp;}

    int surname_cmp = strcmp(contact1.surname, contact2.surname);
    if(surname_cmp){ return surname_cmp;}

    // If booth contacts has the same name, surname, birth_date and email they are
    // considered to be the same contacts
    return strcmp(contact1.email, contact2.email);
}

int compare_contacts_by_emails(struct Contact contact1, struct Contact contact2) {
    int email_cmp = strcmp(contact1.email, contact2.email);
    if(email_cmp){ return email_cmp;}

    int name_cmp = strcmp(contact1.name, contact2.name);
    if(name_cmp){ return name_cmp;}

    int surname_cmp = strcmp(contact1.surname, contact2.surname);
    if(surname_cmp){ return surname_cmp;}

    // If booth contacts has the same name, surname, birth_date and email they are
    // considered to be the same contacts
    return compare_dates(contact1.birth_date, contact2.birth_date);
}

int compare_contacts_by_phone(struct Contact contact1, struct Contact contact2) {
    int phone_cmp = strcmp(contact1.phone, contact2.phone);
    if(phone_cmp){ return phone_cmp;}

    int birth_date_cmp = compare_dates(contact1.birth_date, contact2.birth_date);
    if(birth_date_cmp){ return birth_date_cmp;}

    int name_cmp = strcmp(contact1.name, contact2.name);
    if(name_cmp){ return name_cmp;}

    int surname_cmp = strcmp(contact1.surname, contact2.surname);
    if(surname_cmp){ return surname_cmp;}

    // If booth contacts has the same name, surname, birth_date and email they are
    // considered to be the same contacts
    return strcmp(contact1.email, contact2.email);
}


comparator get_comparator_form_atribute(enum contact_sorting_attributes attribute){
    comparator cmp;
    switch(attribute){
        case NAME :
            cmp = compare_contacts_by_names;
            break;
        case SURNAME :
            cmp = compare_contacts_by_surnames;
            break;
        case BIRTHDATE :
            cmp = compare_contacts_by_birth_dates;
            break;
        case EMAIL :
            cmp = compare_contacts_by_emails;
            break;
        case PHONE :
            cmp = compare_contacts_by_phone;
            break;
        default:
            invalid_sort_by_attribute_error();
    }
    return cmp;
}
