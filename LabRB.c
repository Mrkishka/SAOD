#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#include <locale.h>
#include <sys/time.h>

#define RED 'R'
#define BLACK 'B'

/*структура дерева*/
struct rbtree
{
    struct rbtree *left;
    struct rbtree *right;
    struct rbtree *parent;
    int data;
    char color;
};

/*создание узла*/
struct rbtree *rbtree_create_node(int data)
{
    struct rbtree *node = (struct rbtree *)malloc(sizeof(struct rbtree));
    node->data = data;
    node->color = RED;
    node->left = NULL;
    node->right = NULL;
    node->parent = NULL;
    return node;
}

/*левый поворот*/
void left_rotate(struct rbtree **root, struct rbtree *x)
{
    struct rbtree *y = x->right;
    x->right = y->left;
    if (y->left != NULL)
    {
        y->left->parent = x;
    }
    y->parent = x->parent;
    if (x->parent == NULL)
    {
        *root = y;
    }
    else if (x == x->parent->left)
    {
        x->parent->left = y;
    }
    else
    {
        x->parent->right = y;
    }
    y->left = x;
    x->parent = y;
}

/*правый поворот*/
void right_rotate(struct rbtree **root, struct rbtree *x)
{
    struct rbtree *y = x->left;
    x->left = y->right;
    if (y->right != NULL)
    {
        y->right->parent = x;
    }
    y->parent = x->parent;
    if (x->parent == NULL)
    {
        *root = y;
    }
    else if (x == x->parent->right)
    {
        x->parent->right = y;
    }
    else
    {
        x->parent->left = y;
    }
    y->right = x;
    x->parent = y;
}

/*востановление при добавление элемента*/
void rbtree_add_fixup(struct rbtree **root, struct rbtree *z)
{
    while (z->parent != NULL && (z->parent->color == RED))
    {

        if (z->parent == z->parent->parent->left)
        {
            struct rbtree *y = z->parent->parent->right;

            if (y != NULL && (y->color == RED))
            {
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            }

            else
            {
                if (z == z->parent->right)
                {
                    z = z->parent;
                    left_rotate(root, z);
                }
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                right_rotate(root, z->parent->parent);
            }
        }
        else
        {
            struct rbtree *y = z->parent->parent->left;

            if (y != NULL && (y->color == RED))
            {
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            }
            else
            {
                if (z == z->parent->left)
                {
                    z = z->parent;
                    right_rotate(root, z);
                }
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                left_rotate(root, z->parent->parent);
            }
        }
    }
    (*root)->color = BLACK;
}

/*добавление элемента*/
struct rbtree *rbtree_add(struct rbtree *root, int key)
{
    struct rbtree *z = rbtree_create_node(key);
    struct rbtree *y = NULL;
    struct rbtree *x = root;

    // O(logn)
    while (x != NULL)
    {
        y = x;
        if (z->data < x->data)
        {
            x = x->left;
        }
        else
        {
            x = x->right;
        }
    }
    // -----

    z->parent = y;
    if (y == NULL)
    {
        root = z;
    }
    else if (z->data < y->data)
    {
        y->left = z;
    }
    else
    {
        y->right = z;
    }

    z->left = NULL;
    z->right = NULL;
    z->color = RED;

    rbtree_add_fixup(&root, z);

    return root;
}

/*поиск элемента*/
struct rbtree *rbtree_lookup(struct rbtree *root, int key)
{
    struct rbtree *current = root;

    while (current != NULL && current->data != key)
    {
        if (key < current->data)
        {
            current = current->left;
        }
        else
        {
            current = current->right;
        }
    }
    return current;
}

/*вспомогательная функция при удалении*/
void transplant(struct rbtree **root, struct rbtree *u, struct rbtree *v)
{
    if (u->parent == NULL)
    {
        *root = v;
    }
    else if (u == u->parent->left)
    {
        u->parent->left = v;
    }
    else
    {
        u->parent->right = v;
    }
    if (v != NULL)
    {
        v->parent = u->parent;
    }
}

