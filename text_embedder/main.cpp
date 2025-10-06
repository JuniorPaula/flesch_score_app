#include <iostream>
#include <vector>
#include <utf8proc.h>

std::string normalize(const std::string& input)
{
  std::string out;

  // Decompẽm para NFD (base + cobining marks)
  utf8proc_uint8_t* nfd = utf8proc_NFD(reinterpret_cast<const utf8proc_uint8_t*>(input.c_str()));
  if (!nfd) return input;

  // Itera corretamente avançando pelo número de bytes de cada code point
  for (size_t i = 0; nfd[i] != 0; ) {
    utf8proc_int32_t cp;
    utf8proc_ssize_t len = utf8proc_iterate(nfd + i, -1, &cp);

    if (len < 0) { // byte inválido; pula 1 byte para evitar loop infinito
      i++;
      continue;
    }

    i += static_cast<size_t>(len);

    // descartar marcas de acentuação
    if (utf8proc_category(cp) == UTF8PROC_CATEGORY_MN) continue;

    // lower-case
    cp = utf8proc_tolower(cp);

    // 
    char buff[4];
    int outlen = utf8proc_encode_char(cp, reinterpret_cast<utf8proc_uint8_t*>(buff));
    if (outlen > 0) out.append(buff, outlen);
  }

  free(nfd);
  return out;
}

std::vector<std::string> tokenizer(const std::string& text)
{
  std::vector<std::string> tokens;
  std::string word;

  std::string text_norm = normalize(text);
  for (char ch : text_norm) {
    if (std::isalnum(static_cast<unsigned char>(ch))) {
      word += ch; // já está normalizado
    } else if (!word.empty()) {
      tokens.push_back(word);
      word.clear();
    }
  }

  if (!word.empty()) {
    tokens.push_back(word);
  }

  return tokens;
}

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
