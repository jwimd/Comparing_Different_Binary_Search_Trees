#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>

typedef struct node *ptr;
struct node{//the structure of nodes in a tree 
    int element;
    ptr left;
    ptr right;
    ptr parent;
};
typedef struct node *Tree;

Tree root;//the global root
char *insertfilename="./data/Random/Random_40000.txt";//the path of data file
char *queryfilename="./data/Skew/Skew_40000_100.txt";//the path of data file
char *deletefilename="./data/Random/Random_10000_2.txt";//the path of data file

Tree createtree(int item);//create new node
void Insertion(int item,Tree T);//insert new item in tree T and splay
void deletion(int item);//splay the the key of input item, delete
Tree queries(int item,Tree T);//search the key of item,splay it to root 
void delete(int item);//called by deletion,delete the root node
Tree query(int item,Tree T);//called by queries, return the pointer of item
Tree Splay(Tree T,ptr u);//splay the node u to the root of tree T
Tree RR(Tree u);
Tree LL(Tree u);
int findmax(Tree T);//return the max item of T
void travel(Tree T,char *s);//print the tree in pre order
void printresult(int N,double ticks);

int main(){
    clock_t start,stop; // Record the start and end of program runs
    double ticks; // Record the ticks in calls
    FILE *fp;
    fp=fopen(insertfilename,"r");//get the data
    //insertion create the tree
    int n;
    int item;
    fscanf(fp,"%d",&n);
    //start1
    start=clock(); // Record the ticks at the beginning of calls
    fscanf(fp,"%d",&item);
    root=createtree(item);
    for(int i=1;i<n;i++){
        fscanf(fp,"%d",&item);
        Insertion(item,root);
    }
    stop=clock(); // Record the ticks at the end of calls
    ticks=(double)(stop-start); // the ticks between start and end of calls
    printf("Insert:\n");
    printresult(40000,ticks);
    //end1
    fclose(fp);
    // printf("The tree is below:\n");
    // travel(root,"");
    //queries,read 0 then finish
    fp=fopen(queryfilename,"r");
    int search;
    fscanf(fp,"%d",&n);
    n=40000;
    //start2
    start=clock(); // Record the ticks at the beginning of calls
    for(int i=0;i<n;i++){
        fscanf(fp,"%d",&search);
        root=queries(search,root);
    }
    stop=clock(); // Record the ticks at the end of calls
    ticks=(double)(stop-start); // the ticks between start and end of calls
    printf("Search:\n");
    printresult(40000,ticks);
    //end2
    fclose(fp);
    //deletion,read 0 then finish
    fp=fopen(deletefilename,"r");
    int del;
    fscanf(fp,"%d",&n);
    n=10000;
    //start3
    start=clock(); // Record the ticks at the beginning of calls
    for(int i=0;i<n;i++){
        fscanf(fp,"%d",&del);
        deletion(del);
    }
    stop=clock(); // Record the ticks at the end of calls
    ticks=(double)(stop-start); // the ticks between start and end of calls
    printf("Deletion:\n");
    printresult(10000,ticks);
    //end3
    fclose(fp);
    return 0;
}


Tree createtree(int item){
    //apply for a dynamic space for node
    ptr p=(ptr)malloc(sizeof(struct node));
    p->element=item;
    p->left=NULL;
    p->right=NULL;
    p->parent=NULL;
    return p;
}

void Insertion(int item,Tree T){
    //find the place for the node in T and splay it to root
    if(item>T->element){
        if(T->right)Insertion(item,T->right);
        else{
            T->right=createtree(item);
            T->right->parent=T;
            root=Splay(root,T->right);
        }
    }
    else{
        if(T->left)Insertion(item,T->left);
        else{
            T->left=createtree(item);
            T->left->parent=T;
            root=Splay(root,T->left);
        }
    }
    return;
}

Tree queries(int item,Tree T){
    //find the pointer of item
    ptr temp=query(item,T);
    //splay
    T=Splay(T,temp);
    return T;
}
Tree query(int item,Tree T){
    if(!T)return NULL;
    if(item==T->element)return T;
    else if(item<T->element){
        return query(item,T->left);
    }
    else{
        return query(item,T->right);
    }
}

