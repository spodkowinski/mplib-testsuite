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

char *curtest;

int
mpt_check_readable(int fd, int ver) {
  id3_tag_list *list = mp_get_tag_list_from_fd(fd);
  id3_tag* tag;

  curtest = "Checking if tag is readable...";
  MPT_ASSERT_NOT_NULL(list, "Could not get tag list from file",
		      __FILE__, __LINE__);

  /* MPT_ASSERT_NULL(list->next, "Unexpected tag", __FILE__, __LINE__); */

  tag = list->tag;
  MPT_ASSERT_NOT_NULL(tag, "Could not get tag from list",
		      __FILE__, __LINE__);

  MPT_ASSERT_TRUE(tag->version == ver, "Missmatching tag version",
		  __FILE__, __LINE__);

  mp_free_list(list);

  return 0;

 failed:
  mp_free_list(list);
  return 1;
}

int
mpt_check_v1_mplib(int fd) {
  id3_tag_list *list = NULL;
  id3_tag* tag = NULL;
  id3_content* content = NULL;
  id3_text_content* text_content = NULL;
  id3_comment_content* comment_content = NULL;

  curtest = "Reading mplib v1 tag...";

  list = mp_get_tag_list_from_fd(fd);
  tag = list->tag;

  CHECK_TEXT_CONTENT(MP_TITLE, mp_parse_title, "abc123", __FILE__, __LINE__);
  CHECK_TEXT_CONTENT(MP_ARTIST, mp_parse_artist, "abc123", __FILE__, __LINE__);
  CHECK_TEXT_CONTENT(MP_ALBUM, mp_parse_album, "abc123", __FILE__, __LINE__);
  CHECK_TEXT_CONTENT(MP_YEAR, mp_parse_year, "2001", __FILE__, __LINE__);
  CHECK_TEXT_CONTENT(MP_GENRE, mp_parse_genre, "Rock", __FILE__, __LINE__);
  CHECK_COMMENT_CONTENT("abc123", __FILE__, __LINE__);
  
  mp_free_list(list);
  mp_free_text_content(text_content);
  mp_free_comment_content(comment_content);
  mp_free_content(content);
  
  printf("%s%s\n", curtest, SUCCESS);
  return 0;

 failed:
  mp_free_list(list);
  mp_free_text_content(text_content);
  mp_free_comment_content(comment_content);
  mp_free_content(content);

  return 1;

}

int
mpt_check_v11_mplib(int fd) {
  id3_tag_list *list = NULL;
  id3_tag* tag = NULL;
  id3_content* content = NULL;
  id3_text_content* text_content = NULL;
  id3_comment_content* comment_content = NULL;

  curtest = "Reading mplib v1.1 tag...";

  list = mp_get_tag_list_from_fd(fd);
  MPT_ASSERT_NOT_NULL(list, "Could not get tag list from file",
		      __FILE__, __LINE__);
  tag = list->tag;

  CHECK_TEXT_CONTENT(MP_TITLE, mp_parse_title, "abc123", __FILE__, __LINE__);
  CHECK_TEXT_CONTENT(MP_ARTIST, mp_parse_artist, "abc123", __FILE__, __LINE__);
  CHECK_TEXT_CONTENT(MP_ALBUM, mp_parse_album, "abc123", __FILE__, __LINE__);
  CHECK_TEXT_CONTENT(MP_YEAR, mp_parse_year, "2001", __FILE__, __LINE__);
  CHECK_TEXT_CONTENT(MP_GENRE, mp_parse_genre, "Rock", __FILE__, __LINE__);
  CHECK_TEXT_CONTENT(MP_TRACK, mp_parse_track, "22", __FILE__, __LINE__);
  CHECK_COMMENT_CONTENT("abc123", __FILE__, __LINE__);
  
  mp_free_list(list);
  mp_free_text_content(text_content);
  mp_free_comment_content(comment_content);
  mp_free_content(content);
  
  printf("%s%s\n", curtest, SUCCESS);
  return 0;

 failed:
  mp_free_list(list);
  mp_free_text_content(text_content);
  mp_free_comment_content(comment_content);
  mp_free_content(content);

  return 1;

}


int
mpt_check_v2_mplib(int fd) {
  id3_tag_list *list = NULL;
  id3_tag* tag = NULL;
  id3_content* content = NULL;
  id3_text_content* text_content = NULL;
  id3_comment_content* comment_content = NULL;

  curtest = "Reading mplib v2 tag...";

  list = mp_get_tag_list_from_fd(fd);
  tag = list->tag;

  CHECK_TEXT_CONTENT(MP_TITLE, mp_parse_title, "abc123", __FILE__, __LINE__);
  CHECK_TEXT_CONTENT(MP_ARTIST, mp_parse_artist, "abc123", __FILE__, __LINE__);
  CHECK_TEXT_CONTENT(MP_ALBUM, mp_parse_album, "abc123", __FILE__, __LINE__);
  CHECK_TEXT_CONTENT(MP_YEAR, mp_parse_year, "2001", __FILE__, __LINE__);
  CHECK_TEXT_CONTENT(MP_GENRE, mp_parse_genre, "Rock", __FILE__, __LINE__);
  CHECK_TEXT_CONTENT(MP_TRACK, mp_parse_track, "22", __FILE__, __LINE__);
  CHECK_COMMENT_CONTENT("abc123abc123abc123abc123abc123abc123abc123abc123",
			__FILE__, __LINE__);
  
  mp_free_list(list);
  mp_free_text_content(text_content);
  mp_free_comment_content(comment_content);
  mp_free_content(content);
  
  printf("%s%s\n", curtest, SUCCESS);
  return 0;

 failed:
  mp_free_list(list);
  mp_free_text_content(text_content);
  mp_free_comment_content(comment_content);
  mp_free_content(content);

  return 1;

}


