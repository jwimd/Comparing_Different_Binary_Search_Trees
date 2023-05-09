#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<time.h>
#define red true
#define black false

#define RBT_Left_Child 1
#define RBT_Right_Child 2
#define RBT_Is_Root 3
#define To_Define 4
#define N 10000

struct Red_Black_Tree_Node;

typedef struct Red_Black_Tree_Node RBTree;
typedef RBTree* rbt;

struct Red_Black_Tree_Node
{
	int data;
	rbt left;
	rbt right;
	rbt father;
	bool color;
	int position;
};

bool Node_Is_Red(rbt r);

rbt RBT_Find_Root(rbt p);

rbt RBT_Node_Initialization();

rbt RBT_Left_Rotation(rbt root); 

rbt RBT_Right_Rotation(rbt root);

rbt RBT_Fix_LL(rbt p);

rbt RBT_Fix_LR(rbt p);

rbt RBT_Fix_RR(rbt p);

rbt RBT_Fix_RL(rbt p);

rbt RBT_Fix(rbt p);

rbt RBT_Insert(int data, rbt node, rbt father, int position);

void RBT_Free(rbt r);

void RBT_Print(rbt root);

rbt RBT_Sreach(int data, rbt p);

rbt RBT_Deletion(int data, rbt root);

void RBT_Node_Free(rbt p);

rbt RBT_Find_Smallest(rbt p);

bool Node_Is_Red(rbt r)
{
	if (!r || !r->color)
		return black;
	else
		return red;
}

rbt RBT_Find_Root(rbt p)
{
	if (!p)
		return NULL;
	else if (!p->father)
		return p;
	else
		return RBT_Find_Root(p->father);
}

rbt RBT_Node_Initialization()
{
	rbt r = (rbt)malloc(sizeof(RBTree));
	if (r != NULL)
	{
		r->data = 0;
		r->left = NULL;
		r->right = NULL;
		r->father = NULL;
		r->color = red;
		r->position = To_Define;
	}
	return r;
}

rbt RBT_Left_Rotation(rbt root)//����������������������ת���������ڵ�
{
	rbt right = root->right;
	if (!right)
		return root;
	root->right = right->left;
	right->left = root;
	right->father = root->father;
	root->father = right;
	right->position = root->position;
	root->position = RBT_Left_Child;
	if (root->right)
	{
		root->right->father = root;
		root->right->position = RBT_Right_Child;
	}
	return right;
}

rbt RBT_Right_Rotation(rbt root)
{
	rbt left = root->left;
	if (!left)
		return root;
	root->left = left->right;
	left->right = root;
	left->father = root->father;
	root->father = left;
	left->position = root->position;
	root->position = RBT_Right_Child;
	if (root->left)
	{
		root->left->father = root;
		root->left->position = RBT_Left_Child;
	}
	return left;
}

rbt RBT_Fix_LL(rbt p)
{
	rbt father = p->father;
	rbt grandfather = father->father;
	bool temp = p->color;
	p->color = father->color;
	father->color = temp;
	if (father->position == RBT_Left_Child)
		grandfather->left = RBT_Right_Rotation(father);
	else if (father->position == RBT_Right_Child)
		grandfather->right = RBT_Right_Rotation(father);
	else
		RBT_Right_Rotation(father);
	return p;
}

rbt RBT_Fix_LR(rbt p)
{
	rbt father = p->father;
	rbt grandfather = father->father;
	if (p->position == RBT_Left_Child)
		father->left = RBT_Left_Rotation(p);
	else if (p->position == RBT_Right_Child)
		father->right = RBT_Left_Rotation(p);
	else
		RBT_Left_Rotation(p);
	return RBT_Fix(p);
}

rbt RBT_Fix_RR(rbt p)
{
	rbt father = p->father;
	rbt grandfather = father->father;
	bool temp = p->color;
	p->color = father->color;
	father->color = temp;
	if (father->position == RBT_Left_Child)
		grandfather->left = RBT_Left_Rotation(father);
	else if (father->position == RBT_Right_Child)
		grandfather->right = RBT_Left_Rotation(father);
	else
		RBT_Left_Rotation(father);
	return p;
}

