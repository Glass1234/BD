#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <iomanip>

using namespace std;

struct dish
{
    string dish;
    string dish_name;
    int quantity;
    int price_for_1;
};

class menu
{
private:
    string name_file = "warehouse/";
    dish order;
    vector<vector<string>> tmp_2;
    short int iter;
    short int n;

    vector<string> this_dish;
    short int num;
    short int quantity;

public:
    void print(string str) //вывод меню данного класса блюда
    {
        fstream file;
        this->name_file += str + ".txt";
        file.open(name_file, fstream::in | fstream::out);
        vector<vector<string>> tmp_2;
        if (file.is_open())
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

        setlocale(LC_ALL, "");
        for (int i = 0; i < tmp_2.size(); i++)
        {
            {
                int num = search_max_string(tmp_2);
                cout << i + 1 << ") " << tmp_2[i][0] << setw(num - tmp_2[i][0].size() + 10) << tmp_2[i][1] << endl;
            }
        }
        this->tmp_2 = tmp_2;
        cout << "Выберите блюдо из списка: ";
        cin >> this->num;

        cout << "Количество блюда: ";
        cin >> this->quantity;
        this->this_dish = tmp_2[this->num - 1];
    }

    vector<string> add_dish() //возвращаем наше блюдо и меняем значение в файле
    {
        vector<string> array = this->this_dish;
        int tmp_num = stoi(array[array.size() - 1]) - this->quantity;
        array[array.size() - 1] = to_string(stoi(array[array.size() - 1]) - tmp_num);
        this->this_dish = line_processing(this->this_dish, this->quantity);

        fstream file;
        file.open(name_file, fstream::out | fstream::trunc);

        if (file.is_open())
        {
            file << this->this_dish[0];
            file << ',';
            file << this->this_dish[1];
            file << ',';
            file << this->this_dish[2];
            file << '\n';
            for (int i = 0; i < this->tmp_2.size(); i++)
            {
                if (i != this->num - 1)
                {
                    file << this->tmp_2[i][0];
                    file << ',';
                    file << this->tmp_2[i][1];
                    file << ',';
                    file << this->tmp_2[i][2];
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
            cout << "ошибка 2" << endl;
        }

        return array;
    }

    void top_menu(vector<string> str) //вывод актуального меню
    {
        vector<vector<string>> tmp_2;
        for (int i = 0; i < str.size(); i++)
        {
            fstream file;
            this->name_file += str[i] + ".txt";
            file.open(name_file, fstream::in | fstream::out);

            if (file.is_open())
            {
                while (!file.eof())
                {
                    string tmp;
                    getline(file, tmp);
                    tmp_2.push_back(search(tmp));
                }
                file.close();
                cout << "------------" << endl;
                top_menu_max(tmp_2);
                tmp_2.clear();
                this->name_file = "warehouse/";
            }
            else
            {
                cout << "ошибка 3" << endl;
            }
        }
    }

private:
    void top_menu_max(vector<vector<string>> array) //вывод блюд по количеству заказов, 3 из каждой категории
    {
        bool skip = false;
        while (!skip)
        {
            skip = true;

            for (int i = 0; i < array.size() - 1; i++)
            {
                for (int k = 2; k < array[i].size(); k++)
                {
                    if (stoi(array[i + 1][k]) > stoi(array[i][k]))
                    {
                        vector<string> tmp = array[i];
                        array[i] = array[i + 1];
                        array[i + 1] = tmp;
                        skip = false;
                    }
                }
            }
        }

        setlocale(LC_ALL, "");
        for (int i = 0; i < 3; i++)
        {
            for (int j = 0; j < array[i].size(); j++)
            {
                cout << array[i][j] << ' ';
            }
            cout << endl;
        }
    }

    short int search_max_string(vector<vector<string>> array) //находим максимальную длинну строки
    {
        short int max = 0;
        for (int i = 0; i < array.size(); i++)
        {
            if (array[i][0].size() > max)
            {
                max = array[i][0].size();
            }
        }
        return max;
    }

    double search() //поиск блюда, цены, количества
    {
        fstream file;
        file.open(name_file, fstream::in | fstream::out);
        vector<vector<string>> tmp_2;
        if (file.is_open())
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

        float price = -1;
        this->tmp_2 = tmp_2;

        bool flag = true;
        while (flag)
        {
            for (int i = 0; i < tmp_2.size(); i++)
            {
                string str = tmp_2[i][0];
                if (this->order.dish_name == str)
                {
                    price = atof(tmp_2[i][1].c_str());
                    this->this_dish = line_processing(tmp_2[i], this->n);
                    this->iter = i;
                    flag = false;
                    break;
                }
            }
            if (flag)
            {
                cout << "---->   Блюдо не найдено повторите попытку: ";
                cin >> this->order.dish_name;
            }
        }
        return 0;
    }

    vector<string> line_processing(vector<string> array, short int n) //обробатываем строку, изменя её последний параметр
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

    vector<string> search(string input) //вектор массив строк
    {
        vector<string> array;

        string temp;
        short int triger = 0;
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
                    if (triger != 2)
                        temp.erase(temp.begin() + temp.size() - 1);
                    array.push_back(temp);
                    triger++;
                }
                temp = "";
            }
        }

        return array;
    }
};

void print(vector<vector<string>> array)
{
    long int res = 0;
    cout << "Ваш заказ: " << endl;
    for (int i = 0; i < array.size(); i++)
    {
        res += (stoi(array[i][1]) * stoi(array[i][2]));
        for (int j = 0; j < array[i].size(); j++)
        {
            cout << array[i][j] << ' ';
        }
        cout << endl;
    }
    cout << "Ваша итоговая цена: " << res << endl;
}

int main()
{
    vector<string> dish_str = {"desserts", "salads", "snacks"};
    vector<vector<string>> array;
    while (true)
    {
        short int requests;
        cout << "1) Сделать новый заказ" << endl;
        cout << "2) Добавить пункт заказа" << endl;
        cout << "3) Завершить заказ и вывести чек" << endl;
        cout << "4) Показать актуальное меню" << endl;
        cin >> requests;
        if (requests == 1)
        {
            array.clear();
            while (true)
            {
                menu session;
                cout << "Выдерите класс блюда: " << endl;
                cout << "1) desserts" << endl;
                cout << "2) salads" << endl;
                cout << "3) snacks" << endl;
                cin >> requests;
                session.print(dish_str[requests - 1]);
                array.push_back(session.add_dish());
                cout << "Добавить ещё ? 1-Да 0-Нет ";
                cin >> requests;
                if (requests == 0)
                {
                    break;
                }
            }
        }
        if (requests == 2)
        {
            while (true)
            {
                menu session;
                cout << "Выдерите класс блюда: " << endl;
                cout << "1) desserts" << endl;
                cout << "2) salads" << endl;
                cout << "3) snacks" << endl;
                cin >> requests;
                session.print(dish_str[requests - 1]);
                array.push_back(session.add_dish());
                cout << "Добавить ещё ? 1-Да 0-Нет ";
                cin >> requests;
                if (requests == 0)
                {
                    break;
                }
            }
        }
        if (requests == 3)
        {
            print(array);
            array.clear();
        }
        if (requests == 4)
        {
            menu session;
            session.top_menu(dish_str);
        }
    }
}
