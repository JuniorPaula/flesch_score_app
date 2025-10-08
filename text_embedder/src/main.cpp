#include <iostream>
#include "../include/tokenizer.hpp"
#include "../include/tfidf.hpp"

int main()
{
  std::vector<LabeledDoc> corpus = {
    {"cientifico", "A estrutura do  DNA foi analizada...", {}, {}},
    {"publicitario", "Você não vai acreditar nessa oferta!", {}, {}},
    {"publicitario", "Só hoje um preco imperdível!", {}, {}},
    {"publicitario", "Queima de estoque! Venha conferir", {}, {}},
    {"infantil", "Era uma vez um coelho branco.", {}, {}}
  };

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

  std::string input = "hoje o gerente enlouqueceu, a oferta está imperdivel!";
  auto similar = get_most_similar(input, corpus, vocab, idf);

  std::cout << "Analized text: " << input << "\n";
  std::cout << "Most similar: " << similar.label << "\n";
  
  return 0;
}
