#include <iostream>
#include <vector>
#include <cstdlib>
#include <string>
#include <sstream>
#include <algorithm>
#include <unordered_map>
#include <cmath>
#include <bitset>
using namespace std;

vector< vector<int> >& getinput(vector< vector<int> >& graph, int& drones_to_place);
vector< vector<int> > find_components(vector< vector<int> >& graph);
void print_vector(const vector< vector<int> >& v);
int max_drones(vector< vector<int> >& graph, unsigned long long key, int bound, int starting_point, unordered_map<unsigned long long, int>& memory, unordered_map<int, int>& legend, vector<int>& components);
bool inGraph(const unsigned long long& key, int graph_size, const int& node);
unsigned long long updateKey(const unsigned long long& key, unsigned long long graph_size, const unsigned long long& node);
int degree(const unsigned long long& key, const int& node, vector< vector<int> >& graph, unordered_map<int,int>& legend, vector<int>& components);
vector<int> links(const unsigned long long& key, int&node, vector< vector<int> >& graph,  unordered_map<int, int>& legend, vector<int>& components);
int remove_neighbours(unsigned long long& key, vector<int>& nodes_to_remove, unordered_map<unsigned long long, int>& memory,vector< vector<int> >& graph, unordered_map<int, int>& legend, vector<int>& components);
vector<int> remove_node(unsigned long long& key, const int& node, const int& graph_size, vector< vector<int> >& graph, unordered_map<int, int>& legend, vector<int>& components);
int prune(unsigned long long& key, vector< vector<int> >& graph, unordered_map<unsigned long long, int>& memory, unordered_map<int, int>& legend, vector<int>& components);

//unsigned long long depth = 0;

int prune(unsigned long long& key, vector< vector<int> >& graph, unordered_map<unsigned long long, int>& memory, unordered_map<int, int>& legend, vector<int>& components)
{
    vector<int> nodes_to_remove;
    int drones_placed = 0;
    for (int node = 0; node < components.size(); node ++){
        //cout << "before degree" << endl;
        if(degree(key, node, graph, legend, components) <= 1)
        {
         nodes_to_remove.push_back(node);
         //cout << "adding node "<< components[node] << endl;
        }
    }
    drones_placed = remove_neighbours(key, nodes_to_remove, memory, graph, legend, components);
    return drones_placed;
}


vector<int> remove_node(unsigned long long& key, const int& node, const int& graph_size, vector< vector<int> >& graph, unordered_map<int, int>& legend, vector<int>& components)
{

    vector<int> nodes_placed = {}, immediate_neighbours = graph[components[node]];
    immediate_neighbours.erase(immediate_neighbours.begin());
    for (int i = 0; i < immediate_neighbours.size(); i ++){
        if(!inGraph(key, graph_size, legend[immediate_neighbours[i] - 1]))
            continue;
        nodes_placed.push_back(legend[immediate_neighbours[i] - 1]);
        key = updateKey(key, graph_size, legend[immediate_neighbours[i] - 1]);
    }
    key = updateKey(key, graph_size, node);
    return nodes_placed;
}

int remove_neighbours(unsigned long long& key, vector<int>& nodes_to_remove, unordered_map<unsigned long long, int>& memory, vector< vector<int> >& graph, unordered_map<int, int>& legend, vector<int>& components)
{
    int drones_placed = 0;
    while (nodes_to_remove.size() != 0){
        if (memory.find(key) != memory.end()){
            //cout << "key in memory" << endl;
            return drones_placed;
        }
        if (!inGraph(key, components.size(), nodes_to_remove[0])){
            //cout << "node " << components[nodes_to_remove[0]] << " is not in graph (remove_neighbours), key = "<< bitset<8>(key)<< endl;
            nodes_to_remove.erase(nodes_to_remove.begin());
            continue;
        }
        vector<int> neighbours = links(key, nodes_to_remove[0], graph, legend, components), list_drones_placed = remove_node(key, nodes_to_remove[0], components.size(), graph, legend, components);
        drones_placed++;
        //cout << "removing node "<< components[nodes_to_remove[0]] << " key after removal " << bitset<8>(key) << endl;
        nodes_to_remove.erase(nodes_to_remove.begin());
        for (int k = 0; k < neighbours.size(); k++){
            int node_degree = degree(key, neighbours[k], graph, legend, components);
            //cout << "checking node " << components[neighbours[k]] << " degree of this node = " << node_degree << endl;
            if ( node_degree<= 1){
                //cout << "adding node " << components[neighbours[k]] << endl;
                nodes_to_remove.push_back(neighbours[k]);
            }
        }
    }
    return drones_placed;

}

