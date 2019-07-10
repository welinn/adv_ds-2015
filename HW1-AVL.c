#include<stdio.h>
#include<stdlib.h>
#define NODE 100
#define TABLE 10

typedef struct node{
	
	int val;
	int high;
	struct node*left;
	struct node*right;
	
}node;

typedef struct table{
	
	int No;
	int val;
	struct table* next;
	
}table;

node* add(node*, int);
node* del(node*, int);
node* _delete(node*);
table* newTable(int, int);
node* rotRight(node* root);
node* rotLeft(node* root);
node* rightMostNode(node*, int*, int*);
void row(int, int, int);
void column(int, int, int, int);
void draw(node*);
void freeTable(int);
void buildTable(node*, int, int);
int level(node*);
int leaf(int, int);
int findHigh(node*);

table* tab[TABLE];

int main(int argc, char *argv){
	
	int v, i;
	char in;
	node *root = NULL;

	for(i = 0; i < TABLE; i++) tab[i] = NULL;

	while(scanf("%c",&in)!=EOF){
		
		if(in == 'i' || in == 'I'){  //�s�W 
			scanf("%d",&v);
			root = add(root, v);
			draw(root);
		}
		else if(in == 'd' || in == 'D'){  //�R�� 
			scanf("%d" ,&v); 
			root = del(root, v);
			draw(root);
		}
	}
	
	return 0;
}
node* add(node* root, int in){  //�[�`�I 
	
	node *addN;
	
	if(!root){
		addN =(node*) malloc(sizeof(node));
		addN->val = in;
		addN->left = NULL;
		addN->right = NULL;
		addN->high = 1;
		root = addN;
	}
	else{
		if(in < root->val){  //�[����
			
			root->left = add(root->left, in);
			root->high = level(root);
			if(findHigh(root->left) - findHigh(root->right) >1){  //���l��ӭ�
				
				if(in < root->left->val){  //���ɰw��@�� 
					root = rotRight(root);
				}
				else{  //�f�ɰw�@�� ���ɰw�@�� 
					root->left = rotLeft(root->left);
					root = rotRight(root);
				}
			}
		}

		else{  //�[�k��
			root->right = add(root->right, in);
			root->high = level(root);
			if(findHigh(root->right) - findHigh(root->left) >1){  //�k�l��ӭ�
				
				if(in < root->right->val){  //���ɰw�@�� �f�ɰw�@��
					root->right = rotRight(root->right);
					root = rotLeft(root);
				}
				else{  //�f�ɰw�@��
					root = rotLeft(root);
				}
			}

		}
	}
	root->high = level(root);  //���㰪��

	return root;
}
node* del(node* root, int key){
	
	if(!root){
		printf("Tree empty\n");
		return NULL;
	}
	else if(!root->left && !root->right){
		if(root->val == key){
			free(root);
			return NULL;
		}
		else{
			printf("Not found\n");
			return root;
		}
	}
	else if(key == root->val){
		root = _delete(root);

		if(findHigh(root->right) - findHigh(root->left) >1){  //�k�l��ӭ�
			root = rotLeft(root);  //�f�ɰw�@��
		}
		return root;
	}
	else{
		
		if(key < root->val){
			if(!root->left){
				printf("Not found\n");
				return root;
			}
			root->left = del(root->left, key);
			if(findHigh(root->right) - findHigh(root->left) >1){  //�k�l��ӭ�
				if(findHigh(root->right->right) > findHigh(root->right->left))
					root = rotLeft(root);
				else{
					root->right = rotRight(root->right);
					root = rotLeft(root);
				}
			}
		}
		else{
			if(!root->right){
				printf("Not found\n");
				return root;
			}
			root->right = del(root->right, key);
			if(findHigh(root->left) - findHigh(root->right) >1){  //���l��ӭ�
				if(findHigh(root->left->left) > findHigh(root->left->right))
					root = rotRight(root);
				else{
					root->left = rotLeft(root->left);
					root = rotRight(root);
				}
				
			}
		}
		
		level(root);  //���㰪��

		return root;
	}
}
node* _delete(node* root){  //�n�R��root �^�Ƿsroot 
	
	int high, val, * h = &high, *v = &val;
	node*ptr;
	
	if(!root->left || !root->right){  //�ܤ֦��@��l�𬰪�
		ptr = root->left ? root->left : root->right;
		free(root);
		return ptr;
	}

	root->left = rightMostNode(root->left, h, v);
	root->high = high;
	root->val = val;
	return root;
}
node* rightMostNode(node* root, int *h, int *v){
	
	node* ptr;
	if(!root->right){
		(*h) = root->high;
		(*v) = root->val;
		ptr = root->left;
		free(root);
		return ptr;
	}
	else{
		root->right = rightMostNode(root->right, h, v);
		return root;
	}
}
int findHigh(node* root){

	if(!root) return 0;
	else{
		return root->high;
	}

}
void buildTable(node* root, int i, int no){
	
	table* ptr = NULL;

	if(!root){
		return;
	}
	else{
		ptr = tab[i];
		if(!ptr){
			ptr = newTable(no, root->val);
			tab[i] = ptr;
		}
		else{
			while(ptr->next){
				ptr = ptr->next;
			}
			ptr->next = newTable(no, root->val);
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
node* rotRight(node* root){  //���ɰw�� 
	
	node* newRoot = root->left;
	
	root->left = newRoot->right;
	newRoot->right = root;
	return newRoot;
}
node* rotLeft(node* root){  //�f�ɰw�� 
	
	node* newRoot = root->right;
	
	root->right = newRoot->left;
	newRoot->left = root;
	return newRoot;
	
}
void draw(node* root){  //�e��

	int i, j, k, n, lev, x = 2;
	int space, print, check, count;
	table *ptr;
	if(root){
		buildTable(root, 0, 1);

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
		freeTable(lev);
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
		else check = 0;
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
		root->high = l>r ? l : r;
		return root->high;
	}
}
