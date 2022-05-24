#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <windows.h>
#include <time.h>
typedef enum  // to determine color of node
{
    Red,
    Black
} NodeColor;

typedef union
{
    int IntVal;
    char*StrVal;
} value;
int flag = 0; //choose type of data inserted in a tree
typedef struct Node
{
    value data; //choose type of data to be more dynamic
    struct Node*left;
    struct Node*right;
    struct Node*parent;
    NodeColor color;
} node;
node*root;
node*nill;
int wordValidator(char *s);
void search_int();
void insert_int();
void string_submenu();
void integer_submenu();
void main_menu();
void ShowConsoleCursor(int x );
void drawframe();
void delay(int seconds);
void gotoxy(int x,int y);
void insert_dict();
node* construct_node(value);
void insert(value);
int count_nodes(node*);
void left_rotate(node*);
void right_rotate(node*);
int search(value);
void new_node_fix(node*);
void print_in_order(node*);
int max_branch(int x, int y );
int height (node*);
void build_tree();
void LOAD ();
void search_strings();
void main_menu_frame();
int main()
{

    build_tree();
    main_menu();
    flag=1;


    return 0;
}
void build_tree()
{
    //initialize nill node and root to avoid null pointer error
    nill=malloc(sizeof(node));
    nill->color=Black;
    root=nill;
}
node* construct_node (value val)  //new nodes constructor
{
    node*x=malloc(sizeof(node));
    if(flag==1)
    {
        x->data.StrVal=malloc(strlen(val.StrVal)+1);
        strcpy(x->data.StrVal,val.StrVal);
    }
    else if (flag==0)
    {
        x->data.IntVal=val.IntVal;
    }

    x->color=Red;
    x->left=x->right=nill;
    return x;
}
void insert(value val)
{
// clones so i don't change original pointer

    node*clone=root;//x
    node*new_node=construct_node(val);
    node * parent = nill; // y
    if(flag==0)
    {

        while(clone!=nill)
        {
            parent = clone; // y=x
            if(new_node->data.IntVal<=clone->data.IntVal)
            {
                clone=clone->left;
            }
            else
            {
                clone=clone->right;
            }
        }


        if(parent==nill)
        {

            root=new_node;
        }

        else if(parent->data.IntVal < new_node->data.IntVal) //hena kont 3amel clone->parent hnkhaleha parent bs
        {
            parent->right=new_node; //same her 3la 7ewar el parent
        }
        else
        {
            parent->left=new_node; //and here as well
        }

        new_node->parent=parent;//and here too


        new_node_fix(new_node);
    }
    else if (flag==1)
    {

        while(clone!=nill)
        {

            parent = clone; // y=x
            if(strcasecmp(parent->data.StrVal,new_node->data.StrVal)<0)
            {
                clone=clone->right;
            }
            else
            {
                clone=clone->left;
            }
        }
        if(parent==nill)
        {

            root=new_node;
        }

        else if(strcasecmp(parent->data.StrVal,new_node->data.StrVal)<0) //hena kont 3amel clone->parent hnkhaleha parent bs
        {
            parent->right=new_node; //same her 3la 7ewar el parent
        }
        else
        {
            parent->left=new_node; //and here as well
        }

        new_node->parent=parent;//and here too


        new_node_fix(new_node);
    }
}