vector<int> links(const unsigned long long& key, int&node, vector< vector<int> >& graph, unordered_map<int, int>& legend, vector<int>& components)
{
    int graph_size = components.size();
    vector<int> links;
    vector<int> immediate_neighbours = graph[components[node]];
    immediate_neighbours.erase(immediate_neighbours.begin());
    //cout << "length of neighbours of node " << node << " is " << immediate_neighbours.size() << endl;
    for (int i = 0; i < immediate_neighbours.size(); i ++){
        int nodeN1 = immediate_neighbours[i] - 1;
        //cout << "cycling through nodeN1 = " << nodeN1 << endl;
        if (!inGraph(key, graph_size, legend[nodeN1])){
            continue;
        }
        for (int j = 1; j < graph[nodeN1].size(); j ++){
            int nodeN2 = graph[nodeN1][j] - 1;
            //cout << "cycling through nodeN1 = " << nodeN1 << endl;
            if (((nodeN2 == components[node] || !inGraph(key, graph_size, legend[nodeN2])) || find(links.begin(),links.end(),legend[nodeN2]) != links.end()) || find(immediate_neighbours.begin(),immediate_neighbours.end(),nodeN2 + 1) != immediate_neighbours.end()){
                continue;
            }
            //cout << "adding node " << nodeN2 << endl;
            links.push_back(legend[nodeN2]);
        }
    }
    return links;


}

int degree(const unsigned long long& key, const int& node,vector< vector<int> >& graph, unordered_map<int,int>& legend, vector<int>& components)
{
    int degree = 0, component_size = components.size();
    for (int i = 1; i < graph[components[node]].size(); i ++){
        int nodeN = legend[graph[components[node]][i] - 1] ;
        //cout << "before in-graph" << endl;
        if (inGraph(key, component_size, nodeN))
            degree++;
    }
    //cout << "degree of " << node << " is " << degree << endl;
    return degree;
}

bool inGraph(const unsigned long long& key, int graph_size, const int& node)
{
    return ((key >> (graph_size- node - 1)) & 1) == 1;
}

unsigned long long updateKey(const unsigned long long& key, unsigned long long graph_size, const unsigned long long& node)
{
    unsigned long long one = 1;
    return key ^ (one << (graph_size - node - one));
}

void print_vector(const vector< vector<int> >& v)
{
    for (int i = 0; i < v.size(); i++){
        for (int j = 0; j < v[i].size(); j++){
            cout << v[i][j] << ' ';
        }
        cout << endl;
    }
    return;
}


int max_drones(vector< vector<int> >& graph, unsigned long long key, int bound, int starting_point, unordered_map<unsigned long long, int>& memory, unordered_map<int, int>& legend, vector<int>& components)
{
    //cout << memory.size() << endl;
    //cout << "key = " << bitset<64>(key) << endl;
    //depth ++;
    if (memory.find(key) != memory.end())
    {
        return memory[key];
    }
    while (!inGraph(key, components.size(), starting_point))
        starting_point++;
    int node = starting_point, num1, num2;
    //cout << "startingPoint = " << starting_point << endl;
    vector<int> nodes_to_remove_when_node_not_picked, nodes_to_remove_when_node_picked;
    unsigned long long pick_node_key = key, node_not_picked_key = updateKey(key, components.size(), starting_point);
    remove_node(pick_node_key, node, components.size(), graph, legend, components);
    //cout << "pick node key = " << bitset<64>(pick_node_key) << '\n' << "node_not_picked_key = " << bitset<64>(node_not_picked_key) << endl;
    for (int i = 1; i < graph[components[node]].size(); i ++){
        if ((degree(node_not_picked_key, legend[graph[components[node]][i] - 1], graph, legend, components) <= 1) && (inGraph(key, components.size(), legend[graph[components[node]][i] - 1])))
        {
         nodes_to_remove_when_node_not_picked.push_back(legend[graph[components[node]][i] - 1]);
        }
    }

    num1 = remove_neighbours(node_not_picked_key, nodes_to_remove_when_node_not_picked, memory, graph, legend, components);
    //cout << "node not picked key = " << bitset<64>(node_not_picked_key) << endl;
        //cout << "key not remebered - num1" << endl;
    num1 += max_drones(graph, node_not_picked_key, bound + num1, starting_point + 1, memory, legend, components);
    vector<int> neighbours = links(key, node, graph, legend, components);
    for (int j = 1; j < neighbours.size(); j++){
        if (degree(pick_node_key, neighbours[j], graph, legend, components) <= 1)
            nodes_to_remove_when_node_picked.push_back(neighbours[j]);
    }
    num2 = remove_neighbours(pick_node_key, nodes_to_remove_when_node_picked, memory, graph, legend, components);
    //cout << "pick node key = " << pick_node_key << endl;
    //cout << "key not remebered - num2" << endl;
    num2 += max_drones(graph, pick_node_key, bound + 1 + num2, starting_point + 1, memory, legend, components) + 1;
    int num = max(num1,num2);
    //cout << "key = " << bitset<64>(key) << ", num = " << num << endl;
    memory[key] = num;
    return memory[key];
}


vector< vector<int> >& getinput(vector< vector<int> >& graph, int& drones_to_place)
{
    vector<int> temp;
    string tempr;
    int intersections, num;
    cin >> drones_to_place >> intersections;
    cin.ignore();
    for (int i = 0; i < intersections ; ++i){
        getline(cin, tempr);
        stringstream value;
        value << tempr;
        while(value >> num){
            temp.push_back(num);
        }
        graph.push_back(temp);
        temp.clear();
    }
    return graph;


}