int
mpt_check_v2_winamp(int fd) {
  id3_tag_list *list = NULL;
  id3_tag* tag = NULL;
  id3_content* content = NULL;
  id3_text_content* text_content = NULL;
  id3_comment_content* comment_content = NULL;

  curtest = "Reading winamp 2.73 v2 tag...";

  list = mp_get_tag_list_from_fd(fd);
  tag = list->tag;

  CHECK_TEXT_CONTENT(MP_TITLE, mp_parse_title, "abc123", __FILE__, __LINE__);
  CHECK_TEXT_CONTENT(MP_ARTIST, mp_parse_artist, "abc123", __FILE__, __LINE__);
  CHECK_TEXT_CONTENT(MP_ALBUM, mp_parse_album, "abc123", __FILE__, __LINE__);
  CHECK_TEXT_CONTENT(MP_YEAR, mp_parse_year, "2001", __FILE__, __LINE__);
  CHECK_TEXT_CONTENT(MP_GENRE, mp_parse_genre, "(17)Rock", __FILE__, __LINE__);
  CHECK_TEXT_CONTENT(MP_TRACK, mp_parse_track, "22", __FILE__, __LINE__);
  CHECK_COMMENT_CONTENT("abc123", __FILE__, __LINE__);
  
  mp_free_list(list);
  mp_free_text_content(text_content);
  mp_free_comment_content(comment_content);
  mp_free_content(content);
  
  printf("%s%s\n", curtest, SUCCESS);
  return 0;

 failed:
  mp_free_list(list);
  mp_free_text_content(text_content);
  mp_free_comment_content(comment_content);
  mp_free_content(content);

  return 1;

}

int
mpt_check_v2_mmjukebox(int fd) {
  id3_tag_list *list = NULL;
  id3_tag* tag = NULL;
  id3_content* content = NULL;
  id3_text_content* text_content = NULL;
  id3_comment_content* comment_content = NULL;

  curtest = "Reading music match jukebox 6 v2 tag...";

  list = mp_get_tag_list_from_fd(fd);
  tag = list->tag;

  CHECK_TEXT_CONTENT(MP_TITLE, mp_parse_title, "abc123", __FILE__, __LINE__);
  CHECK_TEXT_CONTENT(MP_ARTIST, mp_parse_artist, "abc123", __FILE__, __LINE__);
  CHECK_TEXT_CONTENT(MP_ALBUM, mp_parse_album, "abc123", __FILE__, __LINE__);
  CHECK_TEXT_CONTENT(MP_YEAR, mp_parse_year, "2001", __FILE__, __LINE__);
  CHECK_TEXT_CONTENT(MP_GENRE, mp_parse_genre, "Rock", __FILE__, __LINE__);
  CHECK_TEXT_CONTENT(MP_TRACK, mp_parse_track, "22", __FILE__, __LINE__);
  /* FIXME CHECK_COMMENT_CONTENT("abc123", __FILE__, __LINE__); */
  
  mp_free_list(list);
  mp_free_text_content(text_content);
  mp_free_comment_content(comment_content);
  mp_free_content(content);
  
  printf("%s%s\n", curtest, SUCCESS);
  return 0;

 failed:
  mp_free_list(list);
  mp_free_text_content(text_content);
  mp_free_comment_content(comment_content);
  mp_free_content(content);

  return 1;

}


int
mpt_check_v2_jokebox(int fd) {
  id3_tag_list *list = NULL;
  id3_tag* tag = NULL;
  id3_content* content = NULL;
  id3_text_content* text_content = NULL;
  id3_comment_content* comment_content = NULL;

  curtest = "Reading real jokebox 2 v2 tag...";

  list = mp_get_tag_list_from_fd(fd);
  tag = list->tag;

  /* this tag cannot be read by mplib because real networks decided
   * to tag files by embodying a proprietary binary object. */
  MPT_ASSERT_NULL(mp_get_content(tag, MP_TITLE), "oopps", __FILE__, __LINE__);
  MPT_ASSERT_NULL(mp_get_content(tag, MP_ARTIST), "oopps", __FILE__, __LINE__);
  MPT_ASSERT_NULL(mp_get_content(tag, MP_ALBUM), "oopps", __FILE__, __LINE__);
  MPT_ASSERT_NULL(mp_get_content(tag, MP_YEAR), "oopps", __FILE__, __LINE__);
  MPT_ASSERT_NULL(mp_get_content(tag, MP_COMMENT), "oopps", __FILE__, __LINE__);
  MPT_ASSERT_NULL(mp_get_content(tag, MP_GENRE), "oopps", __FILE__, __LINE__);
  MPT_ASSERT_NULL(mp_get_content(tag, MP_TRACK), "oopps", __FILE__, __LINE__);
  
  mp_free_list(list);
  mp_free_text_content(text_content);
  mp_free_comment_content(comment_content);
  mp_free_content(content);
  
  printf("%s%s\n", curtest, SUCCESS);
  return 0;

 failed:
  mp_free_list(list);
  mp_free_text_content(text_content);
  mp_free_comment_content(comment_content);
  mp_free_content(content);

  return 1;

}


void
mpt_check_header_v1(int fd) {

  mpeg_header* header = mp_get_mpeg_header_from_fd(fd);

  curtest = "Checking header of clip...";
  
  MPT_ASSERT_NOT_NULL(header, "Could not get header from file", __FILE__, __LINE__);
  MPT_ASSERT_TRUE(strcmp(mp_get_str_version(header), "2.5"), "Unexpected header"\
		  " version", __FILE__, __LINE__);
  /* puts(mp_get_str_version(header)); */
  
  printf("%s%s\n", curtest, SUCCESS);
  xfree(header);
  return;

 failed:
  xfree(header);

}
