# Contact book library
C library containing functions and data structers for managing simple contact book.
## Introduction
Each contact has the following fields:
- name, surname
- birthdate
- email
- phone
- address

Contact book may be created using **doubly linked list** or **binary search tree**. Library provides the follwing functionality (for each implementation):
- creating and deleting contact book
- adding and deleting contacts
- searching for contact
- sorting (linked list) or rebuilding (BST) contact book by one of the contact fields.

## Compile
Manual compile:
```bash
 $ git clone https://github.com/MieszkoMakuch/contact-book-library
 $ cd contact-book-library
 $ mkdir build
 $ cd build
 $ cmake ..
 $ make
```
## Dependencies
Required:
- C compiler
- cmake (3.7 or higher)
- make