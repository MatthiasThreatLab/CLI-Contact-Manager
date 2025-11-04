#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>

#define FIRST_NAME_MAX_LENGTH 50
#define LAST_NAME_MAX_LENGTH 50
#define EMAIL_MAX_LENGTH 75
#define PHONE_MAX_LENGTH 10
#define NOTES_MAX_LENGTH 1023

typedef struct {
    
    char firstName[FIRST_NAME_MAX_LENGTH + 1]; // + 1 to take into account the null terminator '\0'
    char lastName[LAST_NAME_MAX_LENGTH + 1];
    char email[EMAIL_MAX_LENGTH + 1];
    char phone[PHONE_MAX_LENGTH + 1];
    char notes[NOTES_MAX_LENGTH + 1];

} Contact;


int displayAllContacts(FILE* contactFile);
int getNumberOfLines(FILE* file);
int addNewContact(FILE* contactFile);
int displayContact(Contact contact);
bool isAnEmailAddress(char* email);

int main() {

    FILE* contactFile = fopen("contacts.txt", "a+");

    if (contactFile == NULL) {
        printf("Could not open the file");
        return 1;
    }
    
    addNewContact(contactFile);

    // displayAllContacts(contactFile);

    fclose(contactFile);

}

int addNewContact(FILE* contactFile) {
    /* TO DO:
        - regex to valid user input for each element of newContact
    */

    int len;
    bool pass;
    
    Contact newContact = {0};

    char firstName[FIRST_NAME_MAX_LENGTH + 2] = ""; // + 2 to take into account the null terminator '\0' and '\n' for user input
    char lastName[LAST_NAME_MAX_LENGTH + 2] = "";
    char email[EMAIL_MAX_LENGTH + 2] = "";
    char phone[PHONE_MAX_LENGTH + 2] = "";
    char notes[NOTES_MAX_LENGTH + 2] = "";

    int c;
    while ((c = getchar()) != '\n' && c != EOF); // clear input buffer

    pass = false;
    while (!pass) {
        printf("New contact's first name (%d char max): ", FIRST_NAME_MAX_LENGTH);
        fgets(firstName, sizeof(firstName), stdin); // need fgets to accept white spaces.
        firstName[strlen(firstName) - 1] = '\0'; // removes \n that fgets adds at the end

        pass = true;
        
        len = strlen(firstName);
        if (len <= 0 || len >= FIRST_NAME_MAX_LENGTH) {
            printf("\n!!! Error: first name must be 1-%d character long !!!\n\n", FIRST_NAME_MAX_LENGTH);
            pass = false;

            while ((c = getchar()) != '\n' && c != EOF);
        }

        for (int i = 0; i < len; i++)
        {
            if (!(isalpha(firstName[i]) || firstName[i] == '-' || firstName[i] == ' ')) {
                printf("\n!!! Error: Only alpha characters, hyphen and spaces accepted !!!\n\n");
                pass = false;
                
                while ((c = getchar()) != '\n' && c != EOF);

                break;
            }
        }
        
    }

    pass = false;
    while (!pass) {
        printf("New contact's last name (%d char max): ", LAST_NAME_MAX_LENGTH);
        fgets(lastName, sizeof(lastName), stdin); // need fgets to accept white spaces.
        lastName[strlen(lastName) - 1] = '\0'; // removes \n that fgets adds at the end

        pass = true;
        
        len = strlen(lastName);
        if (len <= 0 || len >= LAST_NAME_MAX_LENGTH) {
            printf("\n!!! Error: last name must be 1-%d character long !!!\n\n", LAST_NAME_MAX_LENGTH);
            pass = false;

            while ((c = getchar()) != '\n' && c != EOF);
        }

        for (int i = 0; i < len; i++)
        {
            if (!(isalpha(lastName[i]) || lastName[i] == '-' || lastName[i] == ' ')) {
                printf("\n!!! Error: Only alpha characters, hyphen and spaces accepted !!!\n\n");
                pass = false;
                
                while ((c = getchar()) != '\n' && c != EOF);

                break;
            }
        }
        
    }

    do {
        printf("New contact's email address (%d char max): ", EMAIL_MAX_LENGTH);
        fgets(email, sizeof(email), stdin); // need fgets to accept white spaces.
        email[strlen(email) - 1] = '\0'; // removes \n that fgets adds at the end
        
    } while (!isAnEmailAddress(email));
    

    printf("New contact's phone number (%d char max): ", PHONE_MAX_LENGTH);
    fgets(phone, sizeof(phone), stdin); // need fgets to accept white spaces.
    phone[strlen(phone) - 1] = '\0'; // removes \n that fgets adds at the end

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

int displayContact(Contact contact) {

    printf("First name: %s | Last name: %s | Email: %s | Phone: %s | Notes: %s",
        contact.firstName,
        contact.lastName,
        contact.email,
        contact.phone,
        contact.notes
    );
    printf("\n");

}

int displayAllContacts(FILE* contactFile) {

    char line[sizeof(Contact)] = {0};
    char* column;
    Contact currentContact;
    int columnCounter = 0;
    char c;

    int numberOfContacts = getNumberOfLines(contactFile);

    Contact* contacts = malloc(numberOfContacts * sizeof(Contact));
    int contactIndex = 0;

    char* notes;

    
    while (fgets(line, sizeof(line), contactFile) != NULL) { // loops through each line of the file
        currentContact = (Contact){0};
        columnCounter = 0;

        int i = 0;
        while (line[i] != '\0') { // Loop through each character of each line until the null terminator is found
            c = line[i];

            // if we're not at the notes yet and we find the ',' character, it's a delimiter
            // so we skip it (i++) and change the column
            if (columnCounter < 4 && c == ',') {
                columnCounter++;
                i++;
                continue;
            }

            // add the current character to the right variable of currentContact
            switch (columnCounter) {
                case 0:
                    strncat(currentContact.firstName, &c, 1);
                    break;
                case 1:
                    strncat(currentContact.lastName, &c, 1);
                    break;
                case 2:
                    strncat(currentContact.email, &c, 1);
                    break;
                case 3:
                    strncat(currentContact.phone, &c, 1);
                    break;
                case 4:
                    strncat(currentContact.notes, &c, 1);
                    break;
            }
            
            i++; // next character
        }

        contacts[contactIndex] = currentContact;
        contactIndex++;
        
    }
    
    for (int i = 0; i < numberOfContacts; i++) {
        
        printf("First name: %s | Last name: %s | Email: %s | Phone: %s | Notes: %s",
            contacts[i].firstName,
            contacts[i].lastName,
            contacts[i].email,
            contacts[i].phone,
            contacts[i].notes
        );
        printf("\n");
    }

    free(contacts);
    contacts = NULL;
    

    return 0;
}

int getNumberOfLines(FILE* file) {
    int count = 0;
    char c;

    for (c = getc(file); c != EOF; c = getc(file))
        if (c == '\n') // Increment count if this character is newline
            count = count + 1;
    
    fseek(file, 0L, SEEK_SET);

    return count + 1; //last line doesn't have a \n
}

bool isAnEmailAddress(char* email) {

    if(strlen(email) < 6 || strlen(email) > EMAIL_MAX_LENGTH) { // at least 1 username char, '@', 1 domain char, '.', 2 top-level domain chars
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
        return false;
    }

    // check username:  Only alphanumeric characters, periods, underscores, percent signs, plus signs, and hyphens allowed.
    //                  One or more of these characters must be present.
    
    // pointer difference (atDomainPeriodTopLevelDomain points to the position of @, email points at the start of the string)
    // if this is 0, it means
    if(atDomainPeriodTopLevelDomain - email > 0) {
        for (int i = 0; i < atDomainPeriodTopLevelDomain - email; i++) {
            if (!(isalnum(email[i]) || email[i] == '.' || email[i] == '_' || email[i] == '%' || email[i] == '+' || email[i] == '-')) {
                return false;
            }
        }
    } else {
        return false;
    }

    periodTopLevelDomain = strstr(atDomainPeriodTopLevelDomain, periodSymbol);

    if (periodTopLevelDomain == NULL) {
        return false;
    }

    // check domain:    Only alphanumeric characters, periods, and hyphens allowed.
    //                  One or more of these characters must be present.

    if(periodTopLevelDomain - atDomainPeriodTopLevelDomain > 0) {
        for (int i = 1; i < periodTopLevelDomain - atDomainPeriodTopLevelDomain; i++) { // starts at 1 to skip the @ symbol
            
            if (!(isalnum(atDomainPeriodTopLevelDomain[i]) || atDomainPeriodTopLevelDomain[i] == '.' || atDomainPeriodTopLevelDomain[i] == '-')) {
                return false;
            }
        }
    } else {
        return false;
    }

    // check top-level domain (TLD):    Only alpha characters and periods allowed.
    //                                  2 or more of these characters must be present.

    if(strlen(periodTopLevelDomain) >= 3) { // (period + at least 2 characters for the TLD)
        for (int i = 1; i < strlen(periodTopLevelDomain); i++) { // starts at 1 to skip the . symbol
            
            if (!(isalpha(periodTopLevelDomain[i]) || periodTopLevelDomain[i] == '.')) {
                return false;
            }
        }
    } else {
        return false;
    }

    return true;

}