vector< vector<int> > find_components(vector< vector<int> >& graph)
{
    int graph_size = graph.size(), total_length = 1, current_index = 0;
    vector< vector<int> > components;
    vector<int> nodes = {0}, previous_nodes = {0};
    vector<int> total_nodes;
    if (graph_size == 1){
        components.push_back(nodes);
        return components;
    }
    for (int i = 2; i <= graph_size; i ++ ){
        total_nodes.push_back(i);
    }
    while(total_length != graph_size){
        vector<int> immediate_neighbors = graph[nodes[current_index]];
        immediate_neighbors.erase(immediate_neighbors.begin());
        for (int i = 0; i < immediate_neighbors.size(); i++){
            if (find(nodes.begin(), nodes.end(), immediate_neighbors[i] - 1) == nodes.end()){
                nodes.push_back(immediate_neighbors[i] - 1);
                total_length++;
                total_nodes.erase(find(total_nodes.begin(), total_nodes.end(), immediate_neighbors[i]));
            }
        }
        /*for (int k = 0; k < nodes.size(); k ++)
            cout << nodes[k] << endl;*/
        if (nodes == previous_nodes && current_index == nodes.size() - 1){
            current_index = 0;
            components.push_back(nodes);
            nodes = {};
            nodes.push_back(total_nodes[0] - 1);
            total_nodes.erase(total_nodes.begin());
            total_length++;
            previous_nodes = nodes;
            continue;
        }
        current_index++;
        previous_nodes = nodes;
    }
    components.push_back(nodes);
    return components;
}
int main()
{
    vector< vector<int> > graph, components, nodes_placed;
    //graph = {{4,45,59,11,3}, {4,48,44,20,7}, {4,52,32,34,1}, {4,32,53,17,21}, {4,59,52,9,13}, {4,29,41,8,42}, {4,57,13,2,50}, {4,45,6,30,32}, {4,18,26,22,5}, {3,11,46,54}, {4,31,1,10,29}, {4,20,46,41,40}, {4,49,5,7,31}, {4,28,36,56,59}, {4,24,55,16,33}, {4,49,15,23,34}, {4,25,37,28,4}, {4,42,9,51,47}, {4,25,47,52,39}, {4,12,33,2,54}, {4,53,57,22,4}, {4,9,21,50,60}, {4,41,40,16,54}, {4,49,15,37,28}, {4,55,19,48,17}, {4,27,58,51,9}, {4,58,26,34,53}, {4,14,17,24,55}, {4,6,55,31,11}, {4,33,8,54,59}, {4,11,58,29,13}, {4,4,3,44,8}, {4,45,30,20,15}, {4,60,27,3,16}, {4,49,56,37,46}, {4,14,43,47,44}, {4,35,17,42,24}, {4,48,50,52,56}, {4,47,51,19,56}, {4,44,23,58,12}, {4,60,23,6,12}, {4,18,45,37,6}, {4,53,48,51,36}, {4,40,2,36,32}, {4,33,8,1,42}, {3,12,35,10}, {4,39,19,18,36}, {4,43,25,38,2}, {4,24,35,13,16}, {3,38,22,7}, {4,43,18,26,39}, {4,3,5,19,38}, {4,43,4,27,21}, {4,23,30,20,10}, {4,25,15,29,28}, {4,35,14,38,39}, {2,21,7}, {4,27,26,31,40}, {4,5,1,14,30}, {3,41,34,22}};
    vector< int > component;
    int drones_to_place, drones_placed = 0;
    graph = getinput(graph, drones_to_place);
    unsigned long long key;
    unordered_map<unsigned long long, int> memory;
    unordered_map<int,int> legend;
    //print_vector(components);
    if (graph.size() >= 75){
        cout << "possible" << endl;
        return 0;
    }
    components = find_components(graph);
    for (int i = 0; i < components.size(); i ++){
        legend.clear();
        if (components[i].size() >= 61){
            cout << "possible" << endl;
            return 0;
        }
        for (int j = 0; j < components[i].size(); j++){
            legend[components[i][j]] = j;
        }
        memory.erase(memory.begin(), memory.end());
        memory[0] = 0;
        key = llround(pow(2,components[i].size())) - 1;
        //cout << "key before prune = " << bitset<64>(key) << endl;
        drones_placed += prune(key, graph, memory, legend, components[i]);
        //cout << "key after prune = " << bitset<64>(key) << " drones_placed = " << drones_placed << endl;
        drones_placed += max_drones(graph, key , 0, 0, memory, legend, components[i]);
        //cout << drones_placed << endl;
    }
    if (drones_to_place <= drones_placed)
        cout << "possible" << endl;
    else
        cout << "impossible" << endl;
    //cout << "drones placed = " << drones_placed << endl;
    //cout << "memory size = " << memory.size() << endl;
    //cout << "recursion calls = " << depth << endl;
    return 0;
}