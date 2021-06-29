#include <iostream>
#include "dfa.h"

int main() {
  int numStates, numFinalStates, initialState;
  std::cin >> numStates >> initialState >> numFinalStates;

  for (int i = 0; i < numFinalStates; ++i) {
	int tmp;
	std::cin >> tmp;
  }

  DFA* dfa = new DFA();

  // Create states
  for (int i = 0; i < numStates; ++i) {
	dfa->createState(i);
  }

  int p, a, q;
  for (int i = 0; i < numStates * 2; ++i) {
	std::cin >> p >> a >> q;
	dfa->createTransition(p, q, a);
  }
  // std::cout << *dfa;

  auto powerDFA = dfa->genPowerAutomata(numStates);

  // std::cout << *powerDFA;

  auto hasSyncWord = powerDFA->hasSynchronizingWord();
  std::cout << "Has sync word: " << (hasSyncWord ? "Yes" : "No") << '\n';
  if (hasSyncWord) {
	auto bfs = powerDFA->bfs(numStates);
	auto syncWord = powerDFA->synchronizingWord(bfs);
	std::cout << "Sync word: " << syncWord << '\n';
  }
}
