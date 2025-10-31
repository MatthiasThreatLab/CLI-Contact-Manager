#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define FIRST_NAME_MAX_LENGTH 50
#define LAST_NAME_MAX_LENGTH 50
#define EMAIL_MAX_LENGTH 50
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

int main() {

    FILE* contactFile = fopen("contacts.txt", "r");

    if (contactFile == NULL) {
        printf("Could not open the file");
        return 1;
    }
    
    displayAllContacts(contactFile);

    fclose(contactFile);

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