#include <iostream>
#include <string>
#include "dfa.h"

DFA* genRandomDFA(int numStates) {
  auto r = new DFA();
  for (int i = 1; i <= numStates; i++) {
	r->createState(std::to_string(i));
  }

  for (int i = 1; i <= numStates; i++) {
	auto newNumber = std::to_string(rand() % numStates + 1);
	r->createTransition(std::to_string(i), newNumber, 0);
  }
  for (int i = 1; i <= numStates; i++) {
	auto newNumber = std::to_string(rand() % numStates + 1);
	r->createTransition(std::to_string(i), newNumber, 1);
  }
  return r;
}

DFA* manualTest() {
  int numStates, numFinalStates, initialState;
  std::cin >> numStates >> initialState >> numFinalStates;

  for (int i = 0; i < numFinalStates; ++i) {
	int tmp;
	std::cin >> tmp;
  }

  DFA* dfa = new DFA();

  for (int i = 0; i < numStates; ++i) {
	dfa->createState(std::to_string(i));
  }

  int p, a, q;
  for (int i = 0; i < numStates * 2; ++i) {
	std::cin >> p >> a >> q;
	dfa->createTransition(std::to_string(p), std::to_string(q), a);
  }

  return dfa;
}

int main() {
  int op;

  DFA* dfa;

  while (true) {
	std::cout << "0 for manual testing. 1 for random testing. -1 for quit\n";
	std::cin >> op;
	if (op == 0) {
	  dfa = manualTest();
	} else if (op == 1) {
	  int numStates;
	  std::cout << "Number of states: ";
	  std::cin >> numStates;
	  dfa = genRandomDFA(numStates);
	} else if (op == -1) {
	  return 0;
	}

	auto powerDFA = dfa->genPowerAutomata();

	auto hasSynchronizingWord = powerDFA->hasSynchronizingWord();
	std::cout << (hasSynchronizingWord ? "Si" : "No") << '\n';

	if (hasSynchronizingWord) {
	  auto word = powerDFA->synchronizingWord(dfa);
	  std::cout << "Synchronizing Word: " << word << '\n';
	  std::cout << "Size: " << word.size() << '\n';
	}
  }
}
