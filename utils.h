#include <iostream>
#include <vector>

class State;

class Transition {
 public:
  State* start;
  State* end;
  int symbol;

  Transition() = default;
  Transition(State* s, State* e, int value) : start{ s }, end{ e }, symbol{ value } {}
};

class State {
 public:
  int name;
  std::vector<Transition*> transitions;
  std::vector<State*> states;

  State() = default;
  State(int n) : name{ n } {}

  void addTransition(State* start, State* end, int symbol) {
	transitions.push_back(new Transition(start, end, symbol));
  }
};
