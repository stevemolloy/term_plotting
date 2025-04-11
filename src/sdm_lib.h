#ifndef _SDM_LIB_H
#define _SDM_LIB_H

#include <ctype.h>
#include <errno.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* This library provides the following:
 *
 * char *sdm_shift_args(int *argc, char ***argv);      Peel arguments off the **argv array typically provided to main, decrementing argc appropriately.
 * char *sdm_read_entire_file(const char *file_path);  Read the contents of a file into a character array. This character array is malloc'ed and so should be freed by the user.
 * SDM_FREE_AND_NULL(ptr)                              Free the memory pointed to by ptr, and then set ptr to NULL.
 * #define SDM_FREE SDM_FREE_AND_NULL
 * #define SDM_MALLOC malloc
 * 
 * # DYNAMIC ARRAYS
 * ================
 * SDM_ENSURE_ARRAY_CAP(da, cap)       Ensure that the dynamic array, da, has a capacity equal-to cap. Realloc is used if needed.
 * SDM_ENSURE_ARRAY_MIN_CAP(da, cap)   Ensure that the dynamic array, da, has a capacity equal-to or greater-than cap. Realloc is used if needed.
 * DEFAULT_CAPACITY 128                Default capacity in bytes to be used when the capacity has not been set by the user.
 * SDM_ARRAY_PUSH(da, item)            Push the value of item to the dynamic array, da, reallocing if needed.
 * SDM_ARRAY_SWAP(da, ind1, ind2)      Swap the elements at the marked indices (if length==capacity this will extend the array)
 * SDM_ARRAY_FREE(da)                  Free the memory in the dynamic array, da, and zero things
 * SDM_ARRAY_RESET(da)                 Reset the length of the dynamic array, da, to zero, effectively emptying it. No memory is freed by this.
 * 
 * # STRING VIEWS
 * ==============
 * sdm_string_view sdm_cstr_as_sv(char *cstr);                                 Return a string view containing the provided c-string
 * sdm_string_view sdm_sized_str_as_sv(char *cstr, size_t length);             Return a string view containing the c-string with the provided size.
 * sdm_string_view sdm_sv_pop_by_delim(sdm_string_view *SV, const char delim); Pop off the start of the provided string view up to the given delimiter.
 * void sdm_sv_trim(sdm_string_view *SV);                                      Trim any whitespace from the start of the string view
 * SDM_SV_F "%.*s"                                                             A printf helper.
 * SDM_SV_Vals(S) (int)(S).length, (S).data                                    A printf helper.
 * 
 */

#ifndef SDM_MALLOC
#define SDM_MALLOC malloc
#endif

#ifndef SDM_REALLOC
#define SDM_REALLOC realloc
#endif

#define SDM_FREE_AND_NULL(ptr) \
do {                           \
  if ((ptr)) free((ptr));      \
  (ptr) = NULL;                \
} while (0)

#define SDM_FREE SDM_FREE_AND_NULL

#define SDM_ENSURE_ARRAY_CAP(da, cap) do {                     \
    (da).capacity = cap;                                       \
    (da).data = SDM_REALLOC((da).data,                             \
        (da).capacity * sizeof((da).data[0]));                 \
    if ((da).data == NULL) {                                   \
      fprintf(stderr, "ERR: Couldn't alloc memory.\n");        \
      exit(1);                                                 \
    }                                                          \
  } while (0)

#define SDM_ENSURE_ARRAY_MIN_CAP(da, cap) do {                 \
    if ((da).capacity < cap) {                                 \
      (da).capacity = cap;                                     \
      (da).data = SDM_REALLOC((da).data,                           \
          (da).capacity * sizeof((da).data[0]));               \
      if ((da).data == NULL) {                                 \
        fprintf(stderr, "ERR: Couldn't alloc memory. \n");     \
        exit(1);                                               \
      }                                                        \
    }                                                          \
  } while (0)

#define SDM_ARRAY_POP(da) (da).data[--((da).length)]

#define DEFAULT_CAPACITY 128

