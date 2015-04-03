# include <string.h>
# include <stdlib.h>

# ifdef LEV_CASE_INSENSITIVE
# include <ctype.h>
# define eq(x, y) (tolower(x) == tolower(y))
# else
# define eq(x, y) ((x) == (y))
# endif

# define min(x, y) ((x) < (y) ? (x) : (y))

void Init_levenshtein () {
  // Empty method body
}

unsigned int levenshtein (const char *word1, const char *word2) {
    size_t len1 = strlen(word1),
           len2 = strlen(word2);
    unsigned int *v = calloc(len2 + 1, sizeof(unsigned int));
    unsigned int i, j, current, next, cost;

    /* strip common prefixes */
    while (len1 > 0 && len2 > 0 && eq(word1[0], word2[0]))
        word1++, word2++, len1--, len2--;

    /* handle degenerate cases */
    if (!len1) return len2;
    if (!len2) return len1;
    
    /* initialize the column vector */
    for (j = 0; j < len2 + 1; j++)
        v[j] = j;

    for (i = 0; i < len1; i++) {
        /* set the value of the first row */
        current = i + 1;
        /* for each row in the column, compute the cost */
        for (j = 0; j < len2; j++) {
            /*
             * cost of replacement is 0 if the two chars are the same, or have
             * been transposed with the chars immediately before. otherwise 1.
             */
            cost = !(eq(word1[i], word2[j]) || (i && j &&
                     eq(word1[i-1], word2[j]) && eq(word1[i],word2[j-1])));
            /* find the least cost of insertion, deletion, or replacement */
            next = min(min( v[j+1] + 1,
                            current + 1 ),
                            v[j] + cost );
            /* stash the previous row's cost in the column vector */
            v[j] = current;
            /* make the cost of the next transition current */
            current = next;
        }
        /* keep the final cost at the bottom of the column */
        v[len2] = next;
    }
    free(v);
    return next;
}

# ifdef TEST
# include <stdio.h>
# include "levenshtein.h"

int main (int argc, char **argv) {
    unsigned int distance;
    if (argc < 3) return -1;
    distance = levenshtein(argv[1], argv[2]);
    printf("%s vs %s: %u\n", argv[1], argv[2],distance);
}
# endif
