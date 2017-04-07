//
// Created by Mieszko Makuch on 16.03.2017.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "read_data.h"
#include "error.h"

ssize_t get_line_without_new_line(char **lineptr, size_t *len, FILE *stream) {

    ssize_t chars = getline(lineptr, len, stream);

    if ((*lineptr)[chars - 1] == '\n') {
        (*lineptr)[chars - 2] = 0;
        --chars;
    }

    return chars;
}

void add_contacts_from_csv(struct ContactList *contact_list,
                           struct ContactBST *contactBST,
                           char *file_name) {
    if (!contact_list && !contactBST) { printf("ERROR: At least one structure cannot be null\n"); }

    // Open the file (to read - r)
    FILE *handler = fopen(file_name, "r");
    if (handler == NULL) {
        char file_error_msg[100];
        sprintf(file_error_msg, "ERROR: Cannot open the file: %s", file_name);
        throw_error(file_error_msg);
        return;
    }

    // Load contacts from csv
    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    int count = 0;
    while ((read = get_line_without_new_line(&line, &len, handler)) != -1) {
        //name
        char *name = malloc(read * sizeof(char));
        strcpy(name, line);

        //surname
        read = get_line_without_new_line(&line, &len, handler);
        char *surname = malloc(read * sizeof(char));
        strcpy(surname, line);

        //day
        read = get_line_without_new_line(&line, &len, handler);
        char *day = malloc(read * sizeof(char));
        strcpy(day, line);

        //month
        read = get_line_without_new_line(&line, &len, handler);
        char *month = malloc(read * sizeof(char));
        strcpy(month, line);

        //year
        read = get_line_without_new_line(&line, &len, handler);
        char *year = malloc(read * sizeof(char));
        strcpy(year, line);

        //email
        read = get_line_without_new_line(&line, &len, handler);
        char *email = malloc(read * sizeof(char));
        strcpy(email, line);

        //phone
        read = get_line_without_new_line(&line, &len, handler);
        char *phone = malloc(read * sizeof(char));
        strcpy(phone, line);

        //address
        read = get_line_without_new_line(&line, &len, handler);
        char *address = malloc(read * sizeof(char));
        strcpy(address, line);

        // Create birth_date
        struct Date birth_date = create_date(atoi(day), atoi(month), atoi(year));
        // Create contact
        struct Contact *contact = create_contact(name, surname, birth_date, email, phone, address);
        // Create node
        if (contact_list) {
            struct ContactNode *contact_node = create_contact_node(contact);
            // Add contact node
            add_contact(contact_list, contact_node);
        }
        if (contactBST) {
            struct ContactBSTNode *new_contactBSTNode = create_contactBSTNode(contact);
            // Add contact node
            contactBST = contactBST_insert(contactBST, new_contactBSTNode);
        }
        count++;

    }
    free(line);

    // Close the file
    if (fclose(handler) == EOF) {
        printf("ERROR: Cannot close the file: %s\n", file_name);
        return;
    }
}