void new_node_fix(node*new_node)
{

    while(new_node !=root && new_node->parent->color==Red )
    {

        if(new_node->parent==new_node->parent->parent->left) //check which side are we on in this case left side
        {

            if(new_node->parent->parent->right->color==Red)  //case 1 uncle red
            {
                new_node->parent->color=Black;
                new_node->parent->parent->right->color=Black;
                new_node->parent->parent->color=Red;
                new_node=new_node->parent->parent; // to fix it if there was an error caused by it
            }

            else //case 2 triangle case
            {
                if(new_node==new_node->parent->right)
                {
                    new_node=new_node->parent; // assign to parent as we will rotate parent
                    left_rotate(new_node);
                }
                //we reached the line case so recolor parent and grandparent to perform rotation for grand parent

                new_node->parent->color=Black;
                new_node->parent->parent->color=Red;
                right_rotate(new_node->parent->parent);
            }

        }
        else  //case we are on right side
        {
            if(new_node->parent->parent->left->color==Red)
            {
                new_node->parent->color=Black;
                new_node->parent->parent->left->color=Black;
                new_node->parent->parent->color=Red;
                new_node=new_node->parent->parent; // to fix it if there was an error caused by it as it is the only case that can cause my tree to have 2 consecutive red
            }
            else
            {
                if(new_node==new_node->parent->left )  //triangle case on left subtree
                {
                    new_node=new_node->parent;
                    right_rotate(new_node);
                }
                //we reached line case

                new_node->parent->color=Black;
                new_node->parent->parent->color=Red;
                left_rotate(new_node->parent->parent);

            }
        }
    }

    root->color=Black; //make sure at the end root is black to avoid infinite loops
}
void right_rotate(node*parent_node)
{
    node*new_node=parent_node->left; //child
    parent_node->left=new_node->right; //assign the left of the current parent
    if(new_node->right!=nill)  //there exist a right to the node that i newly inserted
    {
        new_node->right->parent=parent_node; //make the old parent the new parent for the right of the newly inserted node
    }
    new_node->parent=parent_node->parent; //make the parent's parent the new parent for the new node after rotation
    if(new_node->parent==nill)  //no nodes above me so i am the root
    {
        root=new_node;
    }
//adjusting pointers of the orignal's parent parent to point to the new node
    else if(parent_node==parent_node->parent->right)  //the parent is right subtree for the previous node
    {
        parent_node->parent->right=new_node;
    }
    else if(parent_node==parent_node->parent->left)
    {
        parent_node->parent->left=new_node;
    }
//adjust pointers between new node and old parent
    new_node->right=parent_node;
    parent_node->parent=new_node;
}
void left_rotate(node*parent)
{
    node*new_node=parent->right; //y=x->right
    parent->right=new_node->left; // assign all left childs to newly inserted nodes to be pointed by the old parent after rotation
    if(new_node->left!=nill)
    {

        new_node->left->parent=parent;
    }
    new_node->parent=parent->parent; //adjust the parent of the old parent to be the parent of the newly inserted node
    if(new_node->parent==nill)  //no parent for old parent
    {

        root=new_node;
    }
    else if(parent->parent->right==parent)  //check old parents position relative to its parent
    {

        parent->parent->right=new_node;
    }
    else if(parent->parent->left==parent)
    {

        parent->parent->left=new_node;
    }
    //adjust positions of new nodes

    new_node->left=parent;
    parent->parent=new_node;

}
int search(value val)
{
    node*clone=root;
    int found=0;
    if(flag==0)  //integer search
    {
        while(clone!=nill && clone->data.IntVal!=val.IntVal )
        {
            if(clone->data.IntVal>val.IntVal)
            {
                clone=clone->left;
            }
            else if(clone->data.IntVal<val.IntVal)
            {
                clone=clone->right;
            }
        }
    }
    else if (flag==1)  // string search
    {
        while(clone!=nill && strcmp(clone->data.StrVal,val.StrVal)!=0)
        {
            if(strcmp(val.StrVal,clone->data.StrVal)<0)
            {
                clone=clone->left;
            }
            else
            {
                clone=clone->right;
            }
        }
    }
    if(clone!=nill)
    {
        found=1;
    }
    if(found &&flag==1)
    {
        return 1;
    }
    else if (found && flag==0)
    {
        printf("\n The node containing %d is found ",clone->data.IntVal);
        return 1;
    }
    else
    {
        if(flag==0)
        {
            printf("\n The node  is not found\n ");
        }
        return 0;
    }
}
int count_nodes(node*rootx)
{
    if(rootx==nill)
    {
        return 0;
    }
    else
    {
        return 1+count_nodes(rootx->right)+count_nodes(rootx->left);
    }
}
void print_in_order(node*rootx)
{
    if(flag==0)
    {

        if(rootx!=nill)
        {

            print_in_order(rootx->left);
            printf("\n%d \t",rootx->data.IntVal);
            if(rootx->color==Red)
            {
                printf("\033[0;31m");
                printf("RED\n");
            }
            else if(rootx->color==Black)
            {
                printf("0\33[0m");
                printf("Black\n");
            }
            print_in_order(rootx->right);
        }
    }
    else
    {
        if(rootx!=nill)
        {
            print_in_order(rootx->left);
            printf("%s \t",rootx->data.StrVal);
                        if(rootx->color==Red)
            {
                printf("\033[0;31m");
                printf("RED\n");
            }
              else if(rootx->color==Black)
            {
                printf("\033[0m");
                printf("Black\n");
            }
            print_in_order(rootx->right);
        }
    }
     printf("\033[0m");
}
int max_branch(int x, int y )
{
    if(x>y)
    {
        return x;
    }
    else
    {
        return y ;
    }
}

