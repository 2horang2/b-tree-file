#include<stdio.h>
#include<stdlib.h>
#include<string.h>

/*
2014.06.13
HYEMIN JUNG
*/
#define KEY_NUM 9
#define LEAF -10
#define INDEX -100
#define MAX 30

#define CREATE 1
#define PRINT 2
#define INSERT 3
#define SEARCH 4
#define EXIT 5 

typedef struct data{
	int number;
	char name[MAX];
	char ph[MAX];
	char add[MAX];
}data;

typedef struct node{
	int type;

	int index_add;
	int count;

	int parent;
	int next;

	int data_add[KEY_NUM];
	int value[KEY_NUM];
	int branch[KEY_NUM + 1];
}node;



void print_menu();
void create();
void search_data();
void insert_new_data();
void insertkey(int key);
void print();
void print_title();
void array_data_txt();


int find_node(int findkey);
void divide_parent2(int pre_node_add, int next_node_add, int parent_node_add, int origin_key);
void divide_parent1(int pre_node_add, int next_node_add, int parent_node_add, int origin_key);
void insertkey2(int addkey, int tmp_add);
void insertkey1(int addkey, int tmp_add, int data_add);




int data_tmp_add = 0;
int index_add = 0;
int root_add = -1;
int data_count = 0;
int data_number = 0;



void main()
{
	int menu = 0;
	FILE *fo;
	fopen_s(&fo, "index.bin", "wb");
	fwrite(&menu, sizeof(int), 1, fo);
	fclose(fo);


	while (1)
	{

		print_menu();
		printf("SELECT : ");
		scanf_s("%d", &menu);

		switch (menu)	{

		case CREATE:
			create();
			array_data_txt();
			break;
		case PRINT:
			print();
			break;
		case INSERT:
			insert_new_data();
			array_data_txt();
			break;
		case SEARCH:
			search_data();
			break;
		case EXIT:
			exit(1);

		}
	}
}


void insert_new_data()
{

	int i, num, data_add;
	char tmp[MAX];
	node tmp_node;
	data insert_data;
	FILE *fp;



	printf("학번 : ");
	scanf_s("%d", &insert_data.number);
	printf("이름  : ");
	scanf_s("%s", insert_data.name, MAX);
	printf("연락처 : ");
	scanf_s("%s", insert_data.ph, MAX);
	printf("주소 : ");
	scanf_s("%s", insert_data.add, MAX);




	fopen_s(&fp, "data_tmp.txt", "at");
	insertkey(insert_data.number);

	fputc('\n', fp);
	fprintf(fp, "%d", insert_data.number);
	fputc(' ', fp);
	fputs(insert_data.name, fp);
	fputc(' ', fp);
	fputs(insert_data.ph, fp);
	fputc(' ', fp);
	fputs(insert_data.add, fp);



	data_tmp_add = ftell(fp);

	fclose(fp);

}

void create()
{

	int i, num, data_add;
	char tmp[MAX];
	node tmp_node;
	FILE *fp1;

	fopen_s(&fp1, "data_tmp.txt", "r+");

	while (1){

		fscanf_s(fp1, "%d", &num);


		insertkey(num);

		fscanf_s(fp1, "%s", tmp, sizeof(char)*MAX);
		fscanf_s(fp1, "%s", tmp, sizeof(char)*MAX);
		fscanf_s(fp1, "%s", tmp, sizeof(char)*MAX);
		if (feof(fp1)) break;
		data_tmp_add = ftell(fp1);
	}

	fclose(fp1);
}



void search_data()
{

	int i, tmp, search_num;
	node tmp_node;
	data search_node;

	FILE *fp, *fo;

	fopen_s(&fp, "data_tmp.txt", "r+");
	fopen_s(&fo, "index.bin", "r+b");

	fseek(fo, root_add, SEEK_SET);
	fread(&tmp_node, sizeof(node), 1, fo);


	printf("\n학번을 입력하세요 : ");
	scanf_s("%d", &search_num);


	while (1)	{
		if (tmp_node.branch[0] == -1)
			break;

		fseek(fo, tmp_node.branch[0], SEEK_SET);
		fread(&tmp_node, sizeof(node), 1, fo);
	}


	while (1)	{
		i = 0;
		while (1){
			if (tmp_node.value[i] == search_num){

				print_title();

				fscanf_s(fp, "%d", &search_node.number);
				fscanf_s(fp, "%s", search_node.name, MAX);
				fscanf_s(fp, "%s", search_node.ph, MAX);
				fscanf_s(fp, "%s", search_node.add, MAX);

				printf("%10d%20s%20s%20s\n\n\n", search_node.number, search_node.name, search_node.ph, search_node.add);

				fclose(fp);
				fclose(fo);
				return;
			}
			i++;
			if (tmp_node.value[i] <= 0)
				break;
		}

		if (tmp_node.next == -1){
			printf("찾으시는 대상이 존재하지않습니다. \n\n");
			fclose(fp);
			fclose(fo);
			return;
		}
		fseek(fo, tmp_node.next, SEEK_SET);
		fread(&tmp_node, sizeof(node), 1, fo);
	}
}





