#include <unordered_set>
#include <unordered_map>
#include <cmath>
#include "../include/tfidf.hpp"

Vocab build_vocab(const std::vector<std::vector<std::string>>& docs) {
  std::unordered_set<std::string> set;

  for (const auto& doc : docs) {
    for (const auto& t : doc) {
      set.insert(t);
    }
  }

  return Vocab(set.begin(), set.end());
}

std::unordered_map<std::string, float> computerIDF(const std::vector<std::vector<std::string>>& docs) {
  std::unordered_map<std::string, float> idf;
  if (docs.empty()) return idf;

  std::unordered_map<std::string, int> df;
  int N = static_cast<int>(docs.size());

  for (const auto& doc : docs) {
    std::unordered_set<std::string> seen;
    for (const auto& tok : doc) seen.insert(tok);
    for (const auto& tok : seen) df[tok] += 1; 
  }

  // Fórmula do IDF
  // IDF(t) = log(1+n / 1+df(t)) + 1
  for (const auto& [term, d] : df) {
    idf[term] = std::log(1.0f + (static_cast<float>(N) / (1.0f + static_cast<float>(d))));
  }

  return idf;
}
