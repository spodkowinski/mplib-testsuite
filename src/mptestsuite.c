/* $Id$ */

/*
 * This file is part of mptestsuite. Copyright 2001,2002, Stefan Podkowinski.
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

#include <dirent.h>
#include <errno.h>
#include <stdio.h>
#include <fcntl.h>
#include <assert.h>
#include "mplib.h"

#include "mptestsuite.h"

int results_ok = 0;
int results_failed = 0;

int curfd;

int
main(int argc, char** args) {

    if(getopt(argc, args, "s:") != -1 && optarg) {
	printf("Scanning directory %s...\n", optarg);
	mpt_scan(optarg);
	puts("Done.");
	exit(0);
    }

  puts("\n\nmplib testsuite starting...\n\n");
  puts("This will run a number of tests that will check\nif mplib was correctly build"\
       " and works as excpected.\n\n");
  puts(" => Testing tag reading capabilities\n\n");

  curfd = open("r_mplib_v1.mp3", O_RDONLY);
  if(!mpt_check_readable(curfd, 1)) {
      mpt_check_header_v1(curfd);
      mpt_check_v1_mplib(curfd);
  }
  close(curfd);

  curfd = open("r_mplib_v11.mp3", O_RDONLY);
  if(!mpt_check_readable(curfd, 1)) {
    mpt_check_v11_mplib(curfd);
  }
  close(curfd);

  curfd = open("r_mplib_v2.mp3", O_RDONLY);
  if(!mpt_check_readable(curfd, 2)) {
    mpt_check_v2_mplib(curfd);
  }
  close(curfd);

  curfd = open("r_winamp_v2.mp3", O_RDONLY);
  if(!mpt_check_readable(curfd, 2)) {
    mpt_check_v2_winamp(curfd);
  }
  close(curfd);

  curfd = open("r_music_match_jukebox_v2.mp3", O_RDONLY);
  if(!mpt_check_readable(curfd, 2)) {
      mpt_check_v2_mmjukebox(curfd);
  }
  close(curfd);

  puts("\n => Testing tag writing capabilities\n\n");

  system("cp clip.mp3 w_mplib_v1.mp3; cp clip.mp3 w_mplib_v2.mp3");
  
  curfd = open("w_mplib_v1.mp3", O_RDWR);
  if(!mpt_write_tag(curfd, 1)) {
    mpt_check_v11_mplib(curfd);
  }
  close(curfd);

  curfd = open("w_mplib_v2.mp3", O_RDWR);
  if(!mpt_write_tag(curfd, 2)) {
      mpt_check_v2_mplib(curfd);
  }
  close(curfd);

  //unlink("w_mplib_v1.mp3");
  //unlink("w_mplib_v2.mp3");

  //xprint_malloc_stat();

  exit(0);
}


