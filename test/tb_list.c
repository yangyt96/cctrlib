#include "tau/tau.h"
#include "cctrlib/list.h"

TAU_MAIN()

TEST(List, push_front_pop_front)
{
    const uint32_t test_base = 0x33221100;
    const uint32_t test_len = 10;

    List_t *test_list = list_init(sizeof(uint32_t));

    for (uint32_t i = 0; i < test_len; i++)
    {
        uint32_t tmp = test_base + i;
        list_push_front(test_list, &tmp);
    }

    CHECK_EQ(test_len, test_list->size);

    while (test_list->size > 0)
    {
        uint32_t *data = (uint32_t *)list_front(test_list);
        CHECK_EQ(test_base + (uint32_t)test_list->size - (uint32_t)1, *data);
        list_pop_front(test_list);
    }

    CHECK_EQ(0, test_list->size);

    list_destroy(test_list);
}

TEST(List, push_back_pop_back)
{
    const uint32_t test_base = 0x33221100;
    const uint32_t test_len = 10;

    List_t *test_list = list_init(sizeof(uint32_t));

    for (uint32_t i = 0; i < test_len; i++)
    {
        uint32_t tmp = test_base + i;
        list_push_back(test_list, &tmp);
    }

    CHECK_EQ(test_len, test_list->size);

    while (test_list->size > 0)
    {
        uint32_t *data = (uint32_t *)list_back(test_list);
        CHECK_EQ(test_base + test_list->size - 1, *data);
        list_pop_back(test_list);
    }

    CHECK_EQ(0, test_list->size);

    list_destroy(test_list);
}

TEST(List, push_front_pop_back)
{
    const uint32_t test_base = 0x33221100;
    const uint32_t test_len = 10;

    List_t *test_list = list_init(sizeof(uint32_t));

    for (uint32_t i = 0; i < test_len; i++)
    {
        uint32_t tmp = test_base + i;
        list_push_front(test_list, &tmp);
    }

    CHECK_EQ(test_len, test_list->size);

    for (uint32_t i = 0; i < test_len; i++)
    {
        uint32_t *data = (uint32_t *)list_back(test_list);
        CHECK_EQ(test_base + i, *data);
        list_pop_back(test_list);
    }

    CHECK_EQ(0, test_list->size);

    list_destroy(test_list);
}

TEST(List, push_back_pop_front)
{
    const uint32_t test_base = 0x33221100;
    const uint32_t test_len = 10;

    List_t *test_list = list_init(sizeof(uint32_t));

    for (uint32_t i = 0; i < test_len; i++)
    {
        uint32_t tmp = test_base + i;
        list_push_back(test_list, &tmp);
    }

    CHECK_EQ(test_len, test_list->size);

    for (uint32_t i = 0; i < test_len; i++)
    {
        uint32_t *data = (uint32_t *)list_front(test_list);
        CHECK_EQ(test_base + i, *data);
        list_pop_front(test_list);
    }

    REQUIRE_EQ(0, test_list->size);

    list_destroy(test_list);
}

TEST(List, _border_)
{
    const uint32_t test_base = 0x33221100;
    const uint32_t test_len = 10;

    List_t *test_list = list_init(sizeof(uint32_t));

    for (uint32_t i = 0; i < test_len; i++)
    {
        uint32_t tmp = test_base + i;
        list_push_back(test_list, &tmp);
    }

    CHECK_EQ(test_len, test_list->size);

    List_Node_t *left;
    List_Node_t *right;

    // to positive
    for (int64_t i = 0; i <= test_len; i++)
    {
        _list_border_(test_list, i, (uintptr_t *)&left, (uintptr_t *)&right);

        if (i == 0)
            CHECK(NULL == left);
        else
            CHECK_EQ(test_base + i - 1, *(uint32_t *)left->data);

        if (i == test_len)
            CHECK(NULL == right);
        else
            CHECK_EQ(test_base + i, *(uint32_t *)right->data);
    }

    // to negative
    for (int64_t i = -1; i >= -test_len; i--)
    {
        _list_border_(test_list, i, (uintptr_t *)&left, (uintptr_t *)&right);

        if (i == -test_len)
            CHECK(NULL == left);
        else
            CHECK_EQ(test_base + test_len + i - 1, *(uint32_t *)left->data);

        CHECK_EQ(test_base + test_len + i, *(uint32_t *)right->data);
    }

    // exceed range
    _list_border_(test_list, (int64_t)test_list->size + 1, (uintptr_t *)&left, (uintptr_t *)&right);
    CHECK(NULL == left);
    CHECK(NULL == right);
    _list_border_(test_list, -(int64_t)test_list->size - 1, (uintptr_t *)&left, (uintptr_t *)&right);
    CHECK(NULL == left);
    CHECK(NULL == right);

    list_destroy(test_list);
}

TEST(List, insert_check_at)
{
    const uint32_t test_base = 0x33221100;
    const uint32_t test_len = 10;

    List_t *test_list = list_init(sizeof(uint32_t));

    for (uint32_t i = 0; i < test_len; i++)
    {
        uint32_t tmp = test_base + i;
        list_push_back(test_list, &tmp);
    }
}