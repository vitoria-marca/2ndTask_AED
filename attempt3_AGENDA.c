
/*  Agenda pBuffer
    Vitória M S Lucia - 23200577

    Algoritmos e Estrutura de Dados I
    Ciência da Computação - UFPEL
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NAME_SIZE 50

/*
==============================
Function Description
==============================

AddPerson
    The user can entry a person's name, age and e-mail to be added to the notebook 
    
RemovePerson
    A person name is given and is removed from the notebook
    
SearchPerson
    A person name is given and its information's are shown
    
List People
    All people's information in the notebook is shown

Next Record
    It's used in other functions and return a pointer that points the next register into the notebook
    
*/

/*
    Important pointers

    int *peopleCounter = (int *)*pBuffer;
    int *menu = pBuffer + 1;
    size_t *pBufferSize = (size_t *)((char *)pBuffer + 3);
    char *buffer = (char *)(*pBuffer + 4 * sizeof(int)); 
    int *counter = (int *)(*pBuffer + 2 * sizeof(int));
*/

void AddPerson( void **pBuffer, size_t *pBufferSize);
void RemovePerson( void **pBuffer, size_t *pBufferSize);
void SearchPerson( void **pBuffer );
void ListPeople( void *pBuffer);
void *NextRecord(void *current);

int main() {
    void *pBuffer = malloc( 4 * sizeof(int) + NAME_SIZE);
    //counter + menu + char counter + current pbuffer size + temporary entry 
    if ( !pBuffer ) {
        printf("Error allocating memory.\n");
        return 1;
    }
    *(int *)pBuffer = 0;

    size_t *pBufferSize = (size_t *)((char *)pBuffer + 3 * sizeof(int));
    //byte type pointer
    *pBufferSize = NAME_SIZE + 4 * sizeof(int);
    
    int *menu = pBuffer + sizeof(int);
    if ( !menu ){
        printf("Error allocating memory.\n");
    }

    do {

        printf("\n----- Menu -----\n");
        printf("1 - Add Person\n");
        printf("2 - Remove Person\n");
        printf("3 - Search Person\n");
        printf("4 - List people\n");
        printf("5 - Exit\n");
        printf("Choose an option: ");
        scanf("%d", menu);
        getchar();
        printf("---------------\n");

        switch ( *menu ) {
            case 1:
                AddPerson(&pBuffer, pBufferSize);
                break;
            case 2:
                RemovePerson(&pBuffer, pBufferSize);
                break;
            case 3:
                SearchPerson(&pBuffer);
                break;
            case 4:
                ListPeople(pBuffer);
                break;
            case 5:
                break;
            default:
                printf("Invalid entry.\n");
        }
    } while ( *menu != 5 );

    free(pBuffer);
    return 0;
}

void AddPerson(void **pBuffer, size_t *pBufferSize) {
    int *peopleCounter = (int *)*pBuffer;
    char *buffer = (char *)(*pBuffer + 4 * sizeof(int)); 
    int *counter = (int *)(*pBuffer + 2 * sizeof(int));

    printf("Entry a name: ");
    fgets(buffer, NAME_SIZE, stdin);
    *counter = strlen(buffer);
    if (buffer[*counter - 1] == '\n') { // '\n' changes to '\0'
        buffer[*counter - 1] = '\0';
        (*counter)--;
    }
    *pBufferSize += *counter + 1; //new pbuffersize = last pbuffersize + name's entry
    void *temp = realloc(*pBuffer, *pBufferSize);
    if (temp == NULL) {
        printf("Error reallocating memory.\n");
        return;
    }
    *pBuffer = temp;
    void *pos = (char *)*pBuffer + *pBufferSize - *counter - 1; //pos points to empty memory block
    strcpy(pos, buffer); //puts the entry at the temporary buffer in the empty memory block 
    pos += *counter + 1; //now, pos points to the end of pBuffer where the e-mail's entry will be added

    printf("Entry an e-mail: ");
    fgets(buffer, NAME_SIZE, stdin);
    *counter = strlen(buffer);
    if (buffer[*counter - 1] == '\n') {
        buffer[*counter - 1] = '\0';
        (*counter)--;
    }

    *pBufferSize += *counter + 1; //new pbuffersize = last pbuffersize + e-mail's entry
    temp = realloc(*pBuffer, *pBufferSize);
    if (temp == NULL) {
        printf("Error reallocating memory.\n");
        return;
    }
    *pBuffer = temp;
    pos = (char *)(*pBuffer) + *pBufferSize - *counter - 1; //making sure that pos points to the beginning of empty memory
    strcpy(pos, buffer);
    pos += *counter + 1; 

    printf("Entry an age: ");
    *pBufferSize += sizeof(int);
    temp = realloc(*pBuffer, *pBufferSize);
    if (temp == NULL) {
        printf("Error reallocating memory.\n");
        return;
    }
    *pBuffer = temp;
    pos = (char *)*pBuffer + *pBufferSize - sizeof(int);
    scanf("%d", (int *)pos);
    getchar();
    peopleCounter = (int *)*pBuffer;
    (*peopleCounter)++;
}

