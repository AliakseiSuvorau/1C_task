#include <iostream>
#include <vector>
#include <unordered_map>
#include <set>

class Graph {
  struct Edge {
    size_t to = 0;
  };

  size_t num_of_vertexes_ = 1e4;
  size_t num_of_edges_ = 0;
  std::vector<std::pair<bool, std::vector<Edge>>> adj_list;
  std::set<size_t> infected_;
  std::unordered_map<size_t, size_t> painted_;  // vertex -> its color
  std::unordered_map<size_t, size_t> colors_;  // color -> vertexes

 private:
  void EnterGraph();
  void ProcessLeqOneEdge();
  void Infect(size_t index, bool check = false);
  void PrintInfected();
  bool IsInfected(size_t index, bool dont_check_neigh = false);
  void Paint();
  void InfectMinimumColorSum();
  void HealUnnecessary();
  void Heal(size_t index);

 public:
  Graph() { EnterGraph(); };
  void FindInfected();
  void PrintGraph();
};

void Graph::EnterGraph() {
  std::cin >> num_of_edges_;
  adj_list.resize(num_of_vertexes_);
  size_t max_num_v = 0;
  for (size_t i = 0; i < num_of_edges_; ++i) {
    size_t from = 0;
    size_t to = 0;
    std::cin >> from >> to;
    max_num_v = std::max(std::max(from, to),
                         max_num_v);  // Calc actual number of cities
    // Append Both edges since the graph is undirected
    Edge edge;
    edge.to = to - 1;
    adj_list[from - 1].second.push_back(edge);
    edge.to = from - 1;
    adj_list[to - 1].second.push_back(edge);
  }
  num_of_vertexes_ = max_num_v;
  adj_list.resize(num_of_vertexes_);
}

void Graph::ProcessLeqOneEdge() {
  for (size_t i = 0; i < adj_list.size(); ++i) {
    if (adj_list[i].second.size() <= 1) {
      Infect(i);
    }
  }
}

void Graph::Infect(size_t index, bool check) {
  infected_.insert(index);
  if (check && adj_list[index].second.size() > 1 || !check) {
    adj_list[index].first = true;
  }
}

void Graph::FindInfected() {
  ProcessLeqOneEdge();
  Paint();
  InfectMinimumColorSum();
  HealUnnecessary();
  PrintInfected();
}

void Graph::PrintInfected() {
  std::cout << infected_.size() << "\n";
  for (auto& v : infected_) {
    std::cout << v + 1 << " ";
  }
}

void Graph::PrintGraph() {
  std::cout << "V: " << num_of_vertexes_ << "\nE: " << num_of_edges_ << "\n";
  for (size_t i = 0; i < num_of_vertexes_; ++i) {
    std::cout << i + 1 << ": ";
    for (auto& j : adj_list[i].second) {
      std::cout << j.to + 1 << " ";
    }
    std::cout << "\n";
  }
}

bool Graph::IsInfected(size_t index, bool dont_check_neigh) {
  if (adj_list[index].first) {
    return true;
  }
  if (dont_check_neigh) {
    return false;
  }
  size_t num_of_inf_neigh = 0;
  for (size_t i = 0; i < adj_list[index].second.size(); ++i) {
    if (adj_list[adj_list[index].second[i].to].first) {
      ++num_of_inf_neigh;
    }
  }
  return num_of_inf_neigh > 1;
}

void Graph::Paint() {
  for (size_t v = 0; v < adj_list.size(); ++v) {
    // Colors, which have already been taken by the neighbors
    std::set<size_t> taken;
    for (auto& neighbor : adj_list[v].second) {
      if (painted_[neighbor.to]) {
        taken.insert(painted_[neighbor.to]);
      }
    }
    // Let's take a not taken color with minimal index
    // in a set all elements are sorted, so:
    size_t new_color = 1;
    for (auto& c : taken) {
      if (c != new_color) {
        break;
      }
      ++new_color;
    }
    painted_[v] = new_color;
    ++colors_[new_color];
  }
}

void Graph::InfectMinimumColorSum() {
  // To find (n-1) colors with minimum vertexes,
  // let's find 1 color with maximum vertexes
  size_t max_color = (*std::max_element(colors_.begin(), colors_.end(),
                                         [](const std::pair<size_t, size_t>& a,
                                            const std::pair<size_t,
                                                            size_t>& b) {
                                           return a.second < b.second;
                                         })).first;
  // Infect
  for (size_t i = 0; i < adj_list.size(); ++i) {
    if (painted_[i] != max_color) {
      Infect(i, true);
    }
  }
}

void Graph::HealUnnecessary() {
  for (size_t i = 0; i < adj_list.size(); ++i) {
    if (!adj_list[i].first) {
      continue;
    }
    // If the vertex is infected
    size_t num_of_infected = 0;
    for (size_t j = 0; j < adj_list[i].second.size(); ++j) {
      if (adj_list[adj_list[i].second[j].to].first) {
        ++num_of_infected;
      }
    }
    if (num_of_infected > 1) {
      Heal(i);
    }
  }
}

void Graph::Heal(size_t index) {
  adj_list[index].first = false;
  auto it = find(infected_.begin(), infected_.end(), index);
  infected_.erase(it);
}

int main() {
  Graph g;
  g.FindInfected();
  return 0;
}
