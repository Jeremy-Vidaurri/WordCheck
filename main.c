//If this program does not work with MinGW, I used repl.it to write my code.


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

typedef struct tree{
  char word[50]; //I used 50 as an arbitrary size. This was the only way I was able to get my program to work.
  struct tree *left, *right;
}tree_t;

void insert_node(tree_t **root,char * s);
void delete_node(tree_t *root,char * s);
int lowStrCmp(char *str1, char *str2);
void print_preorder(tree_t *root);
void print_inorder(tree_t *root);
void print_postorder(tree_t *root);
tree_t* search(tree_t *root,char * s); 
void spellcheck(tree_t *root, FILE* filename);
char * noPunct(char*s);

int main(void) {
  tree_t *root; //Root of the tree
  root = NULL; //Set it to null for insertion purposes
  FILE *words,*input; //Words is the dictionary, input is the file provided by the user.
  char curWord[50],filename[50];//curName traverses the files one word at a time, filename is file name that the user provides.
  int status; //Status of the fscanf's

  words = fopen("words.txt","r");
  for(status = fscanf(words," %s",curWord);status != EOF;status = fscanf(words," %s",curWord)){
    insert_node(&root,curWord);//Keep inserting words as nodes in the tree until the end of the file is hit
  }
  delete_node(root, "play");
  //Take input from the user for the file name  
  printf("Please enter the file name for spellcheck (ex: input.txt): ");
  scanf("%s",filename);

  //Determine whether the file exists.  
  if(fopen(filename,"r")){
    input = fopen("input.txt","r");
    spellcheck(root, input);
  } else{
    printf("File does not exist.\n");
  }
  return 0;
}

//insert_node inserts a node in the binary tree without breaking any BST rules.
void insert_node(tree_t **root,char * s){
  tree_t *newNode,*ptr;
  ptr = *root;
  int flag=0;

  if(*root == NULL){ //No root, insert at root
	  newNode = (tree_t*)malloc(sizeof(tree_t)); 
    strcpy(newNode->word,s);
	  newNode->left = NULL;
	  newNode->right = NULL;
    *root= newNode;
  }else{
    while(!flag){//flag says either something was inserted or it already exists.
      if (lowStrCmp(ptr->word, s) == -1){ // ptr->word comes before string s.
        if(ptr->right == NULL){ //If the right side doesn't exist, insert there
          newNode = (tree_t*)malloc(sizeof(tree_t));
          ptr->right = newNode;
          strcpy(newNode->word,s);
          newNode->left = NULL;
          newNode->right = NULL;
          flag = 1;
        } else{ //If it does exist, traverse 
          ptr = ptr->right;
        }
      }else if (lowStrCmp(ptr->word, s) == 1) { //ptr->word comes after string s.
        if(ptr->left ==NULL){//If the left side doesn't exist, insert there
          newNode = (tree_t*)malloc(sizeof(tree_t));
          ptr->left = newNode;
          strcpy(newNode->word,s);
          newNode->left = NULL;
          newNode->right = NULL;
          flag = 1;
        } else{ //If it does exist, traverse
          ptr = ptr->left;
        }
      } else{ //ptr->word and string s are the same
        printf("This word is already in the tree: %s.\n\n",s);
        flag = 1;
      }
    }
  }
}


int lowStrCmp(char *str1, char *str2){
  int i;

  if(strlen(str1)<strlen(str2)){//This is so we don't index out of range
    for(i=0;i<strlen(str1);i++){
      //Check characters one by one. If there is ever a case where they don't match, send -1 for first char is lower, 1 for higher
      if(tolower(str1[i]) < tolower(str2[i])){ 
        return -1;
      }else if(tolower(str1[i]) > tolower(str2[i])){
        return 1;
      }
    } 
  } else{
    for(i=0;i<strlen(str2);i++){
      //Check characters one by one. If there is ever a case where they don't match, send -1 for first char is lower, 1 for higher
      if(tolower(str1[i]) < tolower(str2[i])){
        return -1;
      } else if(tolower(str1[i]) > tolower(str2[i])){
        return 1;
      }
    }
  }
  //If the two strings had the same characters but different lengths, the shorter one comes first.
  if(tolower(str1[i-1]) == tolower(str2[i-1]) && strlen(str1)<strlen(str2)){
    return 1;
  } else if (tolower(str1[i-1]) == tolower(str2[i-1]) && strlen(str1)>strlen(str2)){
    return -1;
  }
  //Two words are the same.
  return 0;
}

tree_t* search(tree_t *root,char * s){
  tree_t *ptr = root;//set a pointer to root so we can traverse without changing root
  while(ptr != NULL){//until a null pointer is found, keep searching
    if(lowStrCmp(ptr->word,s)== -1){//Traverse right if the search string is higher
      ptr = ptr->right;
    } else if (lowStrCmp(ptr->word,s)== 1){ //Traverse left if the search string is lower.
      ptr = ptr->left;
    } else{
      return ptr; //Once the node is found, retrun it
    }
  }
  tree_t *nullNode= NULL;
  return nullNode;//if no node is found, send a null node.
}
tree_t searchParent(tree_t*root,char *s);





void delete_node(tree_t *root,char * s){
  tree_t *delNode = search(root,s);

  if (delNode == NULL){ //CASE 0: Node doesn't exist
    printf("NODE NOT FOUND\n");
  } else if(delNode->right == NULL && delNode->left == NULL){ //CASE 1: Node has no children
    free(delNode);
  } else if((delNode->right == NULL && delNode->left != NULL)|| (delNode->right != NULL && delNode->left == NULL)){ //CASE 2: Node has 1 child
    if (delNode->right != NULL){
      strcpy(delNode->word,delNode->right->word);
      free(delNode->right);
    }else{
      strcpy(delNode->word,delNode->left->word);
      free(delNode->left);
    }
  } else{ //Case 3: Node has 2 children
    tree_t *ptr = delNode->left;
    while(ptr->right !=NULL)
      ptr = ptr->right;
    if(delNode->left == ptr){
      delNode->left = ptr->left;
    }
    strcpy(delNode->word,ptr->word);
    free(ptr);
  }
}


void print_preorder(tree_t *root){
  if (root != NULL){
    printf("%s\n",root->word);
    print_preorder(root->left);
    print_preorder(root->right);
  }
}

void print_inorder(tree_t *root){
  if (root != NULL){
    print_inorder(root->left);
    printf("%s\n",root->word);
    print_inorder(root->right);
  }
}

void print_postorder(tree_t *root){
  if(root != NULL){
    print_postorder(root->left);
    print_postorder(root->right);
    printf("%s\n",root->word);
  }
}

//Spellcheck traverses the tree to find any mismatched words from input.
void spellcheck(tree_t *root, FILE* filename){
  char curWord[50];
  int status;
  for(status = fscanf(filename," %s",curWord);status != EOF;status = fscanf(filename," %s",curWord)){
    strcpy(curWord,noPunct(curWord));//Remove any punctuation except apostrophe
    tree_t *node = search(root,curWord);
    if(node ==NULL){
      printf("%s is either misspelled or not in the dictionary\n",curWord); //If a null node was returned, it is not in the tree. Also I had 0 clue how to put the line and word number.
    }
  }
}

char * noPunct(char*s){
  int i;
  for(i=0;i<strlen(s);i++){
    if (!isalpha(s[i]) && s[i] !='\'')//If there is any non alphabetical characters or apostrophe, replace it with the character after.
      s[i]= s[i+1];
  }
  s[i]='\0';//Make sure a null character is at the end
  return s;
}