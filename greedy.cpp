#include <cstdlib>
#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
#include <set>
#include <algorithm>

using namespace std;

bool mySort (pair < int, int > i, pair < int, int > j)
{
    return i.second > j.second;
}

bool mySort1 (pair < int, int[3] > i, pair < int, int[3] > j)
{
    return i.first < j.first;
}

int check_how_much (std::vector < int > vec)
{
    int size = vec.size();
    for (int i = 0; i < size; i++)
        if (vec[i] == -1)
            return i;

    return -1;
}

int check_duplicate (std::vector < int > vec, int pos)
{
    int size = vec.size();
    for (int i = 0; i < size; i++)
        if (vec[i] == pos)
            return -1;

    return 1;
}

int find_first (std::vector < std::vector < int > > vec, std::set < int > proc_set)
{
    std::vector < pair <int, int> > tmp(proc_set.size());

    int i = 0;
    for (std::set<int>::iterator it = proc_set.begin(); it != proc_set.end(); it++)
    {
        tmp[i].first = *it;
        tmp[i++].second = check_how_much(vec[*it]);
    }

    sort(tmp.begin(), tmp.end(), mySort);
    /*for (int j = 0; j < i; j++)
    {
        cout << tmp[j].first << " " << tmp[j].second << endl;
    }
    cout << tmp[0].first << " " << tmp[0].second << endl;*/

    return tmp[0].first;
}

std::vector < std::vector < int > > find_neighbour (std::vector < int > vec, int dim)
{
    std::vector < std::vector < int > > tmp(6);
    for (int i = 0; i < 6; i++)
    {
        tmp[i].resize(3);
        tmp[i] = vec;
    }

    for (int i = 0; i < 3; i++)
    {
        tmp[i][i]++;

        if (tmp[i][i] == dim)
            tmp[i][i] = 0;

        tmp[i + 3][i]--;

        if (tmp[i + 3][i] == -1)
            tmp[i + 3][i] = dim - 1;
    }

    return tmp;
}

