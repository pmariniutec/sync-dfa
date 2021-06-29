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

  void createState(int name) {
	// std::cout << "Creating new state: " << name << '\n';
	states.push_back(new State(name));
  }

  void createTransition(int initialName, int finalName, int symbol) {
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

  DFA* genPowerAutomata(int op) {
	auto power = new DFA();
	uint32_t size;
	if (op <= 5)
	  size = std::pow(states.size(), 2);
	else
	  size = std::pow(2, states.size());

	std::vector<State*> includes;

	for (auto state : states) {
	  power->states.push_back(state);
	  power->states.back()->states.push_back(state);
	}

	for (uint32_t i = 0; i < size; ++i) {
	  for (uint32_t j = 0; j < states.size(); ++j) {
		if (i & (1 << j)) {
		  includes.push_back(states[j]);
		}
	  }
	  if (includes.size() < 2) {
		includes.clear();
		continue;
	  }
	  power->createState(i + states.size());
	  std::copy(includes.begin(), includes.end(), std::back_inserter(power->states.back()->states));
	  includes.clear();
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

  DFA* bfs(int op) {
	State* root;
	DFA* powerDFA = genPowerAutomata(op);
	for (auto state : states) {
	  root = state;
	}

	auto bfs = new DFA();
	auto size = powerDFA->states.size();

	std::map<int, bool> visited;
	for (auto state : powerDFA->states) {
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
		  if (state->name == curr->name) {
			exists = true;
		  }
		}
		if (!exists) {
		  bfs->createState(curr->name);
		}
		visited[curr->name] = true;

		for (auto transition : curr->transitions) {
		  if (!visited[transition->end->name]) {
			q.push(transition->end);
			bool exists = false;
			for (auto state : bfs->states) {
			  if (state->name == transition->end->name) {
				exists = true;
			  }
			}
			if (!exists) {
			  bfs->createState(transition->end->name);
			  bfs->createTransition(curr->name, transition->end->name, transition->symbol);
			}
		  }
		}
	  }
	}
	return bfs;
  }

  bool hasSingletons(State* singleton) {
	for (auto state : states) {
	  if (state->name == singleton->name && state->states.size() == 2) {
		return true;
	  }
	}
	return false;
  }

  std::string synchronizingWord(DFA* bfs) {
	std::string syncWord;

	for (auto state : bfs->states) {
	  for (auto transition : state->transitions) {
		if (state->transitions.size() == 1) {
		  if (!hasSingletons(state)) {
			syncWord = syncWord + std::to_string(transition->symbol);
		  }
		}
	  }
	}
	return syncWord;
  }

  bool hasSynchronizingWord() {
	for (auto state : states) {
	  if (state->states.size() == 3) {
		for (auto transition : state->transitions) {
		  if (transition->end->states.size() == 2) {
			return true;
		  }
		}
	  }
	}
	return false;
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
