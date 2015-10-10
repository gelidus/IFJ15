#include ast.h

#define PRINT 1

struct ast_node* ast_create_node()
{
    struct ast_node* n = (struct ast_node*) malloc(sizeof(struct ast_node));

    if ( PRINT ) printf("ast:ast_create_node:%p\n", (void*)n);
    n->left =  NULL;
    n->right = NULL;

    return n;
}
