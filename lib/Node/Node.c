#include "Node.h"

Node* createNode (Room* room, uint16_t id) {
    if (!room) {
        return NULL;
    }

    Datastore* datastore = room->parentDatastore;
    if (findNodeByID(datastore, id)) {
        return NULL;
    }

    Node* node = (Node*)malloc(sizeof(Node));
    if (node == NULL) {
        // Memory allocation failed
        return NULL;
    }

    list_element* elem = listInsert(room->nodes, node, NULL);
    if (elem == NULL) {
        // Insertion failed
        free(node);
        return NULL;
    }

    list* sensors = newList();
    if (sensors == NULL) {
        free(node);
        listRemove(room->nodes, elem);
        return NULL;
    }

    list* actuators = newList();
    if (actuators == NULL) {
        free(node);
        deleteList(sensors);
        listRemove(room->nodes, elem);
        return NULL;
    }

    node->id = id;
    node->parentRoom = room;
    node->listPtr = elem;
    node->sensors = sensors;
    node->actuators = actuators;

    return node;
}

bool deleteNode (Node* node) {
    if (node == NULL) {
        return 1;
    }

    list_element* elem = node->listPtr;
    Room* room = node->parentRoom;

    list_element* aux;


    // Delete all node's sensors
    aux = listStart(node->sensors);
    while (aux != NULL) {
        if (deleteSensor(aux->ptr)) {
            // Error
            return 1;
        }
        aux = listStart(node->sensors);
    }
    deleteList(node->sensors);

    // Delete all node's actuators
    aux = listStart(node->actuators);
    while (aux != NULL) {
        if (deleteActuator(aux->ptr)) {
            // Error
            return 1;
        }
        aux = listStart(node->actuators);
    }
    deleteList(node->actuators);

    free(node);
    list_element* res = listRemove(room->nodes, elem);
    if (res == NULL && listSize(room->nodes)) {
        return 1;
    }

    return 0;
}

bool setNodeID (Node* node, uint16_t id) {
    if (node == NULL) {
        return 1;
    }

    if (findNodeByID(node->parentRoom->parentDatastore, id)) {
        return 1;
    }

    node->id = id;

    return 0;
}

Node* findNodeByID (Datastore* datastore, uint16_t nodeID) {
    if (!datastore) {
        return NULL;
    }

    for (list_element* room_elem = listStart(datastore->rooms); room_elem != NULL; room_elem = room_elem->next) {
        Room* room = (Room*)room_elem->ptr;
        for (list_element* node_elem = listStart(room->nodes); node_elem != NULL; node_elem = node_elem->next) {
            Node* node = (Node*)node_elem->ptr;
            if (node->id == nodeID) {
                return node;
            }
        }
    }

    return NULL;
}

/**********************************/
/*        DATABASE QUERIES        */
/**********************************/

DBQuery create_table_node = {
    NULL,
    "create_table_node",
    "CREATE TABLE IF NOT EXISTS sinf.node("
    "node_id SERIAL NOT NULL PRIMARY KEY);",
    0
};

DBQuery create_node = {
    NULL,
    "create_node",
    "INSERT INTO sinf.node "
    "DEFAULT VALUES;",
    0
};

DBQuery delete_node = {
    NULL,
    "delete_node",
    "DELETE FROM sinf.node WHERE node_id=$1;",
    1
};

void preparePriorityNodeQueries (list* queryList) {
    addQuerytoList(&create_table_node, queryList);
}

void prepareNodeQueries (list* queryList) {
    addQuerytoList(&create_node, queryList);
    addQuerytoList(&delete_node, queryList);
}
