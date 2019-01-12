#include <stdlib.h>
#include <stdio.h>
#include <math.h>

extern const void * __data_start;
extern const void * __bss_start;

#define DATA_SECTION_ADDR ((void *)&__data_start)
#define DATA_SECTION_SIZE ((long)&__bss_start - (long)&__data_start)
#define BSS_SECTION_ADDR ((void *)&__bss_start)
#define BSS_SECTION_SIZE ((long)0x3898)

typedef struct _pos_t {
  float x;
  float y;
} pos_t;

static int count;
static pos_t pos_history[1024];

static int
move_point (pos_t* pos)
{
  count++;
  pos->x = cosf ((float)count / 100. * M_PI);
  pos->y = sinf ((float)count / 100. * M_PI);

  /* for debug */
  pos_history[count] = *pos;

  /* error !*/
  if (count > (int)(sizeof(pos_history)/ sizeof(pos_t)))
    return -1;

  return 0;
}

static void
dump_data_and_bss ()
{
  FILE *fptr = fopen ("data_and_bss.dump", "w");
  if (fptr == NULL)
    {
      perror ("fopen");
      return;
    }

  int ret = fwrite (DATA_SECTION_ADDR, DATA_SECTION_SIZE, 1, fptr);
  if (ret != 1)
    {
      fprintf (stderr, "DATA_SECTION_ADDR = %p (0x%lx bytes)\n", DATA_SECTION_ADDR, DATA_SECTION_SIZE);
      perror ("fwrite data section");
      goto END;
    }

  ret = fwrite (BSS_SECTION_ADDR, BSS_SECTION_SIZE, 1, fptr);
  if (ret != 1)
    {
      fprintf (stderr, "BSS_SECTION_ADDR = %p (0x%lx bytes)\n", BSS_SECTION_ADDR, BSS_SECTION_SIZE);
      perror ("fwrite bss section");
      goto END;
    }

 END:
  fclose (fptr);
}

int
main (void)
{
  while (1)
    {
      pos_t pos;
      int ret = move_point (&pos);
      if (ret < 0)
        goto ERROR;
    }

  exit (0);

 ERROR:
  dump_data_and_bss ();
  exit (1);
}
