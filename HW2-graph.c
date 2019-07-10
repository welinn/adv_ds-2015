#include<stdio.h>
#include<stdlib.h>
#include<math.h>

#define MAX_NODE 100

typedef struct node{
	
	int No;
	int weight;
	int early;
	int late;
	struct node* next;
}node;

typedef struct table{
	int count;
	node* next;
}table;

typedef struct stack{
	int No;
	struct stack* next;
}stack;

void buildTable(int, int, int, table*);
node* newNode(int, int);
int earlytime(table*, int, int*, int);
int latetime(table*, node*, int);
void draw(table*, node*, int, int*);
int findCycle(table*, node*, int, int, int*);
void cleanTable(table*, int);
void clean(table*, int);

int main(){
	
	table t[MAX_NODE];
	int maxTime[MAX_NODE], drawG[MAX_NODE];
	int checkCycle[MAX_NODE], check;
	int i, n, e,a,b,w,maxW;

	while(scanf("%d", &n) != EOF){  //�`�I�`��

		for(i = 0; i < n; i++){  //�}�C��l��
			t[i].count = 0;
			t[i].next = NULL;
			maxTime[i] = 0;
			checkCycle[i] = 0;
		}

		scanf("%d", &e);  //�䪺�ƶq

		for(i = 0; i < e; i++){
			scanf("%d%d%d" ,&a, &b, &w);
			buildTable(a, b, w, t);
		}
		
		if(t[n-1].next) printf("cycle\n");
		else{
			checkCycle[0] = 1;
			check = findCycle(t, t[0].next, n, 0, checkCycle);
			if(check == 1) printf("cycle\n");
			else{
				maxW = earlytime(t, 0, maxTime, n);  //maxW �̫��`�ɶ�
				latetime(t, t[0].next, maxW);
				cleanTable(t, n);  //�⤣�n�����|�R��
				printf("%d\n", maxW);  //�L�`�ɶ�
				drawG[0] = 0;  //�s�̫�n�L�����|���ǡA[0] = 0(�_�I)
				draw(t, t[0].next, 1, drawG);
				clean(t,n);
			}
		}
	}

	return 0;
}

void buildTable(int a, int b, int w, table* t){
	
	node* n;

	n = newNode(b, w);

	if(t[a].next){
		n->next = t[a].next;
		t[a].next = n;
		t[b].count++;
	}
	else{
		t[a].next = n;
		t[b].count++;
	}
}

node* newNode(int b, int w){
	
	node* n;

	n = (node*)malloc(sizeof(node));
	n->No = b;
	n->weight = w;
	n->early = 0;
	n->late = INT_MAX;
	n->next = NULL;
	return n;
}

int earlytime(table* t, int start, int* maxTime, int n){

	stack* top, *newptr = NULL, *del;
	node* ptr = NULL;
	int max = 0, a, i, topN;
	
	top = (stack*)malloc(sizeof(stack));
	top->next = NULL;
	top->No = start;

	//���̪����|��
	while(top){  //���|�D��
		topN = top->No;
		ptr = t[topN].next;
		del = top;
		top = top->next;
		free(del);
		while(ptr){  //��̪����| ��`�I����|
			a = maxTime[topN] + ptr->weight;
			maxTime[ptr->No] = maxTime[ptr->No] < a ? a : maxTime[ptr->No];
			max = max < maxTime[ptr->No] ? maxTime[ptr->No] : max;
			newptr = (stack*)malloc(sizeof(stack));
			newptr->next = top;
			top = newptr;
			top->No = ptr->No;
			newptr = NULL;

			ptr = ptr->next;
		}
	}

	//  �searliest time
	for(i = 0; i < n; i++){
		ptr = t[i].next;
		while(ptr){
			ptr->early = maxTime[i];
			ptr = ptr->next;
		}
	}

	return max;
}

int latetime(table* t, node* n, int maxW){
	
	int ret = INT_MAX;
	int tmp;

	if(!n){
		return maxW;
	}
	else{
		while(n){
			tmp = latetime(t, t[n->No].next, maxW) - n->weight;
			n->late = n->late > tmp ? tmp : n->late;
			ret = ret > n->late ? n->late : ret;
			n = n->next;
		}
		return ret;

	}
}
void draw(table* t, node* now, int d, int* drawG){
	
	//d ���n�L���}�C������
	int i;

	if(!now){
		for(i = 0; i < d; i++){
			printf("%d ", drawG[i]);
		}
		printf("\n");
	}
	else{
		while(now){
			
			drawG[d] = now->No;
			draw(t, t[now->No].next, d+1, drawG);
			now = now->next;
		}
	}
}
int findCycle(table* t, node* now, int n, int check, int* c){
	
	int i,ccopy[MAX_NODE];

	if(!now){
		return 0;
	}
	else{
		for(i = 0; i < n; i++){
			ccopy[i] = c[i];
		}
		while(now){
			
			if(ccopy[now->No] == 1) return 1;
			ccopy[now->No] = 1;
			check = findCycle(t,t[now->No].next, n, check, ccopy);
			for(i = 1; i < n; i++){
				ccopy[i] = c[i];
			}
			now = now->next;
		}
		return check;
	}
	
}
void cleanTable(table* t, int n){
	
	int i;
	node* ptr, *del;

	for(i = 0; i < n; i++){
		ptr = t[i].next;
		if(ptr){
			while(ptr->next){
			
				if(ptr->next->early != ptr->next->late){
				
					del = ptr->next;
					ptr->next = ptr->next->next;
					free(del);
				}
				else ptr = ptr->next;
			}
			if(t[i].next->early != t[i].next->late){
				del = t[i].next;
				t[i].next = t[i].next->next;
				free(del);
			}
		}
	}

}
void clean(table* t, int n){
	
	int i;
	node* ptr, *del;

	for(i = 0; i < n; i++){
		ptr = t[i].next;
		while(ptr){
			del = ptr;
			ptr = ptr->next;
			free(del);
		}
		t[i].next = NULL;
	}
}