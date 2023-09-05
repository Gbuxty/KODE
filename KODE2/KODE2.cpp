


#include <iostream>
#include<fstream>
#include<string>
#include<vector>
#include<set>
#include<unordered_map>
#include<algorithm>
#include<Windows.h>
#include <map>
#include <cmath>
struct Object {
    std::string name;
    double x;
    double y;
    std::string type;
    double creationTime;
    std::string distanceCategory;
    std::string timeCategory;
 
    /*bool operator==(const Object& other) const {
        return name == other.name && x == other.x && y == other.y && type == other.type && creationTime == other.creationTime;
    }
    bool operator<(const Object& other) const {
        return name < other.name;
    }*/
};



// Функция для добавления нового объекта в список
void addObject(std::vector<Object>& objects) {
    Object newObject;

    std::cout << "Введите имя объекта: ";
    std::cin >> newObject.name;

    std::cout << "Введите координату X: ";
    std::cin >> newObject.x;

    std::cout << "Введите координату Y: ";
    std::cin >> newObject.y;

    std::cout << "Введите тип объекта: ";
    std::cin >> newObject.type;

    std::cout << "Введите время создания: ";
    std::cin >> newObject.creationTime;

    // Добавляем новый объект в список
    objects.push_back(newObject);

    std::cout << "Объект успешно добавлен!" << std::endl;
}
std::vector<Object> groupByName(const std::vector<Object>& objects) {
    std::map<char, std::vector<Object>> nameGroups;

    for (const Object& obj : objects) {
        char firstLetter = obj.name[0];
        if (!std::isalpha(static_cast<unsigned char>(firstLetter))) {
            firstLetter = '#'; // Если имя начинается с цифры или спецсимвола
        }
        nameGroups[firstLetter].push_back(obj);
    }

    std::vector<Object> groupedObjects;
    for (auto it = nameGroups.begin(); it != nameGroups.end(); ++it) {
        char letter = it->first;
        std::vector<Object>& group = it->second;

        std::sort(group.begin(), group.end(), [](const Object& a, const Object& b) {
            return a.name < b.name;
            });

        groupedObjects.insert(groupedObjects.end(), group.begin(), group.end());
    }

    return groupedObjects;
}

