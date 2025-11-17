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
    
    Contact* array;
    size_t contactCounter;

} ContactArray;


int clearInputBuffer();
int displayAllContacts(FILE* contactFile);
// int getNumberOfLines(FILE* file);
int addNewContact(FILE* contactFile);
int displayContact(Contact contact);
bool isAnEmailAddress(char* email);
bool isAFirstName(char* firstName);
bool isALastName(char* lastName);
bool isAPhoneNumber(char* phone);
int getContactFromLineInFile(Contact* contact, char* line);
ContactArray getAllContacts(FILE* contactFile);

int main() {

    FILE* contactFile = fopen("contacts.txt", "a+");

    if (contactFile == NULL) {
        printf("Could not open the file");
        return 1;
    }
    
    // addNewContact(contactFile);

    displayAllContacts(contactFile);

    fclose(contactFile);

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

    if(allContacts.array == NULL) {
        printf("Couldn't retrieve contacts\n");

        free(allContacts.array);
        allContacts.array = NULL;

        return 1;
    }
    
    for (int i = 0; i < allContacts.contactCounter; i++) {
        
        printf("First name: %s | Last name: %s | Email: %s | Phone: %s | Notes: %s",
            allContacts.array[i].firstName,
            allContacts.array[i].lastName,
            allContacts.array[i].email,
            allContacts.array[i].phone,
            allContacts.array[i].notes
        );
        printf("\n");
    }

    free(allContacts.array);
    allContacts.array = NULL;
    

    return 0;
}

ContactArray getAllContacts(FILE* contactFile) {

    char line[sizeof(Contact)] = {0};
    char* column;
    Contact currentContact;
    

    Contact* array = {0};
    int contactCounter = 0;

    
    while (fgets(line, sizeof(line), contactFile) != NULL) { // loops through each line of the file
        currentContact = (Contact){0};

        Contact* temp = realloc(array, (contactCounter + 1) * sizeof(Contact)); // increase the size of the array of Contact by 1 * sizeof(Contact)
        if(temp == NULL) {
            
            printf("Memory reallocation failed!\n");
            return (ContactArray){0};

        } else {

            array = temp;
            temp = NULL;

        }

        getContactFromLineInFile(&currentContact, line);

        array[contactCounter] = currentContact;
        contactCounter++;
        
    }

    ContactArray contactArray = {
        array,
        contactCounter
    };

    return contactArray;
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


// int getNumberOfLines(FILE* file) {
//     int count = 0;
//     char c;

//     for (c = getc(file); c != EOF; c = getc(file))
//         if (c == '\n') // Increment count if this character is newline
//             count = count + 1;
    
//     fseek(file, 0L, SEEK_SET);

//     return count + 1; //last line doesn't have a \n
// }
