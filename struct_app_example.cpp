#include <stdio.h>
#include <cstdlib>
#include <locale.h>
#include <windows.h>

struct Car {
    int id;
    char name[50];
    int year;

    // Метод для вывода информации о машине
    void display() {
        printf("-------------\nID: %d\nНазвание: %s\nГод выпуска: %d\n", id, name, year);
    }
};

// Функция добавления машины
void addCar(const char* filename) {
    FILE* file;
    errno_t err = fopen_s(&file, filename, "r+b");

    if (err != 0 || file == nullptr) {
        err = fopen_s(&file, filename, "w+b");
        if (err != 0 || file == nullptr) {
            printf("Ошибка создания файла\n");
            return;
        }
        int carCount = 0;
        fwrite(&carCount, sizeof(int), 1, file);
    }

    int carCount;
    fseek(file, 0, SEEK_SET);
    if (fread(&carCount, sizeof(int), 1, file) != 1) {
        printf("Ошибка чтения счетчика машин\n");
        fclose(file);
        return;
    }

    fseek(file, 0, SEEK_END);

    Car car;
    printf("Введите ID: ");
    scanf_s("%d", &car.id);
    getchar();

    printf("Введите название: ");
    fgets(car.name, sizeof(car.name), stdin);
    // Меняем \n, который мог остаться на конце на \0
    int len = strlen(car.name);
    if (car.name[len - 1] == '\n') {
        car.name[len - 1] = '\0';
    }

    printf("Введите год выпуска: ");
    scanf_s("%d", &car.year);

    if (fwrite(&car, sizeof(Car), 1, file) != 1) {
        printf("Ошибка записи структуры Car\n");
    }
    else {
        carCount++;
        fseek(file, 0, SEEK_SET);
        fwrite(&carCount, sizeof(int), 1, file);
    }

    fclose(file);
}


// Функция для отображения всех машин из файла
void displayCars(const char* filename) {
    FILE* file;
    errno_t err = fopen_s(&file, filename, "rb");
    if (err != 0 || file == nullptr) {
        printf("Ошибка открытия файла\n");
        return;
    }

    int carCount;
    fread(&carCount, sizeof(int), 1, file); // Считываем количество машин
    if (carCount == 0) {
        printf("Список машин пуст.\n");
        fclose(file);
        return;
    }

    printf("Всего машин: %d\n", carCount);

    Car car;
    while (fread(&car, sizeof(Car), 1, file) == 1) {
        car.display();
    }

    fclose(file);
}

// Функция удаления машины по ID
void deleteCar(const char* filename, int idToDelete) {
    FILE* file;
    errno_t err = fopen_s(&file, filename, "rb");
    if (err != 0 || file == nullptr) {
        printf("Ошибка открытия файла\n");
        return;
    }

    int carCount;
    fread(&carCount, sizeof(int), 1, file);

    Car* cars = new Car[carCount];

    int newCount = 0;
    Car car;

    // Читаем все машины, кроме удаляемой
    for (int i = 0; i < carCount; ++i) {
        fread(&car, sizeof(Car), 1, file);
        if (car.id != idToDelete) {
            cars[newCount++] = car;
        }
    }

    fclose(file);

    err = fopen_s(&file, filename, "wb");
    fwrite(&newCount, sizeof(int), 1, file);
    fwrite(cars, sizeof(Car), newCount, file);
    printf("Машина с ID %d удалена.\n", idToDelete);

    fclose(file);
    delete[] cars;
}

int main() {
    setlocale(LC_ALL, "Ru");
    // Для считывания русских букв
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    const char* filename = "cars.bin";

    while (true) {
        printf("1. Добавить машину\n2. Вывести все машины\n3. Удалить машину\n4. Выход\nВыберите действие: ");
        int choice;
        scanf_s("%d", &choice);

        switch (choice) {
        case 1:
            addCar(filename);
            break;
        case 2:
            displayCars(filename);
            break;
        case 3: {
            int idToDelete;
            printf("Введите ID машины для удаления: ");
            scanf_s("%d", &idToDelete);
            deleteCar(filename, idToDelete);
            break;
        }
        case 4:
            return 0;
        default:
            printf("Некорректный ввод. Попробуйте снова.\n");
        }
    }

    return 0;
}