rbt RBT_Fix_RL(rbt p)
{
	rbt father = p->father;
	rbt grandfather = father->father;
	if (p->position == RBT_Left_Child)
		father->left = RBT_Right_Rotation(p);
	else if (p->position == RBT_Right_Child)
		father->right = RBT_Right_Rotation(p);
	else
		RBT_Right_Rotation(p);
	return RBT_Fix(p);
}

rbt RBT_Fix(rbt p)//����ڵ��Ժ�����޸�����������
{
	if (!p->father)//�ڵ�Ϊ���ڵ�
	{
		p->color = black;
		return p;
	}
	if (!Node_Is_Red(p->father))//���׽ڵ��Ǻ�ɫ
		return RBT_Find_Root(p);
	else if (Node_Is_Red(p->father))//���׽ڵ��Ǻ�ɫ
	{
		rbt father = p->father, grandfather = father->father;
		if (!grandfather)
		{
			father->color = black;
			p->color = red;
			return father;
		}
		rbt uncle = NULL;
		if (father->position == RBT_Left_Child)
			uncle = grandfather->right;
		else
			uncle = grandfather->left;
		if (Node_Is_Red(uncle))//����ڵ��Ǻ�ɫ
		{
			father->color = black;
			uncle->color = black;
			grandfather->color = red;
			return RBT_Fix(grandfather);
		}
		else
		{
			if (p->position == RBT_Left_Child && father->position == RBT_Left_Child)//LL
				return RBT_Find_Root(RBT_Fix_LL(p->father));
			else if (p->position == RBT_Right_Child && father->position == RBT_Left_Child)
				return RBT_Find_Root(RBT_Fix_LR(p->father));
			else if (p->position == RBT_Right_Child && father->position == RBT_Right_Child)
				return RBT_Find_Root(RBT_Fix_RR(p->father));
			else if (p->position == RBT_Left_Child && father->position == RBT_Right_Child)
				return RBT_Find_Root(RBT_Fix_RL(p->father));
		}
		
	}
}

rbt RBT_Insert(int data, rbt node, rbt father ,int position)
{
	if (!node)
	{
		node = RBT_Node_Initialization();
		node->data = data;
		node->father = father;
		if (father)
		{
			if (position == RBT_Left_Child)
				father->left = node;
			else if (position == RBT_Right_Child)
				father->right = node;
		}
		node->position = position;
		return RBT_Fix(node);
	}
	else if (data < node->data)
		return RBT_Insert(data, node->left, node, RBT_Left_Child);
	else if (data > node->data)
		return RBT_Insert(data, node->right, node, RBT_Right_Child);
	else
	{
		printf("The key has existed!\n");
		return RBT_Find_Root(node);
	}
}

void RBT_Print(rbt root)
{
	if (!root)
	{
		printf_s("Empty tree!");
		return;
	}
	rbt* quene1, * quene2;
	quene1 = (rbt*)malloc(sizeof(RBTree) * N);
	quene2 = (rbt*)malloc(sizeof(RBTree) * N);
	int top1 = 0, top2 = 0, tail1 = -1, tail2 = -1;
	quene1[++tail1] = root;
	while (top1 <= tail1)
	{
		if (quene1[top1]->left)
			quene2[++tail2] = quene1[top1]->left;
		if (quene1[top1]->right)
			quene2[++tail2] = quene1[top1]->right;
		if (quene1[top1]->father)
		{
			if (quene1[top1]->color)
			{
				if (quene1[top1]->position == 1)
					printf_s("[%d,red,left child of %d] ", quene1[top1]->data, quene1[top1]->father->data);
				else if (quene1[top1]->position == 2)
					printf_s("[%d,red,right child of %d] ", quene1[top1]->data, quene1[top1]->father->data);
			}
			else
			{
				if (quene1[top1]->position == 1)
					printf_s("[%d,black,left child of %d] ", quene1[top1]->data, quene1[top1]->father->data);
				else if (quene1[top1]->position == 2)
					printf_s("[%d,black,right child of %d] ", quene1[top1]->data, quene1[top1]->father->data);
			}
		}
		else
			printf_s("[%d,root] ", quene1[top1]->data);
		top1++;
		if (top1 > tail1)
		{
			printf_s("\n");
			rbt* temp;
			temp = quene1;
			quene1 = quene2;
			quene2 = temp;
			if (!quene1)
				break;
			top1 = top2;
			tail1 = tail2;
			top2 = 0;
			tail2 = -1;
		}
	}
	printf_s("\n");
	return;
}

