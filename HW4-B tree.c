#include<stdio.h>
#include<stdlib.h>
#define M 50

//struct node;


typedef struct node{
	struct arr{
		int data;
		struct node* right;
	};
	struct arr n[M];
	int tail;
	struct node* a0;
	struct node* parent;
}node;
typedef struct arr arr;

int m;

node* insert(node*, int);
node* del(node*, int);
void newNode(node*);
node* add(node*, int, node*);
node* checkNode(node*);
node* turn(node*);
node* merge(node*, int);
int level(node*, node**, node**);

int main(int argc, char argv[]){

	node* root = NULL, *l = NULL, *r = NULL;
	node** left = &l, **right = &r;
	char op, a;
	int in, i, high;

	printf("格式 : a.exe < filename.txt\n");
	scanf("%d%c", &m, &a);
	while(scanf("%c", &op) != EOF){
		
		scanf("%d%c", &in, &a);
		if(op == 'i' || op == 'I')
			root = insert(root, in);
		else
			root = del(root, in);
	}
	high = level(root, left, right);
	printf("樹高 : %d\n", high);
	if(high!=0){
		printf("root data :\n%d",root->n[0].data);
		for(i = 1; i < root->tail; i++){
			printf(" ,%d",root->n[i].data);
		}
		printf("\n");

		printf("left data :\n%d",l->n[0].data);
		for(i = 1; i < l->tail; i++){
			printf(" ,%d",l->n[i].data);
		}
		printf("\n");
		printf("right data :\n%d",r->n[0].data);
		for(i = 1; i < r->tail; i++){
			printf(" ,%d",r->n[i].data);
		}
		printf("\n");
	}
	system("pause");
	return 0;
}
node* insert(node* root, int in){

	node* ptr = NULL;
	int i;

	if(!root){
		root = (node*)malloc(sizeof(node));
		newNode(root);
		root->n[root->tail].data = in;
		root->n[root->tail].right = NULL;
		root->tail++;
	}
	else{
		ptr = root;
		while(ptr->a0){  //找leaf
			if(in < ptr->n[0].data){
				ptr = ptr->a0;
			}
			else if(in >= ptr->n[ptr->tail-1].data){
				ptr = ptr->n[ptr->tail-1].right;
			}
			else{
				for(i = 1; i < ptr->tail; i++){
					if(in < ptr->n[i].data){
						ptr = ptr->n[i-1].right;
						break;
					}
				}
			}
		}
		ptr = add(ptr, in, NULL);
		if(ptr) root = ptr;
	}
	return root;
}
node* del(node* root, int d){

	node* ptr = NULL, *change = NULL;
	int find, i, j;

	if(!root){
		printf("tree empty\n");
		return NULL;
	}
	find = -1;
	ptr = root;
	while(ptr->a0){
		for(i = 0; i < ptr->tail; i++){
			if(d < ptr->n[i].data){
				ptr = i == 0 ? ptr->a0 : ptr->n[i-1].right;
				break;
			}
			else if(d == ptr->n[i].data){
				find = i;
				break;
			}
			else if(i+1 == ptr->tail){
				ptr = ptr->n[i].right;
				break;
			}
		}
		if(find != -1) break;
	}
	if(find == -1){  //d可能在leaf
		for(i = 0; i < ptr->tail; i++){
			if(d == ptr->n[i].data) break;
		}
		if(i == ptr->tail){
			printf("not found\n");
			return root;
		}
		else{
			for( ; i < ptr->tail-1; i++){
				ptr->n[i] = ptr->n[i+1];
			}
			ptr->tail--;
			if(ptr->parent && ptr->tail < m/2 + m%2 -1){  //資料不足
				change = turn(ptr);
				if(change) root = change;
			}
			return root;
		}
	}
	else{  //d在ptr->n[i]
		
		change = ptr;
		ptr = ptr->n[i].right;
		while(ptr->a0){
			ptr = ptr->a0;
		}
		change->n[i].data = ptr->n[0].data;
		ptr->tail--;
		for(i = 0; i < ptr->tail; i++){
			ptr->n[i] = ptr->n[i+1];
		}
		if(ptr->parent && ptr->tail < m/2 + m%2 -1){  //資料不足
			change = turn(ptr);
			if(change) root = change;
		}
		return root;
	}
}
void newNode(node* ptr){

	ptr->tail = 0;
	ptr->a0 = NULL;
	ptr->parent = NULL;
}
node* add(node* ptr, int in, node* r){

	int i, j;

	for(i = 0; i < ptr->tail; i++){
		if(in < ptr->n[i].data) break;
	}
	for(j = ptr->tail; j > i; j--){
		ptr->n[j] = ptr->n[j-1];
	}
	ptr->n[i].data = in;
	ptr->n[i].right = r;
	ptr->tail++;
	ptr = checkNode(ptr);
	return ptr;
}
node* checkNode(node* ptr){

	node* newN = NULL, *ret = NULL;
	int index, i, j;

	if(ptr->tail == m){  //爆了
		index = m/2 + (m%2-1);  //找到要往上丟的資料
		newN = (node*)malloc(sizeof(node));
		newNode(newN);
		for(i = index+1, j = 0; i < ptr->tail; i++, j++){  //分裂
			newN->n[j] = ptr->n[i];
		}
		if(ptr->n[index].right){  //要往上丟的data的右子交給分裂出的點
			newN->a0 = ptr->n[index].right;
		}
		newN->tail = j;
		newN->parent = ptr->parent;
		ptr->tail = index;
		//分裂新節點之子樹改parent
		if(newN->a0){
			newN->a0->parent = newN;
			for(i = 0; i < newN->tail; i++){
				newN->n[i].right->parent = newN;
			}
		}
		
    //======================================
		if(!ptr->parent){  //新增parent node
			ptr->parent = (node*)malloc(sizeof(node));
			newNode(ptr->parent);
			newN->parent = ptr->parent;
			ptr->parent->a0 = ptr;
			ptr->parent->n[0].data = ptr->n[index].data;
			ptr->parent->n[0].right = newN;
			ptr->parent->tail++;
			return ptr->parent;
		}
		else{  //把中間資料往上丟
			ret = add(ptr->parent, ptr->n[index].data, newN);
			return ret;
		}
	}
	else return NULL;

}
node* turn(node* ptr){  //旋轉 ptr = data不足點
	
	node *father = ptr->parent->a0, *neighbor = NULL;
	node *ret = NULL;
	int i, j;

	i = 0;
	while(father != ptr){
		father = ptr->parent->n[i++].right;
	}
	father = ptr->parent;
	
	if(i == father->tail){    //在最右
		i--;
		if(father->tail == 1) neighbor = father->a0;
		else neighbor = father->n[i-1].right;

		if(neighbor->tail == m/2 + m%2 -1){//不夠借
			ret = merge(father, i);
			return ret;
		}
		for(j = ptr->tail; j > 0; j--){  //跟左邊借 放在最左邊 移位
			ptr->n[j].data = ptr->n[j-1].data;
		}
		ptr->n[0].data = father->n[i].data;
		ptr->tail++;
		neighbor->tail--;
		father->n[i].data = neighbor->n[neighbor->tail].data;
		return NULL;
	}
	else{
		neighbor = father->n[i].right;
		if(neighbor->tail == m/2 + m%2 -1){//不夠借
			ret = merge(father, i);
			return ret;
		}
		ptr->n[ptr->tail].data = father->n[i].data;
		ptr->tail++;
		father->n[i].data = neighbor->n[0].data;
		neighbor->tail--;
		for(j = 0; j < neighbor->tail; j++){  //被借走頭 補位
			neighbor->n[j] = neighbor->n[j+1];
		}
		return NULL;
	}
}
node* merge(node* ptr, int p){  //合併 ptr = 資料不足點之父節點
	
	node* del = NULL, *left = NULL, *ret = NULL;
	int i;

	del = ptr->n[p].right;
	if(p == 0) left = ptr->a0;
	else left = ptr->n[p-1].right;
	left->n[left->tail].data = ptr->n[p].data; //parent的資料*1往下拉
	left->n[left->tail].right = del->a0;
	left->tail++;
	ptr->tail--;
	for(i = p; i < ptr->tail; i++){  //parent少1，位移
		ptr->n[i] = ptr->n[i+1];
	}
	for(i = 0; i < del->tail; i++){  //合併鄰居
		left->n[left->tail] = del->n[i];
		left->tail++;
	}
	if(del->a0){
		del->a0->parent = left;
		for(i = 0; i < del->tail; i++){
			del->n[i].right->parent = left;
		}
	}
	free(del);

	if(!ptr->parent && ptr->tail == 0){
		free(ptr);
		return left;
	}
	if(ptr->tail < m/2 + m%2 -1){  //data被拿走後資料不足
		ret = turn(ptr);
		return ret;
	}
	return NULL;
}
int level(node* root, node** left, node** right){

	int h = 1;
	node* ptr = root;

	if(!root) return 0;
	while(ptr->a0){
		h++;
		ptr = ptr->a0;
	}
	*left = ptr;
	ptr = root;
	while(ptr->a0){
		ptr = ptr->n[ptr->tail-1].right;
	}
	*right = ptr;
	return h;
}