void array_data_txt()
{
	int i, data_add, num;
	char tmp[MAX];
	node tmp_node;
	data tmp_data;

	FILE *fp1, *fp2, *fo;

	fopen_s(&fp1, "data_tmp.txt", "r+");


	fopen_s(&fp2, "data.txt", "w+");
	fopen_s(&fo, "index.bin", "r+b");

	fseek(fo, root_add, SEEK_SET);
	fread(&tmp_node, sizeof(node), 1, fo);


	while (1)	{
		if (tmp_node.branch[0] == -1)
			break;

		fseek(fo, tmp_node.branch[0], SEEK_SET);
		fread(&tmp_node, sizeof(node), 1, fo);
	}

	while (1)	{
		i = 0;
		while (1){

			fseek(fp1, tmp_node.data_add[i], SEEK_SET);
			data_add = ftell(fp2);

			fscanf_s(fp1, "%d", &num);
			fprintf_s(fp2, "%10d", num);
			fscanf_s(fp1, "%s", tmp, sizeof(char)*MAX);
			fprintf_s(fp2, "%30s", tmp);
			fscanf_s(fp1, "%s", tmp, sizeof(char)*MAX);
			fprintf_s(fp2, "%30s", tmp);
			fscanf_s(fp1, "%s", tmp, sizeof(char)*MAX);
			fprintf_s(fp2, "%30s\n", tmp);

			i++;

			if (tmp_node.value[i] <= 0)
				break;
		}

		if (tmp_node.next == -1){
			fclose(fo);
			fclose(fp1);
			fclose(fp2);
			return;
		}

		fseek(fo, tmp_node.next, SEEK_SET);
		fread(&tmp_node, sizeof(node), 1, fo);
	}
}


void print()
{
	int i;
	node tmp_node;
	data tmp_data;

	FILE *fo, *fp;


	fopen_s(&fo, "index.bin", "r+b");
	fopen_s(&fp, "data_tmp.txt", "r+");

	fseek(fo, root_add, SEEK_SET);
	fread(&tmp_node, sizeof(node), 1, fo);

	while (1)	{
		if (tmp_node.branch[0] == -1)
			break;

		fseek(fo, tmp_node.branch[0], SEEK_SET);
		fread(&tmp_node, sizeof(node), 1, fo);
	}

	print_title();

	while (1)	{
		i = 0;
		while (1){

			fseek(fp, tmp_node.data_add[i], SEEK_SET);

			fscanf_s(fp, "%d", &tmp_data.number);
			fscanf_s(fp, "%s", tmp_data.name, MAX);
			fscanf_s(fp, "%s", tmp_data.ph, MAX);
			fscanf_s(fp, "%s", tmp_data.add, MAX);

			printf("%10d%20s%20s%20s\n", tmp_data.number, tmp_data.name, tmp_data.ph, tmp_data.add);

			i++;

			if (tmp_node.value[i] <= 0)
				break;
		}

		if (tmp_node.next == -1){
			fclose(fp);
			fclose(fo);
			return;
		}
		fseek(fo, tmp_node.next, SEEK_SET);
		fread(&tmp_node, sizeof(node), 1, fo);
	}
}



