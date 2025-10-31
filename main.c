#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct {
    
    int id;
    char firstName[50];
    char lastName[50];
    char email[50];
    char phone[10];
    int size_notes;
    char notes[1024];

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

    char line[1024] = {0};
    char* column;
    Contact currentContact = {0};
    int columnCounter = 0;

    int numberOfContacts = getNumberOfLines(contactFile);

    Contact* contacts = malloc(numberOfContacts * sizeof(Contact));
    int contactIndex = 0;

    char* notes;

    
    while (fgets(line, sizeof(line), contactFile) != NULL) { // loops through each line of the file
        column = strtok(line, ","); // explode each element of the line
        columnCounter = 0;
        
        // Loop through each column
        while (column != NULL && columnCounter <= 5) {
            
            switch (columnCounter) {
                case 0:
                    currentContact.id = atoi(column);
                    break;
                case 1:
                    strcpy(currentContact.firstName, column);
                    break;
                case 2:
                    strcpy(currentContact.lastName, column);
                    break;
                case 3:
                    strcpy(currentContact.email, column);
                    break;
                case 4:
                    strcpy(currentContact.phone, column);
                    break;
                case 5:
                    currentContact.size_notes = atoi(column);
                    break;
            }
            
            column = strtok(NULL, ",");
            columnCounter++;
        }

        // because there could be some potential ',' in the notes, we have to create a new variable to which we will concatenate
        // the strings that would be exploded by strtok within the notes by ',' characters
        // works only if notes is the last column of the database
        notes = calloc(currentContact.size_notes, sizeof(char));
        while (column != NULL) {
            
            strcat(notes, column);
            
            column = strtok(NULL, ",");
        }
        strcpy(currentContact.notes, notes);
        free(notes);
        notes = NULL;

        contacts[contactIndex] = currentContact;
        contactIndex++;
        
    }
    
    for (int i = 0; i < numberOfContacts; i++) {
        
        printf("ID: %d | First name: %s | Last name: %s | Email: %s | Phone: %s | Notes: %s",
            contacts[i].id,
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