int height (node*rootx)
{
    if(rootx==nill)
    {
        return 0;
    }
    //i need the maximum height in any branch
    int r_count = 1+height(rootx->right);
    int l_count = 1+height(rootx->left);
    return max_branch(r_count,l_count);
}
void LOAD()
{
    if(root!=nill){
        free(root);
        free(nill);
        build_tree();
    }
    char name[50];
    flag=1;
    printf("Enter file name : ");
    gets(name);
    printf("---------------------------------------------------------------------------------------------------------\n");
    // strcpy(Filname,name);    //copy to global variable
    FILE*f=fopen(name,"r");
    value str;

    if(f==NULL)
    {
        printf("\aError loading file.");
        exit(0);      //make sure that the file exists
    }
    while(!feof(f))
    {
        char*arr=malloc(1000);
        fscanf(f,"%s\n",arr);
        str.StrVal=malloc(strlen(arr)+1);
        for(int i = 0; arr[i]; i++){ //lower all characters before insert
  arr[i] = tolower(arr[i]);
}
        strcpy(str.StrVal,arr);
        insert(str);
        free(arr);
        free(str.StrVal);
    }

    fclose(f);

    printf("The initial  size of the dictionary is %d ",count_nodes(root));
}
void insert_dict()
{
    char word [100];
    printf("Enter the word you wish to insert  : ");
   scanf("%[^\n]%*c", word);
    while(wordValidator(word)==0){
        printf("\nPlease re-enter the word you wish to insert in a correct form containing only letters  : ");
    scanf("%[^\n]%*c", word);
    }
     for(int i = 0; word[i]; i++){ //lower all characters before insert
  word[i] = tolower(word[i]);
}
    value val;
    val.StrVal=malloc(1000);//free
    strcpy(val.StrVal,word);
    int found =search(val);
    if(found)
    {
        printf("Error,Word already exists \n ");
        printf("The number of words has not changed and is %d and the height is %d ",count_nodes(root),height(root));
    }
    else
    {

        insert(val);
        printf("Inserted successfully , New Word Count is %d and the updated height is %d  ",count_nodes(root),height(root));
    }


        printf("\n---------------------------------------------------------------------------------------------------------\nDo you want to enter another word , Y for yes , N for No : ");
    char s [3];

    gets(s);

    while(strcmp(s,"Y")&&strcmp(s,"y")&&strcmp(s,"N")&&strcmp(s,"n"))
    {
        printf("---------------------------------------------------------------------------------------------------------\n");
        printf("Invalid character, Retry : ");
        gets(s);
    }
    if(strcmp(s,"Y")==0|| strcmp(s,"y")==0)
       insert_dict();


}
void search_strings()
{
    char word [100];
    printf("Enter the word you wish to search  : ");
    scanf("%[^\n]%*c", word);
      while(wordValidator(word)==0){
        printf("\nPlease re-enter the word you wish to search for in a correct form containing only letters  : ");
    scanf("%[^\n]%*c", word);
    }
     for(int i = 0; word[i]; i++){ //lower all characters before insert
  word[i] = tolower(word[i]);
}
    value val;
    val.StrVal=malloc(1000);
    strcpy(val.StrVal,word);
    int found=search(val);
    if(found)
    {
        printf("Yes %s is found in the tree \n",val.StrVal);
    }
    else
    {
        printf("No, %s is not found in the tree \n",val.StrVal);
    }

        printf("\n---------------------------------------------------------------------------------------------------------\nDo you want to search for another word , Y for yes , N for No : ");
    char s [3];

    gets(s);

    while(strcmp(s,"Y")&&strcmp(s,"y")&&strcmp(s,"N")&&strcmp(s,"n"))
    {
        printf("---------------------------------------------------------------------------------------------------------\n");
        printf("Invalid character, Retry : ");
        gets(s);
    }
    if(strcmp(s,"Y")==0|| strcmp(s,"y")==0)
       search_strings();


}


