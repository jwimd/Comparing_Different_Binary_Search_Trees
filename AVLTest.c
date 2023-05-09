#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>

typedef struct node *ptr;
struct node{
    int element;
    ptr left;
    ptr right;
    int height;
};
typedef struct node *Tree;

Tree root;//the global root
char *insertfilename="./data/Random/Random_40000.txt";//the path of data file
char *queryfilename="./data/Skew/Skew_40000_100.txt";//the path of data file
char *deletefilename="./data/Random/Random_10000_2.txt";//the path of data file

Tree createtree(int item);//create new node
void Insertion(int item,Tree T);//insert new item
void deletion(int item);//delete the input item
Tree delete(int item,Tree T);//called by deletion
Tree query(int item,Tree T);//search key,print the path
//four kinds of rotations
Tree RR(Tree u);
Tree RL(Tree u);
Tree LL(Tree u);
Tree LR(Tree u);
Tree getheight(Tree T);//update the tree
int findmin(Tree T);//find the min key in tree T
void travel(Tree T,char *s);
void printresult(int N,double ticks);

int main(){
    clock_t start,stop; // Record the start and end of program runs
    double ticks; // Record the ticks in calls
    FILE *fp;
    fp=fopen(insertfilename,"r");
    //insertion
    int n;
    int item;
    //start1
    start=clock(); // Record the ticks at the beginning of calls
    fscanf(fp,"%d",&n);
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
    //start2
    start=clock(); // Record the ticks at the beginning of calls
    for(int i=0;i<n;i++){
        fscanf(fp,"%d",&search);
        query(search,root);
    }
    stop=clock(); // Record the ticks at the end of calls
    ticks=(double)(stop-start); // the ticks between start and end of calls
    printf("search:\n");
    printresult(40000,ticks);
    //end2
    fclose(fp);
    //deletion,read 0 then finish
    fp=fopen(deletefilename,"r");
    int del;
    fscanf(fp,"%d",&n);
    //start3
    start=clock(); // Record the ticks at the beginning of calls
    for(int i=0;i<n;i++){
        fscanf(fp,"%d",&del);
        deletion(del);
    }
    stop=clock(); // Record the ticks at the end of calls
    ticks=(double)(stop-start); // the ticks between start and end of calls
    printf("Delete:\n");
    printresult(10000,ticks);
    //end3
    fclose(fp);
    return 0;
}


Tree createtree(int item){
    //initiate the node
    ptr p=(ptr)malloc(sizeof(struct node));
    p->element=item;
    p->left=NULL;
    p->right=NULL;
    p->height=0;
    return p;
}

void Insertion(int item,Tree T){
    //find the correct place and create new node
    if(item>T->element){
        if(T->right)Insertion(item,T->right);
        else T->right=createtree(item);
    }
    else{
        if(T->left)Insertion(item,T->left);
        else T->left=createtree(item);
    }
    root=getheight(root);
    return;
}
Tree query(int item,Tree T){
    //print out the path of search
    if(!T){
        // printf("%d is not found.\n",item);
        return NULL;
    }
    if(item==T->element){
        // printf("%d is found.\n",item);
        return T;
    }
    else if(item<T->element){
        // printf("%d->left\n",T->element);
        return query(item,T->left);
    }
    else{
        // printf("%d->right\n",T->element);
        return query(item,T->right);
    }
}

void deletion(int item){
    //delete the item
    root=delete(item,root);
    //update the tree
    if(root)getheight(root);
    return;
}
Tree delete(int item,Tree T){
    if(!T)return NULL;
    if(item==T->element){
        if(!T->left&&!T->right){
            //without chilren ,just free it
            if(T->element==root->element)root=NULL;
            free(T);
            return NULL;
        }
        else if(T->left&&T->right){
            //has two children
            //exchange with the minest key of its right subtree
            T->element=findmin(T->right);
            T->right=delete(findmin(T->right),T->right);
            return T;
        }
        else{
            //has one child, replace it with its child
            ptr tempret;
            if(T->right)tempret=T->right;
            else tempret=T->left;
            if(root->element==T->element)root=tempret;
            free(T);
            return tempret;
        }
    }
    //find the key
    else if(item>T->element){
        if(T->right)T->right=delete(item,T->right);
    }
    else{
        if(T->left)T->left=delete(item,T->left);
    }
    return T; 
}

int findmin(Tree T){
    if(T->left)return findmin(T->left);
    else return T->element;
}
Tree RR(Tree u){
    ptr v=u->right;
    u->right=v->left;
    v->left=u;
    if(root->element==u->element)root=v;
    return v;    
}

Tree LL(Tree u){
    ptr v=u->left;
    u->left=v->right;
    v->right=u;
    if(root->element==u->element)root=v;
    return v;    
}
Tree RL(Tree u){
    u->right=LL(u->right);
    return RR(u);
}
Tree LR(Tree u){
    u->left=RR(u->left);
    return LL(u);
}


Tree getheight(Tree T){
    int lh=0,rh=0;
    //recursive, bottom up to update
    if(T->left){
        T->left=getheight(T->left);
        lh=T->left->height;
        }
    if(T->right){
        T->right=getheight(T->right);
        rh=T->right->height;
    }
    //to figure out whether rotation or not
    int sub=(lh>rh)?(lh-rh):(rh-lh);
    if(sub>1){
        //divide into four cases in class
        if(lh>rh){
            if(T->left->left&&T->left->right){
                if(T->left->left->height>T->left->right->height)T=LL(T);
                else T=LR(T);
            }
            else{
                if(T->left->left)T=LL(T);
                else T=LR(T);
            }
        }
        else{
            if(T->right->left&&T->right->right){
                if(T->right->left->height>T->right->right->height)T=RL(T);
                else T=RR(T);
            }
            else{
                if(T->right->left)T=RL(T);
                else T=RR(T);
            }
        }
        //update again because of the rotation
        return getheight(T);
    }
    //update the height
    T->height=(lh>rh)?(lh+1):(rh+1);
    return T;
}
void printresult(int N,double ticks)
{
    //N is the times of function runs, ticks is ticks between start and ending of k times of runs
    printf("The ticks of %d function runs is:%lf\n",N,ticks);
    printf("The total time of %d function runs is:%lf seconds\n",N,ticks/CLK_TCK);
    printf("The average time of a function run is:%lf seconds\n",ticks/N/CLK_TCK);
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