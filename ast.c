#include "ast.h"
#include <stdlib.h>

struct ast_node* ast_create_node()
{
    struct ast_node* node = (struct ast_node*) malloc(sizeof(struct ast_node));
    node->left = NULL;
    node->right = NULL;

    return node;
}

struct ast_list* ast_create_list()
{
    struct ast_list* list = (struct ast_list*) malloc(sizeof(struct ast_list));
    list->next = NULL;

    return list;
}

bool ast_list_insert(struct ast_list* l, struct ast_node* n)
{
    //TODO:: insert!!!!
    return true;
}
