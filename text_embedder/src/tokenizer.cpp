#include <utf8proc.h>
#include <unordered_set>
#include "../include/tokenizer.hpp"

const std::unordered_set<std::string> STOPWORDS = {
  "de","a","o","que","e","do","da","em","um","para","e","com","nao","uma","os","no","se","na","por","mais","as","dos","como",
  "mas","foi","ao","ele","das","tem","a","seu","sua","ou","ser","quando","muito","ha","nos","ja","esta","eu","tambem","so",
  "pelo","pela","ate","isso","ela","entre","era","depois","sem","mesmo","aos","ter","seus","quem","nas","me","esse","eles",
  "estao","voce","tinha","foram","essa","num","nem","suas","meu","as","minha","tem","numa","pelos","elas","havia","seja",
  "qual","sera","nos","tenho","lhe","deles","essas","esses","pelas","este","fosse","dele","tu","te","voces","vos",
  "lhes","meus","minhas","teu","tua","teus","tuas","nosso","nossa","nossos","nossas","dela","delas","esta","estes",
  "estas","aquele","aquela","aqueles","aquelas","isto","aquilo","estou","esta","estamos","estao","estive","esteve",
  "estivemos","estiveram","estava","estavamos","estavam","estivera","estiveramos","esteja","estejamos","estejam","estivesse",
  "estivessemos","estivessem","estiver","estivermos","estiverem","hei","ha","havemos","hao","houve","houvemos","houveram",
  "houvera","houveramos","haja","hajamos","hajam","houvesse","houvessemos","houvessem","houver","houvermos","houverem",
  "houverei","houvera","houveremos","houverao","houveria","houveriamos","houveriam","sou","somos","sao","era","eramos","eram",
  "fui","foi","fomos","foram","fora","foramos","seja","sejamos","sejam","fosse","fossemos","fossem","for","formos","forem",
  "serei","sera","seremos","serao","seria","seriamos","seriam","tenho","tem","temos","tem","tinha","tinhamos","tinham","tive",
  "teve","tivemos","tiveram","tivera","tiveramos","tenha","tenhamos","tenham","tivesse","tivessemos","tivessem","tiver",
  "tivermos","tiverem","terei","tera","teremos","terao","teria","teriamos","teriam"
};

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
      // filtro de stopwords
      if (STOPWORDS.find(word) == STOPWORDS.end()) {
        tokens.push_back(word); 
      }
      word.clear();
    }
  }

  if (!word.empty()) {
  if (STOPWORDS.find(word) == STOPWORDS.end()) {
      tokens.push_back(word); 
    }
  }

  return tokens;
}
