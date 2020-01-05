// Подключение библиотек
#include <stdio.h>
#include <stdlib.h>

// Структура узла списка
struct node{
	int item;
	node *next;
};

// Сам список
struct node *list;

// Получение размера файла
long int filesize(FILE *file) {
	long int save_pos, size_of_file;

	save_pos = ftell(file);
	fseek(file, 0L, SEEK_END);
	size_of_file = ftell(file);
	fseek(file, save_pos, SEEK_SET);

	return(size_of_file);
}

// Вставка в начало списка
void insertIntoBegin(node **list,int item) {
	node *temp;
	
	temp = (node *)malloc(sizeof(node));
	temp->item = item;
	temp->next = *list;

	*list = temp;
	return;
}

// Вставка в конец списка
void insertIntoEnd(node **list, int item) {
	struct node *temp;
	if ((*list) == NULL) {
		temp = (node *)malloc(sizeof(node));
		temp->item = item;
		temp->next = NULL;

		*list = temp;
		return;
	}else{
		insertIntoEnd(&(*list)->next, item);
	}
}


// Отображение элементов списка
void viewList(node **list) {
	if (*list == NULL) {
		return;
	}else {
		printf("%d ", (*list)->item);
		viewList(&(*list)->next);
		
	}
}

// Соранение списка в файл
void saveList(node **list, FILE *file) {
	if ((*list) != NULL) {
		fwrite(&(*list)->item, sizeof(int), 1, file);
		saveList(&(*list)->next, file);
	}
}

// Загрузка списка из файла
void loadList(node **list, FILE *file) {
	int temp;
	while (!feof(file)) {
		fread(&temp, sizeof(int), 1, file);
		if (!feof(file)) {
			insertIntoEnd(&(*list), temp);
		}
	}
}


// Поиск элемента в списке
int searchItem(node **list, int item) {
	if (*list != NULL) {
		if ((*list)->item == item) {
			return 1;
		}else{
			return searchItem(&(*list)->next,item);
		}
	}

	return 0;
}

// Удаление элемента списка
void deleteItem(node *list, int item, node *prev = NULL) {
	if (list != NULL) {
		if (list->item == item) {
			if (prev != NULL) {
				prev->next = list->next;
				free(list);
			}else {
				if (list->next != NULL) {
					node *temp;

					list->item = list->next->item;
					temp = list->next;
					list->next = list->next->next;
					free(temp);
				}else {
					list = NULL;
				}
			
			}

		}else {
			deleteItem(list->next,item,list);
		}
	}

}

// Удаление списка
void deleteList(node *&list) {
	if (list != NULL) {
		deleteList(list->next);
		free(list);
		list = NULL;
	}
}

// Подсчёт длины списка
int countLength(node **list, int prev = 0) {

	if (*list == NULL) {
		return prev;
	}else {
		prev = countLength(&(*list)->next,prev+1);
	}

	return prev;
}

// Сдвиг влево на N
void shiftLeftN(node **list,int n) {
	struct node *temp,*current;
	temp = (node *)malloc(sizeof(node));
	temp = NULL;

	int i;
	int length = countLength(&(*list));
	i = 0;
	current = (*list);
	while (i<length) {
		if (i >= n) {
			insertIntoEnd(&(temp), current->item);
		}
		current = current->next;
		i++;
	}
	i = 0;
	current = (*list);
	while (i < n) {
		insertIntoEnd(&temp, current->item);
		current = current->next;
		i++;
	}
	
	*list = temp;
}

// Сдвиг вправо на N
void shiftRightN(node **list, int n) {
	struct node *temp, *current;
	temp = (node *)malloc(sizeof(node));
	temp = NULL;

	int i;
	int length = countLength(&(*list));
	i = 0;
	current = (*list);
	while (i < length) {
		if (i >= (length-n)) {
			insertIntoEnd(&(temp), current->item);
		}
		current = current->next;
		i++;
	}
	i = 0;
	current = (*list);
	while (i < (length - n)) {
		insertIntoEnd(&temp, current->item);
		current = current->next;
		i++;
	}

	*list = temp;
}


// Отображение меню
void showCommands() {
	system("cls");

	printf("*** PROGRAM MENU ***");
	printf("\n\nActions list:");
	printf("\n1. Delete List");
	printf("\n2. Add item to list");
	printf("\n3. Show List Items");
	printf("\n4. View length of List");
	printf("\n5. Searh Item in List");
	printf("\n6. Delete item");
	printf("\n7. Save List to list.txt");
	printf("\n8. Load List from list.txt");
	printf("\n9. Shift to left on N elements");
	printf("\n10. Shift to right on N elements");
	printf("\n11. Exit\n\n");
}

// Обработки команд
void commandsProcessor() {
	int cmd;
	int item;
	FILE *file;
	errno_t err;
	
	printf("\nType command and press Enter: ");
	scanf_s("%d", &cmd);

	showCommands();

	switch ((int)cmd) {
	case 1:
		deleteList(list);
		printf("List was deleted\n");
		break;
	case 2:
		int item;
		printf("\nType item and press Enter: ");
		scanf_s("%d", &item);
		insertIntoEnd(&list, item);
		printf("Item was added\n");
		break;
	case 3:
		printf("List items:\n");
		viewList(&list);
		printf("\n");
		break;
	case 4:
		printf("Length of List is %d\n", countLength(&list));
		break;
	case 5:
		printf("\nType desired item and press Enter: ");
		scanf_s("%d", &item);
		if (searchItem(&list, item) == 1) {
			printf("Item was found\n");
		}
		else {
			printf("Item wasn`t found\n");
		}
		break;
	case 6:
		printf("\nType desired item for delete and press Enter: ");
		scanf_s("%d", &item);
		deleteItem(list, item);
		printf("Item was deleted\n");
		break;
	case 7:
		err = fopen_s(&file, "list.txt", "wb");
		if (err == 0) {
			saveList(&list, file);
			fclose(file);
			printf("List was saved\n");
		}
		else {
			printf("List wasn`t saved\n");
		}

		break;
	case 8:
		deleteList(list);
		err = fopen_s(&file, "list.txt", "rb");
		if (err == 0) {
			if (filesize(file) > 0) {
				loadList(&list, file);
				fclose(file);
				printf("List was loaded\n");

				break;
			}
		}
		printf("Bad list list.txt\n");

		break;
	case 9:
		int nl;
		printf("\nType number of shift to left: ");
		scanf_s("%d", &nl);
		shiftLeftN(&list, nl);
		printf("The shift to left was done\n");
		break;
	case 10:
		int nr;
		printf("\nType number of shift to right: ");
		scanf_s("%d", &nr);
		shiftRightN(&list, nr);
		printf("The shift to right was done\n");
		break;
	case 11:
		printf("Exit...\n");
		exit(0);
		break;
	default:
		printf("Fail command\n");

		break;
	}

	commandsProcessor();
}


// Вход в программу
int main() {
	showCommands();
	commandsProcessor();
}