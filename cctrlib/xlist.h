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
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include <assert.h>

#pragma once

// ==============================================================
/*
 * Node
 */
typedef struct Xlist_Node_t
{
    void *data;
    Xlist_Node_t *diff;
} Xlist_Node_t;

inline Xlist_Node_t *_xlist_node_construct_(void *data, uint32_t dsize)
{
    assert(data != NULL);
    Xlist_Node_t *node = (Xlist_Node_t *)malloc(sizeof(Xlist_Node_t));
    assert(node != NULL);
    node->data = malloc(dsize);
    assert(node->data != NULL);
    memcpy(node->data, data, dsize);
    return node;
}
inline void _xlist_node_destruct_(Xlist_Node_t *self)
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
    Xlist_Node_t *head;
    Xlist_Node_t *tail;
    uint32_t size; // ! 32 bit for iteration in positive and negative way
    uint32_t dsize;
} Xlist_t;

/*
 * Construct & Desctruct
 */
Xlist_t *xlist_construct(uint32_t dsize)
{
    Xlist_t *ret = (Xlist_t *)calloc(1, sizeof(Xlist_t));
    assert(ret != NULL);
    ret->dsize = dsize;
    return ret;
}
void xlist_clear(Xlist_t *self)
{
    assert(self != NULL);
    Xlist_Node_t *ptr = self->head;
    Xlist_Node_t *prev = NULL;
    while (ptr != NULL)
    {
        Xlist_Node_t *next = _xlist_xor_ptr_(prev, ptr->diff);
        if (prev != NULL)
            _xlist_node_destruct_(prev);
        prev = ptr;
        ptr = next;
    }
    if (prev != NULL)
        _xlist_node_destruct_(prev);

    self->head = NULL;
    self->tail = NULL;
    self->size = 0;
}
void xlist_destroy(Xlist_t *self)
{
    xlist_clear(self);
    free(self);
}

Xlist_t *xlist_copy(Xlist_t *self)
{
    assert(self != NULL);
    Xlist_t *xlist = xlist_construct(self->dsize);
    // ! implement it
}

/*
 * Basic Usage
 */
inline Xlist_Node_t *_xlist_xor_ptr_(Xlist_Node_t *a, Xlist_Node_t *b){return (Xlist_Node_t *)((uintptr_t)a ^ (uintptr_t)b)};
void xlist_push_front(Xlist_t *self, void *data)
{
    assert(self != NULL);
    assert(data != NULL);

    Xlist_Node_t *node = _xlist_node_construct_(data, self->dsize);
    node->diff = self->head; // equivalent: node->diff = _xlist_xor_ptr_(NULL, self->head);

    if (self->head != NULL)
        self->head->diff = _xlist_xor_ptr_(node, self->head->diff);
    self->head = node;

    if (self->size == 0)
        self->tail = node;

    self->size++;
}
void xlist_push_back(Xlist_t *self, void *data)
{
    assert(self != NULL);
    assert(data != NULL);

    Xlist_Node_t *node = _xlist_node_construct_(data, self->dsize);
    node->diff = self->tail; // equivalent: node->diff = _xlist_xor_ptr_(NULL, self->tail);

    if (self->tail != NULL)
        self->tail->diff = _xlist_xor_ptr_(self->tail->diff, node);
    self->tail = node;

    if (self->size == 0)
        self->head = node;

    self->size++;
}
void xlist_pop_front(Xlist_t *self)
{
    assert(self != NULL);
    if (self->head == NULL || self->size == 0)
        return;

    Xlist_Node_t *rmv = self->head;
    self->head = rmv->diff;
    if (self->head != NULL)
        self->head->diff = _xlist_xor_ptr_(rmv, self->head->diff);

    _xlist_node_destruct_(rmv);
    self->size--;

    if (self->size == 0)
        self->tail = NULL;
}
void xlist_pop_back(Xlist_t *self)
{
    assert(self != NULL);
    if (self->tail == NULL || self->size == 0)
        return;

    Xlist_Node_t *rmv = self->tail;
    self->tail = rmv->diff;
    if (self->tail != NULL)
        self->tail->diff = _xlist_xor_ptr_(self->tail->diff, rmv);

    _xlist_node_destruct_(rmv);
    self->size--;

    if (self->size == 0)
        self->head = NULL;
}