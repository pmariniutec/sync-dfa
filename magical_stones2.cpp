#include <iostream>
#include <algorithm>
#include <queue>
#include <string>
#include <utility>
#include <vector>

int main() {
  int t;
  std::cin >> t;

  while (t--) {
	int n;

	std::vector<int> w;
	std::vector<int> b;

	std::vector<std::vector<int>> rW(n);
	std::vector<std::vector<int>> rB(n);

	std::cin >> n;

	for (int i = 0; i < n; ++i) {
	  int tmp;
	  std::cin >> tmp;
	  // start at 0
	  w.push_back(tmp - 1);
	}
	for (int i = 0; i < n; ++i) {
	  int tmp;
	  std::cin >> tmp;
	  // start at 0
	  b.push_back(tmp - 1);
	}

	for (int i = 0; i < n; ++i) {
	  rW[w[i]].push_back(i);
	  rB[b[i]].push_back(i);
	}

	std::vector<std::vector<bool>> found(n, std::vector<bool>(n, false));
	std::vector<std::vector<std::string>> seq(n, std::vector<std::string>(n, ""));
	std::queue<std::pair<int, int>> q;

	for (int i = 0; i < n; ++i) {
	  found[i][i] = true;
	  q.push(std::make_pair(i, i));
	}

	while (!q.empty()) {
	  auto [c, d] = q.front();
	  q.pop();
	  for (auto wc : rW[c]) {
		for (auto wd : rW[d]) {
		  if (!found[wc][wd]) {
			found[wc][wd] = true;
			seq[wc][wd] = "W" + seq[c][d];
			q.push(std::make_pair(wc, wd));
		  }
		}
	  }
	  for (auto bc : rB[c]) {
		for (auto bd : rB[d]) {
		  if (!found[bc][bd]) {
			found[bc][bd] = true;
			seq[bc][bd] = "B" + seq[c][d];
			q.push(std::make_pair(bc, bd));
		  }
		}
	  }
	}

	auto allTrue = std::all_of(found.begin(), found.end(), [](std::vector<bool>& t) {
	  return std::all_of(t.begin(), t.end(), [](bool b) { return b; });
	});

	if (allTrue) {
	  std::vector<bool> stones(n, true);

	  // lambdas to capture local variables easily
	  auto blackSpell = [&]() {
		std::vector<bool> tmp(n);
		for (int i = 0; i < n; ++i) {
		  if (stones[i])
			tmp[b[i]] = true;
		}
		return tmp;
	  };

	  auto whiteSpell = [&]() {
		std::vector<bool> tmp(n);
		for (int i = 0; i < n; ++i) {
		  if (stones[i])
			tmp[w[i]] = true;
		}
		return tmp;
	  };

	  auto findMatching = [&]() {
		int first = -1;
		for (int i = 0; i < n; ++i) {
		  if (stones[i]) {
			if (first == -1)
			  first = i;
			else
			  return std::make_pair(first, i);
		  }
		}
		return std::make_pair(-2, -2);
	  };

	  auto p = findMatching();
	  std::string answer;
	  while (p.first != -2 && p.second != -2) {
		for (auto c : seq[p.first][p.second]) {
		  answer.push_back(c);
		  stones = c == 'W' ? whiteSpell() : blackSpell();
		}
		p = findMatching();
	  }
	  std::cout << answer << '\n';
	} else {
	  std::cout << "impossible\n";
	}
  }
}
