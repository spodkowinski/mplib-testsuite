/* $Id$ */

/*
 * This file is part of mptestsuite. Copyright 2001, Stefan Podkowinski.
 *
 * mptestsuite is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 2.
 *
 * mptestsuite is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with mptestsuite; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */


#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#if STDC_HEADERS
# include <stdlib.h>
# include <string.h>
# include <stdarg.h>
#elif HAVE_STRINGS_H
# include <strings.h>
#endif /*STDC_HEADERS*/

#if HAVE_UNISTD_H
# include <unistd.h>
# include <sys/types.h>
#endif

#include <errno.h>
#include <stdio.h>
#include <fcntl.h>

#include "mplib.h"
#include "xmalloc.h"
#include "mptestsuite.h"


char* curtest;

int
mpt_write_tag(int fd, int ver) {
  int ret;
  id3_tag* tag;
  id3_tag_list *list;
  id3_content *contentabc, *contentno, *contentye, *contentgenr, *contentcomm;
  tag = mp_alloc_tag_with_version(ver);

  curtest = "Writting tag to file...";

  MPT_ASSERT_NOT_NULL(tag, "Could not create new tag", __FILE__, __LINE__);

  contentabc = mp_assemble_text_content("abc123", ISO_8859_1);
  contentno = mp_assemble_text_content("22", ISO_8859_1);
  contentye = mp_assemble_text_content("2001", ISO_8859_1);
  contentgenr = mp_assemble_text_content("Rock", ISO_8859_1);

  mp_set_content(tag, MP_ARTIST, contentabc);
  mp_set_content(tag, MP_TITLE, contentabc);
  mp_set_content(tag, MP_ALBUM, contentabc);
  mp_set_content(tag, MP_TRACK, contentno);
  mp_set_content(tag, MP_YEAR, contentye);
  mp_set_content(tag, MP_GENRE, contentgenr);
  if(ver == 1) 
      contentcomm = mp_assemble_comment_content("abc123", NULL, 
						ISO_8859_1, NULL);
      else
      contentcomm = mp_assemble_comment_content("abc123abc123"\
						"abc123abc123"\
						"abc123abc123"\
						"abc123abc123",
						NULL, ISO_8859_1,
						NULL);
  mp_set_content(tag, MP_COMMENT, contentcomm);

  mp_free_content(contentabc);
  mp_free_content(contentno);
  mp_free_content(contentye);
  mp_free_content(contentgenr);
  mp_free_content(contentcomm);

  list = (id3_tag_list*)malloc(sizeof(id3_tag_list));
  list->first = list;
  list->next = NULL;
  list->tag = tag;

  ret = mp_write_to_fd(list, fd);

  MPT_ASSERT_TRUE(!ret, "Could not write tag to file",
		  __FILE__, __LINE__);

  /* Check if values are affected */
  contentabc = mp_get_content(tag, MP_ARTIST);
  MPT_ASSERT_TRUE(strstr("abc123", mp_parse_text(contentabc)->text), "Content changed", 
		  __FILE__, __LINE__);
  contentabc = mp_get_content(tag, MP_TITLE);
  MPT_ASSERT_TRUE(strstr("abc123", mp_parse_text(contentabc)->text), "Content changed",
		  __FILE__, __LINE__);
  contentabc = mp_get_content(tag, MP_ALBUM);
  MPT_ASSERT_TRUE(strstr("abc123", mp_parse_text(contentabc)->text), "Content changed",
		  __FILE__, __LINE__);
  contentno = mp_get_content(tag, MP_TRACK);
  MPT_ASSERT_TRUE(strstr("22", mp_parse_text(contentno)->text), "Content changed",
		  __FILE__, __LINE__);
  contentye = mp_get_content(tag, MP_YEAR);
  MPT_ASSERT_TRUE(strstr("2001", mp_parse_text(contentye)->text), "Content changed",
		  __FILE__, __LINE__);
  contentgenr = mp_get_content(tag, MP_GENRE);
  MPT_ASSERT_TRUE(strstr("Rock", mp_parse_text(contentgenr)->text), "Content changed",
		  __FILE__, __LINE__);

  printf("%s%s\n", curtest, SUCCESS);

  mp_free_list(list);
  return 0;

 failed:
  mp_free_list(list);
  return 1;
}
