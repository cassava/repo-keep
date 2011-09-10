
/* tab2str: s must be large enough to hold lim+1 > 3 characters. */
static void tab2str(char *s, int lim, struct config_map *tab)
{
    char *t = s;
    int n, m;

    *t++ = '{';
    for (;;) {
        if (tab->key != NULL) {
            n = strlen(tab->key);
            m = (tab->value == NULL ? 0 : strlen(tab->value));
            lim -= n + m + 3; // 2 is for =, comma, and space.
            if (lim > 0) {
                strcpy(t, tab->key);
                t+=n;
                *t++ = '=';
                if (tab->value != NULL) {
                    strcpy(t, tab->value);
                    t += m;
                }
                *t++ = ',';
                *t++ = ' ';
            } else {
                // not enough space anymore
                break;
            }
            tab++;
        } else {
            // we are at end of array
            break;
        }
    }
    *(t-2) = '}';
    *(t-1) = '\0';
}

