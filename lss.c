#include "lss.h"

LSS* LSScreate(void)
{
	LSS* const lss = malloc(sizeof(LSS));
	lss->arr = malloc(0);
	lss->size = 0;
	lss->pos = 0;
	lss->beg = getCursorPos();
	return lss;
}

void LSSfree(LSS* lss)
{
	free(lss->arr);
	free(lss);
}

void LSSadd(LSS* lss, char c[4])
{
	LSSnode* const newBlock = realloc(lss->arr, (lss->size + 1) * sizeof(LSSnode));
	if (newBlock)
	{
		lss->arr = newBlock;
		memmove(&lss->arr[lss->pos + 1], &lss->arr[lss->pos], sizeof(LSSnode) * (lss->size - lss->pos));
		lss->size++;
		memcpy(lss->arr[lss->pos].c, c, sizeof(char[4]));
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

static void gotoNewPos(LSS* lss)
{
	CursorPos cur = lss->beg;
	const ScreenSize ssiz = getScreenSize();
	if (!ssiz.width) return;
	for (size_t i = 0; i < lss->pos; i++)
	{
		const unsigned char w = getcharsize(utf8charcode(lss->arr[i].c));
		if (cur.x + w > ssiz.width)
		{
			cur.x = 1;
			cur.y++;
			if (cur.x + w > ssiz.width + 1)
			{
				cur.x += w;
			}
		}
		else
		{
			cur.x += w;
		}
	}
	if (cur.y > ssiz.height)
	{
		if (cur.x == 1)
		{
			printf("\e[%zuS", cur.y - ssiz.height);
		}
		lss->beg.y -= cur.y - ssiz.height;
	}
	printf("\x1b[%zu;%zuH", cur.y, cur.x);
}

void LSSreprint(LSS* lss)
{
	for (size_t i = lss->pos; i < lss->size; i++)
	{
		for (size_t j = 0; j < 4; j++)
		{
			if (!lss->arr[i].c[j]) break;
			putchar(lss->arr[i].c[j]);
		}
	}
	const size_t pos = lss->pos;
	lss->pos = lss->size;
	gotoNewPos(lss);
	printf("\e[0J");
	lss->pos = pos;
	gotoNewPos(lss);
}

char* LSSgetstr(LSS* lss)
{
	char* const ret = calloc(lss->size * 4 + 1, sizeof(char));
	if (!ret) return NULL;
	size_t pos = 0;
	for (size_t i = 0; i < lss->size; i++)
	{
		for (size_t j = 0; j < 4; j++)
		{
			if (!lss->arr[i].c[j]) break;
			ret[pos++] = lss->arr[i].c[j];
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
		// const CursorPos cur = getCursorPos();
		// if (!cur.x) return;
		// if (cur.x == 1)
		// {
			// const ScreenSize ssiz = getScreenSize();
			// if (!ssiz.width) return;
			// printf("\x1b[1A\x1b[%zuC", ssiz.width - lss->arr[lss->pos].w);
			gotoNewPos(lss);
		// }
		// else if (lss->arr[lss->pos].w)
		// {
		//     printf("\x1b[%iD", lss->arr[lss->pos].w);
		// }
	}
}

void LSSright(LSS* lss)
{
	if (lss->pos < lss->size)
	{
		// const CursorPos cur = getCursorPos();
		// if (!cur.x) return;
		// const ScreenSize ssiz = getScreenSize();
		// if (!ssiz.width) return;
		// if (cur.x + lss->arr[lss->pos].w > ssiz.width)
		// {
			lss->pos++;
			gotoNewPos(lss);
		// }
		// else if (lss->arr[lss->pos].w)
		// {
		//     printf("\x1b[%iC", lss->arr[lss->pos].w);
		//     lss->pos++;
		// }
	}
}