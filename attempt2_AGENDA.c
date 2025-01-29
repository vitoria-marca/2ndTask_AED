
/*  Trabalho Agenda pBuffer
    Vitória M S Lucia - 23200577

    Algoritmos e Estrutura de Dados I
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NAME_SIZE 50
#define EMAIL_SIZE 50

/*
==============================
Function Descriptions
==============================

AddPerson
    The user can entry a person's name, age and e-mail to be added to the notebook 
    
RemovePerson
    A person name is given and is removed from the notebook
    
SearchPerson
    A person name is given and its information's are shown
    
List People
    All people's information in the notebook is shown
    
*/

void AddPerson( void **pBuffer );
void RemovePerson( void **pBuffer );
void SearchPerson( void **pBuffer );
void ListPeople( void *pBuffer );

int main() {
    void *pBuffer = malloc(sizeof(int));
    if ( !pBuffer ) {
        printf("Error allocating memory.\n");
        return 1;
    }
    *(int *)pBuffer = 0;
    //converting void* to int to manipulate the first 4 bytes as an integer number
    int *menu = malloc(sizeof(int));
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
                AddPerson(&pBuffer);
                break;
            case 2:
                RemovePerson(&pBuffer);
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
    free(menu);
    return 0;
}

void AddPerson( void **pBuffer ) {
    //int *counter = (int *)*pBuffer;
    /*if (!*counter){
        printf("da f4");
        exit(1);
    }*/

    *pBuffer = realloc(*pBuffer, (sizeof(int)) + (*(int *)*pBuffer + 1) * (sizeof(int) + NAME_SIZE + EMAIL_SIZE));
     //qtd * data + data that will be added
    if ( *pBuffer == NULL ) {
        printf("Error allocating memory.\n");
        exit(1);
    }

    void *pos = (char *)(*pBuffer) + (sizeof(int)) + (*(int *)*pBuffer) * (sizeof(int) + NAME_SIZE + EMAIL_SIZE);
    //jump the qnt + qnt * data to put new data in the end 

    printf("Entry a name: ");
    fgets((char *)pos + sizeof(int), NAME_SIZE, stdin);
    ((char *)pos + sizeof(int))[strcspn(pos + sizeof(int), "\n")] = '\0';
    //name is insert after the age's designed space    
    
    printf("Entry the age: ");
    scanf("%d", (int *)pos);
    //first data to be insert in the new allocated space
    getchar();

    printf("Entry a e-mail: ");
    fgets((char*)(pos + sizeof(int) + NAME_SIZE), EMAIL_SIZE, stdin);
    ((char *)(pos + sizeof(int) + NAME_SIZE))[strcspn((char*)(pos + sizeof(int) + NAME_SIZE), "\n")] = '\0';
    //e-mail's insert after the space of the age and name.

    (*(int *)*pBuffer)++;
    //increase in 1 the qtd of people in the notebook
}

void RemovePerson( void **pBuffer ) {
    int *counter = (int*)(*pBuffer);
    //qtd of people already registred
    void *pos = *pBuffer + sizeof(int);
    //first entry in the notebook

    if ( *counter == 0 ){
        printf("Anyone registred yet.\n");
        return;
    }

    //Allocating memory to the target name
    *pBuffer = realloc(*pBuffer, sizeof(int) + (*counter) * (sizeof(int) + NAME_SIZE + EMAIL_SIZE) + NAME_SIZE);
    //The target name will be insert after this space of memory
    void *targetName = *pBuffer + sizeof(int) + (*counter) * (sizeof(int) + NAME_SIZE + EMAIL_SIZE);

    printf("Entry the name to be removed from the notebook");
    scanf("%49s", (char *)targetName);

    for ( int i = 0 ; i < *counter ; i++ ){
        char *currentName = (char*)(pos + sizeof(int));
        //this name will be compared to the target one

        if ( strcmp(currentName, (char *)targetName) == 0 ){ //if equal
            void *next = pos + (sizeof(int) + NAME_SIZE + EMAIL_SIZE);
            //next is the people registred after the one who will be removed
            size_t remainingSize = (* counter - i - 1) * (sizeof(int) + NAME_SIZE + EMAIL_SIZE);
            //how many bytes of the remaining data will be moved
            memmove(pos, next, remainingSize);
            //move next to pos position by moving the bytes of remaining size 
            (*counter)--;
            *pBuffer = realloc(*pBuffer, sizeof(int) + (*counter) * (sizeof(int) + NAME_SIZE + EMAIL_SIZE));
            //realloc the notebook considering the deletion of 1 record
            if ( !*pBuffer ){
                printf("Error allocating memory. \n");
                exit(1);
            }

            printf("------------------\n");
            printf("Person removed successfully\n");
            return;
        }
        pos += sizeof(int) + NAME_SIZE + EMAIL_SIZE;
        //avança para o próximo registro caso o currentName e target nao sejam iguais
    }
    printf("Person not found.\n");
}

void SearchPerson( void **pBuffer ) {
    void *pos = *pBuffer + sizeof(int); // first entry
    int *counter = (int *)*pBuffer; //qtd of records

    if ( *counter == 0 ){
        printf("There's no people registred.\n");
        return;
    }

    //Allocating memory to the target name
    *pBuffer = realloc(*pBuffer, sizeof(int) + (*counter) * (sizeof(int) + NAME_SIZE + EMAIL_SIZE) + NAME_SIZE);
    //The target name will be insert after this space of memory
    void *targetName = *pBuffer + sizeof(int) + (*counter) * (sizeof(int) + NAME_SIZE + EMAIL_SIZE);

    printf("Entry the name to be searched in the notebook ");
    scanf("%49s", (char *)targetName);
    printf("\n");
    for ( int i = 0 ; i < *counter ; i++ ){
        char *currentName = (char *)(pos + sizeof(int));
        if ( strcmp(currentName, (char *)targetName) == 0 ){
            printf("Person found! \n");
            printf("\n");
            printf("Name: %s\n", currentName);
            printf("Age: %d\n", *(int *)pos);
            printf("E-mail: %s\n", (char *)(pos + sizeof(int) + NAME_SIZE));
            return;

            *pBuffer = realloc(*pBuffer, sizeof(int) + (*counter) * (sizeof(int) + NAME_SIZE + EMAIL_SIZE));
            if ( !*pBuffer ){
                printf("Error allocating memory.\n");
                exit(1);
            }

        }
        pos += sizeof(int) + NAME_SIZE + EMAIL_SIZE;
        //go to next person-
    }
    printf("Person not found");

}

void ListPeople( void *pBuffer ) {
    //This function is alike the search one.
    int *counter = (int *)pBuffer;
    void *pos = pBuffer + sizeof(int);
    
    if ( *counter == 0 ) {
        printf("There is no records.\n");
        return;
    }

    printf("List of registred people:::\n");
    printf("\n");
    for ( int i = 0 ; i < *counter ; i++ ) {
        printf("Reg. number %d\n", i+1);
        printf("Name: %s\n", (char *)(pos + sizeof(int)));
        printf("Age: %d\n", *(int *)pos);
        printf("E-mail: %s\n", (char *)(pos + sizeof(int) + NAME_SIZE));
        pos += sizeof(int) + NAME_SIZE + EMAIL_SIZE;
        printf("\n");
    }
}
