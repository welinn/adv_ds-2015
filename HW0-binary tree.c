#include<stdio.h>
#include<stdlib.h>
#define NODE 100
#define TABLE 10

typedef struct node{
	
	int val;
	struct node*left;
	struct node*right;
	
}node;

typedef struct table{
	
	int No;
	int val;
	struct table* next;
	
}table;

node* add(node*);
table* newTable(int, int, table*);
void row(int, int, int);
void column(int, int, int, int);
void draw(node*);
int level(node*);
int leaf(int, int);


table* tab[TABLE];

int main(int argc, char *argv){
	
	int in, i;
	node *root = NULL;
	
	for(i = 0; i < TABLE; i++) tab[i] = NULL;
	
	while(1){
		
		printf("1.Add\n2.Desplay\n3.Exit\n");
		scanf("%d",&in);
		
		if(in == 1){
			root = add(root);
		}
		else if(in == 2){
			if(root == NULL){
				printf("empty\n");
			}
			else draw(root);
		}
		else if(in == 3){
			break;
		}
		
	}
	
	return 0;
}

node* add(node* root){  //�[�`�I 
	
	int in, nth, numb, i;
	node *now, *add;
	table *ptr;
	
	scanf("%d",&in);
	add = malloc(sizeof(node));
	add->val = in;
	add->left = NULL;
	add->right = NULL;
	
	if(root){
		now = root;
		nth = 1;  //���Ʀb�P�@�h���ƲĴX�� (�q1�}�l) 
		numb = 0;  //table�h�� (0 = root) 
		do{
			if(in < now->val){  //���� 
				
				nth = nth*2-1;
				if(now->left == NULL){
					now->left = add;
					now = NULL;
				}
				else
					now = now->left;
				
			}
			else{  //��k��
				
				nth*=2;
				if(now->right == NULL){
					now->right = add;
					now = NULL;
				}
				else
					now = now->right;
				
			}
			numb++;
		}while(now != NULL);
		
		
		ptr = tab[numb];
		if(ptr == NULL){  //table�O�Ū� 
			tab[numb] = newTable(nth, add->val, NULL);
		}
		else{
			
			if(nth < ptr->No){  //�Ǹ��̤p������̫e�� 
				tab[numb] = newTable(nth, add->val, ptr);
			}
			
			else if(ptr->next == NULL){  //table�W�ثe�u��1��node 
				ptr->next = newTable(nth, add->val, NULL);
			}
			else{  //table�W��2�ӥH�W 
			
				while(ptr->next != NULL && nth > ptr->next->No){
					ptr = ptr->next;
				}
				ptr->next = newTable(nth, add->val, ptr->next);
			}
		}
	}
	else{  //��O�Ū� 
		root = add;
		tab[0] = newTable(1, add->val, NULL);
	}
	
	return root;
}

table* newTable(int no, int v, table* next){
	
	table *addT;
	
	addT = malloc(sizeof(table));
	addT->No = no;
	addT->val = v;
	addT->next = next;
	
	return addT;
}

void draw(node* root){  //�e��

	int i, j, k, n, lev, x = 2;
	int space, print, check, count;
	table *ptr;
	
	lev = level(root);  //�𪺼h�� 
	for(i=0, n=0; i < lev; i++){  //�⩳�h�`�� = n
		n = n*2+1;
	}
	print = 1;  //�Ӽh���ɦ��X��node
	
	
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
		
		if(i+1 != lev){  //�U���٦� �n�e��u & ���b�b 
			column(i, print, space, 0);
			row(i+1, print*2, n/(x*2));
			column(i+1, print*2, n/(x*2), 1);
		}
		print*=2;
		x*=2;
	}
}

void row(int t, int count, int space){  //�e��u 
	
	table *ptr;
	int i, j, k, check = 0;	
	
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
		if(k%2 == 1 && ptr->No == k){  //���`�I(��)
			printf(" --");
			ptr = ptr->next;
			check = 1;
		}
		else if(ptr->No == k){  //���`�I(�k)
			printf("-- ");
			ptr = ptr->next;
			check = 0;
		}
		else printf("   ");
		
		if(ptr == NULL) break;
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
		}	
		printf("   ");
		
		if(k%2 == 0 && ptr->No == k) check = 1;
	}
	printf("\n");
}

void column(int i, int count, int space, int w){  //�e���b�b 
	
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
				
				//w�аO 0��e�`�I�U��b�b 
				if(w == 0 && leaf(i+1, k*2) == 0){  //�Oleaf 
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

int level(node* root){  //��� 
	
	int l,r;
	
	if(root == NULL) return 0;
	else{
		l = 1+level(root->left);
		r = 1+level(root->right);
		return l>r ? l : r;
	}
}