int find_node(int findkey)
{
	int i;
	int tmp_add;
	node tmp_node, init_node;

	//tmp_node = root;
	FILE *fo;

	fopen_s(&fo, "index.bin", "r+b");


	if (root_add == -1)	{

		// 초기화
		init_node.count = 0;
		init_node.parent = -1;
		init_node.next = -1;

		for (i = 0; i < KEY_NUM; i++){
			init_node.branch[i] = -1;
			init_node.value[i] = 0;
		}
		init_node.branch[i] = -1;


		// 루트시작

		init_node.value[0] = findkey;
		init_node.type = LEAF;
		init_node.parent = -1;

		init_node.count++;
		init_node.data_add[0] = data_tmp_add;
		init_node.index_add = 0;

		root_add = 0;
		//	fwrite(&head, sizeof(headnode), 1, indexfile);
		fwrite(&init_node, sizeof(node), 1, fo);
		data_count++;

		fseek(fo, root_add, SEEK_SET);
		//fread(&head, sizeof(head), 1, indexfile);
		tmp_add = root_add;
		fread(&tmp_node, sizeof(node), 1, fo);

		fclose(fo);

		return -1;
	}

	fseek(fo, root_add, SEEK_SET);
	//fread(&head, sizeof(head), 1, indexfile);
	tmp_add = root_add;
	fread(&tmp_node, sizeof(node), 1, fo);

	while (1)
	{
		if (tmp_node.type != LEAF)	{
			for (i = 0; i < KEY_NUM; i++){
				if (tmp_node.value[i] >= findkey || tmp_node.value[i] == 0)
					break;
			}


			fseek(fo, tmp_node.branch[i], SEEK_SET);
			tmp_add = ftell(fo);
			fread(&tmp_node, sizeof(node), 1, fo);
			//tmp_node = tmp_node->branch[i];
		}
		else
		{

			for (i = 0; i < KEY_NUM; i++)
			if (tmp_node.value[i] == findkey)
			{
				printf("\n── %d 는 이미 존재하는 학번입니다.──\n\n", findkey);
				fclose(fo);
				return -1;
			}
			fclose(fo);
			return tmp_add;
		}
	}

}



