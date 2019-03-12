#include "stdlib.h"
#include "stdio.h"

#include "DataStructure.h"
#include "functions.h"

int main(int argc, char const *argv[])
{
    Datastore* datastore = importConfiguration("GASconfig.json");
    if (!datastore) {
        printf("Error in config file.\n");
        return 1;
    }
    
    for (list_element* elem = listStart(datastore->rooms); elem != NULL; elem = elem->next) {
        Room* room = elem->ptr;
        printf("%d,%s\n", room->id, room->name);

        // Print node data
        for (list_element* elem2 = listStart(room->nodes); elem2 != NULL; elem2 = elem2->next) {
            Node* node = elem2->ptr;
            printf("  %d\n", node->id);

            for (list_element* elem3 = listStart(node->sensors); elem3 != NULL; elem3 = elem3->next) {
                Sensor* sensor = elem3->ptr;
                printf("    s_t: %d\n", sensor->type);
            }

            for (list_element* elem3 = listStart(node->actuators); elem3 != NULL; elem3 = elem3->next) {
                Actuator* actuator = elem3->ptr;
                Position* pos = getActuatorPosition(actuator);
                printf("    a_x: %d | a_y: %d\n", pos->x, pos->y);
            }
        }
    }

    printf("Done\n");
    
    return 0;
}