int main(int argc, char const *argv[])
{
    /*<matrix_file> <num_of_proc>*/
    if (argc != 3)
    {
        cerr << ">Unexpected quantity of arguments, check your comand string:\n ./<prog> <matrix_file> <num_of_proc>" << endl;
        return -1;
    }

    ifstream matrix_file(argv[1]);
    if (matrix_file.is_open() == false)
    {
        cerr << ">Can not open matrix with such name." << endl;
        return -1;
    }

    int num = atoi(argv[2]);

    double tmp = cbrtf(num);
    int dim = tmp;
    if (tmp - dim > 0.0000001)
    {
        cout << ">Can not map your program, because your number of proc is not a 3-rd power of some N." << endl;
        matrix_file.close();
        return -1;
    }

    /*ИНИЦИАЛИЗАЦИЯ И СЧИТЫВАНИЕ КОММУНИКАЦИОННОЙ МАТРИЦЫ*/
    std::vector < std::vector < int > > matrix(num);

    for (int i = 0; i < num; i++)
        matrix[i].resize(num);

    for (int i = 0; i < num; i++)
        for (int j = 0; j < num; j++)
            matrix_file >> matrix[i][j];
    /*****************************************************/

    /*СОЗДАНИЕ МЭППИНГ ВЕКТОРА, НАХОЖДЕНИЕ ПРЕДПОЧТИТЕЛЬНЫХ
    ПРОЦЕССОВ ДЛЯ КАЖДОГО ПРОЦЕССА И ВЫБОРА 6 ОПТИМАЛЬНЫХ*/
    std::vector < std::vector < int > > mapping_vec(num);

    for (int i = 0; i < num; i++)
    {
        mapping_vec[i].resize(6);
        for (int j = 0; j < 6; j++)
            mapping_vec[i][j] = -1;
    }

    std::vector < pair < int, int > > tmp_str(num);

    for (int i = 0; i < num; i++)
    {
        for (int j = 0; j < num; j++)
        {
            tmp_str[j].first = j;
            tmp_str[j].second = matrix[i][j];
        }
        /*добавить if и сделать сорировку не полного массива для ускорения*/
        sort(tmp_str.begin(), tmp_str.end(), mySort);

        /*вывод для дебага*/
        /*cout << i << "   ";
        for (int j = 0; j < num; j++)
        {   
            if (tmp_str[j].second == 0)
            {
                cout << " " << tmp_str[j].first << " ";
                continue;
            }
            cout << tmp_str[j].first << " ";
        }
        cout << endl;*/

        int count_neigh = check_how_much(mapping_vec[i]), conut_num = 0;

        while ((count_neigh < 6) && (count_neigh != -1) && (conut_num < num))
        {
            if (tmp_str[conut_num].second == 0)
                break;

            int place2 = -1;
            int val1 = 0, val2 = 0, pos = tmp_str[conut_num].first;

            if ((val1 = check_duplicate(mapping_vec[i], pos)) == 1)
            {
                if ((place2 = check_how_much(mapping_vec[pos])) != -1)
                    if((val2 = check_duplicate(mapping_vec[pos], i)) == 1)
                    {
                        mapping_vec[i][count_neigh++] = pos;
                        mapping_vec[pos][place2] = i;
                    }
            }
            conut_num++;
        }
    }
    /*****************************************************/

    /*вывод для дебага*/
    /*cout << endl;
    for (int i = 0; i < num; i++)
    {
        cout << i << "   ";
        for (int j = 0; j < 6; j++)
            cout << mapping_vec[i][j] << " ";
        cout << endl;
    }*/

    /*МНОЖЕСТВА ДЛЯ КЛАССИФИКАЦИИ*/
    std::set < int > done, process, last;

    for (int i = 0; i < num; i++)
    {
        if (mapping_vec[i][0] == -1)
        {
            last.insert(i);
            continue;
        }

        process.insert(i);
    }
    /*****************************/

    /*ТОР И ИНИЦИАЛИЗАЦИЯ, ОПРЕДЕЛЕНИЕ ПЕРВОГО ПРОЦЕССА*/
    std::vector < std::vector < std::vector < int > > > torus(dim);

    for (int i = 0; i < dim; i++)
    {
        torus[i].resize(dim);
        for (int j = 0; j < dim; j++)
            torus[i][j].resize(dim);
    }

    for (int i = 0; i < dim; i++)
        for (int j = 0; j < dim; j++)
            for (int k = 0; k < dim; k++)
                torus[i][j][k] = -1;

    int proc = 0;

    std::vector < std::vector < int > > neihgbours(6);
    std::vector < int > proc_cords(3, 0);

    for (int i = 0; i < 6; i++)
        neihgbours[i].resize(3);

    proc = find_first(mapping_vec, process); /*ПОТОМ ЗАКОММЕНТИТЬ И СРАВНИТЬ С ТЕМ АЛГОРИТМОМ, КОГДА ПЕРВЫЙ ВСЕГДА 0*/
    /***************************************************/

    /*РАСПРЕДЕЛЕНИЕ ПРОЦЕССОВ В ТОРЕ ПО МЭППИНГ ВЕКТОРУ*/
    torus[0][0][0] = proc;
    while (process.empty() != true)
    {
        neihgbours = find_neighbour(proc_cords, dim);

        for (int i = 0; i < 6; i++)
        {
            if (mapping_vec[proc][i] == -1)
                break;

            if (done.find(mapping_vec[proc][i]) != done.end())
                continue;

            for (int j = 0; j < 6; j++)
                if (torus[neihgbours[j][0]][neihgbours[j][1]][neihgbours[j][2]] == -1)
                {
                    torus[neihgbours[j][0]][neihgbours[j][1]][neihgbours[j][2]] = mapping_vec[proc][i];
                    done.insert(mapping_vec[proc][i]);
                    break;
                }
        }

        process.erase(proc);
        done.insert(proc);

        int last_proc = proc;

        for (int j = 0; j < 6; j++)
            if ((torus[neihgbours[j][0]][neihgbours[j][1]][neihgbours[j][2]] != -1)
                && (process.find(torus[neihgbours[j][0]][neihgbours[j][1]][neihgbours[j][2]]) != process.end()))
            {
                for (int k = 0; k < 3; k++)
                    proc_cords[k] = neihgbours[j][k];
                proc = torus[neihgbours[j][0]][neihgbours[j][1]][neihgbours[j][2]];

                /*вывод для дебага*/
                /*cout << "!!!" << proc << " " << last_proc << endl;
                for (int k = 0; k < 3; k++)
                    cout << proc_cords[k] << " ";
                cout << endl;*/
                break;
            }

        if ((last_proc == proc) && (process.empty() != true))
        {
            for (std::set<int>::iterator it = process.begin(); it != process.end(); it++)
                if (done.find(*it) == done.end())
                    proc = *it;

            if (proc == last_proc)
                break;

            int flag = 0;
            for (int i = 0; i < dim; i++)
            {
                if (flag)
                    break;
                for (int j = 0; j < dim; j++)
                {
                    if (flag)
                        break;
                    for (int k = 0; k < dim; k++)
                        if (torus[i][j][k] == -1)
                            {
                                proc_cords[0] = i;
                                proc_cords[1] = j;
                                proc_cords[2] = k;
                                flag = 1;
                                break;
                            }
                }
            }

            torus[proc_cords[0]][proc_cords[1]][proc_cords[2]] = proc;

            /*вывод для дебага*/
            /*cout << "???" << proc << " " << last_proc << endl;
            for (int k = 0; k < 3; k++)
                cout << proc_cords[k] << " ";
            cout << endl;*/
        }
    }

    /*вывод для дебага*/
    /*for (std::set<int>::iterator it = last.begin(); it != last.end(); it++)
    {
        cout << *it << endl;
    }*/

    /*for (int i = 0; i < dim; i++)
        for (int j = 0; j < dim; j++)
            for (int k = 0; k < dim; k++)
            {
                cout << torus[i][j][k] << endl;
            }*/

    while (last.empty() != true)
    {
        for (int i = 0; i < dim; i++)
            for (int j = 0; j < dim; j++)
                for (int k = 0; k < dim; k++)
                    if (torus[i][j][k] == -1)
                    {
                        torus[i][j][k] = *last.begin();
                        last.erase(torus[i][j][k]);
                    }
    }
    /***************************************************/

    /*СОРТИРОВКА ВЕКТОРА ДЛЯ ВЫВОДА МЭППИГ-ФАЙЛА*/
    std::vector<pair <int, int[3]> > vec(num);
    int cnt = 0;

    for (int i = 0; i < dim; i++)
        for (int j = 0; j < dim; j++)
            for (int k = 0; k < dim; k++)
            {
                vec[cnt].first = torus[i][j][k];
                vec[cnt].second[0] = i;
                vec[cnt].second[1] = j;
                vec[cnt++].second[2] = k;
            }

    sort(vec.begin(), vec.end(), mySort1);    
    /********************************************/

    cout << endl;
    for (int i = 0; i < num; i++)
    {
        cout << vec[i].first << "   " << vec[i].second[0] << " " << vec[i].second[1] << " " << vec[i].second[2] << endl;
    }

    /*вывод для дебага*/
    /*cout << endl;
    for (int i = 0; i < dim; i++)
        for (int j = 0; j` < dim; j++)
            for (int k = 0; k < dim; k++)
            {
                cout << torus[i][j][k] << "   " << i << " " << j << " " << k << endl;
            }*/

    return 0;
}