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

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#pragma once

/*
 * Strcture
 */
typedef struct List_Node_t
{
    void *data;
    struct List_Node_t *prev;
    struct List_Node_t *next;
} List_Node_t;

typedef struct
{
    List_Node_t *head;
    List_Node_t *tail;
    uint32_t size; // !shorter length to do positive and negative pos
    uint32_t dsize;
} List_t;

/*
 * Construct & Desctruct
 */
List_t *list_init(uint32_t dsize);
void list_clear(List_t *self);
void list_destroy(List_t *self);
List_t *list_copy(List_t *self);

/*
 * Basic Usage
 */
void *list_front(List_t *self);
void *list_back(List_t *self);
void list_push_front(List_t *self, void *data);
void list_push_back(List_t *self, void *data);
void list_pop_front(List_t *self);
void list_pop_back(List_t *self);
void *list_at(List_t *self, int32_t pos);

void _list_border_(List_t *self, int64_t pos, uintptr_t *left, uintptr_t *right);

/*
 * Additional
 */
void list_insert(List_t *self, int64_t pos, void *data);
void list_erase(List_t *self, int32_t pos);
void list_insert_list(List_t *self, int32_t pos, List_t *object);
List_t *list_from_array(void *array, uint32_t asize, uint32_t dsize);
void list_insert_array(List_t *self, int32_t pos, void *array, uint32_t asize);

/*
 * Searching
 */
int32_t list_find_data_range(List_t *self, void *data, uint32_t offset, uint32_t dsize);
int32_t list_find(List_t *self, void *data);

/*
 * Print
 */
void list_print(List_t *self);