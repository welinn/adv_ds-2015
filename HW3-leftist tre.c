#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define TABLE 10

typedef struct node{
	struct node *left;
	struct node *right;
	int data;
	int shortest;
	int weight;
}node;
typedef struct table{
	
	int No;
	int val;
	struct table* next;
	
}table;
void leftTree(FILE*, int);
void mergeL(node*, node*, node*);
void mergeW(node*, node*);
void newNode(node*, int);
void draw(node*);
void row(int, int, int);
void column(int, int, int, int);
int leaf(int, int);
int level(node*);
void buildTable(node*, int, int);
table* newTable(int, int);
void freeTable(int);

table* tab[TABLE];

int main(){

	FILE *come;
	double l, w;
	clock_t start, end;

	come = fopen("leftist tree.txt", "r");
	if (!come) {
        printf("檔案開啟失敗\n");
		return 0;
    }
	start = clock();
	leftTree(come, 0);
	end = clock();
	l = end - start;
	

	rewind(come);
	start = clock();
	leftTree(come, 1);
	end = clock();
	w = end - start;

	fclose(come);

	printf("length time:%lf\nweight time:%lf\n",l, w);

	return 0;
}

void leftTree(FILE *come, int no){
	
	node *root = NULL, *add = NULL, *del = NULL;
	char op;
	int n;

	while(!feof(come)){
		fscanf(come,"%c", &op);
		if(op == 'i' || op == 'I'){    //新增
			fscanf(come, "%d", &n);
			if(!root){
				root = (node*) malloc(sizeof(node));
				newNode(root, n);
			}
			else{
				if(root->data > n){
					add = root;
					root = (node*) malloc(sizeof(node));
					newNode(root, n);
				}
				else{
					add = (node*) malloc(sizeof(node));
					newNode(add, n);
				}
				if(no == 0)
					mergeL(root, add, NULL);
				else
					mergeW(root, add);
			}
		}
		else if(!root){
			printf("tree empty\n");
		}
		else if(op == 'd' || op == 'D'){    //刪除
			printf("%d\n",root->data);
			del = root;
			if(!root->right){
				root = root->left;
			}
			else if(root->left->data < root->right->data){
				add = root->right;
				root = root->left;
				if(no == 0)
					mergeL(root, add, NULL);
				else
					mergeW(root, add);
			}
			else{
				add = root->left;
				root = root->right;
				if(no == 0)
					mergeL(root, add, NULL);
				else
					mergeW(root, add);
			}
			free(del);
			del = NULL;
		}
	}
	//印樹------------------------------
	draw(root);
}

