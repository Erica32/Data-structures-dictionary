#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char foundWord[30], *p;

typedef struct node {
    char value;
    int isCompleteWord;
    struct node *next;
    struct node *prev;
    struct node *children;
    struct node *parent;
} NODE;

typedef struct {
  struct node* root;
} HEAD_NODE;

// Creates an empty Trie
HEAD_NODE* createTrie() {
    HEAD_NODE *trieTree = (HEAD_NODE *) malloc (sizeof(HEAD_NODE));
    trieTree->root = NULL;
    return trieTree;
}
 
// Creates an empty node
NODE *CreateTrieNode(char value)
{
   NODE *node = (NODE *)malloc(sizeof(NODE));
   
   if(node == NULL)
   {
    printf("Malloc failed\n");
    return node;
   }
   
   node->value = value;
   node->next = NULL;
   node->children = NULL;
   node->prev= NULL;
   node->isCompleteWord= 0;
   return node;
}

//search a word on the dictionary
NODE* SearchTrie(NODE *root, const char *value) {
   NODE *level = root;
   
   p = foundWord;
   
   while(1)
   {
      NODE *found = NULL;
      NODE *cur;
     
      for (cur = level; cur != NULL; cur = cur->next) {
   
         if (cur->value == *value) {
            found = cur;

            *p = cur->value;  
            p++;
            
            break;
         }
      }

      if (found == NULL) {
         return NULL;
      } 
     
      level = found->children;
      value++;
      
      if (*value == '\0' && found->isCompleteWord == 1) { 
            *p = '\0';

            return cur;
      }
      
   }
}

void AddTrie(NODE **root, const char *value) {
   NODE *cur;

   /*First Node*/
   if(*root == NULL) {
        int i;
        for (i = 0; *value; i++) {
            if (i == 0) {
                cur = CreateTrieNode(*value);
                *root =  cur;          
                
                //printf("Inserting: [%c]\n",cur->value);  
            } else {
                cur->children = CreateTrieNode(*value);
                cur->children->parent = cur;

                //printf("Inserting: [%c]\n",cur->children->value);  
                cur = cur->children;
            }

            value++;
        } 

        cur->isCompleteWord = 1;

   } else if (*root != NULL && (*root)->children == NULL) {
        cur = *root;

        while (*value) {
            cur->children = CreateTrieNode(*value);
            cur->children->parent = cur;

            //printf("Inserting: [%c]\n",cur->children->value);  
            cur = cur->children;
            value++;     
        }

        cur->isCompleteWord = 1;
      
   } else {
        cur = *root;

        if (SearchTrie(cur, value)) {
            //printf("Duplicate!\n");
            return;
         }

        //find the node to insert new Node
        while(*value && cur->children) {
            if(*value == cur->value) {
                 value++;

                 //printf("Traversing child: [%c]\n",cur->children->value);
                 cur = cur->children;
            } else {
              break;
            }
        }  

        if (*value == cur->value) {
            value++;

            AddTrie(&cur, value);
            return;
        } else {
            //find neighbour
            while(*value && cur->next) {
                if(*value == cur->next->value) {
                  
                  //printf("next[%c]\n",cur->next->value); 
                   AddTrie(&(cur->next), value);
                   
                  // value++;
                   return;
                } 
                cur = cur->next;
            }

        }

       //insert neighbour 
        cur->next = CreateTrieNode(*value);
        cur->next->parent = cur->parent;
        cur->next->prev = cur;
        //printf("Inserting [%c] as neighbour of [%c] \n",cur->next->value, cur->value);
        value++;

        if(!(*value)){
            cur->next->isCompleteWord = 1;
            return;
        }

        //insert children
        for(cur = cur->next; *value; cur = cur->children)
        {
          cur->children = CreateTrieNode(*value);
          cur->children->parent = cur;

          //printf("Inserting: [%c]\n",cur->children->value);

          value++;
        }
       
       cur->isCompleteWord = 1;

   }

}

int main() {   
    /*Create a trie*/
    HEAD_NODE *trieTree = createTrie();

    printf("Dictionary is being pupolating!\n"); 

    //populate the dictionary
    char line[10000];

    /* open the file for reading */
    FILE* fr = fopen ("google-10000-english-usa-no-swears-medium.txt", "rt");  

    while(fgets(line,  sizeof(line), fr) != NULL)
    {
       line[ strlen(line) - 1 ] = '\0';
       AddTrie(&(trieTree->root), line);
    }

    /* close the file prior to exiting the routine */
    fclose(fr); 
    
    printf("Dictionary was pupolated successfully!\n");

    //search a word
    if (SearchTrie(trieTree->root, "login")) {
        printf("Word found:\n%s\n", foundWord);    

    } else {
        printf("Word non found\n");   
    }

    return 0;
} 