/*востановление при удалении элемента*/
void rb_tree_delete_fixup(struct rbtree **root, struct rbtree *x)
{
    while (x != *root && (x == NULL || (x->color == BLACK)))
    {
        if (x == NULL || x->parent == NULL)
        {
            x = *root;
        }
        else if (x == x->parent->left)
        {
            struct rbtree *w = x->parent->right;
            if (w != NULL && (w->color == RED))
            {
                w->color = BLACK;
                x->parent->color = RED;
                left_rotate(root, x->parent);
                w = x->parent->right;
            }
            if ((w->left == NULL || (w->left->color == BLACK)) &&
                (w->right == NULL || (w->right->color == BLACK)))
            {
                w->color = RED;
                x = x->parent;
            }
            else
            {
                if (w->right == NULL || (w->right->color == BLACK))
                {
                    if (w->left != NULL)
                    {
                        w->left->color = BLACK;
                    }
                    w->color = RED;
                    right_rotate(root, w);
                    w = x->parent->right;
                }
                w->color = x->parent->color;
                x->parent->color = BLACK;
                if (w->right != NULL)
                {
                    w->right->color = BLACK;
                }
                left_rotate(root, x->parent);
                x = *root;
            }
        }
        else
        {
            struct rbtree *w = x->parent->left;

            if (w != NULL && (w->color == RED))
            {
                w->color = BLACK;
                x->parent->color = RED;
                right_rotate(root, x->parent);
                w = x->parent->left;
            }
            if ((w->right == NULL || (w->right->color == BLACK)) &&
                (w->left == NULL || (w->left->color == BLACK)))
            {
                w->color = RED;
                x = x->parent;
            }
            else
            {
                if (w->left == NULL || (w->left->color == BLACK))
                {
                    if (w->right != NULL)
                    {
                        w->right->color = BLACK;
                    }
                    w->color = RED;
                    left_rotate(root, w);
                    w = x->parent->left;
                }
                w->color = x->parent->color;
                x->parent->color = BLACK;
                if (w->left != NULL)
                {
                    w->left->color = BLACK;
                }
                right_rotate(root, x->parent);
                x = *root;
            }
        }
    }
    if (x != NULL)
    {
        x->color = BLACK;
    }
}

/*минимальный элемент дерева*/

struct rbtree *rbtree_min(struct rbtree *root)
{
    while (root->left != NULL)
    {
        root = root->left;
    }
    return root;
}

/*удаление элемента дерева*/
void delete_node(struct rbtree **root, struct rbtree *z)
{
    struct rbtree *y = z;
    struct rbtree *x;
    char y_original_color = y->color;

    if (z->left == NULL)
    {
        x = z->right;
        transplant(root, z, z->right);
    }
    else if (z->right == NULL)
    {
        x = z->left;
        transplant(root, z, z->left);
    }
    else
    {
        y = rbtree_min(z->right);
        y_original_color = y->color;
        x = y->right;
        if (y->parent == z)
        {
            if (x != NULL)
            {
                x->parent = y;
            }
        }
        else
        {
            transplant(root, y, y->right);
            y->right = z->right;
            y->right->parent = y;
        }
        transplant(root, z, y);
        y->left = z->left;
        y->left->parent = y;
        y->color = z->color;
    }

    if ((y_original_color == BLACK))
    {
        rb_tree_delete_fixup(root, x);
    }
    free(z);
}

/*удаление дерева*/
struct rbtree *rbtree_delete(struct rbtree *root, int key)
{
    struct rbtree *z = rbtree_lookup(root, key);
    if (z != NULL)
    {
        delete_node(&root, z);
    }
    return root;
}

/*максимальный элемент дерева*/
struct rbtree *rbtree_max(struct rbtree *root)
{
    while (root->right != NULL)
    {
        root = root->right;
    }
    return root;
}

