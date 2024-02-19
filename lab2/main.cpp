#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <regex>
#include <set>
#include <fstream> // Для работы с файлами

using namespace std;
#define epl(a) (a == "e" ? "" : a)
#define min(a,b) (a < b ? a : b)


string remove_special_characters(const string& input) {
    string result;
    for (char c : input) {
        if (c != '*' && c != '(' && c != ')' && c != '|') {
            result += c;
        }
    }
    return result;
}

// Функция для подсчета алфавитной длины строки и количества слов
int calculate_alphabetical_length_and_word_count(const string& input) {
    string cleaned_string = remove_special_characters(input);
    
    int word_count = 0;
    
    bool in_word = false;
    for (char c : cleaned_string) {
        if (isalpha(c)) {
            
            if (!in_word) {
                in_word = true;
                word_count++;
            }
        } else {
            in_word = false;
        }
    }
    
    return word_count;
}



class Graph {
private:
    
    vector<vector<string>> adjacencyMatrix;

public:
    int numVertices;
    // Конструктор
    Graph(int numVertices) : numVertices(numVertices) {
        adjacencyMatrix.resize(numVertices, vector<string>(numVertices, ""));
    }

    // Добавление ребра между вершинами
    void addEdge(int src, int dest,string a) {
        if (src >= 0 && src < numVertices && dest >= 0 && dest < numVertices) {
            adjacencyMatrix[src][dest] = a;
            // Если граф неориентированный, добавьте следующую строку
            // adjacencyMatrix[dest][src] = 1;
        } else {
            //cout << "Неверные индексы вершин." << endl;
        }
    }
    void printReg(){    
    for (int i = 0; i < numVertices; ++i) cout <<  adjacencyMatrix[0][i];
    }
    // Вывод матрицы смежности
    void printAdjacencyMatrix() {
        cout << "  ";
        for (int i = 0; i < numVertices; ++i) cout <<  i << " ";
        cout << endl;
        for (int i = 0; i < numVertices; ++i) {
            cout << i << " ";
            for (int j = 0; j < numVertices; ++j) {
                if ( adjacencyMatrix[i][j] == "" ) cout << "_" << " ";
                else cout << adjacencyMatrix[i][j] << " ";
            }
            cout << endl;
        }
    }
    void delete_edge(int a ){
        vector<int> input; 
        vector<int> output;
        string klini = " ";
        if (adjacencyMatrix[a][a] != "" )  klini = "( " + adjacencyMatrix[a][a] + " ) * ";
        for(int i = 0;i < numVertices;i++) if (a!=i) if (adjacencyMatrix[i][a] != "") input.push_back(i);
        for(int i = 0;i < numVertices;i++) if (a!=i) if (adjacencyMatrix[a][i] != "") output.push_back(i);
        for(int i = 0 ; i < input.size(); i++) {
            for(int j = 0; j < output.size(); j++){
                int in = input[i],ou = output[j];
                auto res = epl(adjacencyMatrix[in][a]) + klini+ epl(adjacencyMatrix[a][ou]);
                if(adjacencyMatrix[in][ou] == "" ) adjacencyMatrix[in][ou] = res;
                else adjacencyMatrix[in][ou] = "((" + adjacencyMatrix[in][ou] + ") " + "|"+ " "+"( "+res +" )) ";
            }
        }
        for (int i = 0; i < numVertices ; i++ ) {adjacencyMatrix[i][a] = "";adjacencyMatrix[a][i] = "";}
    }
    int counter(){
        int c = 0;
    for(int i  = 0; i < numVertices ;i++){
        for(int j  = 0; j < numVertices ;j++){
            if(adjacencyMatrix[i][j] != "" && adjacencyMatrix[i][j] != "e"){
                c+= calculate_alphabetical_length_and_word_count(adjacencyMatrix[i][j]);
                //cout<< adjacencyMatrix[i][j]<<" "<<c<<endl;
            } 
    }

    }
    return c;
    }
};

// Функция для удаления символов * ( ) 
int min_int_zero = 2000000000;
vector<int> minimal_result;
int find_minimal (Graph g,int age,vector<int> a){
    if (age  == 0) {
        int c = g.counter();  
        if(min_int_zero > c){
            min_int_zero = min (min_int_zero,c);
            minimal_result = a;
        }
        
        return c;
    }

    vector<pair<int,int>> bb; 
    for(int i = 1; i < g.numVertices-1; i++){
        auto it = std::find(a.begin(), a.end(), i);
        if (it == a.end()) {
        Graph buff = g;
        buff.delete_edge(i);
        bb.push_back(pair{i,buff.counter()});
        }
    }
    
    
    std::sort(bb.begin(), bb.end(), [](const std::pair<int, int>& a, const std::pair<int, int>& b) {
        return a.second < b.second;
    });
    Graph buff_ = g;
    vector<int> buff_vec = a;
    buff_vec.push_back(bb[0].first);
    buff_.delete_edge(bb[0].first);
    for(int i = 0 ;i< bb.size(); i++){
        //std::cout << bb[i].first<<" "<<bb[i].second<<endl;
    }
    //std::cout << std::endl;
    int min_in_next_age = find_minimal(buff_,age-1,buff_vec);
    
    for(int i = 0 ;i< bb.size(); i++){
        if ( bb[i].second < min_in_next_age) {
            buff_ = g;
            buff_vec = a;
            buff_vec.push_back(bb[i].first);
            buff_.delete_edge(bb[i].first);
            int is_min_in_next_age = find_minimal(buff_,age-1,buff_vec);
            if(is_min_in_next_age< min_in_next_age) min_in_next_age = is_min_in_next_age;
        }
        
    }
    
    return min_in_next_age ;
    
    }
