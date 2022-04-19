typedef struct tk{
    char *token;
    struct tk *next;
} TokenNode;

struct _tokenList {
    TokenNode *tokens;
    int size;
};

typedef struct _hashTable {
    TokenNode *tokens;
    int size;
} HashTable;

TokenList *initialize_tokenlist();
void remove_token(TokenList *tl, char *token);
void add_token(TokenList *tl, char *token);
void destroy_tokenList(TokenList *tl);
