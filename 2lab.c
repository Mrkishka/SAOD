#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef struct trie
{
    char *value;
    char ch;
    struct trie *sibling;
    struct trie *child;
} trie;

trie *trie_create()
{
    trie *node;

    if ((node = (trie *)malloc(sizeof(*node))) == NULL)
        return NULL;
    node->ch = '\0';
    node->value = NULL;
    node->sibling = NULL;
    node->child = NULL;

    return node;
}

char *trie_lookup(struct trie *root, char *key)
{
    struct trie *node, *list;

    for (list = root; *key != '\0'; key++)
    {
        for (node = list; node != NULL; node = node->sibling)
        {
            if (node->ch == *key)
                break;
        }
        if (node != NULL)
            list = node->child;
        else
            return NULL;
    }
    return node->value;
}

struct trie *trie_insert(struct trie *root, char *key, char *value)
{
    struct trie *node, *parent, *list;

    parent = NULL;
    list = root;
    for (; *key != '\0'; key++)
    {
        for (node = list; node != NULL; node = node->sibling)
        {
            if (node->ch == *key)
                break;
        }
        if (node == NULL)
        {
            node = trie_create();
            node->ch = *key;
            node->sibling = list;
            if (parent != NULL)
                parent->child = node;
            else
                root = node;
            list = NULL;
        }
        else
        {
            list = node->child;
        }
        parent = node;
    }
    if (node->value != NULL)
        free(node->value);
    node->value = strdup(value);
    return root;
}

struct trie *
trie_delete_dfs(struct trie *root, struct trie *parent, char *key, int *found)
{
    struct trie *node, *prev = NULL;

    *found = (*key == '\0' && root == NULL) ? 1 : 0;
    if (root == NULL || *key == '\0')
        return root;
    for (node = root; node != NULL; node = node->sibling)
    {
        if (node->ch == *key)
            break;
        prev = node;
    }
    if (node == NULL)
        return root;
    trie_delete_dfs(node->child, node, key + 1, found);
    if (*found > 0 && node->child == NULL)
    {
        if (prev != NULL)
            prev->sibling = node->sibling;
        else
        {
            if (parent != NULL)
                parent->child = node->sibling;
            else
                root = node->sibling;
        }
        free(node->value);
        free(node);
    }
    return root;
}

struct trie *trie_delete(struct trie *root, char *key)
{
    int found;

    return trie_delete_dfs(root, NULL, key, &found);
}

void print_trie(trie *node, int level)
{
    if (node == NULL)
        return;

    for (int i = 0; i < level; i++)
        printf("  ");

    printf("%c", node->ch);

    if (node->value != NULL)
        printf(" -> \"%s\"", node->value);

    printf("\n");

    print_trie(node->child, level + 1);
    print_trie(node->sibling, level);
}

int main()
{
    trie *tree = trie_create();

    printf("Вставка ключей в Trie:\n");
    tree = trie_insert(tree, "test", "a");
    tree = trie_insert(tree, "toaster", "b");
    tree = trie_insert(tree, "toasting", "c");
    tree = trie_insert(tree, "slow", "d");
    tree = trie_insert(tree, "slowly", "e");
    tree = trie_insert(tree, "slowling", "g");

    printf("\nСтруктура Trie после вставки:\n");
    print_trie(tree, 0);

    printf("\nУдаляем ключ 'slowling':\n");
    tree = trie_delete(tree, "slowling");

    printf("\nСтруктура Trie после удаления 'slowling':\n");
    print_trie(tree, 0);

    return 0;
}