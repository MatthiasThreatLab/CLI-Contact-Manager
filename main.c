#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>

#define FIRST_NAME_MAX_LENGTH 50
#define LAST_NAME_MAX_LENGTH 50
#define EMAIL_MAX_LENGTH 75
#define PHONE_MAX_LENGTH 20
#define NOTES_MAX_LENGTH 1023

#define NUMBER_OF_FIELDS_CONTACT_FILE 5

typedef struct {
    
    char firstName[FIRST_NAME_MAX_LENGTH + 1]; // + 1 to take into account the null terminator '\0'
    char lastName[LAST_NAME_MAX_LENGTH + 1];
    char email[EMAIL_MAX_LENGTH + 1];
    char phone[PHONE_MAX_LENGTH + 1];
    char notes[NOTES_MAX_LENGTH + 1];

} Contact;

typedef struct {
    
    Contact* contacts;
    size_t counter;

} ContactArray;


/**
 * @brief   Clears the input buffer
 *
 * @return  0 - On completion
 * 
 */
int clearInputBuffer();

/**
 * @brief   Displays all the contacts in the contact file
 *
 * @param   contactFilePath Path to the contact file
 *
 * @return  0 if the function was successful, 1 otherwise
 * 
 */
int displayAllContacts(char* contactFilePath);

/**
 * @brief   Displays a single contact
 *
 * @param   contact Contact to display
 *
 * @return  0 on completion
 * 
 */
int displayContact(Contact contact);

/**
 * @brief   Adds a new entry to the contact file
 *
 * @param   contactFilePath Path to the contact file
 *
 * @param   contact         The new Contact to add to the file
 * 
 * @return  0 if the Contact was successfully added, 1 otherwise
 * 
 */
int writeContactToFile(char* contactFilePath, Contact contact);

/**
 * @brief   Deletes an entry in the contact file
 *
 * @param   contactFilePath     Path to the contact file
 *
 * @param   contactToDelete     The Contact to be deleted
 * 
 * @return  0 if the contact was successfully deleted, 1 otherwise
 * 
 */
int deleteContactFromFile(char* contactFilePath, Contact contactToDelete);

/**
 * @brief   Prompts the user for the new values for contactToEdit and updates the
 *          contact in the contact file
 *
 * @param   contactToEdit   The Contact to be edited
 * 
 * @return  0 if the contact was successfully deleted, 1 otherwise
 * 
 */
int editContact(char* contactFilePath, Contact contactToEdit);

/**
 * @brief   Prompts the user for the infos on a new contact to add to the file,
 *          validate the inputs and write the contact to the file
 *
 * @param   contactFilePath Path to the contact file
 *
 * @return  0 if the contact was successfully deleted, 1 otherwise
 * 
 */
int addNewContactPrompt(char* contactFilePath);

/**
 * @brief   Prompts the user for some info about the contact they want to edit to retrieve it,
 *          if no contact is found              --> sends user back to main menu
 *          if only one contact is found        --> sends to editContact() that will prompt the user
 *                                                  to get the new info about the contact they want
 *                                                  to edit and will edit the contact file
 *          if more than one conctact is found  --> displays the list of all the contacts found, prompts
 *                                                  the user into picking one and then send to editContact()
 *
 * @param   contactFilePath Path to the contact file
 *
 * @return  0 if the contact was successfully deleted, 1 otherwise
 * 
 */
int editContactPrompt(char* contactFilePath);

/**
 * @brief   Prompts the user for some info about the contact they want to search for to retrieve it,
 *          then sends it to findContact() that will return a ContactArray of all the contacts that
 *          match the infos
 *
 * @param   contactFilePath Path to the contact file
 *
 * @return  ContactArray of the matching contacts
 * 
*/
ContactArray contactSearchPrompt(char* contactFilePath);

/**
 * @brief   Transforms a line from the contact file into a Contact object
 *
 * @param   contact Contact pointer that will receive the values of the line
 * 
 * @param   line    One line of the contact file
 *
 * @return  0 if the contact was successfully deleted, 1 otherwise
 * 
 */
int getContactFromLineInFile(Contact* contact, char* line);

/**
 * @brief   Reads the contact file line by line into a ContactArray
 *
 * @param   contactFilePath Path to the contact file
 *
 * @return  ContactArray of all the contacts in the contact file
 * 
 */
ContactArray getAllContacts(char* contactFilePath);

