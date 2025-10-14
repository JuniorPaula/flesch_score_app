#include <unordered_set>
#include <unordered_map>
#include <cmath>
#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>
#include "../include/tfidf.hpp"
#include "../include/tokenizer.hpp"

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

std::unordered_map<std::string, float> computerTF(const std::vector<std::string>& tokens) {
  std::unordered_map<std::string, float> tf;
  if (tokens.empty()) return tf;

  for (const auto& t : tokens) {
    tf[t] += 1.0f;
  }

  float N = static_cast<float>(tokens.size());

  for (auto& [ k, v ] : tf) {
    v /= N; // frequência relativa
  }

  return tf;
}

std::vector<float> vectorize(const std::vector<std::string>& tokens,
                            const Vocab& vocab,
                            const std::unordered_map<std::string, float>& idf) {

  auto tf = computerTF(tokens);
  std::vector<float> vec;
  vec.reserve(vocab.size());                              

  for (const auto& term : vocab) {
    float w_tf = tf.count(term) ? tf[term] : 0.0f;
    float w_idf = idf.count(term) ? idf.at(term) : 0.0f;
    vec.push_back(w_tf * w_idf);
  }
  return vec;
}

static inline float dot(const std::vector<float>& a, const std::vector<float>& b) {
  float s = 0.0f;
  size_t n = std::min(a.size(), b.size());

  for (size_t i = 0; i < n; ++i) {
    s += a[i] * b[i];
  }
  return s;
}

static inline float norm(const std::vector<float>& a) {
  return std::sqrt(dot(a, a)) + 1e-8f; // evitar div/0
}

float cosine(const std::vector<float>& a, const std::vector<float>& b) {
  return dot(a, b) / (norm(a) * norm(b));
}

void print_vector(const std::vector<float>& vec, const Vocab& vocab) {
  std::cout << "\nTF-IDF Vector:\n";
  for (size_t i = 0; i < vec.size(); ++i) {
    std::cout << " - " << vocab[i] << ": " << vec[i] << std::endl;
  }
}

LabeledDoc get_most_similar(const std::string& input,
                            const std::vector<LabeledDoc>& corpus,
                            const Vocab vocab,
                            const std::unordered_map<std::string, float>& idf) {
                              
  LabeledDoc result;
  result.label = "";
  result.text = "";
  float best = -1.0f;

  auto tokens = tokenizer(input);
  auto vec = vectorize(tokens, vocab, idf);

  for (const auto& doc : corpus) {
    float sim = cosine(vec, doc.vector);
    if (sim > best) {
      best = sim;
      result = doc;
    }
  }
  return result;
}

std::vector<LabeledDoc> load_corpus_from_json(std::string& filename) {
  std::vector<LabeledDoc> corpus;
  std::ifstream in(filename);
  if (!in.is_open()) {
    std::cerr << "Error on load corpus from file: " << filename << std::endl;
    return corpus;
  }

  nlohmann::json_abi_v3_11_3::json j;
  in >> j;

  for (const auto& item : j) {
    LabeledDoc d;
    d.label = item["label"];
    d.text = item["text"];
    d.tokens = tokenizer(d.text);
    corpus.push_back(std::move(d));
  }

  return corpus;
}