void divide_parent2(int pre_node_add, int next_node_add, int parent_node_add, int origin_key)
{
	int i, j, tmep_add;
	node add_node, add_index_node;
	int tmp_branch[KEY_NUM + 2];
	int tmp_value[KEY_NUM + 1];
	int tmp_data_add[KEY_NUM + 1];
	FILE *fo;
	int key; //= next_node->value[0];
	node parent_node; // = pre_node->parent;
	node next_node, pre_node;
	node temp;
	int add_node_index_add;

	fopen_s(&fo, "index.bin", "r+b");

	fseek(fo, parent_node_add, SEEK_SET);
	fread(&parent_node, sizeof(node), 1, fo);

	fseek(fo, pre_node_add, SEEK_SET);
	fread(&pre_node, sizeof(node), 1, fo);

	fseek(fo, next_node_add, SEEK_SET);
	fread(&next_node, sizeof(node), 1, fo);

	next_node.parent = pre_node.parent;

	fseek(fo, next_node_add, SEEK_SET);
	fread(&next_node, sizeof(node), 1, fo);

	if (origin_key == -1)
		key = next_node.value[0];
	else
		key = origin_key;

	//int key = next_node->value[0];
	//node* parent_node = pre_node->parent;


	for (i = 0; i < KEY_NUM + 1; i++)
		tmp_branch[i] = parent_node.branch[i];

	for (i = 0; i < KEY_NUM; i++)
		tmp_value[i] = parent_node.value[i];


	for (i = 0; i < KEY_NUM; i++){
		if (tmp_value[i] == key)
			return;
		if (tmp_value[i] >= key || tmp_value[i] == 0)
			break;
	}


	if (tmp_value[i] > 0)
	{
		for (j = KEY_NUM + 1; j > i; j--)
			tmp_branch[j] = tmp_branch[j - 1];

		for (j = KEY_NUM; j > i; j--)
			tmp_value[j] = tmp_value[j - 1];
	}

	tmp_value[i] = key;
	tmp_branch[i] = pre_node_add;
	tmp_branch[i + 1] = next_node_add;


	//add_node = (node*)malloc(sizeof(node));
	//init(add_node);

	data_count++;

	add_node.count = 0;
	add_node.next = -1;

	for (i = 0; i <KEY_NUM; i++){
		add_node.value[i] = 0;
		add_node.branch[i] = -1;
		add_node.data_add[i] = -1;
	}
	add_node.branch[i] = -1;

	add_node_index_add = sizeof(node)*(data_count - 1);
	add_node.index_add = add_node_index_add;

	add_node.type = INDEX;
	add_node.parent = pre_node.parent;

	//tmep_add = parent_node.parent;

	fseek(fo, add_node_index_add, SEEK_SET);
	fwrite(&add_node, sizeof(node), 1, fo);




	//init(parent_node);

	parent_node.count = 0;
	parent_node.next = -1;

	for (i = 0; i <KEY_NUM; i++){
		parent_node.value[i] = 0;
		parent_node.branch[i] = -1;
		parent_node.data_add[i] = -1;
	}
	parent_node.branch[i] = -1;

	parent_node.type = INDEX;
	parent_node.parent = add_node.parent;



	for (i = 0; i < KEY_NUM / 2 + 1; i++)
	{
		parent_node.branch[i] = tmp_branch[i];
		parent_node.value[i] = tmp_value[i];
		parent_node.count++;

		//parent_node->branch[i]->parent = parent_node;

		fseek(fo, parent_node.branch[i], SEEK_SET);
		fread(&temp, sizeof(node), 1, fo);

		temp.parent = parent_node_add;
		fseek(fo, temp.index_add, SEEK_SET);
		fwrite(&temp, sizeof(node), 1, fo);


	}

	parent_node.branch[i] = tmp_branch[i];

	//parent_node.branch[i]->parent = parent_node;
	fseek(fo, parent_node.branch[i], SEEK_SET);
	fread(&temp, sizeof(node), 1, fo);

	temp.parent = parent_node_add;
	fseek(fo, temp.index_add, SEEK_SET);
	fwrite(&temp, sizeof(node), 1, fo);


	i++;
	for (j = 0; i < KEY_NUM + 1; j++, i++)
	{
		add_node.branch[j] = tmp_branch[i];
		add_node.value[j] = tmp_value[i];
		add_node.count++;

		//add_node->branch[j]->parent = add_node;

		fseek(fo, add_node.branch[i], SEEK_SET);
		fread(&temp, sizeof(node), 1, fo);

		temp.parent = add_node_index_add;
		fseek(fo, temp.index_add, SEEK_SET);
		fwrite(&temp, sizeof(node), 1, fo);

	}
	add_node.branch[j] = tmp_branch[i];

	//add_node->branch[j]->parent = add_node;
	fseek(fo, add_node.branch[i], SEEK_SET);
	fread(&temp, sizeof(node), 1, fo);

	temp.parent = add_node_index_add;
	fseek(fo, temp.index_add, SEEK_SET);
	fwrite(&temp, sizeof(node), 1, fo);


	fseek(fo, parent_node.index_add, SEEK_SET);
	fwrite(&parent_node, sizeof(node), 1, fo);
	fseek(fo, add_node.index_add, SEEK_SET);
	fwrite(&add_node, sizeof(node), 1, fo);


	if (parent_node.parent < 0)
	{
		//add_index_node = (node*)malloc(sizeof(node));
		//init(add_index_node);
		data_count++;

		add_index_node.count = 0;
		add_index_node.next = -1;

		for (i = 0; i <KEY_NUM; i++){
			add_index_node.value[i] = 0;
			add_index_node.branch[i] = -1;
			add_index_node.data_add[i] = -1;
		}
		add_index_node.branch[i] = -1;


		add_index_node.type = INDEX;
		add_index_node.branch[0] = parent_node_add;
		add_index_node.branch[1] = add_node_index_add;
		add_index_node.value[0] = tmp_value[KEY_NUM / 2 + 1];
		add_index_node.count++;


		//parent_node->branch[KEY_NUM - 1] = add_node->branch[0];
		//for (i = 0; i < KEY_NUM; i++){
		//	add_node->value[0] = add_node->value[i + 1];
		//	add_node->branch[0] = add_node->branch[i + 1];
		//}



		add_index_node.index_add = sizeof(node)*(data_count - 1);
		fseek(fo, add_index_node.index_add, SEEK_SET);
		fwrite(&add_index_node, sizeof(node), 1, fo);

		parent_node.parent = add_index_node.index_add;
		add_node.parent = add_index_node.index_add;

		fseek(fo, parent_node.index_add, SEEK_SET);
		fwrite(&parent_node, sizeof(node), 1, fo);
		fseek(fo, add_node.index_add, SEEK_SET);
		fwrite(&add_node, sizeof(node), 1, fo);


		root_add = add_index_node.index_add;
		fclose(fo);
	}
	else
	{
		fclose(fo);
		divide_parent1(parent_node.index_add, add_node.index_add, parent_node.parent, tmp_value[KEY_NUM / 2 + 1]); //key값주기
	}

}