/**
 * @brief   Retrieves all the contacts that match the parameters. Contact parameters are optional, if none
 *          are provided, retrieves all the contacts
 *
 * @param   contactFilePath Path to the contact file
 * 
 * @param   firstNameSearch NULL or first name of the contact
 * @param   lastNameSearch  NULL or last name of the contact
 * @param   emailSearch     NULL or email of the contact
 * @param   phoneSearch     NULL or phone number of the contact
 *
 * @return  ContactArray of all the contacts that matches the parameters
 * 
 */
ContactArray findContacts(char* contactFilePath, char* firstNameSearch, char* lastNameSearch, char* emailSearch, char* phoneSearch);

/**
 * @brief   Creates a new Contact instance
 *
 * @param   firstName   First name of the contact
 * @param   lastName    Last name of the contact
 * @param   email       Email of the contact
 * @param   phone       Phone number of the contact
 * @param   notes       Notes about the contact
 *
 * @return  Contact
 * 
 */

Contact createContact(char* firstName, char* lastName, char* email, char* phone, char* notes);

bool isAnEmailAddress(char* email);
bool isAFirstName(char* firstName);
bool isALastName(char* lastName);
bool isAPhoneNumber(char* phone);

int main() {

    char* contactFilePath = "contacts.txt";
    
    ContactArray contactsFound = {NULL, 0};
    int choice = 3;

    while(true) {
        printf("\n---------- Menu ----------\n");
        printf("1. Display all contacts\n");
        printf("2. Search for a contact\n");
        printf("3. Edit contact\n");
        printf("---------- Menu ----------\n\n");

        printf("Select an option: ");
        scanf(" %d", &choice);

        if (choice == 1) {
            
            displayAllContacts(contactFilePath);

        } else if (choice == 2) {
            
            printf("\n\n---- Contact search ----\n");
            
            contactsFound = contactSearchPrompt(contactFilePath);

            if(contactsFound.counter > 0) {
                printf("\n\n%d contact%s found:\n", contactsFound.counter, contactsFound.counter > 1 ? "s" : "");
                printf("--------------------------\n");

                for (size_t i = 0; i < contactsFound.counter; i++)
                {
                    displayContact(contactsFound.contacts[i]);
                }
                
            } else {
                printf("\nNo contacts found.\n\n");
            }

            free(contactsFound.contacts);
            contactsFound.contacts = NULL;

            

        } else if (choice == 3) {

            printf("---- Edit contact ----\n");
            editContactPrompt(contactFilePath);

            // displayAllContacts(contactFilePath);

            // Contact contacToEdit = {"ell","south","ell@south.com","0422222222","hey"};

            // editContact(contactFilePath, contacToEdit);
            
        } else {
            clearInputBuffer();
            printf("!!!! Error, please try again !!!\n\n");
        }
        
    }

}


int clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);

    return 0;
}

int displayAllContacts(char* contactFilePath) {
    
    ContactArray allContacts = getAllContacts(contactFilePath);

    if(allContacts.contacts == NULL) {
        printf("Couldn't retrieve contacts\n");

        free(allContacts.contacts);
        allContacts.contacts = NULL;

        return 1;
    }
    
    for (int i = 0; i < allContacts.counter; i++) {
        
        printf("First name: %s | Last name: %s | Email: %s | Phone: %s | Notes: %s",
            allContacts.contacts[i].firstName,
            allContacts.contacts[i].lastName,
            allContacts.contacts[i].email,
            allContacts.contacts[i].phone,
            allContacts.contacts[i].notes
        );
        printf("\n");
    }

    free(allContacts.contacts);
    allContacts.contacts = NULL;
    

    return 0;
}

int displayContact(Contact contact) {
    
    printf("First name: %s | Last name: %s | Email: %s | Phone: %s | Notes: %s",
        contact.firstName,
        contact.lastName,
        contact.email,
        contact.phone,
        contact.notes
    );
    printf("\n");

    return 0;
}

int writeContactToFile(char* contactFilePath, Contact contact) {
    FILE* contactFile = fopen(contactFilePath, "a");

    if (contactFile == NULL) {
        printf("Could not open the file");
        fclose(contactFile);
        return 1;
    }
    
    fprintf(contactFile, "\n%s,%s,%s,%s,%s",
        contact.firstName,
        contact.lastName,
        contact.email,
        contact.phone,
        contact.notes
    );

    fclose(contactFile);

    return 0;
}