void gotoxy(int x,int y)
{
    COORD coord= {0,0};             //This function allows us move the cursor in the output screen
    coord.X=x;
    coord.Y=y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),coord);
}
void delay(int seconds)
{
    int milliseconds = 1000 * seconds;
    clock_t start_time = clock();
    while (clock() < start_time + milliseconds);
}
void drawframe()
{
    int i;
    gotoxy(0,0);
    printf("%c",201);               // ascii codes of the edges of the frame
    for(i=1; i<45; i++)printf("%c",205);
    printf("%c",187);
    gotoxy(0,1);
    printf("%c",186);               //186 is code for the vertical walls of the frame
    for(i=1; i<45; i++)printf(" ");
    printf("%c",186);
    gotoxy(0,2);
    printf("%c",186);
    for(i=1; i<45; i++)printf(" ");
    printf("%c",186);
    gotoxy(0,3);
    printf("%c",186);
    for(i=1; i<45; i++)printf(" ");
    printf("%c",186);
    gotoxy(0,4);
    printf("%c",186);
    for(i=1; i<45; i++)printf(" ");
    printf("%c",186);
    gotoxy(0,5);
    printf("%c",186);
    for(i=1; i<45; i++)printf(" ");
    printf("%c",186);
    gotoxy(0,6);
    printf("%c",186);
    for(i=1; i<45; i++)printf(" ");
    printf("%c",186);
    gotoxy(0,7);
     printf("%c",186);
    for(i=1; i<45; i++)printf(" ");
    printf("%c",186);

    gotoxy(0,8);
    printf("%c",200);
    for(i=1; i<45; i++)printf("%c",205);
    printf("%c",188);                 //lower right edge of the frame
}
void ShowConsoleCursor(int x )
{
    HANDLE out=GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cp;
    GetConsoleCursorInfo(out,&cp);
    cp.bVisible=x;                 //shows the cursor or hides it
    SetConsoleCursorInfo(out,&cp);
}
void main_menu()
{
    int pos=1;
    char ch;
    //drawframe();
    main_menu_frame();
    HANDLE console=GetStdHandle(STD_OUTPUT_HANDLE);
    ShowConsoleCursor(0);
    do
    {
        SetConsoleTextAttribute(console,15);
        gotoxy(1,1);
        printf("1- Integer Tree ");
        gotoxy(1,2);
        printf("2- Strings Tree ");
        gotoxy(1,3);
        printf("3-Exit ");
        printf("\n\n");
        switch(pos)
        {
        case 1 :
            SetConsoleTextAttribute(console,240);
            gotoxy(1,1);
            printf("1- Integer Tree  ");
            break;
        case 2:
            SetConsoleTextAttribute(console,240);
            gotoxy(1,2);
            printf("2- Strings Tree  ");
            break;
        case 3:
              SetConsoleTextAttribute(console,240);
            gotoxy(1,3);
            printf("3- Exit   ");
            break;
        }

        ch=getch();
        if(ch==-32)ch=getch();  //if input is space
        if(ch==72)
        {
            pos--;     //decrement position if input is arrow up
            if(pos==0)pos=3;
        }
        if(ch==80)
        {
            pos++;     //increment position if input is arrow down
            if(pos==4)pos=1;
        }
    }
    while(ch!=13);
    SetConsoleTextAttribute(console,15);
    gotoxy (0,4);
    ShowConsoleCursor(1);
    switch(pos)
    {
    case 1:

        integer_submenu();
        break;
    case 2:

        string_submenu();
        break;
    case 3:
        exit(0);
    }
}
void integer_submenu()
{
    if(flag!=0){
            if(root!=nill){
        free(root);
            }
        free(nill);
        build_tree();
        flag=0;
    }
    int pos=1;
    char ch;
    drawframe();
    HANDLE console=GetStdHandle(STD_OUTPUT_HANDLE);  //control the foreground and background of text
    ShowConsoleCursor(0);   //hide cursor
    do
    {
        SetConsoleTextAttribute(console,15);   //black background white text
        gotoxy(1,1);
        printf("1-Insert elements into RedBlack Tree ");
        gotoxy(1,2);
        printf("2-Search Tree ");
        gotoxy(1,3);
        printf("3-Print Tree Size ");
        gotoxy(1,4);
        printf("4-Print Tree height");
        gotoxy(1,5);
        printf("5-Print Tree in order ");
        gotoxy(1,6);
        printf("6-Main Menu");
        printf("\n\n");    //construct menu

        switch(pos)
        {
        case 1 :
            SetConsoleTextAttribute(console,240);   //shaded background on selection
            gotoxy(1,1);
            printf("1-Insert elements into RedBlack Tree  ");
            break;
        case 2:
            SetConsoleTextAttribute(console,240);
            gotoxy(1,2);
            printf("2-Search Tree ");
            break;
        case 3:
            SetConsoleTextAttribute(console,240);
            gotoxy(1,3);
            printf("3-Print Tree Size");
            break;
        case 4:
            SetConsoleTextAttribute(console,240);
            gotoxy(1,4);
            printf("4-Print Tree height");
            break;
        case 5:
            SetConsoleTextAttribute(console,240);
            gotoxy(1,5);
           printf("5-Print Tree in order ");
            break;
        case 6:
            SetConsoleTextAttribute(console,240);
            gotoxy(1,6);
          printf("6-Main Menu");
            break;
        }
        ch=getch();
        if(ch==-32)ch=getch();
        if(ch==72)
        {
            pos--;
            if(pos==0)pos=6;  //if moving up from first option in menu brings cursor back to the last option
        }
        if(ch==80)
        {
            pos++;
            if(pos==7)pos=1; //brings cursor back to first menu
        }
    }
    while(ch!=13);    //exit loop when entered character is a new line
    SetConsoleTextAttribute(console,15);   //return to default foreground and background
    gotoxy (0,8);
    ShowConsoleCursor(1);   //make cursor appear
    switch(pos)  //position of cursor corresponding to which task
    {
    case 1:
                insert_int();
                delay(5);
                system("cls");
                integer_submenu();
        break;
    case 2:
    search_int();
    delay(5);
                system("cls");
                integer_submenu();
    break;
    case 3:

        printf( "\nThe number of nodes in the dictionary is %d \n ", count_nodes(root));
        delay(5);
                system("cls");
                integer_submenu();
        break;
    case 4:

       printf(" \n The tree height is %d \n", height(root));
       delay(5);
                system("cls");
                integer_submenu();
        break;
    case 5:
        print_in_order(root);
        delay(10);
                system("cls");
                integer_submenu();
        break;
    case 6:
        main_menu_frame();
         main_menu();
        break;

    }

}
void string_submenu(){
    if(flag!=1 && root!=nill){

        free(root);
        free(nill);
        build_tree();

    }
    flag=1;
      int pos=1;
    char ch;
    drawframe();
    HANDLE console=GetStdHandle(STD_OUTPUT_HANDLE);  //control the foreground and background of text
    ShowConsoleCursor(0);   //hide cursor
    do
    {
        SetConsoleTextAttribute(console,15);   //black background white text
        gotoxy(1,1);
        printf("1-Insert elements into RedBlack Tree ");
        gotoxy(1,2);
        printf("2-Search Tree ");
        gotoxy(1,3);
        printf("3-Print Tree Size ");
        gotoxy(1,4);
        printf("4-Print Tree height");
        gotoxy(1,5);
        printf("5-Print Tree in order ");
        gotoxy(1,6);
         printf("6-Load tree from existing file ");
        gotoxy(1,7);
        printf("7-Main Menu");
        printf("\n\n");    //construct menu

        switch(pos)
        {
        case 1 :
            SetConsoleTextAttribute(console,240);   //shaded background on selection
            gotoxy(1,1);
            printf("1-Insert elements into RedBlack Tree  ");
            break;
        case 2:
            SetConsoleTextAttribute(console,240);
            gotoxy(1,2);
            printf("2-Search Tree ");
            break;
        case 3:
            SetConsoleTextAttribute(console,240);
            gotoxy(1,3);
            printf("3-Print Tree Size");
            break;
        case 4:
            SetConsoleTextAttribute(console,240);
            gotoxy(1,4);
            printf("4-Print Tree height");
            break;
        case 5:
            SetConsoleTextAttribute(console,240);
            gotoxy(1,5);
            printf("5- Print Tree in order ");
            break;
        case 6:
            SetConsoleTextAttribute(console,240);
            gotoxy(1,6);
              printf("6-Load tree from existing file ");
            break;
            case 7:
             SetConsoleTextAttribute(console,240);
            gotoxy(1,7);
             printf("7-Main Menu");
            break;
        }
        ch=getch();
        if(ch==-32)ch=getch();
        if(ch==72)
        {
            pos--;
            if(pos==0)pos=7;  //if moving up from first option in menu brings cursor back to the last option
        }
        if(ch==80)
        {
            pos++;
            if(pos==8)pos=1; //brings cursor back to first menu
        }
    }
    while(ch!=13);    //exit loop when entered character is a new line
    SetConsoleTextAttribute(console,15);   //return to default foreground and background
    gotoxy (0,9);
    ShowConsoleCursor(1);   //make cursor appear
switch(pos)  //position of cursor corresponding to which task
    {
    case 1:
        insert_dict();
        delay(5);
                system("cls");
                string_submenu();
        break;
    case 2:
search_strings();
delay(5);
                system("cls");
                string_submenu();
 break;
    case 3:

        printf("The number of nodes in the dictionary is %d \n ", count_nodes(root));
        delay(5);
                system("cls");
                string_submenu();
        break;
    case 4:

       printf(" The tree height is %d \n", height(root));
       delay(5);
                system("cls");
                string_submenu();
        break;
    case 5:
        print_in_order(root);
        delay(10);
                system("cls");
                string_submenu();
        break;
    case 6:
        LOAD();
         delay(5);
                system("cls");
                string_submenu();
        break;
    case 7:
        main_menu_frame();
        main_menu();
        break;
    }



}
void insert_int(){
value val;
printf("\nEnter the value you wish to insert in the tree : ");
scanf("%d",&val.IntVal);
insert(val);
printf("\nInserted successfully , New Word Count is %d and the updated height is %d ",count_nodes(root),height(root));
 printf("\n---------------------------------------------------------------------------------------------------------\nDo you want to enter another word , Y for yes , N for No : ");
    char s [2];
    char ch;
    ch=getchar();
    gets(s);
    while(strcmp(s,"Y")&&strcmp(s,"y")&&strcmp(s,"N")&&strcmp(s,"n"))
    {
        printf("---------------------------------------------------------------------------------------------------------\n");
        printf("Invalid character, Retry : ");
        gets(s);
    }
  if(strcmp(s,"Y")==0|| strcmp(s,"y")==0)
       insert_int();


}
void search_int(){
    int x;
    printf("\nEnter the number you wish to search for in the tree : ");
    scanf("%d", &x);
    value val;
    val.IntVal=x;
    search(val);
     printf("\n---------------------------------------------------------------------------------------------------------\nDo you want to search for another word , Y for yes , N for No : ");
    char s [3];
    char ch;
    ch=getchar();
    gets(s);

    while(strcmp(s,"Y")&&strcmp(s,"y")&&strcmp(s,"N")&&strcmp(s,"n"))
    {
        printf("---------------------------------------------------------------------------------------------------------\n");
        printf("Invalid character, Retry : ");
        gets(s);
    }
    if(strcmp(s,"Y")==0|| strcmp(s,"y")==0)
       search_int();

}
void main_menu_frame(){
 int i;
    gotoxy(0,0);
    printf("%c",201);               // ascii codes of the edges of the frame
    for(i=1; i<45; i++)printf("%c",205);
    printf("%c",187);
    gotoxy(0,1);
    printf("%c",186);               //186 is code for the vertical walls of the frame
    for(i=1; i<45; i++)printf(" ");
    printf("%c",186);
    gotoxy(0,2);
    printf("%c",186);
    for(i=1; i<45; i++)printf(" ");
    printf("%c",186);
    gotoxy(0,3);
     printf("%c",186);
    for(i=1; i<45; i++)printf(" ");
    printf("%c",186);
    gotoxy(0,4);
    printf("%c",200);
    for(i=1; i<45; i++)printf("%c",205);
    printf("%c",188);
    for (i=5;i<9;i++){
            gotoxy(0,i);
        printf("---------------------------------------------- ");
    }
}
int wordValidator(char *s)
{
    int i;
    for(i=0;i<strlen(s);i++)
    {
        if(!((s[i]>='a' && s[i]<='z') || (s[i]>='A' && s[i]<='Z')||s[i]==39||s[i]==32||s[i]==45))///once a character is not one of the 2 cases letters, return 0 (false)
            return 0;
    }
    return 1;
}
