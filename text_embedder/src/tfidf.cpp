#include <unordered_set>
#include "../include/tfidf.hpp"

Vocab build_vocab(const std::vector<std::vector<std::string>> docs) {
  std::unordered_set<std::string> set;

  for (const auto& doc : docs) {
    for (const auto& t : doc) {
      set.insert(t);
    }
  }

  return Vocab(set.begin(), set.end());
}