int deleteContactFromFile(char* contactFilePath, Contact contactToDelete) {
    char* tempFilePath = "temp.txt";

    FILE* contactFile = fopen(contactFilePath, "r");
    FILE* tempFile = fopen(tempFilePath, "w");

    if (contactFile == NULL) {
        printf("Could not open the contcat file");
        fclose(contactFile);
        fclose(tempFile);
        return 1;
    }

    if (tempFile == NULL) {
        printf("Could not open the temp file");
        fclose(contactFile);
        fclose(tempFile);
        return 1;
    }

    char line[sizeof(Contact)] = {0};
    Contact currentContact;
    
    while (fgets(line, sizeof(line), contactFile) != NULL) { // loops through each line of the file

        currentContact = (Contact){0};
        getContactFromLineInFile(&currentContact, line);

        if(strcmp(currentContact.firstName, contactToDelete.firstName) == 0 &&
        strcmp(currentContact.lastName, contactToDelete.lastName) == 0 &&
        strcmp(currentContact.email, contactToDelete.email) == 0 &&
        strcmp(currentContact.phone, contactToDelete.phone) == 0)
        {
            continue;
        } else {
            fputs(line, tempFile);
        }

    }

    fclose(tempFile);
    fclose(contactFile);

    remove(contactFilePath); // Deletes the original file
    rename(tempFilePath, contactFilePath); // Renames the temporary file

    return 0;
}

int editContact(char* contactFilePath, Contact contactToEdit) {
    char newFirstName[FIRST_NAME_MAX_LENGTH + 2] = ""; // + 2 to take into account the null terminator '\0' and '\n' for user input
    char newLastName[LAST_NAME_MAX_LENGTH + 2] = "";
    char newEmail[EMAIL_MAX_LENGTH + 2] = "";
    char newPhone[PHONE_MAX_LENGTH + 2] = "";
    char newNotes[NOTES_MAX_LENGTH + 2] = "";

    char* oldFirstName = contactToEdit.firstName;
    char* oldLastName = contactToEdit.lastName;
    char* oldEmail = contactToEdit.email;
    char* oldPhone = contactToEdit.phone;
    char* oldNotes = contactToEdit.notes;

    // char oldFirstName[FIRST_NAME_MAX_LENGTH + 2] = contactToEdit.firstName;
    // char oldLastName[LAST_NAME_MAX_LENGTH + 2] = contactToEdit.lastName;
    // char oldEmail[EMAIL_MAX_LENGTH + 2] = contactToEdit.email;
    // char oldPhone[PHONE_MAX_LENGTH + 2] = contactToEdit.phone;
    // char oldNotes[NOTES_MAX_LENGTH + 2] = contactToEdit.notes;

    while(true) {
        clearInputBuffer();
        printf("Old first name: %s\n", oldFirstName);
        printf("New first name (%d char max, leave blank to keep it the same): ", FIRST_NAME_MAX_LENGTH);
        fgets(newFirstName, sizeof(newFirstName), stdin); // need fgets to accept white spaces.
        newFirstName[strlen(newFirstName) - 1] = '\0'; // removes \n that fgets adds at the end

        if(strcmp(newFirstName, "") == 0) {
            strcpy(newFirstName, oldFirstName);
            break;
        } else if (isAFirstName(newFirstName)) {
            break;
        }
    }

    while(true) {
        clearInputBuffer();
        printf("Old last name: %s\n", oldLastName);
        printf("New last name (%d char max, leave blank to keep it the same): ", LAST_NAME_MAX_LENGTH);
        fgets(newLastName, sizeof(newLastName), stdin); // need fgets to accept white spaces.
        newLastName[strlen(newLastName) - 1] = '\0'; // removes \n that fgets adds at the end

        if(strcmp(newLastName, "") == 0) {
            strcpy(newLastName, oldLastName);
            break;
        } else if (isALastName(newLastName)) {
            break;
        }  
    }

    while(true) {
        clearInputBuffer();
        printf("Old email: %s\n", oldEmail);
        printf("New email address (%d char max, leave blank to keep it the same): ", EMAIL_MAX_LENGTH);
        fgets(newEmail, sizeof(newEmail), stdin); // need fgets to accept white spaces.
        newEmail[strlen(newEmail) - 1] = '\0'; // removes \n that fgets adds at the end

        if(strcmp(newEmail, "") == 0) {
            strcpy(newEmail, oldEmail);
            break;
        } else if (isAnEmailAddress(newEmail)) {
            break;
        }
    }
    
    while(true) {
        clearInputBuffer();
        printf("Old phone number: %s\n", oldPhone);
        printf("New phone number (%d char max, leave blank to keep it the same): ", PHONE_MAX_LENGTH);
        fgets(newPhone, sizeof(newPhone), stdin); // need fgets to accept white spaces.
        newPhone[strlen(newPhone) - 1] = '\0'; // removes \n that fgets adds at the end

        if(strcmp(newPhone, "") == 0) {
            strcpy(newPhone, oldPhone);
            break;
        } else if (isAPhoneNumber(newPhone)) {
            break;
        }
    }
    
    clearInputBuffer();
    printf("Old notes:\n");
    printf("---------------------------\n");
    printf("%s\n", oldNotes);
    printf("---------------------------\n");
    printf("New notes about this contact (%d char max): ", NOTES_MAX_LENGTH);
    fgets(newNotes, sizeof(newNotes), stdin); // need fgets to accept white spaces.
    newNotes[strlen(newNotes) - 1] = '\0'; // removes \n that fgets adds at the end

    if(strcmp(newNotes, "") == 0) {
        strcpy(newNotes, oldNotes);
        newNotes[strlen(newNotes) - 1] = '\0'; // removes \n that is at the end of the notes
    }

    deleteContactFromFile(contactFilePath, contactToEdit);

    Contact editedContact = createContact(newFirstName, newLastName, newEmail, newPhone, newNotes);
    writeContactToFile(contactFilePath, editedContact);

    printf("\nUpdated contact:\n");
    displayContact(editedContact);

    return 0;
}

