#include "lss.h"

LSS* LSScreate(void)
{
    LSS* const lss = malloc(sizeof(LSS));
    lss->arr = malloc(0);
    lss->size = 0;
    lss->pos = 0;
    return lss;
}

void LSSfree(LSS* lss)
{
    free(lss->arr);
    free(lss);
}

void LSSadd(LSS* lss, char c[4], int w)
{
    LSSnode* const newBlock = realloc(lss->arr, (lss->size + 1) * sizeof(LSSnode));
    if (newBlock)
    {
        lss->arr = newBlock;
        memmove(&lss->arr[lss->pos + 1], &lss->arr[lss->pos], sizeof(LSSnode) * (lss->size - lss->pos));
        lss->size++;
        memcpy(lss->arr[lss->pos].c, c, sizeof(char[4]));
        lss->arr[lss->pos].w = w;
        lss->pos++;
    }
}

void LSSpop(LSS* lss)
{
    if (lss->size - lss->pos)
    {
        memmove(&lss->arr[lss->pos], &lss->arr[lss->pos + 1], sizeof(LSSnode) * (lss->size - lss->pos - 1));
        lss->size--;
    }
}

static void calcNewPos(LSS* lss, CursorPos* cur)
{
    const ScreenSize ssiz = getScreenSize();
    if (!ssiz.width) return;
    for (size_t i = 0; i < lss->pos; i++)
    {
        if (cur->x + lss->arr[i].w > ssiz.width)
        {
            cur->x = 1;
            cur->y++;
        }
        cur->x += lss->arr[i].w;
    }
}

void LSSreprint(LSS* lss)
{
    const CursorPos beg = getCursorPos();
    for (size_t i = lss->pos; i < lss->size; i++)
    {
        for (size_t j = 0; j < 4; j++)
        {
            if (!lss->arr[i].c[j]) break;
            putchar(lss->arr[i].c[j]);
        }
    }
    printf("\x1b[0J\x1b[%zu;%zuH", beg.y, beg.x);
}

char* LSSgetstr(LSS* lss)
{
    char* const ret = calloc(lss->size + 1, sizeof(char));
    if (!ret) return NULL;
    for (size_t i = 0; i < lss->size; i++)
    {
        for (size_t j = 0; j < 4; j++)
        {
            if (!lss->arr[i].c[j]) break;
            ret[i] = lss->arr[i].c[j];
        }
    }
    return ret;
}

char* LSSrelease(LSS* lss)
{
    char* const ret = LSSgetstr(lss);
    LSSfree(lss);
    return ret;
}

void LSSleft(LSS* lss)
{
    if (lss->pos)
    {
        lss->pos--;
        const CursorPos cur = getCursorPos();
        if (!cur.x) return;
        if (cur.x == 1)
        {
            const ScreenSize ssiz = getScreenSize();
            if (!ssiz.width) return;
            printf("\x1b[1A\x1b[%zuC", ssiz.width - lss->arr[lss->pos].w);
        }
        else
        {
            printf("\x1b[%iD", lss->arr[lss->pos].w);
        }
    }
}

void LSSright(LSS* lss)
{
    if (lss->pos < lss->size)
    {
        const CursorPos cur = getCursorPos();
        if (!cur.x) return;
        const ScreenSize ssiz = getScreenSize();
        if (!ssiz.width) return;
        if (cur.x + lss->arr[lss->pos].w > ssiz.width)
        {
            putchar('\n');
        }
        else
        {
            printf("\x1b[%iC", lss->arr[lss->pos].w);
        }
        lss->pos++;
    }
}