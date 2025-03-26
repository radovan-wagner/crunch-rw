#ifndef __CRUNCH_RW__
#define __CRUNCH_RW__

#include <assert.h>
#include <locale.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <ctype.h>
#include <errno.h>
#include <signal.h>
#include <math.h>
#include <pthread.h>
#include <unistd.h>
#include <limits.h>
#include <sys/wait.h>
#include <sys/types.h>

struct thread_data {
	unsigned long long finalfilesize; /* total size of output */
	unsigned long long bytetotal;  /* total number of bytes so far */
	unsigned long long bytecounter; /* count number of bytes in output resets to 0 */
	unsigned long long finallinecount; /* total size of output */
	unsigned long long linetotal; /* total number of lines so far */
	unsigned long long linecounter; /* counts number of lines in output resets to 0 */
};

/* pattern info */
struct pinfo {
	wchar_t* cset; /* character set pattern[i] is member of */
	size_t clen;
	int is_fixed; /* whether pattern[i] is a fixed value */
	size_t start_index, end_index; /* index into cset for the start and end strings */
	size_t duplicates;
};

/* program options */
struct opts_struct {
	wchar_t* low_charset;
	wchar_t* upp_charset;
	wchar_t* num_charset;
	wchar_t* sym_charset;
	size_t clen, ulen, nlen, slen;
	wchar_t* pattern;
	size_t plen;
	wchar_t* literalstring;
	wchar_t* startstring;
	wchar_t* endstring;
	size_t duplicates[4]; /* allowed number of duplicates for each charset */

	size_t min, max;

	wchar_t* last_min;  /* last string of length min */
	wchar_t* first_max; /* first string of length max */
	wchar_t* min_string;
	wchar_t* max_string; /* either startstring/endstring or calculated using the pattern */

	struct pinfo* pattern_info; /* information generated from pattern */
};
typedef struct opts_struct options_type;

static int wcstring_cmp(const void *a, const void *b);
static size_t force_wide_string(wchar_t *wout, const char *s, size_t n);
static size_t make_wide_string(wchar_t *wout, const char *s, size_t n, int* r_is_unicode);
static wchar_t *alloc_wide_string(const char *s, int* r_is_unicode);
static size_t force_narrow_string(char *out, const wchar_t* src, size_t n);
static size_t make_narrow_string(char *out, const wchar_t* src, size_t n);
static int getmblen(wchar_t wc);
static wchar_t *dupwcs(const wchar_t *s);

/* return 0 if string1 does not comply with options.pattern and options.literalstring */
static int check_member(const wchar_t *string1, const options_type* options);

/* NOTE: similar to strpbrk but length limited and only searches for a single char */
static void fill_minmax_strings(options_type *options);
static void fill_pattern_info(options_type *options);
static unsigned long long fill_next_count(size_t si, size_t ei, int repeats, unsigned long long sum, /*@null@*/ unsigned long long *current_count, /*@out@*/ unsigned long long *next_count, size_t len);

/* calculate the number of strings from start to end taking into account duplicate removal worst case: O(2^n), could be improved using memoize */
static unsigned long long calculate_with_dupes(int start_point, int end_point, size_t first, size_t last, size_t pattern_index, int repeats, unsigned long long current_sum, /*@null@*/ unsigned long long *current_count, const options_type options, size_t plen);

/* calculate the number of strings from start to end O(n)  */
static unsigned long long calculate_simple(const wchar_t *startstring, const wchar_t *endstring, const wchar_t *cset, size_t clen);

/* calculate the number of strings from start to end, inclusive, taking into account the pattern O(n) */
static unsigned long long calculate_with_pattern(const wchar_t *startstring, const wchar_t *endstring, const options_type options);

/* calculate the number of lines and bytes to output */
static void count_strings(unsigned long long *lines, unsigned long long *bytes, const options_type options);

static int finished(const wchar_t *block, const options_type options);
static int too_many_duplicates(const wchar_t *block, const options_type options);
static void increment(wchar_t *block, const options_type options);
static void *PrintPercentage(void *threadarg);
static void renamefile(const size_t end, const char *fpath, const char *outputfilename, const char *compressalgo);
static void Permute(const char *fpath, const char *outputfilename, const char *compressalgo, wchar_t **wordarray, const options_type options, const size_t sizePerm, size_t unchanged);
static void Permutefilesize(wchar_t **wordarray, const size_t sizePerm, const size_t length, size_t unchanged);
static void loadstring(wchar_t *block2, const size_t j, const wchar_t *startblock, const options_type options);
static void chunk(const size_t start, const size_t end, const wchar_t *startblock, const options_type options, const char *fpath, const char *outputfilename, const char *compressalgo);
static void usage();
static wchar_t *resumesession(const char *fpath, const wchar_t *charset);
static wchar_t *readcharsetfile(const char *charfilename, const char *charsetname, int* r_is_unicode);
static wchar_t **readpermute(const char *charfilename, int* r_is_unicode);
static void copy_without_dupes(wchar_t* dest, wchar_t* src);

#endif