void newNode(node* a, int n){
	
	a->data = n;
	a->left = NULL;
	a->right = NULL;
	a->shortest = 1;
	a->weight = 1;
}
void mergeL(node *a, node *b, node *parent){   //合併(長
	
	node *tmp;

	if(a->data > b->data){  //保持a小b大
		tmp = a;
		a = b;
		b = tmp;
		if(parent)
			parent->right = a;
	}
	if(!a->right) a->right = b;  //合併
	else{                        //還沒找到點 遞迴
		mergeL(a->right, b, a);
	}
	b = NULL;
	
	//改高度
	if(!a->left || !a->right)
		a->shortest = 1;
	else{
		a->shortest = a->left->shortest > a->right->shortest ?
					  a->right->shortest + 1 : a->left->shortest + 1;
	}

	//檢查左右長度
	if(!a->left || !a->right){
		if(!a->left){
			a->left = a->right;
			a->right = NULL;
		}
	}
	else if(a->left->shortest < a->right->shortest){
		tmp = a->left;
		a->left = a->right;
		a->right = tmp;
	}
}
void mergeW(node *a, node *b){   //合併(重
	
	node *tmp, *parent = NULL;
	int p = 0;

	while(b){
		if(a->data > b->data){  //保持a小b大
			tmp = a;
			a = b;
			b = tmp;
			if(parent){
				if(p == 0) parent->right = a;
				else parent->left = a;
				
			}
		}
		a->weight += b->weight;  //調整root的重量

		if(!a->right){    //合併
			a->right = b;
			//右子重 左右子樹交換
			if(!a->left){
					a->left = a->right;
					a->right = NULL;
			}
			else if(a->left->weight < a->right->weight){
				tmp = a->left;
				a->left = a->right;
				a->right = tmp;
			}
			b = NULL;
		}
		else{
			parent = a;
			//右子重 左右子樹交換
			if(a->left->weight < (a->right->weight + b->weight)){
				tmp = a->left;
				a->left = a->right;
				a->right = tmp;
				a = a->left;
				p = 1;
			}
			else{
				a = a->right;
				p = 0;
			}
		}
	}
}
//===========================
void draw(node* root){  //畫樹

	int i, j, k, n, lev, x = 2;
	int space, print, check, count;
	table *ptr;
	if(root){
		buildTable(root, 0, 1);

		lev = level(root);  //樹的層數 
		for(i=0, n=0; i < lev; i++){  //算底層總長 = n
			n = n*2+1;
		}
		print = 1;  //該層滿時有幾個node
	
		for(i = 0; i < lev; i++){
			count = print;
			space = n/x;
			check = 0;
			j = 0;
			k = 1;
			ptr = tab[i];
		
			while(count){
			
				if(check == 0 && j < space){
					printf("   ");
					j++;
				}
				else if(j < space){
					printf("      ");
					j++;
				}
				else{
					check = 1;
					j = 0;
					if(k == ptr->No){
						printf("%3d   ", ptr->val);
						ptr = ptr->next;
						k++;
					}
					else{
						printf("      ");
						k++;
					}
					if(ptr == NULL) break;
				
					count--;
				}
			}
			printf("\n");
		
			if(i+1 != lev){  //下面還有 要畫橫線 & 直槓槓 
				column(i, print, space, 0);
				row(i+1, print*2, n/(x*2));
				column(i+1, print*2, n/(x*2), 1);
			}
			print*=2;
			x*=2;
		}
		freeTable(lev);
	}
}
void row(int t, int count, int space){  //畫橫線 
	
	table *ptr;
	int i, j, k, check = 0, c = 0;	
	
	ptr = tab[t];
	for(i = 0, k = 1; i < count; i++){
		
		if(check == 0){
			for(j = 0; j < space; j++){
				printf("   ");
			}
		}
		else{
			for(j = 0; j < space; j++){
				printf("---");
			}
		}
//----------------------------		
		if(k%2 == 1 && ptr->No == k){  //有節點(左)
			printf(" --");
			ptr = ptr->next;
			check = 1;
		}
		else if(ptr->No == k){  //有節點(右)
			printf("-- ");
			ptr = ptr->next;
			check = 0;
		}
		else printf("   ");
		
		if(ptr == NULL && check == 1) c = 1;
		else if(ptr == NULL) break;
		k++;
//------------------------------			
		if(check == 0){
			for(j = 0; j < space; j++){
				printf("   ");
			}
		}
		else{
			for(j = 0; j < space; j++){
				printf("---");
			}
			if(c == 1){
				c = 0;
				break;
			}
		}	
		printf("   ");
		
		if(k%2 == 0 && ptr->No == k) check = 1;
		else check = 0;
	}
	printf("\n");
}
void column(int i, int count, int space, int w){  //畫直槓槓 
	
	table *ptr;
	int check = 0, j = 0, k = 1;
	
	ptr = tab[i];
	while(count){
			
		if(check == 0 && j < space){
			printf("   ");
			j++;
		}
		else if(j < space){
			printf("      ");
			j++;
		}
		else{
			check = 1;
			j = 0;
			if(k == ptr->No){
				
				//w標記 0表畫節點下方槓槓 
				if(w == 0 && leaf(i+1, k*2) == 0){  //是leaf 
					printf("      ");
				}
				else printf(" |    ");
				
				ptr = ptr->next;
				k++;
			}
			else{
				printf("      ");
				k++;
			}
			if(ptr == NULL) break;
			
			count--;
		}
	}
	printf("\n");
}
int leaf(int i, int k){
	
	int ret = 0;
	table * ptr = tab[i];
	
	while(ptr){
		if(ptr->No == k-1 || ptr->No == k){
			ret = 1;
			break;
		}
		if(ptr->No > k) break;
		ptr = ptr->next;
	}	
	return ret;
}
int level(node* root){  //算樹高 
	
	int l,r;
	
	if(root == NULL) return 0;
	else{
		l = 1+level(root->left);
		r = 1+level(root->right);
		root->shortest = l>r ? l : r;
		return root->shortest;
	}
}
void buildTable(node* root, int i, int no){
	
	table* ptr = NULL;

	if(!root)
		return;
	else{
		ptr = tab[i];
		if(!ptr){
			ptr = newTable(no, root->data);
			tab[i] = ptr;
		}
		else{
			while(ptr->next)
				ptr = ptr->next;
			ptr->next = newTable(no, root->data);
		}
		buildTable(root->left, i+1, no*2-1);
		buildTable(root->right, i+1, no*2);
	}
}
table* newTable(int no, int v){
	
	table *addT;
	
	addT = (table*)malloc(sizeof(table));
	addT->No = no;
	addT->val = v;
	addT->next = NULL;
	
	return addT;
}
void freeTable(int lev){
	int i;
	table* ptr, *del;

	for(i = 0; i < lev; i++){
		ptr = tab[i];
		while(ptr){
			del = ptr;
			ptr = ptr->next;
			free(del);
		}
		tab[i] = NULL;
	}
}