
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define CHAR_TO_INDEX(c) ((int)c - (int)'a')
#define ALPHABET_SIZE 26
#define SIZE 256
//create a ctructure of the trie
typedef struct trie
 {
  bool key;
  struct trie * children[ALPHABET_SIZE];
  
 }trieNode;
//create a new node
struct trie *newNode(void)
{
	struct trie* node = (struct trie*)malloc(sizeof(trieNode));
	//node key set to false.
	node->key = false;
	int i;
	//every parent can have 26 childrens
	// and 26 new node create and set them as null node
	for(i=0;i<ALPHABET_SIZE;i++){
		node->children[i]=NULL;
	}

	return node;
	
}
//add function
void insert(trieNode* root, const char* word) {
    int length = strlen(word);
    int i, index;

    trieNode* node = root;

    for(i = 0; i < length; i++)
    {
        index = CHAR_TO_INDEX(word[i]);
        if(!node->children[index])
            node->children[index] = newNode();
        
        node = node->children[index];
    }
    //after finishing add new word into trie change the key tag as true.
	//defoult key tag is seted to false.
    node->key = true;
}


// //search function
bool search(struct trie* root, const char* str){
    int level;
    int length = strlen(str);//get the word length
    struct trie* curr = root;
 
    for (level = 0; level < length; level++)
    {
 //if given cword(str) is not exist in the trie, set to false
        if (curr->children[*str - 'a'] == NULL)
            return false;
 //move to next node
        curr = curr->children[*str - 'a'];
    }
 //return the key tag
    return (curr != NULL && curr->key);
}

void addChar(char* str, char c){
	int len = strlen(str);
	//char* str2[len+2];
	str[len] = c;
	str[len+1] = '\0';
}

//pop chars from the nodes
void popChar(char* str) {
    int len = strlen(str);
    str[len - 1] = 0;
}

//convert char into its' lowercase
void toLowerCase(char* str) {
    int i, len;
    len = strlen(str);
    
    for(i = 0; i < len; i++){
        str[i] = tolower(str[i]);
    }
}

//check whether consider node is final node
//if that is final node key tag is set to true and athewise false.
bool isLastNode(struct trie* root){
	int count;
	for(count = 0;count<ALPHABET_SIZE;count++){
		if(root->children[count]){
			return false;
		}
	}

	return true;
}
//disply the suggestion using recercive function 
//first check the consider root has or not key as 1
//if key is equal 1 , prefix is one word in the dic.
//then check that nod is the last node , if return no any word suggestions
//then go to child to child and check above all of thing 
void displaySuggestions(trieNode* root, char* prefix) {

    if (root->key) {
        printf("%s\n", prefix);
    }

    if(isLastNode(root)) {
        return;
    }

    int i;
    for(i = 0; i < ALPHABET_SIZE; i++){
        if (root->children[i]) {       	
            addChar(prefix, 97 + i);
            displaySuggestions(root->children[i], prefix);
            popChar(prefix);
        }
    }
    
}

// Recursive function to print auto-suggestions for given
// node.
int printSuggestions(trieNode* root, const char* word) {
    trieNode* parentnode = root;

    int i, len;
    len = strlen(word);
    
    // search for the word in the trie
    // if not found well sadly its not there
    for( i = 0; i < len; i++) {
        int idx = CHAR_TO_INDEX(word[i]);
        if (!parentnode->children[idx]){
        	return 0;
        } 
        parentnode = parentnode->children[idx];
    }

    bool isWord = (parentnode->key == true);
    bool isLast = isLastNode(parentnode);

    // word is found, no childs....
    if (isWord && isLast) {
        //printf("%s\n", word);
        return 1;
    }
    
    // there is more to show
    char prefix[SIZE];
    strcpy(prefix, word);
    displaySuggestions(parentnode, prefix);

    return 1;
}



//main method
int main( int argc, char **argv )
{
	FILE* fp;
    char buf[SIZE];
    trieNode *root = newNode();
    char prefix[SIZE];

	//printf("Enter a word to find in dictionary :");
	//struct trie* newTree;
	struct trie* newTree = (struct trie*)malloc(sizeof(struct trie));
	int tag = 0;
	//char word[30];
	if (argc != 2)
    {
        fprintf(stderr, "Usage: %s <soure-file>\n", argv[0]);
        return 1;
    }
    
    if ((fp = fopen(argv[1], "r")) == NULL)
    { 
        /* Open source file. */
        perror("fopen source-file");
        return 1;
    }
	while (fgets(buf, sizeof(buf), fp) != NULL)
    {
        buf[strcspn(buf, "\r\n")] = 0; 
        toLowerCase(buf);
        insert(root, buf);
    }
    fclose(fp);
    //
 	while(1) {
        printf("Enter a word to find from dictionary  \n");
        printf("Or enter 'exit' to exit the program : ");
        scanf("%s", prefix);

        if(!strcmp(prefix,"exit")){
        	printf("....Closed program....");
        	return 0;
        }
        //if there no word ,  exit and if exist, print all word .
        else if(!printSuggestions(root, prefix)){
            printf("Word does not exist!\n");
        }
        
     
    }
    

    return 0;
}



