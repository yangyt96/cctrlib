/*
Copyright (c) 2021 Yee Yang Tan

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include "cctrlib/list.h"

void list_push_pop_tb()
{

    List_t *test_list = list_init(sizeof(uint32_t));

    // create data
    for (int i = 0; i < 3; i++)
    {
        int tmp = 0x33221100 + i;
        list_push_front(test_list, &tmp);
    }
    printf("Before: \n");
    list_print(test_list);

    while (test_list->size > 0)
    {
        int *data = (int *)list_front(test_list);
        printf("test list size:%i data: 0x%08x \n", test_list->size, *data);
        list_pop_front(test_list);
        printf("pop front done\n");
    }

    // create data
    for (int i = 0; i < 3; i++)
    {
        int tmp = 0x33221100 + i;
        list_push_back(test_list, &tmp);
    }
    printf("Before: \n");
    list_print(test_list);

    while (test_list->size > 0)
    {
        int *data = (int *)list_back(test_list);
        printf("test list size:%i data: 0x%08x \n", test_list->size, *data);
        list_pop_back(test_list);
        printf("pop back done\n");
    }

    list_destroy(test_list);
}

void _list_border__tb(void)
{
    // create data
    List_t *test_list = list_init(sizeof(int));
    for (int i = 0; i < 5; i++)
    {
        int tmp = i << 4;
        list_push_back(test_list, &tmp);
    }
    list_print(test_list);

    List_Node_t *left;
    List_Node_t *right;
    for (int i = -test_list->size - 1; i < test_list->size + 2; i++)
    {
        printf("pos=%i ", i);
        _list_border_(test_list, i, (uintptr_t *)&left, (uintptr_t *)&right);
        printf("left=%p right=%p ", left, right);

        if (left != NULL)
            printf("left_data:%x ", *(int *)left->data);

        if (right != NULL)
            printf("right_data:%x ", *(int *)right->data);

        printf("\n");
    }
}

void list_insert_tb(void)
{
    // create data
    List_t *test_list = list_init(sizeof(uint32_t));
    for (int i = 0; i < 5; i++)
    {
        int tmp = i * 10 + 101;
        list_push_back(test_list, &tmp);
    }
    printf("Before: \n");
    list_print(test_list);

    int tmp;

    tmp = 0x11223344;
    list_insert(test_list, 3, &tmp);
    printf("After insert at 3: \n");
    list_print(test_list);

    tmp = 0x22334455;
    list_insert(test_list, 0, &tmp);
    printf("After insert at 0: \n");
    list_print(test_list);

    tmp = 0x77889900;
    list_insert(test_list, -1, &tmp);
    printf("After insert at -1: \n");
    list_print(test_list);

    tmp = 0x66778833;
    list_insert(test_list, test_list->size, &tmp);
    printf("After insert at test_list->size: \n");
    list_print(test_list);

    list_destroy(test_list);
}

void list_erase_tb(void)
{
    // create data
    List_t *test_list = list_init(sizeof(uint32_t));
    for (int i = 0; i < 10; i++)
    {
        int tmp = i * 10 + 101;
        list_push_back(test_list, &tmp);
    }
    printf("Before: \n");
    list_print(test_list);

    list_erase(test_list, 5);
    printf("normal erase at 5: \n");
    list_print(test_list);

    list_erase(test_list, 0);
    printf("erase side first 0: \n");
    list_print(test_list);

    list_erase(test_list, -1);
    printf("erase side last -1: \n");
    list_print(test_list);

    list_erase(test_list, -test_list->size);
    printf("erase side first negative: \n");
    list_print(test_list);

    list_erase(test_list, test_list->size - 1);
    printf("erase side last positive: \n");
    list_print(test_list);

    list_erase(test_list, test_list->size + 1);
    printf("erase positive exceed range: \n");
    list_print(test_list);

    list_erase(test_list, -test_list->size - 1);
    printf("erase negative exceed range: \n");
    list_print(test_list);

    list_destroy(test_list);
}

void list_insert_list_tb()
{
    // create data
    List_t *test_list = list_init(sizeof(uint32_t));
    for (int i = 0; i < 3; i++)
    {
        int tmp = 0x33221100 + i;
        list_push_front(test_list, &tmp);
    }
    printf("Before: \n");
    list_print(test_list);

    List_t *cpy = list_copy(test_list);

    list_insert_list(test_list, 1, cpy);
    printf("Insert normal at 1: \n");
    list_print(test_list);

    list_insert_list(test_list, 0, cpy);
    printf("Insert pos 0: \n");
    list_print(test_list);

    list_insert_list(test_list, -1, cpy);
    printf("Insert pos -1: \n");
    list_print(test_list);

    list_insert_list(test_list, test_list->size, cpy);
    printf("Insert at test_list->size: \n");
    list_print(test_list);

    List_t *empty_list = list_init(sizeof(uint32_t));
    printf("Before insert to empty list:\n");
    printf("empty_list size: %i \n", empty_list->size);
    list_print(empty_list);
    printf("List to insert:\n");
    list_print(cpy);
    list_insert_list(empty_list, 0, cpy);
    printf("Insert to empty list: \n");
    list_print(empty_list);

    list_destroy(empty_list);
    list_destroy(test_list);
    list_destroy(cpy);
}

void list_from_array_tb()
{
    int k[5];
    for (int i = 0; i < 5; i++)
        k[i] = i;

    List_t *test_list = list_from_array(k, 5, sizeof(int));
    list_print(test_list);

    list_destroy(test_list);
}

int main()
{

    list_push_pop_tb();
    _list_border__tb();
    list_insert_tb();
    list_erase_tb();
    list_insert_list_tb();
    list_from_array_tb();

    return EXIT_SUCCESS;
}