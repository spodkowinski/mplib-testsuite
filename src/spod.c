/*
 *  Copyright (C) 2000  Stefan  Podkowinski
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public Licensse as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */


#ifdef STDC_HEADERS
# include <stdlib.h>
# include <string.h>
#elif HAVE_STRINGS_H
# include <strings.h>
#endif /*STDC_HEADERS*/

#ifdef HAVE_UNISTD_H
# include <unistd.h>
# include <sys/types.h>
#endif

#include <stdio.h>

#include "xmalloc.h"
#include "spod.h"

int
spod_in(char c, char *set)
{
  if(!set) return 1;
  if(c == set[0]) return 0;
  else spod_in(c, set+1);
}

/* 
   splits a pathname into a head and tail where head is everything up to the
   leading slash and tail the rest of the path (both may be null) 
*/
spod_path_splited*
spod_split_path(char *p)
{
  spod_path_splited *sps;
  char *i;
  size_t plen;

  if(!p) return NULL;

  sps = (spod_path_splited*)xmallocd(sizeof(spod_path_splited), "spod");
  sps->head = NULL;
  sps->tail = NULL;

  plen = strlen(p);
  i = (char *)rindex(p, '/');
  
  if(!i)
    {
      sps->tail = (char*)xmallocd(plen+1, "spod");
      strcpy(sps->tail, p);
      return sps;
    }

  /* Head */
  sps->head = (char*)xmallocd(i - p, "spod") + 1;
  if(i == p)
    {
      strncpy(sps->head, p, 1);
      sps->head[1] = 0;
    }
  else 
    {
      strncpy(sps->head, p, i - p);
      sps->head[i-p] = 0;
    }

  /* Tail */
  if(p + plen-1 != i)
    {
      sps->tail = (char*)xmallocd(plen + p - i + 1, "spod");
      strncpy(sps->tail, i+1, plen + p - i);
    }

  return sps;
}

char*
spod_trim(char* c)
{

  char *start, *end, *ret;

  start = c;
  end = c + strlen(c);

  while(isspace(*start)) start++;
  while(isspace(*(end-1)) && end > start) end--;

  ret = (char*)xmallocd(end - start + 1, "spod");
  memcpy(ret, start, end - start);
  ret[end - start] = 0;

  return ret;

}
