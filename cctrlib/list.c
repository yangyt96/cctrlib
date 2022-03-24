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

#include "list.h"

List_t *list_init(uint32_t dsize)
{
    List_t *ret = (List_t *)malloc(sizeof(List_t));
    ret->size = 0;
    ret->head = NULL;
    ret->tail = NULL;
    ret->dsize = dsize;
    return ret;
}

void *list_front(List_t *self)
{
    return self->head->data;
}

void *list_back(List_t *self)
{
    return self->tail->data;
}

void list_push_front(List_t *self, void *data)
{
    List_Node_t *tmp = (List_Node_t *)calloc(sizeof(List_Node_t), 1);
    tmp->data = malloc(self->dsize);
    memcpy(tmp->data, data, self->dsize);

    if (self->head == NULL && self->tail == NULL)
    {
        self->head = tmp;
        self->tail = tmp;
    }
    else
    {
        self->head->prev = tmp;
        tmp->next = self->head;
        self->head = tmp;
    }

    self->size++;
}

void list_push_back(List_t *self, void *data)
{
    List_Node_t *tmp = (List_Node_t *)calloc(sizeof(List_Node_t), 1);
    tmp->data = malloc(self->dsize);
    memcpy(tmp->data, data, self->dsize);

    if (self->head == NULL && self->tail == NULL)
    {
        self->head = tmp;
        self->tail = tmp;
    }
    else
    {
        self->tail->next = tmp;
        tmp->prev = self->tail;
        self->tail = tmp;
    }

    self->size++;
}

void list_pop_front(List_t *self)
{
    if (self->head == NULL || self->size == 0)
        return;

    List_Node_t *tmp = self->head;
    self->head = tmp->next;

    if (self->head != NULL)
        self->head->prev = NULL;

    free(tmp->data);
    free(tmp);
    self->size--;

    if (self->size == 0)
        self->tail = NULL;
}

void list_pop_back(List_t *self)
{
    if (self->tail == NULL || self->size == 0)
        return;

    List_Node_t *tmp = self->tail;
    self->tail = tmp->prev;

    if (self->tail != NULL)
        self->tail->next = NULL;

    free(tmp->data);
    free(tmp);
    self->size--;

    if (self->size == 0)
        self->head = NULL;
}

void *list_at(List_t *self, int32_t pos)
{
    if (pos >= self->size || pos < -self->size)
        return NULL;

    if (pos >= 0)
    {
        List_Node_t *ptr = self->head;
        int32_t i = 0;
        while (i < pos)
        {
            ptr = ptr->next;
            i++;
        }
        return ptr->data;
    }
    else // pos < 0
    {
        List_Node_t *ptr = self->tail;
        int32_t i = -1;
        while (i > pos)
        {
            ptr = ptr->prev;
            i--;
        }
        return ptr->data;
    }
}

void list_clear(List_t *self)
{
    List_Node_t *ptr = self->head;
    while (ptr != NULL)
    {
        List_Node_t *to_del = ptr;
        ptr = ptr->next;
        free(to_del->data);
        free(to_del);
    }
    self->size = 0;
}

void list_destroy(List_t *self)
{
    list_clear(self);
    free(self);
}

List_t *list_copy(List_t *self)
{
    List_t *ret = list_init(self->dsize);
    for (List_Node_t *ptr = self->head; ptr != NULL; ptr = ptr->next)
        list_push_back(ret, ptr->data);
    return ret;
}

void _list_border_(List_t *self, int32_t pos, uintptr_t *left, uintptr_t *right)
{
    // ! requires optimization, decide left to right or right to left
    // ! cautious of pos exceeds range
    // ! Cautious if the self list is empty and it comes into this function, then left & right are NULLs too.
    // __________
    // | 0| 1| 2|
    // |-3|-2|-1|
    // _________|3| <- list size

    List_Node_t *ptr;
    if (pos == self->size)
    {
        *left = (uintptr_t)self->tail;
        *right = (uintptr_t)NULL;
    }
    else if (pos == 0 || pos == -self->size)
    {
        *left = (uintptr_t)NULL;
        *right = (uintptr_t)self->head;
    }
    else if (pos > self->size || pos < -self->size)
    {
        *left = (uintptr_t)NULL;
        *right = (uintptr_t)NULL;
    }
    else if (pos > 0)
    {
        // iterate left to right
        ptr = self->head;
        for (int32_t i = 0; i < pos; i++)
            ptr = ptr->next;
        *left = (uintptr_t)ptr->prev;
        *right = (uintptr_t)ptr;
    }
    else if (pos < 0)
    {
        // iterate right to left
        ptr = self->tail;
        for (int32_t i = 0; pos < i; i--)
            ptr = ptr->prev;
        *left = (uintptr_t)ptr;
        *right = (uintptr_t)ptr->next;
    }
}

// ------------------------------------------------------------------

