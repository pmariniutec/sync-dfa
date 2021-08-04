#include <string>
#include <vector>
#include <iomanip>
#include <algorithm>
#include <math.h>
#include <array>
#include <map>
#include <queue>
#include "utils.h"

class DFA {
 public:
  DFA() = default;

  void createState(std::string name) {
	states.push_back(new State(name));
  }

  void createTransition(std::string initialName, std::string finalName, int symbol) {
	State* initial = nullptr;
	State* end = nullptr;

	for (auto state : states) {
	  if (state->name == initialName) {
		initial = state;
	  }
	  if (state->name == finalName) {
		end = state;
	  }
	}

	if (!initial || !end) {
	  std::cout << "Error\n";
	  return;
	} else {
	  initial->addTransition(initial, end, symbol);
	}
  }

  // Now only generates singleton and pair states. (n * (n + 1)) / 2
  DFA* genPowerAutomata() {
	auto power = new DFA();
	uint32_t size = states.size() * (states.size() + 1) / 2;

	std::vector<State*> includes;

	for (auto state : states) {
	  power->states.push_back(state);
	  power->states.back()->states.push_back(state);
	}

	for (uint32_t i = 0; i < states.size(); ++i) {
	  for (uint32_t j = i + 1; j < states.size(); ++j) {
		includes.push_back(states[i]);
		includes.push_back(states[j]);

		std::string name;
		std::for_each(includes.begin(), includes.end(), [&](auto piece) { name += piece->name; });
		power->createState(name);

		std::copy(includes.begin(), includes.end(), std::back_inserter(power->states.back()->states));
		includes.clear();
	  }
	}

	std::vector<State*> point;
	bool match;
	int matches;

	for (auto state : power->states) {
	  if (state->states.size() < 2)
		continue;

	  for (auto symbol : language) {
		for (auto state : state->states) {
		  for (auto transition : state->transitions) {
			if (transition->symbol == symbol && std::find(point.begin(), point.end(), transition->end) == point.end()) {
			  point.push_back(transition->end);
			}
		  }
		}

		for (auto finalState : power->states) {
		  match = true;
		  matches = 0;
		  if (point.size() == finalState->states.size()) {
			for (auto state : point) {
			  for (auto finalIncludes : finalState->states) {
				if (state->name == finalIncludes->name) {
				  matches++;
				  break;
				}
			  }
			}
			if (matches != finalState->states.size()) {
			  match = false;
			}
		  } else {
			continue;
		  }

		  if (match) {
			state->addTransition(state, finalState, symbol);
		  }
		}
		point.clear();
	  }
	}

	return power;
  }

  bool bfs(State* starting) {
	std::map<std::string, bool> visited;
	for (auto state : states) {
	  visited.emplace(state->name, false);
	}

	std::queue<State*> q;
	auto curr = starting;
	q.push(curr);

	while (!q.empty()) {
	  curr = q.front();
	  q.pop();
	  if (!visited[curr->name]) {
		for (auto transition : curr->transitions) {
		  if (transition->end->states.size() == 1) {
			return true;
		  }

		  if (!visited[transition->end->name]) {
			q.push(transition->end);
		  }
		}
		visited[curr->name] = true;
	  }
	}

	return false;
  }

  DFA* bfsForest() {
	auto root = states.back();

	auto bfs = new DFA();

	std::map<std::string, bool> visited;
	for (auto state : states) {
	  visited.emplace(state->name, false);
	}

	std::queue<State*> q;
	auto curr = root;
	q.push(curr);

	while (!q.empty()) {
	  curr = q.front();
	  q.pop();
	  if (!visited[curr->name]) {
		bool exists = false;
		for (auto state : bfs->states) {
		  if (state->name == curr->name)
			exists = true;
		}
		if (!exists && curr->states.size() == 2)
		  bfs->createState(curr->name);

		for (auto transition : curr->transitions) {
		  if (!visited[transition->end->name]) {
			q.push(transition->end);
			bool exists = false;
			for (auto state : bfs->states) {
			  if (state->name == transition->end->name) {
				exists = true;
			  }
			}
			if (!exists && curr->states.size() == 2) {
			  bfs->createState(transition->end->name);
			  bfs->createTransition(curr->name, transition->end->name, transition->symbol);
			}
		  }
		}

		visited[curr->name] = true;
	  }
	}

	return bfs;
  }

  std::string synchronizingWord(DFA* base) {
	std::string syncWord;

	auto bfs = bfsForest();

	for (auto state : bfs->states) {
	  for (auto transition : state->transitions) {
		auto tPrime = std::to_string(transition->symbol);
		syncWord += tPrime;
	  }
	}

	return syncWord;
  }

  bool hasSynchronizingWord() {
	for (auto state : states) {
	  if (state->states.size() == 2) {
		if (!bfs(state))
		  return false;
	  }
	}
	return true;
  }

  friend std::ostream& operator<<(std::ostream& out, DFA& dfa) {
	out << std::setw(5) << "State\t"
		<< "|"
		<< std::setw(5) << "\tTransition\n";

	for (auto state : dfa.states) {
	  out << std::setw(5) << state->name << "\t|\t";
	  for (auto transition : state->transitions) {
		out << std::setw(5) << transition->symbol
			<< " -> "
			<< transition->end->name << ' ';
	  }
	  out << '\n';
	}
	return out;
  }

 private:
  std::vector<State*> states;
  std::vector<Transition*> transitions;
  static constexpr std::array<int, 2> language = { 0, 1 };
};
