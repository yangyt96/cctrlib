/*
Copyright (c) 2022 Yee Yang Tan

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

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include <assert.h>

#pragma once

// ==============================================================
/*
 * Node
 */
typedef struct Slist_Node_t
{
    void *data;
    struct Slist_Node_t *next;
} Slist_Node_t;

inline Slist_Node_t *_slist_node_construct_(void *data, uint32_t dsize)
{
    assert(data != NULL);
    Slist_Node_t *node = (Slist_Node_t *)malloc(sizeof(Slist_Node_t));
    assert(node != NULL);
    node->data = malloc(dsize);
    assert(node->data != NULL);
    memcpy(node->data, data, dsize);
    return node;
}
inline void _slist_node_destruct_(Slist_Node_t *self)
{
    assert(self != NULL);
    free(self->data);
    free(self);
}

// ==============================================================
/*
 * List
 */
typedef struct
{
    Slist_Node_t *head;
    Slist_Node_t *tail;
    uint64_t size; // ! since it is read in single way, so it can be uint64_t, but it probably wont use so much
    uint32_t dsize;
} Slist_t;

/*
 * Construct & Desctruct
 */
Slist_t *slist_construct(uint32_t dsize)
{
    Slist_t *ret = (Slist_t *)calloc(1, sizeof(Slist_Node_t));
    assert(ret != NULL);
    ret->dsize = dsize;
    return ret;
}
void slist_clear(Slist_t *self)
{
    assert(self != NULL);
    assert(self->head != NULL);
    Slist_Node_t *ptr = self->head;
    while (ptr != NULL)
    {
        Slist_Node_t *rmv = ptr;
        ptr = ptr->next;
        _slist_node_destruct_(rmv);
    }
    self->head = NULL;
    self->tail = NULL;
    self->size = 0;
}
void slist_destroy(Slist_t *self)
{
    slist_clear(self);
    free(self);
}
Slist_t *slist_copy(Slist_t *self)
{
    assert(self != NULL);
    Slist_t *ret = slist_construct(self->dsize);

    Slist_Node_t *prev = NULL;
    for (Slist_Node_t *ptr = self->head; ptr != NULL; ptr = ptr->next)
    {
        Slist_Node_t *node = _slist_node_construct_(ptr->data, self->dsize);

        if (prev != NULL)
            prev->next = node;
        prev = node;

        if (ptr == self->head)
            ret->head = node;
        else if (ptr == self->tail)
            ret->tail = node;
    }
    ret->tail->next = NULL;

    ret->size = self->size;
    return ret;
}

/*
 * Basic Usage
 */
void *slist_front(Slist_t *self)
{
    assert(self != NULL);
    assert(self->head != NULL);
    return self->head->data;
}
void *slist_back(Slist_t *self)
{
    assert(self != NULL);
    assert(self->tail != NULL);
    return self->tail->data;
}
void slist_push_front(Slist_t *self, void *data)
{
    assert(self != NULL);
    assert(data != NULL);

    Slist_Node_t *node = _slist_node_construct_(data, self->dsize);
    node->next = self->head;

    self->head = node;
    if (self->size == 0)
        self->tail = node;

    self->size++;
}
void slist_push_back(Slist_t *self, void *data)
{
    assert(self != NULL);
    assert(data != NULL);

    Slist_Node_t *node = _slist_node_construct_(data, self->dsize);
    node->next = NULL;

    if (self->tail != NULL)
        self->tail->next = node;
    self->tail = node;
    if (self->size == 0)
        self->head = node;

    self->size++;
}
void slist_pop_front(Slist_t *self)
{
    assert(self != NULL);
    Slist_Node_t *rmv = self->head;

    if (rmv == NULL)
        return;

    self->head = rmv->next;
    self->size--;

    if (self->size == 0)
        self->tail = NULL;

    _slist_node_destruct_(rmv);
}

/*
 * Iteration
 */
Slist_Node_t *_slist_at_(Slist_t *self, uint64_t position)
{
    assert(self != NULL);

    if (position > self->size)
        return NULL;
    else if (position == self->size)
        return self->tail;

    Slist_Node_t *node = self->head;
    for (uint64_t i = 1; i <= position; i++)
        node = node->next;

    return node;
}
void *slist_at(Slist_t *self, uint64_t position)
{
    Slist_Node_t *node = _slist_at_(self, position);
    return node->data;
}
void slist_insert(Slist_t *self, uint64_t position, void *data)
{
    assert(self != NULL);
    assert(data != NULL);
    assert(position <= self->size);

    Slist_Node_t *prev_node;
    Slist_Node_t *next_node;

    if (position > 0)
    {
        prev_node = _slist_at_(self, position - 1);
        next_node = prev_node->next;
    }
    else
    {
        prev_node = NULL;
        next_node = self->head;
    }

    Slist_Node_t *node = _slist_node_construct_(data, self->dsize);

    if (position > 0)
        prev_node->next = node;
    else
        self->head = node;
    node->next = next_node;
    self->size++;
}
void slist_erase(Slist_t *self, uint64_t position)
{
    assert(self != NULL);

    Slist_Node_t *prev_node = _slist_at_(self, position - 1);
}