/**int main() {
    // Создание графа с 5 вершинами
    Graph graph(6);
    vector<int> a;
    string a = "Q
Q1, Q3
(Q,a) -> Q1
(Q1,a) -> QQ
(Q1,b) -> Q3
(Q3,b) -> Q
(QQ,b) -> Q1";
    // Добавление рёбер
    graph.addEdge(0, 1,"e");
    graph.addEdge(1,2,"a");
    graph.addEdge(2,3,"a");
    graph.addEdge(2,4,"b");
    graph.addEdge(4,1,"b");
    graph.addEdge(3,2,"b");
    graph.addEdge(2,5,"e");
    graph.addEdge(4,5,"e");

    // Вывод матрицы смежности
    cout << "Матрица смежности графа:" << endl;
    graph.printAdjacencyMatrix();
    find_minimal(graph,4,a);
    
    for(int i = 0 ; i < minimal_result.size() ; i++){ 
        graph.delete_edge(minimal_result[i]);
    }
    graph.printAdjacencyMatrix();
    return 0;
}
**/
vector<string> split(const string& s, char delimiter) {
    vector<string> tokens;
    string token;
    istringstream tokenStream(s);
    while (getline(tokenStream, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}

int main(int argc, char* argv[]) {
    string filePath = "";
    if (argc != 2) {
        cout << "Использование: " << argv[0] << " <путь_к_файлу>" << endl;
         filePath = "C:/Users/Admi/Desktop/Lab2/INPUT.TXT ";
    }
    else {
         filePath = argv[1];
    }
     // Путь к файлу из аргумента командной строки

    /**string input = 
        "Q\n"
        "Q1\n"
        "(Q,a) -> Q\n"
        "(Q1,a) -> Q1\n"
        "(Q1,b) -> Q\n"
        "(Q,b) -> Q1\n";
    **/
    ifstream file(filePath); // Открываем файл для чтения
    string input; // Строка для хранения содержимого файла
    if (file.is_open()) {
        string line;
        while (getline(file, line)) {
            input += line + "\n"; // Считываем содержимое файла построчно
        }
        file.close(); // Закрываем файл
    } else {
        cout << "Невозможно открыть файл input.txt" << endl;
        return 1;
    }
    // Получение всех строк из ввода
    vector<string> lines = split(input, '\n');

    // Инициализация множества для хранения всех вершин
    set<string> verticesSet;

    // Извлечение вершин из строк описания рёбер
    regex edgeRegex("\\(([^,]+),([^\\)]+)\\) -> ([^\\s]+)");
    for (int i = 2; i < lines.size(); ++i) {
        smatch match;
        if (regex_match(lines[i], match, edgeRegex)) {
            verticesSet.insert(match[1]);
            verticesSet.insert(match[3]);
        }
    }

    // Преобразование множества вершин в вектор для удобства работы с индексами
    vector<string> vertices(verticesSet.begin(), verticesSet.end());

    // Создание объекта графа
    Graph graph(vertices.size()+2);

    // Заполнение графа рёбрами
    for (int i = 2; i < lines.size(); ++i) {
        smatch match;
        if (regex_match(lines[i], match, edgeRegex)) {
            string src = match[1];
            string dest = match[3];
            string label = match[2];
            int srcIndex = find(vertices.begin(), vertices.end(), src) - vertices.begin();
            int destIndex = find(vertices.begin(), vertices.end(), dest) - vertices.begin();
            if (srcIndex != vertices.size() && destIndex != vertices.size()) {
                graph.addEdge(srcIndex+1, destIndex+1, label);
            } else {
                cout << "Ошибка: Некорректные вершины в строке " << i + 1 << endl;
            }
        } else {
            cout << "Ошибка: Некорректный формат строки " << i + 1 << endl;
        }
    }
    lines[1].erase(remove(lines[1].begin(), lines[1].end(), ' '), lines[1].end());
    vector<string> lastVertices = split(lines[1], ',');
    for (const string& vertex : lastVertices) {
        int index = find(vertices.begin(), vertices.end(), vertex) - vertices.begin();
        if (index != vertices.size()) {
            graph.addEdge(index + 1, vertices.size() + 1, "e");
        }
    }
    lines[0].erase(remove(lines[0].begin(), lines[0].end(), ' '), lines[0].end());
    vector<string> firstVertices = split(lines[0], ',');
    for (const string& vertex : firstVertices) {
        int index = find(vertices.begin(), vertices.end(), vertex) - vertices.begin();
        if (index != vertices.size()) {
            graph.addEdge(0,index + 1, "e");
        }
    }
    // Вывод матрицы смежности
    //graph.printAdjacencyMatrix();
    vector<int> a;
    find_minimal(graph,vertices.size(),a);
    
    for(int i = 0 ; i < minimal_result.size() ; i++){ 
        graph.delete_edge(minimal_result[i]);
    }
    //graph.printAdjacencyMatrix();
    graph.printReg();
    return 0;
}