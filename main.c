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

#define MATCH 1
#define NO_MATCH 2
#define IGNORE 3

typedef struct {
    
    char firstName[FIRST_NAME_MAX_LENGTH + 1]; // + 1 to take into account the null terminator '\0'
    char lastName[LAST_NAME_MAX_LENGTH + 1];
    char email[EMAIL_MAX_LENGTH + 1];
    char phone[PHONE_MAX_LENGTH + 1];
    char notes[NOTES_MAX_LENGTH + 1];

} Contact;

typedef struct {
    
    Contact* contacts;
    size_t contactCounter;

} ContactArray;


int clearInputBuffer();
int displayAllContacts(FILE* contactFile);
int addNewContact(FILE* contactFile);
int displayContact(Contact contact);
bool isAnEmailAddress(char* email);
bool isAFirstName(char* firstName);
bool isALastName(char* lastName);
bool isAPhoneNumber(char* phone);
int getContactFromLineInFile(Contact* contact, char* line);
ContactArray getAllContacts(FILE* contactFile);
ContactArray findContacts(FILE* contactFile, char* firstName, char* lastName, char* email, char* phone);
int contactSearch(FILE* contactFile);

int main() {

    FILE* contactFile = fopen("contacts.txt", "a+");

    if (contactFile == NULL) {
        printf("Could not open the file");
        return 1;
    }
    
    int choice = 0;

    do {
        printf("\n---------- Menu ----------\n");
        printf("1. Display all contacts\n");
        printf("2. Search for a contact\n");
        printf("3. Edit contact\n");
        printf("---------- Menu ----------\n\n");

        printf("Select an option: ");
        scanf(" %d", &choice);

        if (choice == 1) {
            
            displayAllContacts(contactFile);

        } else if (choice == 2) {
            
            contactSearch(contactFile);
            
        } else {
            clearInputBuffer();
            printf("!!!! Error, please try again !!!\n\n");
        }
        
    } while (true);
    
    

    fclose(contactFile);

}

int contactSearch(FILE* contactFile) {
    char firstName[FIRST_NAME_MAX_LENGTH + 2] = ""; // + 2 to take into account the null terminator '\0' and '\n' for user input
    char lastName[LAST_NAME_MAX_LENGTH + 2] = "";
    char email[EMAIL_MAX_LENGTH + 2] = "";
    char phone[PHONE_MAX_LENGTH + 2] = "";

    printf("---- Contact search ----\n");
    printf("Press Enter when ready\n");
            
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

    ContactArray contactsFound = findContacts(contactFile, strcmp(firstName, "") == 0 ? NULL : firstName, strcmp(lastName, "") == 0 ? NULL : lastName, strcmp(email, "") == 0 ? NULL : email, strcmp(phone, "") == 0 ? NULL : phone);

    if(contactsFound.contactCounter > 0) {
        printf("\n\n%d contact%s found:\n", contactsFound.contactCounter, contactsFound.contactCounter > 1 ? "s" : "");
        printf("--------------------------\n");

        for (size_t i = 0; i < contactsFound.contactCounter; i++)
        {
            displayContact(contactsFound.contacts[i]);
        }
        
    } else {
        printf("\nNo contacts found.\n\n");
    }
}

ContactArray findContacts(FILE* contactFile, char* firstNameSearch, char* lastNameSearch, char* emailSearch, char* phoneSearch) {
    ContactArray contactsFound = {NULL, 0};
    
    if(firstNameSearch == NULL && lastNameSearch == NULL && emailSearch == NULL && phoneSearch == NULL) {
        return getAllContacts(contactFile);
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

        Contact* temp = realloc(contactsFound.contacts, (contactsFound.contactCounter + 1) * sizeof(Contact)); // increase the size of the array of Contact by 1 * sizeof(Contact)
        if(temp == NULL) {
            
            printf("Memory reallocation failed!\n");
            return (ContactArray){0};

        } else {

            contactsFound.contacts = temp;
            temp = NULL;

        }

        contactsFound.contacts[contactsFound.contactCounter] = currentContact;
        contactsFound.contactCounter++;
        
    }

    return contactsFound;

}

int addNewContact(FILE* contactFile) {
    
    Contact newContact = {0};

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

    strcpy(newContact.firstName, firstName);
    strcpy(newContact.lastName, lastName);
    strcpy(newContact.email, email);
    strcpy(newContact.phone, phone);
    strcpy(newContact.notes, notes);

    fprintf(contactFile, "\n%s,%s,%s,%s,%s",
        newContact.firstName,
        newContact.lastName,
        newContact.email,
        newContact.phone,
        newContact.notes
    );

    printf("\nNew contact:\n");
    displayContact(newContact);

    return 0;

}

int clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);

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

int displayAllContacts(FILE* contactFile) {

    ContactArray allContacts = getAllContacts(contactFile);

    printf("\n%d", allContacts.contactCounter);

    if(allContacts.contacts == NULL) {
        printf("Couldn't retrieve contacts\n");

        free(allContacts.contacts);
        allContacts.contacts = NULL;

        return 1;
    }
    
    for (int i = 0; i < allContacts.contactCounter; i++) {
        
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

ContactArray getAllContacts(FILE* contactFile) {

    char line[sizeof(Contact)] = {0};
    Contact currentContact;

    Contact* contacts = {0};
    int contactCounter = 0;
    
    while (fgets(line, sizeof(line), contactFile) != NULL) { // loops through each line of the file
        currentContact = (Contact){0};

        Contact* temp = realloc(contacts, (contactCounter + 1) * sizeof(Contact)); // increase the size of the array of Contact by 1 * sizeof(Contact)
        if(temp == NULL) {
            
            printf("Memory reallocation failed!\n");
            return (ContactArray){0};

        } else {

            contacts = temp;
            temp = NULL;

        }

        getContactFromLineInFile(&currentContact, line);

        contacts[contactCounter] = currentContact;
        contactCounter++;
        
    }

    return (ContactArray){contacts, contactCounter};
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