#define SDM_ARRAY_PUSH(da, item) do {                             \
    if (((da).capacity == 0) || ((da).data == NULL)) {            \
      (da).capacity = DEFAULT_CAPACITY;                           \
      (da).data = SDM_MALLOC((da).capacity * sizeof((da).data[0]));   \
      if ((da).data == NULL) {                                    \
        fprintf(stderr, "ERR: Couldn't alloc memory.\n");         \
        exit(1);                                                  \
      }                                                           \
      memset((da).data, 0, (da).capacity * sizeof((da).data[0])); \
    }                                                             \
    while ((da).length >= (da).capacity) {                        \
      (da).capacity *= 2;                                         \
      (da).data = SDM_REALLOC((da).data,                              \
           (da).capacity * sizeof((da).data[0]));                 \
      if ((da).data == NULL) {                                    \
        fprintf(stderr, "ERR: Couldn't alloc memory.\n");         \
        exit(1);                                                  \
      }                                                           \
    }                                                             \
    (da).data[(da).length++] = item;                              \
  } while (0);

#define SDM_ARRAY_SWAP(da, ind1, ind2)                                   \
do {                                                                     \
  assert((int)ind1 < (int)da.length && "First index is out of bounds");  \
  assert((int)ind2 < (int)da.length && "Second index is out of bounds"); \
  if (ind1 != ind2) {                                                    \
    SDM_ARRAY_PUSH(da, da.data[ind1]);                                   \
    da.data[ind1] = da.data[ind2];                                       \
    da.data[ind2] = da.data[da.length - 1];                              \
    da.length--;                                                         \
  }                                                                      \
} while (0)

#define SDM_ARRAY_FREE(da) do {                                \
    SDM_FREE((da).data);                                           \
    (da).length = 0;                                           \
    (da).capacity = 0;                                         \
  } while (0);

#define SDM_ARRAY_RESET(da) do { (da).length = 0; } while (0)

typedef struct {
  size_t length;
  char *data;
} sdm_string_view;

#define SDM_SV_F "%.*s"
#define SDM_SV_Vals(S) (int)(S).length, (S).data

char *sdm_shift_args(int *argc, char ***argv);

char *sdm_read_entire_file(const char *file_path);

sdm_string_view sdm_cstr_as_sv(char *cstr);
char *sdm_sv_to_cstr(sdm_string_view sv);
sdm_string_view sdm_sized_str_as_sv(char *cstr, size_t length);
sdm_string_view sdm_sv_pop_by_length(sdm_string_view *SV, size_t len);
sdm_string_view sdm_sv_pop_by_delim(sdm_string_view *SV, const char delim);
sdm_string_view sdm_sv_pop_line(sdm_string_view *SV);
sdm_string_view sdm_sv_pop_by_whitespace(sdm_string_view *SV);
int sdm_sv_pop_integer(sdm_string_view *SV);
double sdm_sv_pop_double(sdm_string_view *SV);
char sdm_sv_pop_one_char(sdm_string_view *SV);
void sdm_sv_trim(sdm_string_view *SV);
int sdm_svncmp(sdm_string_view SV, const char *cmp);
bool sdm_sv_compare(sdm_string_view SV1, sdm_string_view SV2);

typedef struct {
  char key[32];
  double value;
  bool occupied;
} Dbl_KVPair;

typedef struct {
  Dbl_KVPair *data;
  size_t length;
  size_t capacity;
} DblArray;

#define DEFAULT_HM_CAP 256

#define SET_HM_CAPACITY(hm, cap)                                              \
  do {                                                                        \
    (hm)->capacity = (cap);                                                   \
    if ((hm)->data == NULL) {                                                 \
      (hm)->data = SDM_MALLOC(hm->capacity * sizeof(hm->data[0]));                \
    }                                                                         \
    else {                                                                    \
      (hm)->data = SDM_REALLOC((hm)->data, (hm)->capacity * sizeof(hm->data[0])); \
    }                                                                         \
    if ((hm)->data == NULL) {                                                 \
      fprintf(stderr, "ERR: Can't alloc.\n");                                 \
      exit(1);                                                                \
    }                                                                         \
    memset((hm)->data, 0, (hm)->capacity * sizeof(hm->data[0]));              \
  } while (0)

#define FREE_HASHMAP(hm) \
  do {                   \
    (hm).capacity = 0;   \
    SDM_FREE((hm).data);     \
  } while (0);

typedef enum {
  SDM_LOG_ERR,
  SDM_LOG_WARN,
  SDM_LOG_INFO,
} LogLevel;

