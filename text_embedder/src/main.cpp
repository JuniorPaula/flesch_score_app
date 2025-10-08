#include <iostream>
#include "../include/tokenizer.hpp"
#include "../include/tfidf.hpp"

int main()
{
  std::string t1 = "O carro está na garagem.";
  std::string t2 = "O automóvel está na garagem.";
  std::string t3 = "A comida esta na mesa";

  auto tok1 = tokenizer(t1);
  auto tok2 = tokenizer(t2);
  auto tok3 = tokenizer(t3);

  std::vector<std::vector<std::string>> docs = { tok1, tok2, tok3 };
  auto vocab = build_vocab(docs);

  for (const auto& t : vocab) {
    std::cout << " - " << t << std::endl;
  }

  std::cout << "----------------\n";

  auto idf = computerIDF(docs);

  for (auto [k, v] : idf) {
    std::cout << " Key -> " << k << "\t | frequency -> " << v << std::endl;
  }
  
  return 0;
}