void RemovePerson(void **pBuffer, size_t *pBufferSize) {
    int *peopleCounter = (int *)*pBuffer;
    if (*peopleCounter == 0) {
        printf("There's no registers to remove.\n");
        return;
    }
    char *targetName = (char *)((char *)*pBuffer +  4 * sizeof(int));
    if (*peopleCounter == 0){
        printf("There's no one registred.");
        return;
    }
    printf("Entry a name (the same way as registred):: ");
    scanf("%49s", (char *)targetName);
    char *current = (char *)*pBuffer + (4 * sizeof(int) + NAME_SIZE); //points to the 1st register now
    void *next = NULL;
    
    for (int i = 0; i < *peopleCounter; i++) { // loop to cycle through memory
        next = NextRecord(current); //now, next is pointing to current's next register 

        if (strcmp(current, targetName) == 0) { //if we find the name
            size_t recordSize = (char *)next - (char *)current; //how many bytes will be removed
            memmove(current, next, *pBufferSize - ((char *)next - (char *)(*pBuffer))); //moves  x bytes after next to current,
            //x is total memory - memory until next = remaining memory
            *pBufferSize -= recordSize;
            void *temp = realloc(*pBuffer, *pBufferSize);
            if (temp == NULL) {
                printf("Error reallocating memory.\n");
                return;
            }
            *pBuffer = temp;
            // making sure the pointers are in the right address
            peopleCounter = (int *)*pBuffer;
            (*peopleCounter)--;
            printf("Register removed successfully.\n");
            return;
        }
        current = next; //if ist'n the name, goes to next register
    }
    printf("\n");
    printf("There's no %s to be removed.",targetName);
    return;
}

void SearchPerson(void **pBuffer){
    //works the same way as Remove Person, but just show up the infos
    //about the register the function is looking for
    int *peopleCounter = (int *)*pBuffer;
    void *current = (char *)*pBuffer + (4 * sizeof(int)) + NAME_SIZE;
    char *targetName = (char *)((char *)*pBuffer + ( 4 * sizeof(int)));

    if (*peopleCounter == 0){
        printf("There's no one registred.");
        return;
    }

    printf("Entry a name (the same way as registred):: ");
    scanf("%49s", (char *)targetName);

    for (int i = 0; i < (*peopleCounter + 1 ); i++) {
        if (strcmp(current, (char *)targetName) == 0){
            printf("\n");
            printf("Person found! \n");
            char *name = (char *)current;
            char *email = name + strlen(name) + 1;
            int *age = (int *)(email + strlen(email) + 1);
            printf("Register number:: %d\n", i + 1);
            printf("Name: %s \nEmail: %s\nAge: %d\n", name, email, *age);
            printf("--------------------\n");
            return;
        }
        current = NextRecord(current);
    }
    printf("No one called as %s in the notebook.", (char *)targetName);
}

void *NextRecord(void *current) {
    if (!current) return NULL;

    char *name = (char *)current;
    char *email = name + strlen(name) + 1;  // name + '\0'
    int *age = (int *)(email + strlen(email) + 1); // e-mail + '\0'

    return (void *)(age + 1); // next register begins after age + sizeof(int)
}

void ListPeople(void *pBuffer) {
    int *peopleCounter = (int *)pBuffer; 
    void *current = (char *)pBuffer + (4 * sizeof(int)) + NAME_SIZE;

    if (*peopleCounter == 0){
        printf("There's no records to be shown");
        return;
    }

    printf("\nAll registers:\n");

    for (int i = 0; i < (*peopleCounter); i++) { 
        char *name = (char *)current;
        char *email = name + strlen(name) + 1;
        int *age = (int *)(email + strlen(email) + 1);

        printf("Register number:: %d\n", i + 1);
        printf("Name: %s \nEmail: %s\nAge: %d\n", name, email, *age);
        printf("--------------------\n");

        current = NextRecord(current);
    }
}
