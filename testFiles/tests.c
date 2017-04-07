//
// Created by Mieszko Makuch on 16.03.2017.
//

/**
 * @file tests.c
 *
 * @brief Test correctness and time complexity of the following contact book implementations:
 *  -binary search tree
 *  -linked list
 */

#include <stdio.h>
#include <sys/times.h>
#include <time.h>
#include <stdlib.h>
#include "../contact_book_library.h"
#include "../error.h"

void print_time_difference_on_screen_and_to_file(FILE *f, clock_t prev, struct tms *t0, clock_t curr, struct tms *t1) {
    printf("real time: %lf\nuser time: %lf\nsystem time: %lf\n\n", (double) (curr - prev) / CLOCKS_PER_SEC,
           (double) (t1->tms_utime - t0->tms_utime) / CLOCKS_PER_SEC,
           (double) (t1->tms_stime - t0->tms_stime) / CLOCKS_PER_SEC);
    fprintf(f, "real time: %lf\nuser time: %lf\nsystem time: %lf\n\n", (double) (curr - prev) / CLOCKS_PER_SEC,
            (double) (t1->tms_utime - t0->tms_utime) / CLOCKS_PER_SEC,
            (double) (t1->tms_stime - t0->tms_stime) / CLOCKS_PER_SEC);
}

void print_time_diff_start(FILE *f, clock_t prev, struct tms *t0, clock_t curr, struct tms *t1) {
    printf("\nTime since beginning:\n");
    print_time_difference_on_screen_and_to_file(f, prev, t0, curr, t1);
    fprintf(f, "\nTime since beginning:\n");
    print_time_difference_on_screen_and_to_file(f, prev, t0, curr, t1);
}


void print_time_diff_prev(FILE *f, clock_t prev, struct tms *t0, clock_t curr, struct tms *t1) {
    printf("Time since last checkpoint:\n");
    print_time_difference_on_screen_and_to_file(f, prev, t0, curr, t1);
    fprintf(f, "Time since last checkpoint:\n");
    print_time_difference_on_screen_and_to_file(f, prev, t0, curr, t1);
}