int addNewContactPrompt(char* contactFilePath) {
    char firstName[FIRST_NAME_MAX_LENGTH + 2] = ""; // + 2 to take into account the null terminator '\0' and '\n' for user input
    char lastName[LAST_NAME_MAX_LENGTH + 2] = "";
    char email[EMAIL_MAX_LENGTH + 2] = "";
    char phone[PHONE_MAX_LENGTH + 2] = "";
    char notes[NOTES_MAX_LENGTH + 2] = "";

    
    do {
        clearInputBuffer();
        printf("New contact's first name (%d char max): ", FIRST_NAME_MAX_LENGTH);
        fgets(firstName, sizeof(firstName), stdin); // need fgets to accept white spaces.
        firstName[strlen(firstName) - 1] = '\0'; // removes \n that fgets adds at the end
        
    } while(!isAFirstName(firstName));

    do {
        clearInputBuffer();
        printf("New contact's last name (%d char max): ", LAST_NAME_MAX_LENGTH);
        fgets(lastName, sizeof(lastName), stdin); // need fgets to accept white spaces.
        lastName[strlen(lastName) - 1] = '\0'; // removes \n that fgets adds at the end
        
    } while(!isALastName(lastName));

    do {
        clearInputBuffer();
        printf("New contact's email address (%d char max): ", EMAIL_MAX_LENGTH);
        fgets(email, sizeof(email), stdin); // need fgets to accept white spaces.
        email[strlen(email) - 1] = '\0'; // removes \n that fgets adds at the end
        
    } while (!isAnEmailAddress(email));
    
    do {
        clearInputBuffer();
        printf("New contact's phone number (%d char max): ", PHONE_MAX_LENGTH);
        fgets(phone, sizeof(phone), stdin); // need fgets to accept white spaces.
        phone[strlen(phone) - 1] = '\0'; // removes \n that fgets adds at the end
        
    } while (!isAPhoneNumber(phone));
    
    clearInputBuffer();
    printf("Notes about this contact (%d char max): ", NOTES_MAX_LENGTH);
    fgets(notes, sizeof(notes), stdin); // need fgets to accept white spaces.
    notes[strlen(notes) - 1] = '\0'; // removes \n that fgets adds at the end

    Contact newContact = createContact(firstName, lastName, email, phone, notes);

    writeContactToFile(contactFilePath, newContact);

    printf("\nNew contact:\n");
    displayContact(newContact);

    return 0;

}

