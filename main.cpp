#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
using namespace std;

struct dish
{
    string dish;
    string dish_name;
    short int quantity = 1;
    float price = 0;
};

class menu
{
private:
    string name_file = "warehouse/";
    dish order;
    vector<vector<string>> tmp_2;
    vector<string> this_dish;
    short int iter;
    short int n;

public:
    void add(dish this_dish, int n)
    {
        this->order = to_lower(this_dish);
        this->name_file += this_dish.dish + ".txt";
        this->n = n;
        this->order.price = search();
        rewriting();
    }
    float price()
    {
        return this->order.price * n;
    }

private:
    double search() //поиск блюда, цены, количества
    {
        fstream file;
        file.open(name_file, fstream::in | fstream::out);
        vector<vector<string>> tmp_2;
        if (file.is_open()) //ищими такое блюдо
        {
            while (!file.eof())
            {
                string tmp;
                getline(file, tmp);
                tmp_2.push_back(search(tmp));
            }
            file.close();
        }
        else
        {
            cout << "ошибка 1" << endl;
        }

        bool flag = true;
        float price = -1;
        this->tmp_2 = tmp_2; //делаем слепок меню

        for (int i = 0; i < tmp_2.size(); i++) //ищем имя блюда, что бы потом выцепить его цену и вернуть её
        {
            string str = tmp_2[i][0].erase(tmp_2[i][0].size() - 1, 1);
            if (this->order.dish_name == str)
            {
                price = atof(tmp_2[i][1].c_str());
                flag = false;
                this->this_dish = line_processing(tmp_2[i], this->n); //содержит нашу строчку с уже изменёмнным значением на n больше
                this->iter = i;
                break;
            }
        }

        if (flag) //добавление нового блюда
        {
            file.open(name_file, fstream::in | fstream::app);
            if (file.is_open())
            {
                cout << "Не найдено: " << order.dish_name << endl;
                cout << "Добавление нового: " << order.dish << endl;
                string tmp;
                string tmp_swap;
                tmp += order.dish_name;
                cout << "Введите стоимость " << order.dish_name << ": ";
                cin >> tmp_swap;
                tmp += ',' + tmp_swap + ',' + to_string(order.quantity);

                file << '\n';
                file << tmp;
                file.close();
                price = atof(tmp_swap.c_str());
            }
            else
            {
                cout << "ошибка 2" << endl;
            }
        }
        return price;
    }

    vector<string> line_processing(vector<string> array, short int n) //обробатываем строка, изменяяя её последний параметр
    {
        vector<string> new_array;
        for (int i = 0; i < array.size() - 1; i++)
        {
            new_array.push_back(array[i]);
        }
        short int tmp = atoi(array[array.size() - 1].c_str());
        tmp += n;
        new_array.push_back(to_string(tmp));
        return new_array;
    }

    void rewriting() //запись в файл с последним изменнённым значением
    {
        fstream file;
        file.open(name_file, fstream::out | fstream::trunc);
        if (file.is_open())
        {
            for (int i = 0; i < this->this_dish.size(); i++)
            {
                if (i == 1)
                {
                    file << ',';
                }
                file << this->this_dish[i];
            }
            file << '\n';

            for (int i = 0; i < this->tmp_2.size(); i++)
            {
                if (i != this->iter)
                {
                    for (int j = 0; j < this->tmp_2[i].size(); j++)
                    {
                        file << tmp_2[i][j];
                    }
                    if (i != this->tmp_2.size() - 1)
                    {
                        file << '\n';
                    }
                }
            }
            file.close();
        }
        else
        {
            cout << "ошибка 3" << endl;
        }
    }

    string to_string(short int n) //перевод в string в int
    {
        ostringstream str;
        str << n;
        return str.str();
    }

    dish to_lower(dish str) //перевод в нижний регистр
    {
        dish new_str;
        new_str.dish = str.dish;
        new_str.price = str.price;

        for (int i = 0; i < str.dish_name.size(); i++)
        {
            char c = str.dish_name[i];
            if (isalpha(c))
            {
                new_str.dish_name += tolower(c);
            }
            else
            {
                new_str.dish_name += c;
            }
        }
        return new_str;
    }

    vector<string> search(string input) //вектор массив строк
    {
        vector<string> array;

        string temp;
        for (int i = 0; i < input.size(); i++)
        {
            char c = input[i];
            if (isalpha(c))
            {
                temp += tolower(c);
            }
            else
            {
                temp += c;
            }
            if (c == ',' || i == input.size() - 1)
            {
                if (temp.size() > 0)
                {
                    array.push_back(temp);
                }
                temp = "";
            }
        }

        return array;
    }
};

int main()
{
    float num = 0;
    dish order;
    while (true)
    {
        menu sessions;
        cout << "*---------------------------------------------" << endl;
        cout << "| Введите dish:";
        cin >> order.dish;
        cout << "| Введите dish name:";
        cin.ignore();
        getline(cin, order.dish_name);
        cout << "| Введите колчество этого блюда:";
        cin >> order.quantity;
        sessions.add(order, order.quantity);
        num += sessions.price();
        cout << "| Сделать ещё заказ ? | (1) - да, (0) - нет:";
        int n;
        cin >> n;
        cout << "*---------------------------------------------" << endl;
        if (!n)
        {
            break;
        }
    }
    cout << "#####################" << endl;
    cout << "# Ваша сумма: " << num << endl;
    cout << "#####################" << endl;
}
