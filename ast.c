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
    list->elem = NULL;

    return list;
}

bool ast_list_insert(struct ast_list* l, struct ast_node* n) {
    if (l->elem == NULL) {
        l->elem = n;
    } else {
        while(l->next != NULL) {
            l = l->next;
        }
        l->next = ast_create_list();
        l->next->elem = n;
    }

    return true;
}
