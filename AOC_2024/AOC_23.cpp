#include "utils.hpp"

typedef std::map<std::string, std::vector<std::string>> graph_map;

typedef std::set<std::string> strset;
void BronKerbosch(const graph_map& graph, strset& R, strset& P, strset& X, strset& maxR){
    if(P.empty() && X.empty()) {
        if(R.size() > maxR.size()){
            maxR = R;
        }
        return;
    }
    auto copy_P = P;
    for(auto& v: copy_P){
        strset newP, newX;
        auto newR = R;
        newR.emplace(v);
        for(auto& nb: graph.at(v)){
            if(P.contains(nb)) newP.emplace(nb);
            if(X.contains(nb)) newX.emplace(nb);
        }
        BronKerbosch(graph, newR, newP, newX, maxR);
        P.erase(v);
        X.emplace(v);
    }
}

std::pair<uint64_t, std::string> solve_puzzle(std::string filename){
    std::pair<uint64_t, std::string> res{0, ""};

    auto data = read_file_as_lines(filename);

    graph_map graph;
    std::set<std::string> allnodes;

    for(auto& line: data){
        auto node = line.substr(0, 2);
        auto tgt = line.substr(3, 2);
        allnodes.emplace(node);
        allnodes.emplace(tgt);
        if(!graph.try_emplace(node, std::vector<std::string>{tgt}).second){
            graph[node].push_back(tgt);
        }
        if(!graph.try_emplace(tgt, std::vector<std::string>{node}).second){
            graph[tgt].push_back(node);
        }
    }
    std::vector<std::vector<std::string>> triangles;
    for(auto& n: allnodes){
        //pairwise iterate over all nodes connected to n, check if they connect
        for(auto it = graph.at(n).begin(); it != graph.at(n).end() - 1; it++){
            for(auto it2 = it+1; it2 != graph.at(n).end(); it2++){
                if(std::find(graph.at(*it).begin(), graph.at(*it).end(), *it2) != graph.at(*it).end()){
                    triangles.push_back(std::vector<std::string>{n, *it, *it2});
                }
            }
        }
    }

    for(auto& tr: triangles){
        if(std::any_of(tr.begin(), tr.end(),[](std::string i){return i[0] == 't';})) res.first++;
    }
    res.first/=3;
    strset R{}, X{}, maxR{};
    BronKerbosch(graph, R, allnodes, X, maxR);
    for(auto& s: maxR){
        res.second += s + ",";
    }
    return res;
}

int main(){
    std::pair<uint64_t, std::string> res = solve_puzzle("inputs/Test_23.txt");
    print("Test res pt 1:", res.first, "pt 2:", res.second);
    double time_spent;
    res = profile_function(solve_puzzle, time_spent, "inputs/Data_23.txt");
    print("Puzzle res pt 1:", res.first, "pt 2:", res.second, "puzzle calculation took:", time_spent, "ms");

    return 0;
}