#ifndef H_TABLEHACHAGE
#define H_TABLEHACHAGE

typedef struct hach hach_s;

hach_s *hach_new (void);
void hach_add (hach_s *, const char *, void *);
void *hach_search (hach_s *, const char *);
void hach_delete (hach_s **);

#endif /* not H_TABLEHACHAGE */
