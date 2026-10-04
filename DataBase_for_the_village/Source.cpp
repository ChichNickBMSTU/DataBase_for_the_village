#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <locale.h>
#include <string.h>
#include <stdlib.h>
typedef struct
{
	int floors;
	float area;
	int year;
	char material[50];
	float price;
	char owner[50];
} House;
void printfile(FILE* f)
{
	House house;
	int k = 0;
	rewind(f);
	setlocale(LC_ALL, "Russian");
	puts("--------------------------------ДОМА-------------------------------");
	puts("--------------------------------------------------------------------");
	puts("| N | Владелец     |Этаж|  Материал  | Год  | Площадь |   Цена    |");
	puts("-------------------------------------------------------------------");
	while (fread(&house, sizeof(house), 1, f) == 1)
	{
		k++;
		printf("|%2d | %-12.12s | %2d | %-10.10s | %4d | %6.1f | %7.2f |\n",
			k,
			house.owner,
			house.floors,
			house.material,
			house.year,
			house.area,
			house.price);
	}
	puts("--------------------------------------------------------------------");
}
void safe_input(char* buf, int size)
{
	fgets(buf, size, stdin);
	buf[strcspn(buf, "\n")] = '\0';
}
FILE* formopen(char* fname)
{
	House house;
	FILE* f = NULL;
	char input[100];
	int d;
	int input_active = 1;
	setlocale(0, "russian");
	printf("Введите имя файла: ");
	safe_input(fname, 100);
	f = fopen(fname, "ab+");
	if (f == NULL)
	{
		f = fopen(fname, "wb+");
		printf("Создан новый файл. Введите данные (пустая строка - завершение):\n");
	}
	else
	{
		printf("Файл открыт. Добавить данные? (1 - Да, 2 - Нет): ");
		scanf("%d", &d);
		while (getchar() != '\n');
		if (d != 1)
		{
			return f;
		}
	}
	while (input_active)
	{
		printf("ФИО владельца: ");
		safe_input(input, sizeof(input));
		if (strlen(input) == 0)
		{
			input_active = 0;
		}
		else
		{
			strcpy(house.owner, input);
			printf("Этажность: ");
			scanf("%d", &house.floors);
			while (getchar() != '\n');
			printf("Материал: ");
			safe_input(house.material, sizeof(house.material));
			printf("Год постройки: ");
			scanf("%d", &house.year);
			while (getchar() != '\n');
			printf("Площадь: ");
			scanf("%f", &house.area);
			while (getchar() != '\n');
			printf("Стоимость: ");
			scanf("%f", &house.price);
			while (getchar() != '\n');
			fseek(f, 0, SEEK_END);
			fwrite(&house, sizeof(house), 1, f);
			fflush(f);
		}
	}
	return f;
}
void searchByFloors(FILE* f)
{
	int target;
	House house;
	rewind(f);
	printf("Введите этажность для поиска: ");
	scanf("%d", &target);
	while (getchar() != '\n');
	setlocale(0, "russian");
	printf("\nВладельцы домов с %d этажами:\n", target);
	while (fread(&house, sizeof(house), 1, f) == 1)
	{
		if (house.floors == target)
		{
			printf("- %s\n", house.owner);
		}
	}
}
void searchByMaterial(FILE* f)
{
	char material[50];
	House house;
	rewind(f);
	printf("Введите материал для поиска: ");
	safe_input(material, sizeof(material));
	setlocale(0, "russian");
	printf("\nДома из материала '%s':\n", material);
	while (fread(&house, sizeof(house), 1, f) == 1)
	{
		if (strcmp(house.material, material) == 0)
		{
			printf("- %s (Этажи: %d, Год: %d)\n", house.owner, house.floors, house.year);
		}
	}
}
void searchWoodenAfterYear(FILE* f)
{
	int year;
	House house;
	rewind(f);
	printf("Введите год для поиска деревянных домов: ");
	scanf("%d", &year);
	while (getchar() != '\n');
	setlocale(0, "russian");
	printf("\nДеревянные дома после %d года:\n", year);
	while (fread(&house, sizeof(house), 1, f) == 1)
	{
		if (strcmp(house.material, "wooden") == 0 && house.year > year)
		{
			printf("- %s (Год: %d)\n", house.owner, house.year);
		}
	}
}

FILE* deleteRecord(FILE* f, char* fname)
{
	int num, k = 0;
	House house;
	FILE* tmp = NULL;
	if (f == NULL) {
		printf("Файл не открыт!\n");
		return NULL;
	}
	printfile(f);
	printf("Введите номер записи для удаления: ");
	scanf("%d", &num);
	while (getchar() != '\n');
	rewind(f);
	int count = 0;
	while (fread(&house, sizeof(house), 1, f) == 1) count++;
	if (num < 1 || num > count) {
		printf("Неверный номер записи!\n");
		return f;
	}
	tmp = fopen("temp.dat", "wb");
	rewind(f);
	k = 0;
	while (fread(&house, sizeof(house), 1, f) == 1)
	{
		k++;
		if (k != num)
		{
			fwrite(&house, sizeof(house), 1, tmp);
		}
	}
	fclose(f);
	fclose(tmp);
	remove(fname);
	rename("temp.dat", fname);
	return fopen(fname, "rb+");
}

void searchMenu(FILE* f)
{
	int choice = 0;
	int menu_active = 1;
	while (menu_active)
	{
		setlocale(0, "russian");
		printf("\nМеню поиска:\n");
		printf("1. Поиск по этажности\n");
		printf("2. Поиск по материалу\n");
		printf("3. Деревянные дома после года\n");
		printf("4. Назад\n");
		printf("Выберите: ");
		scanf("%d", &choice);
		while (getchar() != '\n');
		rewind(f);
		if (choice == 1)
		{
			searchByFloors(f);
		}
		else if (choice == 2)
		{
			searchByMaterial(f);
		}
		else if (choice == 3)
		{
			searchWoodenAfterYear(f);
		}
		else if (choice == 4)
		{
			menu_active = 0;
		}
		if (choice != 4)
		{
			printf("\nНажмите Enter для продолжения...");
			getchar();
		}
	}
}

int main()
{
	FILE* f = NULL;
	char fname[100];
	int choice = 0;
	int program_active = 1;
	setlocale(0, "russian");
	while (program_active)
	{
		printf("\nГлавное меню:\n");
		printf("1. Создать/открыть файл\n");
		printf("2. Просмотреть данные\n");
		printf("3. Поиск\n");
		printf("4. Удалить запись\n");
		printf("5. Выход\n");
		printf("Выберите: ");
		scanf("%d", &choice);
		while (getchar() != '\n');
		if (choice == 1)
		{
			f = formopen(fname);
		}
		else if (choice == 2)
		{
			printfile(f);
		}
		else if (choice == 3)
		{
			searchMenu(f);
		}
		else if (choice == 4)
		{
			f = deleteRecord(f, fname);
		}
		else if (choice == 5)
		{
			program_active = 0;
		}
		if (choice != 5)
		{
			printf("\nНажмите Enter для продолжения...");
			getchar();
		}
	}
	if (f)
	{
		fclose(f);
	}
	return 0;
}
