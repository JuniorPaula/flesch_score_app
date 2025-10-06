#include <iostream>
#include "../include/tokenizer.hpp"

int main()
{
  std::string input = "Olá mundo! este é o teste para a função tokenizer.";

  auto tokens = tokenizer(input);

  std::cout << "Tokens:\n";
  for (const auto& t : tokens) {
    std::cout << " - " << t << std::endl;
  }
  
  return 0;
}