void deletion(int item){
    //splay the item to root
    root=queries(item,root);
    //delete the root
    delete(item);
    return;
}
void delete(int item){
    if(root&&item==root->element){
        if(!root->left&&!root->right){
            //without child, just delete
            free(root);
            root=NULL;
        }
        else if(root->left&&root->right){
            //two children, find the max item in the left subtree
            //splay the node to the root of left subtree;
            //restructure the tree
            root->left=queries(findmax(root->left),root->left);
            root->left->right=root->right;
            root=root->left;
            root->right->parent=root;
            ptr del=root->parent;
            root->parent=NULL;
            free(del);
        }
        else{//one child, free the root
            if(root->right)root=root->right;
            else root=root->left;
            ptr del=root->parent;
            root->parent=NULL;
            free(del);
        }
    }
    return;
}

int findmax(Tree T){
    if(T->right)return findmax(T->right);
    else return T->element;
}
Tree RR(Tree u){
    //rotetion
    ptr v=u->right;
    if(v->left){
        ptr vl=v->left;
        u->right=vl;
        vl->parent=u;
    }
    else{
        u->right=NULL;
    }
    v->left=u;
    if(u->parent){
        v->parent=u->parent;
        if(u->parent->element>u->element){
            u->parent->left=v;
        }
        else{
            u->parent->right=v; 
        }
    }
    else{
        v->parent=NULL;
    }
    u->parent=v;
    if(!v->parent)root=v;
    return v; 
}

Tree LL(Tree u){
    ptr v=u->left;
    if(v->right){
        ptr vr=v->right;
        u->left=vr;
        vr->parent=u;
    }
    else{
        u->left=NULL;
    }
    v->right=u;
    if(u->parent){
        v->parent=u->parent;
        if(u->parent->element>u->element){
            u->parent->left=v;
        }
        else{
            u->parent->right=v;
        }
    }
    else{
        v->parent=NULL;
    }
    u->parent=v;
    if(!v->parent)root=v;
    return v;
}

void travel(Tree T,char *s){
    if(!T)return; 
    //show the root
    printf("%d\n",T->element);
    //show the right root
    if(T->right){
        printf("%s\033[0m\033[1;32mright:\033[0m",s);
        char *r=(char*)malloc(sizeof(s)+1);
        r[0]='\t';
        strcpy(&r[1],s);
        travel(T->right,r);
    }
    //show the left root
    if(T->left){
        printf("%s\033[0m\033[1;34mleft:\033[0m",s);
        char *l=(char*)malloc(sizeof(s)+1);
        l[0]='\t';
        strcpy(&l[1],s);
        travel(T->left,l);
    }
    return;
}

Tree Splay(Tree T,ptr u){
    //if it's root, done
    if(T->element==u->element)return T;
    //if root is its parent, rotation
    else if(u->parent&&T->element==u->parent->element){
        if(T->element>u->element)return LL(T);
        else return RR(T);
    }
    else if(u->parent->parent){
        //if it has grandparents
        if(u->parent->parent->element>u->parent->element){
            //divide into four different case
            if(u->parent->element>u->element){
                ptr temp=LL(u->parent->parent);
                if(temp->right->element==T->element)T=temp;
                temp=LL(temp);
                if(temp->right->element==T->element)T=temp;                
                return Splay(T,temp);
            }
            else{
                ptr temp=RR(u->parent);
                temp=LL(temp->parent);
                if(temp->right->element==T->element)T=temp;
                return Splay(T,temp);
            }
        }
        else{
            if(u->parent->element>u->element){
                ptr temp=LL(u->parent);
                temp=RR(temp->parent);
                if(temp->left->element==T->element)T=temp;
                return Splay(T,temp);
            }
            else{
                ptr temp=RR(u->parent->parent);
                if(temp->left->element==T->element)T=temp;
                temp=RR(temp);
                if(temp->left->element==T->element)T=temp;
                return Splay(T,temp);                
            }
        }
    }
    else{
        return NULL;
    }
}
void printresult(int N,double ticks)
{
    //N is the times of function runs, ticks is ticks between start and ending of k times of runs
    printf("The ticks of %d function runs is:%lf\n",N,ticks);
    printf("The total time of %d function runs is:%lf seconds\n",N,ticks/CLK_TCK);
    printf("The average time of a function run is:%lf seconds\n",ticks/N/CLK_TCK);
}