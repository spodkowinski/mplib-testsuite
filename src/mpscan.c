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
# include <stdarg.h>
# include <string.h>
#elif HAVE_STRINGS_H
# include <strings.h>
#endif /*STDC_HEADERS*/

#if HAVE_UNISTD_H
# include <unistd.h>
# include <sys/types.h>
#endif

#ifndef NAME_MAX
# define NAME_MAX 48
#endif

#include <errno.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <dirent.h>

#include "spod.h"
#include "mplib.h"
#include <xmalloc.h>
#include "mptestsuite.h"

typedef struct _proc_list
{
    ino_t inode;
    void *next;
} proc_list;

typedef struct _results
{
    int checked;
    char** bad_header;
    char** strange_header;
    char** missing_header;
} results;

char *curtest;
proc_list *first_elem, *last_elem;
results* res;

#define MAXDEPTH 15

int
mpt_scan(char* file) {
    char cwd[NAME_MAX+1];
    spod_path_splited* sp;
    struct stat stats;
    res = XMALLOCD0(results, "ignore");

    if(stat(file, &stats) == -1)
    {
	printf("Could not get stats for %s\n", file);
	perror(file);
	exit(1);
    }

    printf("Going to check recursivly %s\n", file);

    if(access(file, F_OK | R_OK))
    {
	perror(file);
	return -1;
    }
    
    stats.st_mode &= S_IFMT;
    if((stats.st_mode & S_IFREG) == S_IFREG)
    {
	puts("Target is a regular file. Doing expr. and perish.");
	
	sp = spod_split_path(file);
	do_expr(sp->head, sp->tail);
	exit(0);
    }
    
    if(access(file, F_OK | X_OK))
    {
	perror(file);
	return -1;
    }
    
    /* go for it */
    desc_dir(file, 0);

    //xprint_malloc_stat();

    return 0;

 failed:
    return 1;
}