int main() {

#ifdef DLL
    void *handle = dlopen("./contactBookLibrary/libcontactBookLibrary.so", RTLD_LAZY);

    struct ContactList* (*create_contact_list)(char *) = dlsym(handle, "create_contact_list");
    struct ContactList* (*create_contact_list_from_file)(char *) = dlsym(handle, "create_contact_list_from_file");
    struct ContactNode* (*find_contact_node)(struct ContactList, char *, char *, struct Date) = dlsym(handle, "find_contact_node");
    struct Date (*create_date)(int day, int month, int year) = dlsym(handle, "create_date");
    void (*print_contact_node)(struct ContactNode) = dlsym(handle, "print_contact_node");
    bool (*delete_contact_from_list) (struct ContactList *,char *, char *, struct Date ) = dlsym(handle, "delete_contact_from_list");
    struct Contact* (*create_mock_contact)(char *,char *,struct Date,char *,char *,char *) = dlsym(handle, "create_mock_contact");
    struct ContactNode* (*create_contact_node)(struct Contact *) = dlsym(handle, "create_contact_node");
    void (*add_contact)(struct ContactList *, struct ContactNode *) = dlsym(handle, "add_contact");
    void (*sort_contact_list)(struct ContactList *, enum contact_sorting_attributes ) = dlsym(handle, "sort_contact_list");
    void (*print_contact_list)(struct ContactList contact_list) = dlsym(handle, "print_contact_list");
    void (*delete_contact_list) (struct ContactList **) = dlsym(handle, "delete_contact_list");
    int (*compare_contacts_by_names)(struct Contact , struct Contact ) = dlsym(handle, "compare_contacts_by_names");
    int (*compare_contacts_by_surnames)(struct Contact , struct Contact ) = dlsym(handle, "compare_contacts_by_surnames");
    int (*compare_contacts_by_birth_dates)(struct Contact , struct Contact ) = dlsym(handle, "compare_contacts_by_birth_dates");
    int (*compare_contacts_by_emails)(struct Contact , struct Contact ) = dlsym(handle, "compare_contacts_by_emails");
    struct ContactBST *(*create_contactBST)(enum contact_sorting_attributes sorted_by) = dlsym(handle, "create_contactBST");
    void (*delete_whole_contactBST)(struct ContactBST **contactBST) = dlsym(handle, "delete_whole_contactBST");
    void (*rebuild_contactBST_with_new_key)(struct ContactBST *contactBST, enum contact_sorting_attributes new_key) = dlsym(handle, "rebuild_contactBST_with_new_key");
    struct ContactBSTNode *(*create_contactBSTNode)(struct Contact *contact) = dlsym(handle, "create_contactBSTNode");
    struct ContactBSTNode *(*contactBST_find_min)(struct ContactBST *contactBST) = dlsym(handle, "contactBST_find_min");
    struct ContactBSTNode *(*contactBST_find_max)(struct ContactBST *contactBST) = dlsym(handle, "contactBST_find_max");
    struct ContactBST *(*contactBST_insert)(struct ContactBST *contactBST, struct ContactBSTNode *new_node) = dlsym(handle, "contactBST_insert");
    void (*delete_contactBSTNode) (struct ContactBSTNode **contactBST_node) = dlsym(handle, "delete_contactBSTNode");
    struct ContactBST *(*contactBST_delete_contact)(struct ContactBST *contactBST, struct Contact *contact_to_delete) = dlsym(handle, "contactBST_delete_contact");
    struct ContactBSTNode *(*contactBST_find_by_attributes)(struct ContactBST *contactBST,
                                                        char *name, char *surname,
                                                        struct Date birth_date,
                                                        char *email) = dlsym(handle, "contactBST_find_by_attributes");
    void (*contactBST_print_inorder)(struct ContactBST *contactBST) = dlsym(handle, "contactBST_print_inorder");
    void (*add_contacts_from_csv)(struct ContactList *contact_list,
                           struct ContactBST *contactBST,
                           char *file_name) = dlsym(handle, "add_contacts_from_csv");

#endif
    FILE *f = fopen("results_live.txt", "w+");
    if (f == NULL) {
        throw_error("Error while opening the file");
        exit(1);
    }

    char *generated_contacts = "testFiles/10contacts-generated-data.csv";

    //// --------------------------- Contact list tests ------------------------------------
    struct ContactList *contact_list;
    struct tms t0, t1, t2, t3, t4, t5, t6, t7, t8, t9, t10, t11, t12, t13, t14, t15, t16, t17;
    int test_no = 0;
    clock_t start_time, prev_time, curr_time;

    printf("-----------------------------TEST%i: create_contact_list_from_file: ----------------------------------\n",
           test_no++);
    start_time = clock();
    times(&t0);
    contact_list = create_contact_list_from_file(generated_contacts);
    curr_time = clock();
    times(&t1);
    print_time_diff_start(f, start_time, &t0, curr_time, &t1);

    printf("-----------------------------TEST%i: find_contact_node: ----------------------------------\n", test_no++);
    struct Date birth_date = create_date(6, 27, 1978);
    print_contact_node(*find_contact_node((*contact_list), "Keely", "Odonnell", birth_date));
    prev_time = curr_time;
    curr_time = clock();
    times(&t2);
    print_time_diff_start(f, start_time, &t0, curr_time, &t2);
    print_time_diff_prev(f, prev_time, &t1, curr_time, &t2);

    printf("-----------------------------TEST%i: find_contact_node (optimistic): ----------------------------------\n",
           test_no++);
    struct Date birth_date_first = create_date(29, 1, 1971);
    find_contact_node(*contact_list, "Jorden", "Gould", birth_date_first);
    prev_time = curr_time;
    curr_time = clock();
    times(&t3);
    print_time_diff_start(f, start_time, &t0, curr_time, &t3);
    print_time_diff_prev(f, prev_time, &t2, curr_time, &t3);

    printf("-----------------------------TEST%i: delete_contact_from_list (pessimistic): ----------------------------------\n",
           test_no++);
    struct Date birth_date_last = create_date(10, 2, 1988);
    find_contact_node(*contact_list, "Brianna", "Callahan", birth_date_last);
    prev_time = curr_time;
    curr_time = clock();
    times(&t4);
    print_time_diff_start(f, start_time, &t0, curr_time, &t4);
    print_time_diff_prev(f, prev_time, &t3, curr_time, &t4);

    printf("-----------------------------TEST%i: add_contact ----------------------------------\n", test_no++);
    struct Contact *contact_jan = create_mock_contact("Jan",
                                                      "Kowalski",
                                                      create_date(6, 9, 1940),
                                                      "janKowalski@g.com",
                                                      "854-855-774",
                                                      "Krakow, Dluga 21");
    add_contact(contact_list, create_contact_node(contact_jan));
    prev_time = curr_time;
    curr_time = clock();
    times(&t4);
    print_time_diff_start(f, start_time, &t0, curr_time, &t5);
    print_time_diff_prev(f, prev_time, &t4, curr_time, &t5);

    printf("-----------------------------TEST%i: delete_contact_from_list (optimistic): ----------------------------------\n",
           test_no++);
    delete_contact_from_list(contact_list, "Jorden", "Gould", birth_date_first);
    prev_time = curr_time;
    curr_time = clock();
    times(&t3);
    print_time_diff_start(f, start_time, &t0, curr_time, &t6);
    print_time_diff_prev(f, prev_time, &t5, curr_time, &t6);

    printf("-----------------------------TEST%i: delete_contact_from_list (pessimistic): ----------------------------------\n",
           test_no++);
    delete_contact_from_list(contact_list, "Brianna", "Callahan", birth_date_last);
    prev_time = curr_time;
    curr_time = clock();
    times(&t4);
    print_time_diff_start(f, start_time, &t0, curr_time, &t7);
    print_time_diff_prev(f, prev_time, &t6, curr_time, &t7);

    printf("-----------------------------TEST%i: sort_contact_list (NAME): ----------------------------------\n",
           test_no++);
    sort_contact_list(contact_list, NAME);
    prev_time = curr_time;
    curr_time = clock();
    times(&t4);
    print_time_diff_start(f, start_time, &t0, curr_time, &t8);
    print_time_diff_prev(f, prev_time, &t7, curr_time, &t8);
    print_contact_list(*contact_list);

    printf("-----------------------------TEST%i: delete_contact_list (NAME): ----------------------------------\n",
           test_no++);
    delete_contact_list(&contact_list);
    if (contact_list == NULL) printf("OK: contact_list is NULL\n");
    prev_time = curr_time;
    curr_time = clock();
    times(&t4);
    print_time_diff_start(f, start_time, &t0, curr_time, &t9);
    print_time_diff_prev(f, prev_time, &t8, curr_time, &t9);

    printf("-----------------------------ADDITIONAL TEST%i: contact comparison: ----------------------------------\n",
           test_no++);
    // Test:    compare_contacts_by_names,
    //          compare_contacts_by_surnames,
    //          compare_contacts_by_birth_dates,
    //          compare_contacts_by_emails
    contact_jan = create_mock_contact("Jan",
                                      "Kowalski",
                                      create_date(6, 9, 1940),
                                      "janKowalski@g.com",
                                      "854-855-774",
                                      "Krakow, Dluga 21");
    struct Contact *contact_anna = create_mock_contact("Anna",
                                                       "Antonowicz",
                                                       create_date(5, 9, 1940),
                                                       "annaBor@g.com",
                                                       "654-855-774",
                                                       "Gdansk, Dluga 21");
    printf("Contact names (contact_jan, contact_anna) comparison should return more than 0: %i\n",
           compare_contacts_by_names(*contact_jan, *contact_anna));
    printf("Contact surnames (contact_jan, contact_anna) comparison should return more than 0: %i\n",
           compare_contacts_by_surnames(*contact_jan, *contact_anna));
    printf("Contact dates (contact_jan, contact_anna) comparison should return more than 0: %i\n",
           compare_contacts_by_birth_dates(*contact_jan, *contact_anna));
    printf("Contact emails (contact_jan, contact_anna) comparison should return more than 0: %i\n",
           compare_contacts_by_emails(*contact_jan, *contact_anna));



    //// --------------------------- Contact BST functions tests ------------------------------------
    // Insert first contact
    struct ContactBST *contactBST = create_contactBST(NAME);
    struct Contact *contact_maciej = create_mock_contact("Maciej",
                                                         "Majewski",
                                                         create_date(5, 12, 1945),
                                                         "mMajewski@g.com",
                                                         "654-855-774",
                                                         "Gdansk, Dluga 21");
    contactBST = contactBST_insert(contactBST, create_contactBSTNode(contact_maciej));

    printf("-----------------------------TEST%i: add_contacts_from_csv to BST: ----------------------------------\n",
           test_no++);
    add_contacts_from_csv(NULL, contactBST, generated_contacts);
    prev_time = curr_time;
    curr_time = clock();
    times(&t4);
    print_time_diff_start(f, start_time, &t0, curr_time, &t10);
    print_time_diff_prev(f, prev_time, &t9, curr_time, &t10);

    printf("-----------------------------TEST%i: contactBST_insert: ----------------------------------\n", test_no++);
    struct Contact *contact_andrzej = create_mock_contact("Andrzej",
                                                          "Antonowicz",
                                                          create_date(5, 9, 1940),
                                                          "andrzejA@g.com",
                                                          "654-855-774",
                                                          "Gdansk, Dluga 21");
    contactBST = contactBST_insert(contactBST, create_contactBSTNode(contact_andrzej));
    prev_time = curr_time;
    curr_time = clock();
    times(&t4);
    print_time_diff_start(f, start_time, &t0, curr_time, &t11);
    print_time_diff_prev(f, prev_time, &t10, curr_time, &t11);

    printf("-----------------------------TEST%i: contactBST_find_by_attributes (optimistic): ----------------------------------\n",
           test_no++);
    struct ContactBSTNode *found_BSTNode1 = contactBST_find_by_attributes(contactBST,
                                                                          contact_maciej->name,
                                                                          contact_maciej->surname,
                                                                          contact_maciej->birth_date,
                                                                          contact_maciej->email);
    if (found_BSTNode1->contact->id == contact_maciej->id) {
        printf("Contact was found successfully.\n");
    } else {
        printf("Contact which was found is not the same as we were looking for.\n");
    }
    prev_time = curr_time;
    curr_time = clock();
    times(&t3);
    print_time_diff_start(f, start_time, &t0, curr_time, &t12);
    print_time_diff_prev(f, prev_time, &t11, curr_time, &t12);

    printf("-----------------------------TEST%i: contactBST_find_by_attributes (pessimistic): ----------------------------------\n",
           test_no++);
    struct ContactBSTNode *found_BSTNode2 = contactBST_find_by_attributes(contactBST,
                                                                          contact_andrzej->name,
                                                                          contact_andrzej->surname,
                                                                          contact_andrzej->birth_date,
                                                                          contact_andrzej->email);
    prev_time = curr_time;
    curr_time = clock();
    times(&t4);
    print_time_diff_start(f, start_time, &t0, curr_time, &t13);
    print_time_diff_prev(f, prev_time, &t12, curr_time, &t13);

    if (found_BSTNode2->contact->id == contact_andrzej->id) {
        printf("Contact was found successfully.\n");
    } else {
        printf("Contact which was found is not the same as we were looking for.\n");
    }

    printf("-----------------------------TEST%i: contactBST_delete_contact (optimistic): ----------------------------------\n",
           test_no++);
    contactBST_delete_contact(contactBST, contact_maciej);
    prev_time = curr_time;
    curr_time = clock();
    times(&t3);
    print_time_diff_start(f, start_time, &t0, curr_time, &t14);
    print_time_diff_prev(f, prev_time, &t13, curr_time, &t14);

    printf("-----------------------------TEST%i: contactBST_delete_contact (pessimistic): ----------------------------------\n",
           test_no++);
    contactBST_delete_contact(contactBST, contact_andrzej);
    prev_time = curr_time;
    curr_time = clock();
    times(&t4);
    print_time_diff_start(f, start_time, &t0, curr_time, &t15);
    print_time_diff_prev(f, prev_time, &t14, curr_time, &t15);

    printf("-----------------------------TEST%i: rebuild_contactBST_with_new_key (SURNAME): ----------------------------------\n",
           test_no++);
    rebuild_contactBST_with_new_key(contactBST, SURNAME);
    contactBST_print_inorder(contactBST);
    prev_time = curr_time;
    curr_time = clock();
    times(&t4);
    print_time_diff_start(f, start_time, &t0, curr_time, &t16);
    print_time_diff_prev(f, prev_time, &t15, curr_time, &t16);

    printf("-----------------------------TEST%i: delete_whole_contactBST : ----------------------------------\n",
           test_no++);
    delete_whole_contactBST(&contactBST);
    if (contactBST == NULL) printf("OK: contactBST is NULL\n");
    prev_time = curr_time;
    curr_time = clock();
    times(&t4);
    print_time_diff_start(f, start_time, &t0, curr_time, &t17);
    print_time_diff_prev(f, prev_time, &t16, curr_time, &t17);

/**/

#ifdef DLL
    dlclose(handle);
#endif

    return 0;
}