/*освобождение памяти из дерева*/
void rbtree_free(struct rbtree *root)
{
    if (root != NULL)
    {
        rbtree_free(root->left);
        rbtree_free(root->right);
        free(root);
    }
}

/*вывод дерева*/
void rbtree_print_dfs(struct rbtree *root, int level)
{
    if (root != NULL)
    {
        rbtree_print_dfs(root->left, level + 1);
        printf("%*s%d (%c)\n", 4 * level, "", root->data, root->color);
        rbtree_print_dfs(root->right, level + 1);
    }
}

/*высота дерева*/
int height(struct rbtree *node)
{
    if (node == NULL)
        return 0;
    return 1 + fmaxf(height(node->left), height(node->right));
}

double wtime()
{
    struct timeval t;
    gettimeofday(&t, NULL);
    return (double)t.tv_sec + (double)t.tv_usec * 1E-6;
}

void demonstrate_log_dependence(struct rbtree *root)
{
    root = NULL;
    printf("Demonstrating logarithmic dependence of tree height on the number of nodes added:\n");
    printf("|\tNodes\t|\tHeight\t|\ttime\t\n");

    double t = wtime();
    for (int i = 1; i <= 50000; i++)
    {
        root = rbtree_add(root, i);

        rbtree_lookup(root, i);
        if (i % 1000 == 0)
        {
            double t_new = wtime();
            printf("|\t%d\t|\t%d\t|\t%lf\n", i, height(root), (t_new - t) * 1000);
        }
    }

    rbtree_free(root);
}

int main()
{
    setlocale(LC_ALL, "ru");
    struct rbtree *root = NULL;

    printf("1. Продемонстрировать логарифмическую зависимость высоты дерева от количества добавленных в него узлов\n");
    printf("2. Вставить в Черно-красное дерево ~50000 элементов, заранее отсортированных в порядке возрастания\n");
    printf("3. Удаление элементов\n");
    printf("4. Обход дерева\n");
    printf("5. Выход\n");

    while (true)
    {

        int chose_current;
        scanf("%d", &chose_current);

        if (root != NULL)
        {
            root = NULL;
        }

        if (chose_current == 1)
        {
            demonstrate_log_dependence(root);
        }
        else if (chose_current == 2)
        {
            root = NULL;
            for (int i = 1; i <= 50000; i++)
            {
                root = rbtree_add(root, i);
                if (i % 1000 == 0)
                {
                    printf("Number of nodes: %d, Tree height: %d\n", i, height(root));
                }
            }

            rbtree_free(root);
        }
        else if (chose_current == 3)
        {
            root = NULL;
            for (int i = 1; i <= 50000; i++)
            {
                root = rbtree_add(root, i);
            }

            for (int i = 1; i <= 50000; i += 1000)
            {
                root = rbtree_delete(root, i);
                struct rbtree *node = rbtree_lookup(root, i);
                if (node != NULL)
                {
                    printf("Node with key %d is still present.\n", i);
                }
                else
                {
                    printf("Node with key %d successfully deleted.\n", i);
                }
            }

            rbtree_free(root);
        }
        else if (chose_current == 4)
        {
            root = NULL;
            for (int i = 1; i <= 50000; i += 1000)
            {
                root = rbtree_add(root, i);
            }
            printf("Tree structure:\n");
            rbtree_print_dfs(root, 0);
            rbtree_free(root);
        }
        else if (chose_current == 5)
        {
            rbtree_free(root);
            break;
        }
        else if (chose_current == -1)
        {
            root = NULL;

            int userInput;

            printf("NEW NUM: ");
            scanf("%d", &userInput);

            while (userInput != -1)
            {
                if (userInput == -1)
                {
                    break;
                }
                else
                {
                    root = rbtree_add(root, userInput);
                    rbtree_print_dfs(root, 0);
                    printf("\n\nNEW NUM: ");

                    scanf("%d", &userInput);
                }
            }
            rbtree_free(root);
        }
        printf("\n\n");
    }

    return 0;
}