rbt RBT_Sreach(int data, rbt p)
{
	if (!p)
	{
		// printf_s("The key doesn't exist!\n");
		return NULL;
	}
	if (data == p->data)
		return p;
	else if (data < p->data)
		return RBT_Sreach(data, p->left);
	else if (data > p->data)
		return RBT_Sreach(data, p->right);
}

void RBT_Node_Free(rbt p)
{
	if (p->position == RBT_Left_Child)
		p->father->left = NULL;
	else if (p->position == RBT_Right_Child)
		p->father->right = NULL;
	free(p);
	p = NULL;
	return;
}

rbt RBT_Find_Smallest(rbt p)
{
	if (p->left)
		RBT_Find_Smallest(p->left);
	else
		return p;
}

rbt RBT_Deletion_Rotation(rbt p)
{
	if (Node_Is_Red(p)|| p->position == RBT_Is_Root)
	{
		p->color = black;
		return RBT_Find_Root(p);
	}
	rbt father = p->father;
	rbt brother = NULL;
	rbt near_nephew = NULL;
	rbt far_nephew = NULL;
	if (p->position == RBT_Left_Child)
	{
		brother = father->right;
		near_nephew = brother->left;
		far_nephew = brother->right;
	}
	else if (p->position == RBT_Right_Child)
	{
		brother = father->left;
		near_nephew = brother->right;
		far_nephew = brother->left;
	}
	if (!Node_Is_Red(p) && Node_Is_Red(brother))
	{
		father->color = red;
		brother->color = black;
		rbt grandfather = father->father;
		if (father->position == RBT_Left_Child)
		{
			if (p->position == RBT_Left_Child)
			{
				grandfather->left = RBT_Left_Rotation(father);
			}
			else if (p->position == RBT_Right_Child)
			{
				grandfather->left = RBT_Right_Rotation(father);
			}
		}
		else if (father->position == RBT_Right_Child)
		{
			if (p->position == RBT_Left_Child)
			{
				grandfather->right = RBT_Left_Rotation(father);
			}
			else if (p->position == RBT_Right_Child)
			{
				grandfather->right = RBT_Right_Rotation(father);
			}
		}
		else
		{
			if (p->position == RBT_Left_Child)
			{
				RBT_Left_Rotation(father);
			}
			else if (p->position == RBT_Right_Child)
			{
				RBT_Right_Rotation(father);
			}
		}
		return RBT_Deletion_Rotation(p);
	}
	if (!Node_Is_Red(p) && !Node_Is_Red(brother))
	{
		if (!Node_Is_Red(near_nephew) && !Node_Is_Red(far_nephew))
		{
			brother->color = red;
			return RBT_Deletion_Rotation(father);
		}
		else if (Node_Is_Red(near_nephew) && !Node_Is_Red(far_nephew))
		{
			brother->color = red;
			near_nephew->color = black;
			if (p->position == RBT_Left_Child)
				father->right = RBT_Right_Rotation(brother);
			else if (p->position == RBT_Right_Child)
				father->left = RBT_Left_Rotation(brother);
			return RBT_Deletion_Rotation(p);
		}
		else if (Node_Is_Red(far_nephew))
		{
			brother->color = Node_Is_Red(father);
			father->color = black;
			far_nephew->color = black;
			rbt grandfather = father->father;
			if (father->position == RBT_Left_Child)
			{
				if (p->position == RBT_Left_Child)
				{
					grandfather->left = RBT_Left_Rotation(father);
				}
				else if (p->position == RBT_Right_Child)
				{
					grandfather->left = RBT_Right_Rotation(father);
				}
			}
			else if (father->position == RBT_Right_Child)
			{
				if (p->position == RBT_Left_Child)
				{
					grandfather->right = RBT_Left_Rotation(father);
				}
				else if (p->position == RBT_Right_Child)
				{
					grandfather->right = RBT_Right_Rotation(father);
				}
			}
			else
			{
				if (p->position == RBT_Left_Child)
				{
					RBT_Left_Rotation(father);
				}
				else if (p->position == RBT_Right_Child)
				{
					RBT_Right_Rotation(father);
				}
			}
			return RBT_Find_Root(p);
		}
	}
}