std::map<std::string, std::vector<Object>> groupByTime(const std::vector<Object>& objects) {
    std::map<std::string, std::vector<Object>> groupedObjects;

    // Получите текущее время
    time_t currentTime;
    time(&currentTime);

    // Определите временные интервалы
    time_t oneDay = 24 * 60 * 60; // Один день в секундах
    time_t oneWeek = 7 * oneDay;  // Одна неделя в секундах

    // Разбейте объекты на группы на основе времени создания
    for (const Object& obj : objects) {
        double timeDifference = difftime(currentTime, obj.creationTime);
        std::string timeCategory;

        if (timeDifference <= oneDay) {
            timeCategory = "Сегодня";
        }
        else if (timeDifference <= 2 * oneDay) {
            timeCategory = "Вчера";
        }
        else if (timeDifference <= oneWeek) {
            timeCategory = "На этой неделе";
        }
        else if (timeDifference <= 30 * oneDay) {
            timeCategory = "В этом месяце";
        }
        else if (timeDifference <= 365 * oneDay) {
            timeCategory = "В этом году";
        }
        else {
            timeCategory = "Ранее";
        }

        // Добавляем объект в соответствующую группу времени
        groupedObjects[timeCategory].push_back(obj);
    }

    // Сортируем объекты внутри каждой группы по времени
    for (auto& pair : groupedObjects) {
        std::sort(pair.second.begin(), pair.second.end(),
            [](const Object& a, const Object& b) {
                return a.creationTime < b.creationTime;
            });
    }

    return groupedObjects;
}
double calculateDistance(double x1, double y1, double x2, double y2) {
    return std::sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
}
std::vector<Object> groupByDistance(const std::vector<Object>& objects) {

    std::vector<Object> groupedObjects;

    std::vector<Object> distance100;
    std::vector<Object> distance1000;
    std::vector<Object> distance10000;
    std::vector<Object> distanceOver10000;

    for (const Object& obj : objects) {
        // Вычисляем расстояние между объектом и точкой (0, 0)
        double distance = calculateDistance(obj.x, obj.y, 0.0, 0.0);
        Object groupedObj = obj;
        // В зависимости от расстояния добавляем объект в соответствующую группу
        if (distance <= 100.0) {
            groupedObj.distanceCategory = "До 100 единиц";
            distance100.push_back(groupedObj);
        }
        else if (distance <= 1000.0) {
            groupedObj.distanceCategory = "До 1000 единиц";
            distance1000.push_back(groupedObj);
        }
        else if (distance <= 10000.0) {
            groupedObj.distanceCategory = "До 10000 единиц";
            distance10000.push_back(groupedObj);
           
        }
        else {
            groupedObj.distanceCategory = "Слишком далеко";
            distanceOver10000.push_back(groupedObj);
        }
    }

    // Добавляем группы объектов в результирующий вектор
    groupedObjects.insert(groupedObjects.end(), distance100.begin(), distance100.end());
    groupedObjects.insert(groupedObjects.end(), distance1000.begin(), distance1000.end());
    groupedObjects.insert(groupedObjects.end(), distance10000.begin(), distance10000.end());
    groupedObjects.insert(groupedObjects.end(), distanceOver10000.begin(), distanceOver10000.end());

    return groupedObjects;
}
std::vector<Object> groupByType(const std::vector<Object>& objects) {
    // Создаем мапу для группировки объектов по типу
    std::map<std::string, std::vector<Object>> typeGroups;

    // Проходим по всем объектам и добавляем их в соответствующие группы
    for (const Object& obj : objects) {
        typeGroups[obj.type].push_back(obj);
    }

    // Объединяем объекты из групп в новый вектор с сохранением порядка и сортировкой
    std::vector<Object> groupedObjects;
    for (auto& pair : typeGroups) {
        // Сортируем объекты внутри текущей группы по вашему кастомному сравнению
        std::sort(pair.second.begin(), pair.second.end(), [](const Object& a, const Object& b) {
            return a.name < b.name; // Сортировка по имени (по первой букве, затем по второй и т.д.)
            });

        // Добавляем объекты из текущей группы в результирующий вектор
        groupedObjects.insert(groupedObjects.end(), pair.second.begin(), pair.second.end());
    }

    return groupedObjects;
}
void saveToTextFile(const std::vector<Object>& groupedObjects, const std::string& filename) {
    std::fstream fs;
    fs.open(filename, std::fstream::in | std::fstream::out | std::fstream::app);

    if (!fs.is_open()) {
        std::cout << "Ошибка открытия файла: " << filename << std::endl;
        return;
    }

    for (const Object& obj : groupedObjects) {

        fs << obj.name << "," << obj.x << "," << obj.y << "," << obj.type << "," << obj.creationTime << std::endl;
    }

  
        fs.close();
    std::cout << "Результаты сохранены в файл: " << filename << std::endl;
}
int main()
{ 

    // Группировка по Времени надо доделать ,по расстонию слово
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    std::vector<Object> objects; // Создаем пустой список объектов

    while (true) {
        std::cout << "Меню:" << std::endl;
        std::cout << "1. Добавить объект" << std::endl;
        std::cout << "2. Группировка по имени" << std::endl;
        std::cout << "3. Группировка по расстоянию" << std::endl;
        std::cout << "4. Группировка по времени" << std::endl;
        std::cout << "5. Группировка по типу" << std::endl;
        std::cout << "6. Сохранить результаты в файл" << std::endl;
        std::cout << "7. Выйти из программы" << std::endl;

        int choice;
        std::cout << "Выберите действие: ";
        std::cin >> choice;

        switch (choice) {
        case 1:
            addObject(objects);
            break;
        case 2:
        {
            // Вызываем функцию для группировки по имени
            std::vector<Object> groupedByName = groupByName(objects);

            // Выводим результаты группировки на экран
            for (const Object& obj : groupedByName) {
                std::cout << "Имя: " << obj.name << ", Тип: " << obj.type << std::endl;
            }
        }
        break;
        case 3:
        {
            // Вызов функции для группировки по расстоянию
            std::vector<Object> groupedByDistance = groupByDistance(objects);

            // Вывод результатов группировки на экран
            for (const Object& obj : groupedByDistance) {
                std::cout << "Имя: " << obj.name << ", Расстояние: " << obj.distanceCategory << std::endl;
            }
        }
        break;
        case 4:{
        
            // Вызов функции для группировки по времени
              // Вызов функции для группировки по времени
            std::map<std::string, std::vector<Object>> groupedByTime = groupByTime(objects);

            // Вывод имен групп и объектов внутри каждой группы
            std::cout << "Группы по времени:" << std::endl;
            for (const auto& pair : groupedByTime) {
                std::cout << pair.first << std::endl; // Имя группы
                // Вывод объектов внутри группы
                for (const Object& obj : pair.second) {
                    std::cout << "   " << obj.name << std::endl;
                }
            }
        }
              break;
           
        case 5:
        {
            // Вызов функции для группировки по типу
            std::vector<Object> groupedByType = groupByType(objects);

            // Вывод результатов группировки на экран
            for (const Object& obj : groupedByType) {
                std::cout << "Имя: " << obj.name << ", Тип: " << obj.type << std::endl;
            }
        }
        break;
        case 6:
            saveToTextFile(objects, "file.txt");
            break;
        case 7:
            std::cout << "Выход из программы." << std::endl;
            return 0;
        default:
            std::cout << "Некорректный выбор. Попробуйте снова." << std::endl;
            break;
        }
    }

    return 0;
}