int editContactPrompt(char* contactFilePath) {
    
    printf("Please enter the information about the contact you want to edit\n");

    ContactArray contactsFound = contactSearchPrompt(contactFilePath);

    if(contactsFound.counter == 0) {

        printf("\nNo contacts found.\n\n");

    } else if (contactsFound.counter == 1) {

        printf("\n\n1 contact found, please edit below:\n\n");
        editContact(contactFilePath, contactsFound.contacts[0]);

    } else if (contactsFound.counter > 1) {

        printf("\n\n%d contact%s found:\n", contactsFound.counter, contactsFound.counter > 1 ? "s" : "");
        printf("Refine your search, please\n");
        // TODO: display a menu the user can select from each contact found to edit this one (or 0 if they want to return to the main menu)
        printf("--------------------------\n");

        for (size_t i = 0; i < contactsFound.counter; i++)
        {
            displayContact(contactsFound.contacts[i]);
        }
        
    } else {

        printf("\n!!! ERROR please try again\n\n");

    }

    free(contactsFound.contacts);
    contactsFound.contacts = NULL;

    return 0;
}

ContactArray contactSearchPrompt(char* contactFilePath) {
    char firstName[FIRST_NAME_MAX_LENGTH + 2] = ""; // + 2 to take into account the null terminator '\0' and '\n' for user input
    char lastName[LAST_NAME_MAX_LENGTH + 2] = "";
    char email[EMAIL_MAX_LENGTH + 2] = "";
    char phone[PHONE_MAX_LENGTH + 2] = "";

    printf("Press Enter when ready\n\n");
            
    do {
        clearInputBuffer();
        printf("Contact's first name (%d char max, optional): ", FIRST_NAME_MAX_LENGTH);
        fgets(firstName, sizeof(firstName), stdin); // need fgets to accept white spaces.
        firstName[strlen(firstName) - 1] = '\0'; // removes \n that fgets adds at the end

        if(strcmp(firstName, "") == 0) {
            break;
        }
        
    } while(!isAFirstName(firstName));

    do {
        clearInputBuffer();
        printf("Contact's last name (%d char max, optional): ", LAST_NAME_MAX_LENGTH);
        fgets(lastName, sizeof(lastName), stdin); // need fgets to accept white spaces.
        lastName[strlen(lastName) - 1] = '\0'; // removes \n that fgets adds at the end

        if(strcmp(lastName, "") == 0) {
            break;
        }
        
    } while(!isALastName(lastName));

    do {
        clearInputBuffer();
        printf("Contact's email address (%d char max, optional): ", EMAIL_MAX_LENGTH);
        fgets(email, sizeof(email), stdin); // need fgets to accept white spaces.
        email[strlen(email) - 1] = '\0'; // removes \n that fgets adds at the end

        if(strcmp(email, "") == 0) {
            break;
        }
        
    } while (!isAnEmailAddress(email));
    
    do {
        clearInputBuffer();
        printf("Contact's phone number (%d char max, optional): ", PHONE_MAX_LENGTH);
        fgets(phone, sizeof(phone), stdin); // need fgets to accept white spaces.
        phone[strlen(phone) - 1] = '\0'; // removes \n that fgets adds at the end

        if(strcmp(phone, "") == 0) {
            break;
        }
        
    } while (!isAPhoneNumber(phone));

    return findContacts(contactFilePath, strcmp(firstName, "") == 0 ? NULL : firstName, strcmp(lastName, "") == 0 ? NULL : lastName, strcmp(email, "") == 0 ? NULL : email, strcmp(phone, "") == 0 ? NULL : phone);
}

int getContactFromLineInFile(Contact* contact, char* line) {
    char currentCharacter;
    int columnCounter = 0;
    
    int i = 0;
    while (line[i] != '\0') { // Loop through each character of the line until the null terminator is found
        currentCharacter = line[i];

        // if we're not at the notes yet and the current character is ',', it's a delimiter
        // so we skip it (i++) and change the column
        if (columnCounter < NUMBER_OF_FIELDS_CONTACT_FILE - 1 && currentCharacter == ',') {
            columnCounter++;
            i++;
            continue;
        }

        // add the current character to the right variable of contact
        switch (columnCounter) {
            case 0:
                strncat(contact->firstName, &currentCharacter, 1);
                break;
            case 1:
                strncat(contact->lastName, &currentCharacter, 1);
                break;
            case 2:
                strncat(contact->email, &currentCharacter, 1);
                break;
            case 3:
                strncat(contact->phone, &currentCharacter, 1);
                break;
            case 4:
                strncat(contact->notes, &currentCharacter, 1);
                break;
        }
        
        i++; // next character
    }
}