rbt RBT_Deletion_Fix(rbt p)
{
	if (!p->left && !p->right)
	{
		if (p->color == red)
		{
			rbt root = RBT_Find_Root(p->father);
			RBT_Node_Free(p);
			return root;
		}
		else
		{
			rbt root = RBT_Deletion_Rotation(p);
			if (p->position == RBT_Is_Root)
			{
				RBT_Node_Free(p);
				return NULL;
			}
			RBT_Node_Free(p);
			return root;
		}
	}
	if (p->left && !p->right)
	{
		int temp = p->data;
		p->data = p->left->data;
		p->left->data = temp;
		return RBT_Deletion_Fix(p->left);
	}
	if (!p->left && p->right)
	{
		int temp = p->data;
		p->data = p->right->data;
		p->right->data = temp;
		return RBT_Deletion_Fix(p->right);
	}
	if (p->left && p->right)
	{
		rbt smallest = RBT_Find_Smallest(p->right);
		int temp = p->data;
		p->data = smallest->data;
		smallest->data = temp;
		return RBT_Deletion_Fix(smallest);
	}
}

rbt RBT_Deletion(int data, rbt root)
{
	rbt Node_Deleted = RBT_Sreach(data, root);
	if (!Node_Deleted)
		return root;
	root = RBT_Deletion_Fix(Node_Deleted);
	return root;
}

void RBT_Free(rbt r)
{
	if (!r)
		return;
	RBT_Free(r->left);
	RBT_Free(r->right);
	free(r);
	r = NULL;
	return;
}

void printresult(int times, double ticks)
{
    //N is the times of function runs, ticks is ticks between start and ending of k times of runs
    printf("The ticks of %d function runs is:%lf\n",times,ticks);
    printf("The total time of %d function runs is:%lf seconds\n",times,ticks/CLK_TCK);
    printf("The average time of a function run is:%lf seconds\n",ticks/times/CLK_TCK);
}
char *insertfilename="./data/Random/Random_40000.txt";//the path of data file
char *queryfilename="./data/skew/Skew_40000_100.txt";//the path of data file
char *deletefilename="./data/Random/Random_10000_2.txt";//the path of data file

int main()
{
    clock_t start,stop; // Record the start and end of program runs
    double ticks; // Record the ticks in calls
	rbt RBT_Root = NULL;
    FILE *fp;
    fp=fopen(insertfilename,"r");
    //insertion
    int n;
    int item;
    fscanf(fp,"%d",&n);
    //start1
	n=10000;
    start=clock(); // Record the ticks at the beginning of calls
    for(int i=0;i<n;i++){
        fscanf(fp,"%d",&item);
        RBT_Root = RBT_Insert(item, RBT_Root, NULL, RBT_Is_Root);
    }
    stop=clock(); // Record the ticks at the end of calls
    ticks=(double)(stop-start); // the ticks between start and end of calls
    printf("Insert:\n");
    printresult(10000,ticks);
    //end1
    fclose(fp);
    fp=fopen(queryfilename,"r");
    int search;
    fscanf(fp,"%d",&n);
	n=10000;
    //start2
    start=clock(); // Record the ticks at the beginning of calls
    for(int i=0;i<n;i++){
        fscanf(fp,"%d",&search);
        RBT_Sreach(search,RBT_Root);
    }
    stop=clock(); // Record the ticks at the end of calls
    ticks=(double)(stop-start); // the ticks between start and end of calls
    printf("search:\n");
    printresult(10000,ticks);
    //end2
    fclose(fp);
    //deletion,read 0 then finish
    // fp=fopen(deletefilename,"r");
    // int del;
    // fscanf(fp,"%d",&n);
    // //start3
    // start=clock(); // Record the ticks at the beginning of calls
    // for(int i=0;i<n;i++){
    //     fscanf(fp,"%d",&del);
    //     RBT_Root = RBT_Deletion(del, RBT_Root);
    // }
    // stop=clock(); // Record the ticks at the end of calls
    // ticks=(double)(stop-start); // the ticks between start and end of calls
    // printf("Delete:\n");
    // printresult(10000,ticks);
    // //end3
    // fclose(fp);
    return 0;
}

