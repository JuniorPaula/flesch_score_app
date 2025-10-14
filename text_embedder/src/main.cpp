#include <iostream>
#include "../include/tokenizer.hpp"
#include "../include/tfidf.hpp"

int main()
{
  std::string filename = "corpus/corpus_test.json";
  std::vector<LabeledDoc> corpus = load_corpus_from_json(filename);

  if (corpus.empty()) {
    std::cerr << "Error: Corpus empty or not be loaded!" << std::endl;
  }

  for (auto& doc : corpus) {
    doc.tokens = tokenizer(doc.text);
  }

  std::vector<std::vector<std::string>> all_tokens;
  for (auto& d : corpus) all_tokens.push_back(d.tokens);

  Vocab vocab = build_vocab(all_tokens);
  auto idf = computerIDF(all_tokens);

  for (auto& doc : corpus) {
    doc.vector = vectorize(doc.tokens, vocab, idf);
  }

  std::string input = "Dia 14 de outubro haverá uma queima de estoque total.";
  auto similar = get_most_similar(input, corpus, vocab, idf);

  std::cout << "Analized text: " << input << "\n";
  std::cout << "Most similar: " << similar.label << "\n";
  
  return 0;
}
