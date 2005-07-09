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


#ifndef __MPTESTSUITE_H
#define __MPTESTSUITE_H


/* __BEGIN_DECLS should be used at the beginning of your declarations,
   so that C++ compilers don't mangle their names.  Use __END_DECLS at
   the end of C declarations. */
#undef __BEGIN_DECLS
#undef __END_DECLS
#ifdef __cplusplus
# define __BEGIN_DECLS extern "C" {
# define __END_DECLS }
#else
# define __BEGIN_DECLS /* empty */
# define __END_DECLS /* empty */
#endif

/* __P is a macro used to wrap function prototypes, so that compilers
   that don't understand ANSI C prototypes still work, and ANSI C
   compilers can issue warnings about type mismatches. */
#undef __P
#if defined (__STDC__) || defined (_AIX) \
        || (defined (__mips) && defined (_SYSTYPE_SVR4)) \
        || defined(WIN32) || defined(__cplusplus)
# define __P(protos) protos
#else
# define __P(protos) ()
#endif


#ifndef FAILED
# define FAILED " failed"
#endif
#ifndef SUCCESS
# define SUCCESS " success"
#endif

#define MPT_ASSERT_NOT_NULL(v, x, y, z) \
if(v == NULL) { \
printf("%s:%d - Assertation failed: %s\n",\
y, z, x);\
printf("%s%s\n", curtest, FAILED);\
goto failed;\
}

#define MPT_ASSERT_NULL(v, x, y, z) \
if(v != NULL) { \
printf("%s:%d - Assertation failed: %s\n",\
y, z, x);\
printf("%s%s\n", curtest, FAILED);\
goto failed;\
}

#define MPT_ASSERT_TRUE(v, x, y, z) \
if(!(v)) { \
printf("%s:%d - Assertation failed: %s\n",\
y, z, x);\
printf("%s%s\n", curtest, FAILED);\
goto failed;\
}

#define CHECK_TEXT_CONTENT(field, func, value, file, line) \
  mp_free_content(content);\
  mp_free_text_content(text_content);\
  content = mp_get_content(tag, field);\
  MPT_ASSERT_NOT_NULL(content, "Could not get tag content", file, line);\
  text_content = func (content);\
  MPT_ASSERT_TRUE(text_content->encoding == ISO_8859_1, "Wrong encoding type",\
                  file, line);\
  MPT_ASSERT_NOT_NULL(text_content->text, "Could not get text", file, line);\
  MPT_ASSERT_TRUE(!strcmp(text_content->text, value), "Mismatching field value",\
                  file, line);

#define CHECK_COMMENT_CONTENT(value, file, line) \
  mp_free_content(content);\
  mp_free_comment_content(comment_content);\
  content = mp_get_content(tag, MP_COMMENT);\
  MPT_ASSERT_NOT_NULL(content, "Could not get tag content", file, line);\
  comment_content = mp_parse_comment(content);\
  MPT_ASSERT_TRUE(comment_content->encoding == ISO_8859_1, "Wrong encoding type",\
                  file, line);\
  MPT_ASSERT_NOT_NULL(comment_content->text, "Could not get comment", file, line);\
  MPT_ASSERT_TRUE(!strcmp(comment_content->text, value), "Mismatching MP_COMMENT value",\
                  file, line);

__BEGIN_DECLS

int
mpt_check_v1_readable __P((int));

int
mpt_check_v1_mplib __P((int));

int
mpt_check_v11_mplib __P((int));

int
mpt_write_v1 __P((int));

int
mpt_scan __P((char*));

void
do_expr __P((char*, char*));


void
desc_dir __P((char*, unsigned int));

int
mpt_scan __P((char*));

void
do_expr __P((char*, char*));

__END_DECLS

#endif /* __MPTESTSUITE_H */