#endif /* ifndef _SDM_LIB_H */

#ifdef SDM_LIB_IMPLEMENTATION

char *sdm_read_entire_file(const char *file_path) {
  // Reads an entire file into a char array, and returns a ptr to this. The ptr should be freed by the caller
  FILE *f = fopen(file_path, "r");
  if (f==NULL) {
    fprintf(stderr, "Could not read %s: %s\n", file_path, strerror(errno));
    exit(1);
  }

  fseek(f, 0L, SEEK_END);
  int sz = ftell(f);
  fseek(f, 0L, SEEK_SET);

  char *contents = SDM_MALLOC((sz + 1) * sizeof(char));
  if (contents==NULL) {
    fprintf(stderr, "Could not allocate memory. Buy more RAM I guess?\n");
    exit(1);
  }
  fread(contents, 1, sz, f);

  fclose(f);
  
  return contents;
}

sdm_string_view sdm_cstr_as_sv(char *cstr) {
  return (sdm_string_view){
    .data = cstr,
    .length = strlen(cstr)
  };
}

char *sdm_sv_to_cstr(sdm_string_view sv) {
  char *ret = SDM_MALLOC(sv.length + 1);
  memset(ret, 0, sv.length + 1);
  memcpy(ret, sv.data, sv.length);
  return ret;
}

sdm_string_view sdm_sized_str_as_sv(char *cstr, size_t length) {
  return (sdm_string_view) {
    .data = cstr,
    .length = length
  };
}

sdm_string_view sdm_sv_pop_by_length(sdm_string_view *SV, size_t len) {
  sdm_string_view retval = (sdm_string_view) {
    .data = SV->data,
    .length = len,
  };
  SV->data += len;
  SV->length -= len;
  return retval;
}

sdm_string_view sdm_sv_pop_by_whitespace(sdm_string_view *SV) {
  sdm_string_view ret = {0};
  ret.data = SV->data;

  while (!isspace(*SV->data) && (SV->length>0)) {
    SV->data++;
    SV->length--;
    ret.length++;
  }
  SV->data++;
  SV->length--;

  return ret;
}

sdm_string_view sdm_sv_pop_by_delim(sdm_string_view *SV, const char delim) {
  sdm_string_view ret = {0};
  ret.data = SV->data;

  while ((*SV->data != delim) && (SV->length>0)) {
    SV->data++;
    SV->length--;
    ret.length++;
  }
  SV->data++;
  SV->length--;

  return ret;
}

sdm_string_view sdm_sv_pop_line(sdm_string_view *SV) {
    return sdm_sv_pop_by_delim(SV, '\n');
}

int sdm_sv_pop_integer(sdm_string_view *SV) {
  char *new_pos;
  int retval = strtol(SV->data, &new_pos, 0);
  size_t diff = new_pos - SV->data;
  SV->data = new_pos;
  SV->length -= diff;
  return retval;
}

double sdm_sv_pop_double(sdm_string_view *SV) {
  char *new_pos;
  double retval = strtod(SV->data, &new_pos);
  size_t diff = new_pos - SV->data;
  SV->data = new_pos;
  SV->length -= diff;
  return retval;
}

char sdm_sv_pop_one_char(sdm_string_view *SV) {
  char retval = SV->data[0];
  SV->data++;
  SV->length--;
  return retval;
}

void sdm_sv_trim(sdm_string_view *SV) {
  while (isspace(*SV->data) && SV->length>0) {
    SV->data++;
    SV->length--;
  }
}

int sdm_svncmp(sdm_string_view SV, const char *cmp) {
  size_t n = strlen(cmp) < SV.length ? strlen(cmp) : SV.length;
  return strncmp(SV.data, cmp, n);
}

char *sdm_shift_args(int *argc, char ***argv) {
  if (*argc <= 0) return NULL;
  (*argc)--;
  char **ret = *argv;
  (*argv)++;
  return *ret;
}

bool sdm_sv_compare(sdm_string_view SV1, sdm_string_view SV2) {
  if (SV1.length != SV2.length) return false;
  for (size_t i=0; i<SV1.length; i++) {
    if (SV1.data[i] != SV2.data[i]) return false;
  }
  return true;
}

#endif // SDM_LIB_IMPLEMENTATION

