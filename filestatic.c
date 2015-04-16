/**
 *  \file   mobilerobotsmovement.c
 *  \brief  Mobility from file
 *  \author tahiry razafindralambo
 *  \date   2007
 **/

#include <include/modelutils.h>
#define USE_ACTUAL_POSITION
//#define DEBUG_POSITION

/* ************************************************** */
/* ************************************************** */
model_t model =  {
    "Node position from a static file: topo.conf",
    "Tahiry Razafindralambo",
    "0.1",
    MODELTYPE_MOBILITY, 
    {NULL, 0}
};


/* ************************************************** */
/* ************************************************** */
struct data_from_file {
};

struct nodedata {
  int test;
};

struct entitydata {
    FILE *file;
};
/* ************************************************** */
/* ************************************************** */
int init(call_t *c, void *params) {
    struct entitydata *entitydata = malloc(sizeof(struct entitydata));
    param_t *param;
    char *filepath = NULL;
    /* default values */
    filepath = "topo.conf";
    /* get parameters */
    
    das_init_traverse(params);
    while ((param = (param_t *) das_traverse(params)) != NULL) {
        if (!strcmp(param->key, "file")) {
            filepath = param->value;
        }
    }
    
    /* open file */
    if ((entitydata->file = fopen(filepath, "r")) == NULL) {
        fprintf(stderr, "filestatic: can not open file %s in init()\n", filepath);
        goto error;
    }
    set_entity_private_data(c, entitydata);
    return 0;

 error:
    free(entitydata);
    return -1;
}

int destroy(call_t *c) {
    struct entitydata *entitydata = get_entity_private_data(c);
    if (entitydata->file != NULL) {
        fclose(entitydata->file);
    }

    free(entitydata);
    return 0;
}


/* ************************************************** */
/* ************************************************** */
int setnode(call_t *c, void *params) {
    struct nodedata *nodedata = malloc(sizeof(struct nodedata));
    struct entitydata *entitydata = get_entity_private_data(c);
    double x, y, z;
    int id, found = 0;
    char str[128];
      
    /* position at the beginning */
    fseek(entitydata->file, 0L, SEEK_SET);
    while (fgets(str, 128, entitydata->file) != NULL) {
        sscanf(str, "%d %lf %lf %lf\n",  &id, &x, &y, &z);
        if (id == c->node) {
            found = 1;
            get_node_position(c->node)->x = x;
            get_node_position(c->node)->y = y;
            get_node_position(c->node)->z = z;
            break; 
        }
    }
    
    if (found == 0) {
        fprintf(stderr, "filestatic: node %d position not found (setnode())\n", c->node);
        return -1;
    }
    
    set_node_private_data(c, nodedata);
    return 0;
}

int unsetnode(call_t *c) {
    free(get_node_private_data(c));
    return 0;
}


/* ************************************************** */
/* ************************************************** */
int bootstrap(call_t *c) {
    return 0;
}

int ioctl(call_t *c, int option, void *in, void **out) {
    return 0;
}

/* ************************************************** */
/* ************************************************** */
void update_position(call_t *c) {
    return;
}


/* ************************************************** */
/* ************************************************** */
mobility_methods_t methods = {update_position};