/* This function is recursivly scaning a directory and all file in it for a match */
void
desc_dir(char *dir, unsigned int parent_depth)
{
    DIR *dir_struct = NULL;
    struct dirent *entry;

#ifdef DEBUG
    assert(dir && parent_depth >= 0);
#endif
    
    /* Check max depth */
    if(MAXDEPTH > 0 && parent_depth > MAXDEPTH)
    {
#ifdef DEBUG
	printf("DEBUG: Max depth prevents mpfind descending into level %d (trying to scan %s)\n", parent_depth, dir);
#endif
	return;
    }
    ++parent_depth;
    
    /* Change dir */
    if(chdir(dir) == -1)
    {
#ifdef DEBUG
	puts("DEBUG: Could not change directory");
#endif
	perror(dir);
	return;
    }
    
    dir_struct = opendir(dir);
    if(!dir_struct) perror(dir);

  /* Looping through every dir-entry and examine file or descend down further */
    while(1)
    {
	struct stat stats;
	
	entry = readdir(dir_struct);

	if(!entry)
	{
#ifdef DEBUG
	    puts("DEBUG: EOF");
#endif
	    /* perror(dir);*/
	    break;
	}
	
	/* skipping . and .. */
	if(strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
	{
	    continue;
	}
	
#ifdef DEBUG
	printf("DEBUG: Checking dir entry %s\n", entry->d_name);
#endif
	
      /* Get stats for entry */
	if(stat(entry->d_name, &stats) == -1)
	{
	    char *fullfilename = xmallocd(NAME_MAX + 1, "ignore");
#ifdef DEBUG
	    printf("DEBUG: Could not get stats for %s\n", entry->d_name);
#endif
	    if(dir[strlen(dir)-1] == '/') snprintf(fullfilename, NAME_MAX, "%s%s", dir, entry->d_name);
	    else snprintf(fullfilename, NAME_MAX, "%s/%s", dir, entry->d_name);
	    perror(fullfilename);
	    xfree(fullfilename);
	    continue;
	}

	/* init inode list if necessary and check if inode was allready scanned */
	if(!first_elem)
	{
	    /* First run, eh? */
#ifdef DEBUG
	    puts("DEBUG: Setting up list of scanned inodes");
#endif
	    last_elem = xmallocd(sizeof(proc_list), "ignore");
	    last_elem->inode = stats.st_ino;
	    last_elem->next = NULL;
	    first_elem = last_elem;
	}
	else
	{
	    proc_list *ptr;
	    int skip = 0;
	    
#ifdef DEBUG
	    assert(last_elem && first_elem);
#endif
	    
	    ptr = first_elem;
	    while(ptr)
	    {
		if(stats.st_ino == ptr->inode)
		{
#ifdef DEBUG
		    puts("DEBUG: Inode allready scanned, skipping");
#endif
		    skip = 1;
		    break;
		}
		ptr = (proc_list*)ptr->next;
	    }
	    if(skip) continue;
	    
	    /* Add this inode to the end of the list */
	    last_elem->next = xmallocd(sizeof(proc_list), "ignore");
	    last_elem = (proc_list*)last_elem->next;
	    last_elem->inode = stats.st_ino;
	    last_elem->next = NULL;
	}
	
	
	if(access(entry->d_name, F_OK | R_OK) == -1)
	{
	    char *fullfilename = xmallocd(NAME_MAX + 1, "ignore");
	    if(dir[strlen(dir)-1] == '/') snprintf(fullfilename, NAME_MAX, "%s%s", dir, entry->d_name);
	    else snprintf(fullfilename, NAME_MAX, "%s/%s", dir, entry->d_name);
	    perror(fullfilename);
	    continue;
	}
	
	
#ifdef DEBUG
	printf("DEBUG: Scanning inode %d\n", stats.st_ino);
#endif
	
	/* Checking stats and decide what to do */
	stats.st_mode &= S_IFMT;
	if((stats.st_mode & S_IFLNK) == S_IFLNK)
	{
#ifdef DEBUG
	    puts("DEBUG: inode is symbolic link, going to ignore for now..");
#endif
	    continue;
	}
	if((stats.st_mode & S_IFREG) == S_IFREG)
	{
#ifdef DEBUG
	    puts("DEBUG: inode is a regular file.");
#endif
	    
	    if(stats.st_size > 128)
	    {
		do_expr(dir, entry->d_name);
	    }
#ifdef DEBUG
	    else
	    {
		puts("DEBUG: File has invalid size to have a tag");
	    }
#endif
	}
	else if((stats.st_mode & S_IFDIR) == S_IFDIR)
	{
	    char *todesc;
#ifdef DEBUG
	    puts("DEBUG: inode is a directory");
#endif
	    if(access(entry->d_name, R_OK | X_OK) == -1) 
	    {
		perror(entry->d_name);
	    }
	    else
	    {
		todesc = xmallocd(NAME_MAX + 1, "ignore");
		memset(todesc, 0, NAME_MAX+1);
		if(dir[strlen(dir)-1] == '/') snprintf(todesc, NAME_MAX+1, "%s%s", dir, entry->d_name);
		else snprintf(todesc, NAME_MAX+1, "%s/%s", dir, entry->d_name);
		desc_dir(todesc, parent_depth) ;
		
		/* Change back to our dir */
		if(chdir(dir) == -1)
		{
#ifdef DEBUG
		    puts("DEBUG: Could not change directory");
#endif
		    perror(dir);
		    return;
		}
	    }
	}
    }
    
    closedir(dir_struct);
}


void
do_expr(char *dir, char *file) 
{
  id3_tag_list *list, *fstlist;
  id3_tag *tag;
  mpeg_header *head;

  printf("Checking tag of file %s\n", file);

  res->checked++;
  head = mp_get_mpeg_header_from_file(file);
  if(!head) {
      printf("W A R N I N G: Cannot get mpeg header for file %s\n", file);
  }
  // TODO

  xfree(head);

  /* Get tag list */
  if((fstlist = list = mp_get_tag_list_from_file(file)))
  {
      id3_content* cont;
      id3_text_content* tcont;
      id3_comment_content* ccont;
      do {
	  char* curver = (list->tag->version == 2 ? "v2" : "v1");
	  tag = list->tag;
	  if(!tag) {
	      printf("%s: empty tag as list element\n");
	  }
	  cont = mp_get_content(tag, MP_ARTIST);
	  if(cont) {
	      tcont = mp_parse_artist(cont);
	      if(!tcont)
		  printf("%s: artist text content is null", file);
	      else
		  if(!tcont->text)
		      printf("%s: artist string provided with value null", file);
		  else
		      printf("artist %s: %s\n", curver, tcont->text);
	      
	      mp_free_text_content(tcont);
	      mp_free_content(cont);
	  }

	  /* Beware! C&P code */

	  cont = mp_get_content(tag, MP_TITLE);
	  if(cont) {
	      tcont = mp_parse_title(cont);
	      if(!tcont)
		  printf("E R R O R: title text content is null", file);
	      else
		  if(!tcont->text)
		      printf("E R R O R: artist string provided with value null", file);
		  else
		      printf("title %s: %s\n", curver, tcont->text);
	      
	      mp_free_text_content(tcont);
	      mp_free_content(cont);
	  }

	  cont = mp_get_content(tag, MP_ALBUM);
	  if(cont) {
	      tcont = mp_parse_album(cont);
	      if(!tcont)
		  printf("E R R O R: album text content is null", file);
	      else
		  if(!tcont->text)
		      printf("E R R O R: album string provided with value null", file);
		  else
		      printf("album %s: %s\n", curver, tcont->text);
	      
	      mp_free_text_content(tcont);
	      mp_free_content(cont);
	  }

	  cont = mp_get_content(tag, MP_YEAR);
	  if(cont) {
	      tcont = mp_parse_year(cont);
	      if(!tcont)
		  printf("E R R O R: year text content is null", file);
	      else
		  if(!tcont->text)
		      printf("E R R O R: year string provided with value null", file);
		  else
		      printf("year %s: %s\n", curver, tcont->text);
	      
	      mp_free_text_content(tcont);
	      mp_free_content(cont);
	  }

	  cont = mp_get_content(tag, MP_TRACK);
	  if(cont) {
	      tcont = mp_parse_track(cont);
	      if(!tcont)
		  printf("E R R O R: track text content is null", file);
	      else
		  if(!tcont->text)
		      printf("E R R O R: track string provided with value null", file);
		  else
		      printf("track %s: %s\n", curver, tcont->text);
	      
	      mp_free_text_content(tcont);
	      mp_free_content(cont);
	  }

	  cont = mp_get_content(tag, MP_GENRE);
	  if(cont) {
	      tcont = mp_parse_genre(cont);
	      if(!tcont)
		  printf("E R R O R: genre text content is null", file);
	      else
		  if(!tcont->text)
		      printf("E R R O R: genre string provided with value null", file);
		  else
		      printf("genre %s: %s\n", curver, tcont->text);
	      
	      mp_free_text_content(tcont);
	      mp_free_content(cont);
	  }

	  cont = mp_get_content(tag, MP_COMMENT);
	  if(cont) {
	      ccont = mp_parse_comment(cont);
	      if(!ccont)
		  printf("E R R O R: comment text content is null", file);
	      else
		  if(!ccont->text)
		      printf("E R R O R: comment string provided with value null", file);
		  else
		      printf("comment %s: %s\n", curver, ccont->text);
	      
	      mp_free_comment_content(ccont);
	      mp_free_content(cont);
	  }

	  
      } while((list = list->next));

      mp_free_list(fstlist);
  }
  else
  {
      puts("No tags found");
  }
}