void divide_parent1(int pre_node_add, int next_node_add, int parent_node_add, int origin_key)
{
	int i, j;
	FILE *fo;
	int key; //= next_node->value[0];
	node parent_node; // = pre_node->parent;
	node next_node;


	fopen_s(&fo, "index.bin", "r+b");

	fseek(fo, parent_node_add, SEEK_SET);
	fread(&parent_node, sizeof(node), 1, fo);

	fseek(fo, next_node_add, SEEK_SET);
	fread(&next_node, sizeof(node), 1, fo);

	if (origin_key == -1)
		key = -1;
	else
		key = origin_key;

	if (parent_node.count == KEY_NUM){

		fclose(fo);
		divide_parent2(pre_node_add, next_node_add, parent_node_add, key);
		return;

	}

	if (origin_key == -1)
		key = next_node.value[0];

	for (i = 0; i < KEY_NUM; i++)
	if (parent_node.value[i] >= key || parent_node.value[i] == 0)
		break;

	if (parent_node.value[i] != 0)
	{
		for (j = KEY_NUM; j > i; j--){
			parent_node.branch[j] = parent_node.branch[j - 1];
		}

		for (j = KEY_NUM - 1; j > i; j--){
			parent_node.value[j] = parent_node.value[j - 1];
		}
	}

	parent_node.value[i] = key;
	parent_node.branch[i] = pre_node_add;
	parent_node.branch[i + 1] = next_node_add;
	parent_node.count++;

	fseek(fo, parent_node_add, SEEK_SET);
	fwrite(&parent_node, sizeof(node), 1, fo);

	fclose(fo);
}


void insertkey2(int addkey, int tmp_add)
{
	int i, add_node_index_add;
	int tmp;
	node p_node;
	node tmp_node;
	node add_node;
	node parent_node;
	FILE *fo;

	fopen_s(&fo, "index.bin", "r+b");
	fseek(fo, tmp_add, SEEK_SET);
	fread(&tmp_node, sizeof(node), 1, fo);


	data_count++;

	add_node.count = 0;
	add_node.next = -1;

	for (i = 0; i <KEY_NUM; i++){
		add_node.value[i] = 0;
		add_node.branch[i] = -1;
		add_node.data_add[i] = -1;
	}
	add_node.branch[i] = -1;

	add_node.type = LEAF;
	add_node.index_add = sizeof(node)*(data_count - 1);

	fseek(fo, sizeof(node)*(data_count - 1), SEEK_SET);
	add_node_index_add = ftell(fo);
	fwrite(&add_node, sizeof(node), 1, fo);


	fclose(fo);

	insertkey1(tmp_node.value[KEY_NUM - 1], add_node_index_add /*add_node*/, tmp_node.data_add[KEY_NUM - 1]);

	fopen_s(&fo, "index.bin", "r+b");

	fseek(fo, add_node.index_add, SEEK_SET);
	fread(&add_node, sizeof(node), 1, fo);

	tmp_node.value[KEY_NUM - 1] = 0;
	tmp_node.data_add[KEY_NUM - 1] = -1;
	tmp_node.count--;

	fseek(fo, tmp_add, SEEK_SET);
	fwrite(&tmp_node, sizeof(node), 1, fo);


	fclose(fo);
	insertkey1(addkey, tmp_add, -1);
	fopen_s(&fo, "index.bin", "r+b");

	fseek(fo, tmp_node.index_add, SEEK_SET);
	fread(&tmp_node, sizeof(node), 1, fo);


	tmp_node.count++;
	fseek(fo, tmp_add, SEEK_SET);
	fwrite(&tmp_node, sizeof(node), 1, fo);

	for (i = KEY_NUM / 2 + 1; i < KEY_NUM; i++){

		fclose(fo);
		insertkey1(tmp_node.value[i], add_node_index_add, tmp_node.data_add[i]);
		fopen_s(&fo, "index.bin", "r+b");

		fseek(fo, add_node.index_add, SEEK_SET);
		fread(&add_node, sizeof(node), 1, fo);

		tmp_node.value[i] = 0;
		tmp_node.count--;

		fseek(fo, tmp_add, SEEK_SET);
		fwrite(&tmp_node, sizeof(node), 1, fo);

		fseek(fo, tmp_node.index_add, SEEK_SET);
		fread(&tmp_node, sizeof(node), 1, fo);
	}

	tmp_node.value[KEY_NUM - 1] = 0;
	tmp_node.data_add[KEY_NUM - 1] = -1;
	tmp_node.count--;

	add_node.next = tmp_node.next;
	tmp_node.next = add_node_index_add;

	add_node.parent = tmp_node.parent;

	fseek(fo, tmp_add, SEEK_SET);
	fwrite(&tmp_node, sizeof(node), 1, fo);

	fseek(fo, add_node_index_add, SEEK_SET);
	fwrite(&add_node, sizeof(node), 1, fo);

	if (tmp_node.parent == -1)
	{
		//parent_node = (node*)malloc(sizeof(node));
		//init(parent_node);
		data_count++;
		parent_node.index_add = sizeof(node)*(data_count - 1);

		parent_node.count = 0;
		parent_node.next = -1;

		for (i = 0; i <KEY_NUM; i++){
			parent_node.value[i] = 0;
			parent_node.branch[i] = -1;
			parent_node.data_add[i] = -1;
		}
		parent_node.branch[i] = -1;
		parent_node.parent = -1;

		parent_node.type = INDEX;


		parent_node.branch[0] = tmp_add;
		parent_node.branch[1] = add_node_index_add;

		fseek(fo, parent_node.index_add, SEEK_SET);
		fread(&p_node, sizeof(node), 1, fo);
		fseek(fo, p_node.branch[1], SEEK_SET);
		fread(&p_node, sizeof(node), 1, fo);
		parent_node.value[0] = p_node.value[0];

		//parent_node.value[0] = parent_node->branch[1]->value[0];

		parent_node.count++;

		fseek(fo, parent_node.index_add, SEEK_SET);
		fwrite(&parent_node, sizeof(node), 1, fo);

		tmp_node.parent = parent_node.index_add;
		add_node.parent = parent_node.index_add;

		fseek(fo, tmp_add, SEEK_SET);
		fwrite(&tmp_node, sizeof(node), 1, fo);
		fseek(fo, add_node_index_add, SEEK_SET);
		fwrite(&add_node, sizeof(node), 1, fo);

		root_add = parent_node.index_add;
		fclose(fo);
	}
	else
	{
		fclose(fo);
		divide_parent1(tmp_add, add_node_index_add, tmp_node.parent, -1);
	}
}


