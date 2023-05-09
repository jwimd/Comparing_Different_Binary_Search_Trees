#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef struct node *ptr;
struct node{
    int element;
    ptr left;
    ptr right;
    int height;
};
typedef struct node *Tree;

Tree root;

Tree createtree(int item);//create new node
void Insertion(int item,Tree T);//insert new item
void deletion(int item,Tree T);//delete the input item
Tree delete(int item,Tree T);//called by deletion
Tree query(int item,Tree T);//search key,print the path
Tree RR(Tree u);
Tree RL(Tree u);
Tree LL(Tree u);
Tree LR(Tree u);
Tree getheight(Tree T);//update the tree
int findmin(Tree T);
void travel(Tree T,char *s);//print the tree in pre order

int main(){
    //insertion
    int n;
    int item;
    scanf("%d",&n);    
    scanf("%d",&item);
    root=createtree(item);
    for(int i=1;i<n;i++){
        scanf("%d",&item);
        Insertion(item,root);
    }
    travel(root,""); 
    //queries,read 0 then finish
    int search;
    scanf("%d",&search);
    while(search){
        query(search,root);
        scanf("%d",&search);
    }
    //deletion,read 0 then finish
    int del;
    scanf("%d",&del);
    while(del){
        deletion(del,root);
        travel(root,"");
        scanf("%d",&del);
    }
    return 0;
}


Tree createtree(int item){
    ptr p=(ptr)malloc(sizeof(struct node));
    p->element=item;
    p->left=NULL;
    p->right=NULL;
    p->height=0;
    return p;
}

void Insertion(int item,Tree T){
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
    if(!T){
        printf("%d is not found.\n",item);
        return NULL;
    }
    if(item==T->element){
        printf("%d is found.\n",item);
        return T;
    }
    else if(item<T->element){
        printf("%d->left\n",T->element);
        return query(item,T->left);
    }
    else{
        printf("%d->right\n",T->element);
        return query(item,T->right);
    }
}

void deletion(int item,Tree T){
    root=delete(item,T);
    if(root)getheight(root);
    return;
}
Tree delete(int item,Tree T){
    if(!T)return NULL;
    if(item==T->element){
        if(!T->left&&!T->right){
            if(T->element==root->element)root=NULL;
            free(T);
            return NULL;
        }
        else if(T->left&&T->right){
            T->element=findmin(T->right);
            delete(findmin(T->right),T->right);
            return T;
        }
        else{
            ptr tempret;
            if(T->right)tempret=T->right;
            else tempret=T->left;
            if(root->element==T->element)root=tempret;
            free(T);
            return tempret;
        }
    }
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
    if(T->left){
        T->left=getheight(T->left);
        lh=T->left->height;
        }
    if(T->right){
        T->right=getheight(T->right);
        rh=T->right->height;
    }
    int sub=(lh>rh)?(lh-rh):(rh-lh);
    if(sub>1){
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
        return getheight(T);
    }
    T->height=(lh>rh)?(lh+1):(rh+1);
    return T;
}

void travel(Tree T,char *s){
    if(!T)return;
    printf("%d\n",T->element);
    if(T->left){
        printf("%s\033[0m\033[1;34mleft:\033[0m",s);
        char *l=(char*)malloc(sizeof(s)+1);
        l[0]='\t';
        strcpy(&l[1],s);
        travel(T->left,l);
    }
    if(T->right){
        printf("%s\033[0m\033[1;32mright:\033[0m",s);
        char *r=(char*)malloc(sizeof(s)+1);
        r[0]='\t';
        strcpy(&r[1],s);
        travel(T->right,r);
    }
    return;
}