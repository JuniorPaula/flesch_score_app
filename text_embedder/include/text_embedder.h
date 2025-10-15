#ifndef EMBEDDER_H
#define EMBEDDER_H

#ifdef __cplusplus
extern "C" {
#endif 

// Inicializar o corpus e IDF com base no arquivo
void init_text_embedder(const char* path);

// Retornar o insight
const char* generate_insight(const char* input);

#ifdef __cplusplus
}
#endif

#endif // EMBEDDER_H