ContactArray getAllContacts(char* contactFilePath) {
    FILE* contactFile = fopen(contactFilePath, "r");

    if (contactFile == NULL) {
        printf("Could not open the file");
        fclose(contactFile);
        return (ContactArray){NULL,0};
    }
    
    rewind(contactFile); // reset file pointer to the beginning


    char line[sizeof(Contact)] = {0};
    Contact currentContact;

    Contact* contacts = {0};
    int contactCounter = 0;
    
    while (fgets(line, sizeof(line), contactFile) != NULL) { // loops through each line of the file
        currentContact = (Contact){0};

        Contact* temp = realloc(contacts, (contactCounter + 1) * sizeof(Contact)); // increase the size of the array of Contact by 1 * sizeof(Contact)
        if(temp == NULL) {
            
            printf("Memory reallocation failed!\n");
            fclose(contactFile);
            return (ContactArray){0};

        } else {

            contacts = temp;
            temp = NULL;

        }

        getContactFromLineInFile(&currentContact, line);

        contacts[contactCounter] = currentContact;
        contactCounter++;
        
    }

    fclose(contactFile);

    return (ContactArray){contacts, contactCounter};
}

ContactArray findContacts(char* contactFilePath, char* firstNameSearch, char* lastNameSearch, char* emailSearch, char* phoneSearch) {
    ContactArray contactsFound = {NULL, 0};

    FILE* contactFile = fopen(contactFilePath, "r");
    
    if(firstNameSearch == NULL && lastNameSearch == NULL && emailSearch == NULL && phoneSearch == NULL) {
        return getAllContacts(contactFilePath);
    }

    char line[sizeof(Contact)] = {0};
    Contact currentContact;
    
    while (fgets(line, sizeof(line), contactFile) != NULL) { // loops through each line of the file

        currentContact = (Contact){0};
        getContactFromLineInFile(&currentContact, line);

        if(firstNameSearch != NULL) {
            if(strcmp(currentContact.firstName, firstNameSearch) != 0) {
                continue;
            }
        }

        if(lastNameSearch != NULL) {
            if(strcmp(currentContact.lastName, lastNameSearch) != 0) {
                continue;
            }
        }

        if(emailSearch != NULL) {
            if(strcmp(currentContact.email, emailSearch) != 0) {
                continue;
            }
        }

        if(phoneSearch != NULL) {
            if(strcmp(currentContact.phone, phoneSearch) != 0) {
                continue;
            }
        }

        Contact* temp = realloc(contactsFound.contacts, (contactsFound.counter + 1) * sizeof(Contact)); // increase the size of the array of Contact by 1 * sizeof(Contact)
        if(temp == NULL) {
            
            printf("Memory reallocation failed!\n");
            fclose(contactFile);
            return (ContactArray){0};

        } else {

            contactsFound.contacts = temp;
            temp = NULL;

        }

        contactsFound.contacts[contactsFound.counter] = currentContact;
        contactsFound.counter++;
        
    }

    fclose(contactFile);

    return contactsFound;

}

Contact createContact(char* firstName, char* lastName, char* email, char* phone, char* notes) {
    Contact newContact = (Contact){0};
    
    strcpy(newContact.firstName, firstName);
    strcpy(newContact.lastName, lastName);
    strcpy(newContact.email, email);
    strcpy(newContact.phone, phone);
    strcpy(newContact.notes, notes);

    return newContact;
}

