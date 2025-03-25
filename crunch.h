#ifndef __CRUNCH-RW__
#define __CRUNCH-RW__

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
