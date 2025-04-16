#include <iostream>
#include <vector>
#include <unordered_map>
#include <algorithm>
using namespace std;

vector<unordered_map<int, int>> graph; //граф
vector<int> way; //вектор для временного хранения пути
vector<bool> mark; //вектор для пометки пройденных вершин
vector<vector<int>> badEdges; //вектор для пометки ребер, в которые не следует возвращаться
vector<int> answer; //ыектор для хранения веса гамильтонога цикла на кадую переменную
vector<vector<int>> finalWay; //вектор для хранения пути гамильтонова цикла на каждую перемнную

bool dAlg(int start, int startVertex, int ans) {
    //проверка на изолированные вершины
    if (graph[start].size() == 0) return 0;

    mark[start] = 1;
    way.push_back(start);
    int min = 1000;
    int min_v = -1;

    bool status = all_of(mark.begin(), mark.end(), [](bool value) {return value;});
    //условие выхода из рекурсии(вернулись в исходную вершину и все другие вершины пройдены)
    if (start == startVertex && status) {
        answer[start] = ans;
        finalWay[start] = way;
        return 1;
    }
    //поиск минимального ребра
    for (const auto vertex : graph[start]) {
        if (vertex.first == startVertex && status) {
            min = vertex.second;
            min_v = vertex.first;
        }
        if (vertex.second < min && !mark[vertex.first] && badEdges[start].end() == find(badEdges[start].begin(), badEdges[start].end(), vertex.first)) {
            min = vertex.second;
            min_v = vertex.first;
        }
    }
    //условие отката
    if (min_v == -1){
        if (way.size() == 2) return 0;
        int a = way[way.size() - 1];
        way.pop_back();
        int b = way[way.size() - 1];
        way.pop_back();
        mark[a] = 0;
        badEdges[b].push_back(a);
        badEdges[a].clear();
        dAlg(b, startVertex, ans);
        return 1;
    }

    ans += min;
    dAlg(min_v, startVertex, ans);
    return 0;
}

int main()
{
    setlocale(LC_ALL, "ru");
    unsigned int n, k;//n-количество вершин, k-количество ребер
    cin >> n >> k;
    //создание графа
    for (int i = 0; i <= n; i++) {
        unordered_map<int, int> map;
        graph.push_back(map);
    }
    //заполнение графа
    for (int i = 0; i < k; i++) {
        int a, b, c;//а, b - номера смежных вершин, с - вер ребра, соединяющего вершины
        cin >> a >> b >> c;
        graph[a][b] = c;
        graph[b][a] = c;
    }
    int counter = 0; //для проверки существования ответа
    //цикл запуска функции
    for (int i = 1; i <= n; i++) {
        way.clear();
        badEdges.clear();
        for (int j = 0; j <= n; j++) {
            vector<int> ver;
            badEdges.push_back(ver);
            answer.push_back(0);
            finalWay.push_back(ver);
        }
        mark.clear();
        mark.push_back(1);
        for (int j = 1; j <= n; j++) {
            mark.push_back(0);
        }
        int startVertex = i;
        bool status = dAlg(i, startVertex, 0); //запуск функции
        if (!status) {
            cout << "Гамильтонов цикл невозможен от вершины "<< i << endl;
            counter++;
        }
    }
    if (counter != n) {
        int min = answer[1];
        int min_ind = 1;
        for (int i = 1; i <= n; i++) {
            if (answer[i] < min) {
                min = answer[i];
                min_ind = i;
            }
        }
        cout << "Минимальный вес: " << min << "\nПуть: ";
        for (const auto& el : finalWay[min_ind]) {
            cout << el << " ";
        }
        cout << endl;
    }
    
}

