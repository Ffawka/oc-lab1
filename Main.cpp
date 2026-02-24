#include <windows.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;

struct employee
{
    int num;
    char name[10];
    double hours;
};

string findExeFile(string filename)
{
    vector<string> searchPaths = 
    {
        "",                    
        "Debug\\",                      
        "x64\\Debug\\",          
        "..\\Debug\\",        
    };

    for (string path : searchPaths)
    {
        string fullPath = path + filename;
        ifstream test(fullPath.c_str());
        if (test)
        {
            test.close();
            cout << "Найден в" << fullPath << endl;
            return fullPath;
        }
    }

    return "";
}

void showBinaryFile(string filename)
{
    ifstream file(filename, ios::binary);
    if (!file)
    {
        cout << "Не удалось открыть файл" << filename << endl;
        return;
    }

    cout << "Содержимое бинарного файла:" << endl;
    cout << "ID\tИмя\tЧасы" << endl;

    employee emp;
    while (file.read((char*)&emp, sizeof(employee)))
    {
        cout << emp.num << "\t" << emp.name << "\t" << emp.hours << endl;
    }
    file.close();
}

void showReport(string filename)
{
    ifstream file(filename);
    if (!file)
    {
        cout << "Не удалось открыть отчет " << filename << endl;
        return;
    }

    cout << "Содержимое отчета:" << endl;
    string line;
    while (getline(file, line))
    {
        cout << line << endl;
    }
    file.close();
}

int main()
{
    system("chcp 1251 > NULL");

    cout << "Поиск Creator.exe" << endl;
    string creatorPath = findExeFile("Creator.exe");
    if (creatorPath.empty())
    {
        cout << "Creator.exe не найден" << endl;
        return 1;
    }

    cout << "Поиск Reporter.exe" << endl;
    string reporterPath = findExeFile("Reporter.exe");
    if (reporterPath.empty())
    {
        cout << "Reporter.exe не найден" << endl;
        return 1;
    }

    string binFileName;
    int recordCount;

    cout << "Введите имя бинарного файла: ";
    cin >> binFileName;
    cout << "Введите количество записей: ";
    cin >> recordCount;

    string cmd = "\"" + creatorPath + "\" " + binFileName + " " + to_string(recordCount);
    cout << "Запуск Creator... (команда: " << cmd << ")" << endl;

    int result = system(cmd.c_str());
    if (result != 0)
    {
        cout << "Ошибка при запуске Creator" << result << endl;
        return 1;
    }

    ifstream testFile(binFileName);
    if (!testFile)
    {
        cout << "Файл " << binFileName << " не был создан!" << endl;
        return 1;
    }
    testFile.close();

    showBinaryFile(binFileName);

    string reportFileName;
    double hourlyRate;

    cout << "Введите имя файла отчета: ";
    cin >> reportFileName;
    cout << "Введите оплату за час: ";
    cin >> hourlyRate;

    cmd = "\"" + reporterPath + "\" " + binFileName + " " + reportFileName + " " + to_string(hourlyRate);
    cout << "Запуск Reporter (команда: " << cmd << ")" << endl;

    result = system(cmd.c_str());
    if (result != 0)
    {
        cout << "Ошибка при запуске Reporter" << result << endl;
        return 1;
    }

    showReport(reportFileName);
    return 0;
}
