#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef struct node *ptr;
struct node{
    int element;
    ptr left;
    ptr right;
    ptr parent;
};
typedef struct node *Tree;

Tree root;

Tree createtree(int item);//create new node
void Insertion(int item,Tree T);//insert new item,splay
void deletion(int item);//delete the input item,splay
Tree queries(int item,Tree T);//search key,splay the tree
void delete(int item);//called by deletion
Tree query(int item,Tree T);//called by queries
Tree Splay(Tree T,ptr u);
Tree RR(Tree u);
Tree LL(Tree u);
int findmax(Tree T);
void travel(Tree T,char *s);//print the tree in pre order

int main(){
    //insertion
    int n;
    int item;
    scanf("%d",&n);    
    scanf("%d",&item);
    root=createtree(item);
    travel(root,"\t"); 
    for(int i=1;i<n;i++){
        scanf("%d",&item);
        Insertion(item,root);
        travel(root,"\t"); 
    }
    //queries,read 0 then finish
    int search;
    printf("Choose the key you want to search:");
    scanf("%d",&search);
    while(search){
        root=queries(search,root);
        travel(root,"\t"); 
        printf("Choose the key you want to search:");
        scanf("%d",&search);
    }
    //deletion,read 0 then finish
    int del;
    printf("Choose the key you want to delete:");
    scanf("%d",&del);
    while(del){
        deletion(del);
        if(root)travel(root,"\t");
        else break;
        printf("Choose the key you want to delete:");
        scanf("%d",&del);
    }
    return 0;
}


Tree createtree(int item){
    ptr p=(ptr)malloc(sizeof(struct node));
    p->element=item;
    p->left=NULL;
    p->right=NULL;
    p->parent=NULL;
    return p;
}

void Insertion(int item,Tree T){
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
    ptr temp=query(item,T);
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
    root=queries(item,root);
    delete(item);
    return;
}
void delete(int item){
    if(root&&item==root->element){
        if(!root->left&&!root->right){//without child
            free(root);
            root=NULL;
        }
        else if(root->left&&root->right){//two children
            root->left=queries(findmax(root->left),root->left);
            root->left->right=root->right;
            root=root->left;
            root->right->parent=root;
            ptr del=root->parent;
            root->parent=NULL;
            free(del);
        }
        else{//one child
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
    printf("%d\n",T->element);
    if(T->right){
        printf("%s\033[0m\033[1;32mright:\033[0m",s);
        char *r=(char*)malloc(sizeof(s)+1);
        r[0]='\t';
        strcpy(&r[1],s);
        travel(T->right,r);
    }
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
    if(T->element==u->element)return T;
    else if(u->parent&&T->element==u->parent->element){
        if(T->element>u->element)return LL(T);
        else return RR(T);
    }
    else if(u->parent->parent){
        if(u->parent->parent->element>u->parent->element){
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