void list_insert(List_t *self, int32_t pos, void *data)
{
    List_Node_t *left;
    List_Node_t *right;

    _list_border_(self, pos, (uintptr_t *)&left, (uintptr_t *)&right);
    if (left == NULL && right == NULL)
        return;

    List_Node_t *center = (List_Node_t *)calloc(sizeof(List_Node_t), 1);
    center->data = malloc(self->dsize);
    memcpy(center->data, data, self->dsize);
    center->prev = left;
    center->next = right;

    if (left == NULL && right != NULL)
    {
        right->prev = center;
        self->head = center;
    }
    else if (right == NULL && left != NULL)
    {
        left->next = center;
        self->tail = center;
    }
    else
    {
        left->next = center;
        right->prev = center;
    }
    self->size++;
}

void list_erase(List_t *self, int32_t pos)
{
    if (pos == 0 || pos == -self->size)
    {
        list_pop_front(self);
        return;
    }
    if (pos == -1 || pos == self->size - 1)
    {
        list_pop_back(self);
        return;
    }

    List_Node_t *left;
    List_Node_t *right;

    _list_border_(self, pos, (uintptr_t *)&left, (uintptr_t *)&right);
    if (left == NULL && right == NULL)
        return;

    List_Node_t *to_del = right;
    right = to_del->next;

    left->next = right;
    right->prev = left;

    free(to_del->data);
    free(to_del);
    self->size--;
}

void list_insert_list(List_t *self, int32_t pos, List_t *object)
{
    List_t *cpy = list_copy(object);

    if (self->size == 0)
    {
        *self = *cpy;
        return;
    }

    List_Node_t *left;
    List_Node_t *right;

    _list_border_(self, pos, (uintptr_t *)&left, (uintptr_t *)&right);

    if (left == NULL && right == NULL)
        return;

    if (left == NULL && right != NULL)
    {
        cpy->tail->next = self->head;
        self->head->prev = cpy->tail;
        self->head = cpy->head;
    }
    else if (left != NULL && right == NULL)
    {
        cpy->head->prev = self->tail;
        self->tail->next = cpy->head;
        self->tail = cpy->tail;
    }
    else
    {
        cpy->head->prev = left;
        cpy->tail->next = right;
        left->next = cpy->head;
        right->prev = cpy->tail;
    }
    free(cpy);
}

List_t *list_from_array(void *array, uint32_t asize, uint32_t dsize)
{
    List_t *ret = list_init(dsize);

    uint8_t *aptr = (uint8_t *)array;
    for (uint32_t i = 0; i < asize; i++)
    {
        list_push_back(ret, (void *)aptr);
        aptr += dsize;
    }

    return ret;
}

void list_insert_array(List_t *self, int32_t pos, void *array, uint32_t asize)
{
    List_t *atmp = list_from_array(array, asize, self->dsize);
    list_insert_list(self, pos, atmp);
    list_destroy(atmp);
}

// ---------------------------------------------------------------------------------------
int32_t list_find_data_range(List_t *self, void *data, uint32_t offset, uint32_t dsize)
{
    if ((offset + dsize) > self->dsize)
        return -1;
    int32_t pos = 0;
    for (List_Node_t *ptr = self->head; ptr != NULL; ptr = ptr->next, pos++)
        if (!memcmp(data, ptr->data + offset, dsize))
            return pos;
    return -1;
}

int32_t list_find(List_t *self, void *data)
{
    // ! check optimization for single memcmp or switch for different data size is better
    int32_t pos = 0;
    for (List_Node_t *ptr = self->head; ptr != NULL; ptr = ptr->next, pos++)
    {
        switch (self->dsize)
        {
        case sizeof(uint8_t):
            if (*(uint8_t *)data == *(uint8_t *)ptr->data)
                return pos;
            break;

        case sizeof(uint16_t):
            if (*(uint16_t *)data == *(uint16_t *)ptr->data)
                return pos;
            break;

        case sizeof(uint32_t):
            if (*(uint32_t *)data == *(uint32_t *)ptr->data)
                return pos;
            break;

        case sizeof(uint64_t):
            if (*(uint64_t *)data == *(uint64_t *)ptr->data)
                return pos;
            break;

        default:
            if (!memcmp(data, ptr->data, self->dsize))
                return pos;
        }
    }
    return -1;
}

// ------------------------------------ Test --------------------------------------------------
void list_print(List_t *self)
{
    int itr = 0;
    for (List_Node_t *ptr = self->head; ptr != NULL; ptr = ptr->next)
    {
        printf("itr-%i: ", itr++);

        uint8_t *data = (uint8_t *)ptr->data;

        for (uint32_t i = 0; i < self->dsize; i++)
            printf("%02x ", data[i]);

        // if (__BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__)
        //     for (int i = self->size; i > 0; i--)
        //         printf("%02x", data[i - 1]);

        // if (__BYTE_ORDER__ == __ORDER_BIG_ENDIAN__)
        //     for (int i = 0; i < self->size; i--)
        //         printf("%02x", data[i]);

        printf("\n");
    }
}
