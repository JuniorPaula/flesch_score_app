#include <mutex>
#include <cstring>
#include <string>
#include "../include/text_embedder.h"
#include "../include/tfidf.hpp"

// Global corpus
static std::vector<LabeledDoc> corpus;
static std::unordered_map<std::string, float> idf;
static Vocab vocab;
static std::once_flag initialized; 

// Inicializar o corpus e IDF com base no arquivo
void init_text_embedder(const char* path) {
  std::call_once(initialized, [path]() {
    std::string p = path;
    corpus = load_corpus_from_json(p);

    std::vector<std::vector<std::string>> all_tokens;
    for (auto& doc : corpus) {
      all_tokens.push_back(doc.tokens);
    }

    vocab = build_vocab(all_tokens);
    idf = computerIDF(all_tokens);

    // Calcular e armazenar os vetores de TF-IDF de cada doc no corpus
    for (auto& doc : corpus) {
      doc.vector = vectorize(doc.tokens, vocab, idf);
    }
  });
}

// Retornar o insight
const char* generate_insight(const char* text) {
  std::string input = text;

  LabeledDoc similar = get_most_similar(input, corpus, vocab, idf);

  // Alocar e retornar o insight
  std::string resp = similar.label.empty() ? "Sem correspondência" : similar.label;

  // usar strdup para deixar que o Go (via C.CString/C.free) ou quem chamar essa função, libere a memória alocada.
  return strdup(resp.c_str());
}