bool isAnEmailAddress(char* email) {

    if(strlen(email) < 6 || strlen(email) > EMAIL_MAX_LENGTH) { // at least 1 username char, '@', 1 domain char, '.', 2 top-level domain chars
        printf("!!! Error: Wrong format of email address\n");
        return false;
    }

    // /!\ /!\ 
    //      In this function, strings and pointers to these strings are used interchangeably (makes sense because a
    //      string is simply a pointer that points to the first character of the string in memory)
    // /!\ /!\ 

    char atSymbol[] = "@";
    char periodSymbol[] = ".";
    char* atDomainPeriodTopLevelDomain;
    char* periodTopLevelDomain;

    atDomainPeriodTopLevelDomain = strstr(email, atSymbol); // If atSymbol is found in email, atDomainPeriodTopLevelDomain will point to the first character of @ within email, NULL otherwise.

    if (atDomainPeriodTopLevelDomain == NULL) {
        printf("!!! Error: Wrong format of email address\n");
        return false;
    }

    // check username:  Only alphanumeric characters, periods, underscores, percent signs, plus signs, and hyphens allowed.
    //                  One or more of these characters must be present.
    
    // pointer difference (atDomainPeriodTopLevelDomain points to the position of @, email points at the start of the string)
    // if this is 0, it means
    if(atDomainPeriodTopLevelDomain - email > 0) {
        for (int i = 0; i < atDomainPeriodTopLevelDomain - email; i++) {
            if (!(isalnum(email[i]) || email[i] == '.' || email[i] == '_' || email[i] == '%' || email[i] == '+' || email[i] == '-')) {
                printf("!!! Error: Wrong format of email address\n");
                return false;
            }
        }
    } else {
        printf("!!! Error: Wrong format of email address\n");
        return false;
    }

    periodTopLevelDomain = strstr(atDomainPeriodTopLevelDomain, periodSymbol);

    if (periodTopLevelDomain == NULL) {
        printf("!!! Error: Wrong format of email address\n");
        return false;
    }

    // check domain:    Only alphanumeric characters, periods, and hyphens allowed.
    //                  One or more of these characters must be present.

    if(periodTopLevelDomain - atDomainPeriodTopLevelDomain > 0) {
        for (int i = 1; i < periodTopLevelDomain - atDomainPeriodTopLevelDomain; i++) { // starts at 1 to skip the @ symbol
            
            if (!(isalnum(atDomainPeriodTopLevelDomain[i]) || atDomainPeriodTopLevelDomain[i] == '.' || atDomainPeriodTopLevelDomain[i] == '-')) {
                printf("!!! Error: Wrong format of email address\n");
                return false;
            }
        }
    } else {
        printf("!!! Error: Wrong format of email address\n");
        return false;
    }

    // check top-level domain (TLD):    Only alpha characters and periods allowed.
    //                                  2 or more of these characters must be present.

    if(strlen(periodTopLevelDomain) >= 3) { // (period + at least 2 characters for the TLD)
        for (int i = 1; i < strlen(periodTopLevelDomain); i++) { // starts at 1 to skip the . symbol
            
            if (!(isalpha(periodTopLevelDomain[i]) || periodTopLevelDomain[i] == '.')) {
                printf("!!! Error: Wrong format of email address\n");
                return false;
            }
        }
    } else {
        printf("!!! Error: Wrong format of email address\n");
        return false;
    }

    return true;

}

bool isAFirstName(char* firstName) {
    bool pass = true;
        
    int len = strlen(firstName);
    if (len <= 0 || len >= FIRST_NAME_MAX_LENGTH) {
        printf("\n!!! Error: first name must be 1-%d character long !!!\n\n", FIRST_NAME_MAX_LENGTH);
        pass = false;
    }

    for (int i = 0; i < len; i++)
    {
        if (!(isalpha(firstName[i]) || firstName[i] == '-' || firstName[i] == ' ')) {
            printf("\n!!! Error: Only alpha characters, hyphen and spaces accepted !!!\n\n");
            pass = false;

            break;
        }
    }

    return pass;
}

bool isALastName(char* lastName) {
    int pass = true;
        
    int len = strlen(lastName);
    if (len <= 0 || len >= LAST_NAME_MAX_LENGTH) {
        printf("\n!!! Error: last name must be 1-%d character long !!!\n\n", LAST_NAME_MAX_LENGTH);
        pass = false;
    }

    for (int i = 0; i < len; i++)
    {
        if (!(isalpha(lastName[i]) || lastName[i] == '-' || lastName[i] == ' ')) {
            printf("\n!!! Error: Only alpha characters, hyphen and spaces accepted !!!\n\n");
            pass = false;

            break;
        }
    }

    return pass;
}

bool isAPhoneNumber(char* phone) {
    bool pass = true;

    int len = strlen(phone);
    if (len != 10)
    {
        printf("!!! Error: Phone number must be 10 characters long !!!\n\n");
        pass = false;
    }

    for (int i = 0; i < len; i++)
    {
        if (!isdigit(phone[i])) {
            printf("\n!!! Error: Only digits are accepted !!!\n\n");
            pass = false;

            break;
        }
    }

    return pass;
}