void insertkey1(int addkey, int tmp_add, int data_add)
{

	int i, j;
	node tmp_node;

	FILE *fo;


	if (tmp_add == -1)
		return;


	fopen_s(&fo, "index.bin", "r+b");
	fseek(fo, tmp_add, SEEK_SET);
	fread(&tmp_node, sizeof(node), 1, fo);


	if (tmp_node.count == KEY_NUM)	{
		fclose(fo);
		insertkey2(addkey, tmp_add);
		return;
	}

	for (i = 0; i < KEY_NUM; i++)
	if (tmp_node.value[i] >= addkey || tmp_node.value[i] == 0)
		break;

	if (tmp_node.value[i] != 0)
	{
		for (j = KEY_NUM - 1; j > i; j--){
			tmp_node.value[j] = tmp_node.value[j - 1];
			tmp_node.data_add[j] = tmp_node.data_add[j - 1];
		}
	}


	tmp_node.value[i] = addkey;
	tmp_node.count++;

	if (data_add == -1)
		tmp_node.data_add[i] = data_tmp_add;
	else
		tmp_node.data_add[i] = data_add;


	fseek(fo, tmp_add, SEEK_SET);
	fwrite(&tmp_node, sizeof(node), 1, fo);


	fclose(fo);

}



void insertkey(int key)
{
	if (key <= 0)
		return;
	insertkey1(key, find_node(key), -1);

}




void print_title()
{
	printf_s("\n───────────────────────────────────────\n");
	printf_s("\t학번\t\t   이름\t\t   연락처\t\t  주소\n");
	printf_s("───────────────────────────────────────\n");
}

void print_menu()
{

	printf("┌─────────────────┐\n");
	printf("│           1.  CREATE             │\n");
	printf("│           2.  PRINT              │\n");
	printf("│           3.  INSERT             │\n");
	printf("│           4.  SEARCH             │\n");
	printf("│           5.  EXIT               │\n");
	printf("└─────────────────┘\n");
}
