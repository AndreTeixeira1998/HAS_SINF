#include "stdlib.h"
#include "stdio.h"
#include <string.h>

#include "DataStructure.h"

#define BUFFER 256 
#define MAX_DATASIZE 5 
#define PAYLOAD_SIZE 23 

#define START 0 
#define AM_MESSAGE 2
#define DESTINATION_ADDRESS 3
#define MOTE_ID 5
#define PAYLOAD_LENGHT 7
#define GROUP_ID 8
#define RAW_VOLTAGE 10
#define RAW_VISIBLE_LIGHT 12
#define RAW_CURRENT 14
#define RAW_TEMPERATURE 16
#define RAW_HUMIDITY 18
#define MESSAGE_HANDLING_INFO 20
#define END 22

char* filterString (char* str) {
    if (!str) {
        return NULL;
    }

    for (int i = 0; i < strlen(str); i++) {
        if (str[i] == '\r' || str[i] == '\n' || str[i] == '#') {
            str[i] = '\0';
            return str;
        }
    }

    return str;
}


int main(int argc, char const *argv[])
{
    Datastore* datastore = createDatastore();
    if (datastore == NULL)
        printf("Error\n");
    //printf("%x\n", datastore);

    Room* room1 = createRoom(datastore);
    if (room1 == NULL)
        printf("Error: room1\n");
    //printf("%x\n", room);

    Room* room2 = createRoom(room1->parentDatastore);
    if (room2 == NULL)
        printf("Error: room2\n");
    //printf("%x\n", room2);

    Node* node1 = createNode(room1);
    if (node1 == NULL){
        printf("Error: node1\n");
    }
    if (setNodeID(node1, 1)) {
        printf("Error setNodeID(node1)\n");
    }

    Node* node2 = createNode(room2);
    if (node2 == NULL){
        printf("Error: node2\n");
    }
    if (!setNodeID(node2, 1)) { // supposed to throw error
        printf("Error setNodeID(node2)\n");
    }

    if (findNodeByID(datastore, 1) != node1) {
        printf("Error: findNodeByID(datastore, 1) != node1\n");
    }

    if (setRoomName(room1, "Bedroom")) {
        printf("Error: setRoomName(room1, \"Bedroom\")\n");
    }

    if (findRoomByName(datastore, "Bedroom") != room1) {
        printf("Error: findRoomByName(datastore, \"Bedroom\") != room1\n");
    }

    if (deleteDatastore(datastore)) {
        printf("Error: deleteDatastore(datastore)\n");
    }
    
    FILE *fp;
    char filename[BUFFER + 1];
    char str[BUFFER +1];
    char *token; 
    char data[PAYLOAD_SIZE][MAX_DATASIZE];
    int data_type=0, k=0;

    puts("chateei-me com os channels por isso por agora mandem o path para a file");
    
    fgets( filename, BUFFER, stdin);
    //gets(filename);
    filterString (filename);
    
    printf("\n\n\nOpening #%s#\n\n", filename );
    
    fp=fopen(filename, "r");
    
    if(fp){
        
        printf("\n\n\nOpening #%s#\n\n", filename );
        
        while(fgets(str, BUFFER , fp)){
            printf("%s\n", str);
            token= strtok(str, " \n");
            while(token !=NULL){
                //printf("token:%s\n",token); // debugging
                strcpy(data[data_type], token);
                //printf("#%s# - %d\n", data[data_type], data_type); //debugging
                if(data_type==(START +1) || data_type==(DESTINATION_ADDRESS+1) || data_type==(MOTE_ID+1) || data_type==(GROUP_ID+1) || data_type==(RAW_VOLTAGE+1) || data_type==(RAW_VISIBLE_LIGHT+1) || data_type==(RAW_CURRENT+1) || data_type==(RAW_TEMPERATURE+1) || data_type==(RAW_HUMIDITY+1) || data_type==(MESSAGE_HANDLING_INFO+1)) {
                    strcat(data[data_type-1], data[data_type]);
                }
                //printf("#%s# - %d\n", data[data_type], data_type); //debugging

                data_type++;
                token= strtok(NULL, " \n");
                if(data_type==PAYLOAD_SIZE) data_type=0;
                //printf("#%s# - %d\n", data[data_type], data_type); //debugging

            }

            //printf("#%s# - %d\n", data[MOTE_ID], MOTE_ID); //debugging

            for (k=0; k<23; k++) printf("#%s# - %d\n", data[k], k); //debugging
            // aqui falta meter um switch para meter os valores
            printf("----------------------------------------------------------------------\n");
	    }
        fclose(fp);
    } else {
    puts("error opening file to read"); 
    
    }
    	
    printf("\n\nDone\n");
    
    return 0;
}
