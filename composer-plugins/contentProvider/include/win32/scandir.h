#ifndef scandir_h
#define scandir_h

#ifdef WIN32
#include <iostream>
#include <string>

#ifdef __cplusplus
extern "C" {
#endif

#include <dirent.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#ifndef SCANDIR
#define SCANDIR scandir
#define READDIR readdir
#define DIRENT_TYPE struct dirent
#endif

#if !defined(_D_ALLOC_NAMLEN)
#if defined(__svr4__) || defined(mips)
#define _D_ALLOC_NAMLEN(DP) ((DP)->d_reclen)
#else
#define _D_ALLOC_NAMLEN(DP) ((DP)->d_namlen)
#endif
#endif

long getFileSize(char *filePath) { 
	FILE * fp;
	long lngSize;

	if( (fp = fopen ( filePath, "rb" )) == NULL) {
		exit(1);
	}
 
	fseek(fp, 0, SEEK_END); 
	lngSize = ftell(fp); 
	rewind(fp); // paranoia 
	fclose(fp); 
	return lngSize; 
}

int
SCANDIR (
     const char *dir,
     DIRENT_TYPE ***namelist,
     int (*select) (const DIRENT_TYPE *),
     int (*cmp) (const void *, const void *))
{
  DIR *dp = opendir (dir);
  DIRENT_TYPE **v = NULL;
  size_t vsize = 0, i;
  DIRENT_TYPE *d;
  int save;

  if (dp == NULL)
    return -1;

  save = errno;
  errno = 0;

  i = 0;
  while ((d = READDIR (dp)) != NULL) {
    if (select == NULL || (*select) (d)) {
    	DIRENT_TYPE *vnew = NULL;
    	size_t dsize;

    	/* Ignore errors from select or readdir */
    	errno = 0;

    	if (__builtin_expect (i == vsize, 0)) {
    		DIRENT_TYPE **cnew = NULL;
    		if (vsize == 0) {
    			vsize = 10;
    		} else {
    			vsize *= 2;
    		}
    		cnew = (DIRENT_TYPE **) realloc (v, vsize * sizeof (*v));
    		if (cnew == NULL) {
    			break;
    		}
    		v = cnew;
    	}

    	//dsize = &d->d_name[_D_ALLOC_NAMLEN (d)] - (char *) d;
    	dsize = sizeof (DIRENT_TYPE);
    	vnew = (DIRENT_TYPE *) malloc (dsize);
    	if (vnew == NULL) {
    		break;
    	}

    	v[i++] = (DIRENT_TYPE *) memcpy (vnew, d, dsize);
    }
  }

  if (__builtin_expect (errno, 0) != 0)
    {
      save = errno;

      while (i > 0)
	free (v[--i]);
      free (v);

      i = (size_t)-1;
    }
  else
    {
      /* Sort the list if we have a comparison function to sort with.  */
      if (cmp != NULL)
	qsort (v, i, sizeof (*v), cmp);

      *namelist = v;
    }

  (void) closedir (dp);
  errno = save;

  return i;
}

#ifdef __cplusplus
}
#endif /*__cplusplus*/
#endif /*WIN32*/
#endif /*scandir_h*/
