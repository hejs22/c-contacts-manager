#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>

int bc_fgets(char* tab, int size, FILE* stream) {       // enhanced fgets with ability to clear buffer
    if (tab == NULL || size <= 0 || stream == NULL) return -1;
    fgets(tab, size, stream);
    if (*((tab)+strlen(tab)-1) != '\n') {
        while (getchar() != '\n');
        return 1;
    } else {
        *((tab) + strlen(tab) - 1) = '\0';
    }
    return 0;
}

struct Contact {                              // double linked list gives more functionality
    struct Contact *next;
    struct Contact *prev;
    char *firstName;
    char *lastName;
    char *phoneNumber;
    char *email;
};

struct ContactBook {
    struct Contact *head;
    int numberOfContacts;
};

struct ContactBook *initContactBook() {             // allocs memory for main structure and returns pointer to this structure
    struct ContactBook *book = calloc(1, sizeof(struct ContactBook));
    if (book == NULL) return NULL;
    book->head = NULL;
    book->numberOfContacts = 0;
    return book;
};

struct Contact *createContact(char *firstName, char *lastName, char *number, char *email) {     // allocs memory for new entry and fills all data fields, at the end return pointer to structure
    struct Contact *new = malloc(sizeof(struct Contact));
    if (new == NULL) return NULL;

    new->firstName = strdup(firstName);
    if (new->firstName == NULL) {
        free(new);
        return NULL;
    }

    new->lastName = strdup(lastName);
    if (new->lastName == NULL) {
        free(new->firstName);
        free(new);
        return NULL;
    }

    new->phoneNumber = strdup(number);
    if (new->phoneNumber == NULL) {
        free(new->lastName);
        free(new->firstName);
        free(new);
        return NULL;
    }

    new->email = strdup(email);
    if (new->email == NULL) {
        free(new->phoneNumber);
        free(new->lastName);
        free(new->firstName);
        free(new);
        return NULL;
    }

    new->prev = NULL;
    new->next = NULL;
    return new;
}

void printContactBook(struct ContactBook *book) {
    if (book == NULL) {
        printf("\nContact book doesn't exist.\n");
    } else if (book->head == NULL) {
        printf("\nContact book is empty.\n");
    } else {
        printf("\nList of contacts:\n-------------------------------------------------\n");
        struct Contact *temp = book->head;
        while (temp != NULL) {
            if (!temp->firstName || !temp->lastName || !temp->phoneNumber || !temp->email) break;
            printf("%s %s | %s | %s\n", temp->firstName, temp->lastName, temp->phoneNumber, temp->email);
            temp = temp->next;
        }
        printf("-------------------------------------------------\n");
    }
}

int addContact(struct ContactBook *book, struct Contact *contact) {          // adds contacts in alphabetical order
    if (book == NULL || contact == NULL) return 1;
    if (book->head == NULL) {
        book->head = contact;
        book->numberOfContacts++;
        return 0;
    } else {
        struct Contact *temp = book->head;
        if (strcmp(temp->lastName, contact->lastName) > 0) {
            book->head = contact;
            contact->prev = NULL;
            contact->next = temp;
            temp->prev = contact;
            book->numberOfContacts++;
            return 0;
        }

        while (temp->next != NULL) {
            if ((strcmp(temp->lastName, contact->lastName) <= 0) && (strcmp(temp->next->lastName, contact->lastName) > 0)) {
                temp->next->prev = contact;
                contact->next = temp->next;
                contact->prev = temp;
                temp->next = contact;
                book->numberOfContacts++;
                return 0;
            }
            temp = temp->next;
        }

        temp->next = contact;
        contact->prev = temp;
        book->numberOfContacts++;
    }

    return 0;
}

void deleteContact(struct ContactBook *book, struct Contact *contact) {         // deletes given contact and updates pointers in book
    if (contact != NULL) {
        if (contact->firstName != NULL) free(contact->firstName);
        if (contact->lastName != NULL) free(contact->lastName);
        if (contact->phoneNumber != NULL)  free(contact->phoneNumber);
        if (contact->email != NULL)  free(contact->email);
        contact->firstName = NULL;
        contact->lastName = NULL;
        contact->phoneNumber = NULL;
        contact->email = NULL;

        struct Contact *prevContact = NULL;
        if (contact->prev != NULL) prevContact = contact->prev;

        struct Contact *nextContact = NULL;
        if (contact->next != NULL) nextContact = contact->next;

        if (prevContact == NULL) {
            book->head = nextContact;
        } else if (nextContact == NULL) {
            prevContact->next = nextContact;
        } else {
            nextContact->prev = prevContact;
            prevContact->next = nextContact;
        }

        book->numberOfContacts--;
        free(contact);
        contact = NULL;
    }
}

struct Contact *findContact(struct ContactBook *book, const char *firstName, const char *lastName) {       // searches for a contact in book
    if (book == NULL || firstName == NULL || lastName == NULL) return NULL;
    struct Contact *temp = book->head;
    while (temp != NULL) {
        if (strcmp(temp->lastName, lastName) == 0) {
            if (strcmp(temp->firstName, firstName) == 0) {
                return temp;
            }
        }
        temp = temp->next;
    }
    return NULL;
}

void deleteContactBook(struct ContactBook *book) {      // deletes all entries and whole structure
    if (book != NULL) {
        if (book->numberOfContacts != 0) {
            struct Contact *temp = book->head;
            while (temp->next != NULL) {
                struct Contact *toDelete = temp;
                temp = temp->next;
            }
            while (temp->prev != NULL) {
                temp = temp->prev;
                deleteContact(book, temp->next);
            }
            deleteContact(book, temp);
        }
        free(book);
        book = NULL;
    }
    book = NULL;
}

int main() {
    printf("Contacts management system. List of commands available below: ");
    struct ContactBook *book = initContactBook();
    while (1) {
        puts("\n0 - list all commands.\n1 - add entry to contact book.\n2 - delete entry from contact book.\n3 - list all entries from phone book.\n9 - close.\n");

        char option = getchar();
        fflush(stdin);
        if (option == '3') {
            printContactBook(book);
        } else if (option == '2') {
            char fName[100];
            char lName[100];
            printf("First name: ");
            bc_fgets(fName, 100, stdin);
            printf("Last name: ");
            bc_fgets(lName, 100, stdin);
            deleteContact(book, findContact(book, fName, lName));
            printf("%s %s deleted from contacts.\n", fName, lName);
        } else if (option == '1') {
            char fName[100];
            char lName[100];
            char phone[15];
            char email[50];
            printf("First name: ");
            bc_fgets(fName, 100, stdin);
            printf("Last name: ");
            bc_fgets(lName, 100, stdin);
            printf("Phone number: ");
            bc_fgets(phone, 15, stdin);
            printf("Email: ");
            bc_fgets(email, 50, stdin);
            addContact(book, createContact(fName, lName, phone, email));
            printf("%s %s added to contacts.\n", fName, lName);
        } else if (option == '9') {
            break;
        }
    }

    deleteContactBook(book);

    return 0;
}
