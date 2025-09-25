# Fesch Score App -  Readability Analyzer

Este projeto é um sistema híbrido de análise de legibilidade textual com capacidade de geração de insights com base no nível de dificuldade da leitura. Ele combina múltiplas linguagens (Go, Rust e C) em um pipeline interoperável de alta performance, ideal para sistemas embarcados, interfaces gráficas, CLI e WebAssembly.

## Principais Funcionalidades

- **Cálculo do Flesch Reading Ease Score** via biblioteca escrita em Rust.
- **Classificação de legibilidade** com base em faixas: `muito_fácil`, `fácil`, `média`, `difícil`, `muito_difícil`.
- **Geração de insights** com base no resutado do flesch score do texto.
- **Interoperabilidade**: integração Rust → C → Go para máxima performance e flexibilidade.
- **UI simples e eficaz** escrita em Go, com capacidade de carregar arquivos e processar texto em tempo real.

## Como Funciona

O sistema possui dois módulos principais:

1. **Flesch Score Engine (`flesch_score` - Rust)**:
   - Expõe uma função C ABI (`get_text_stats`) para consumo externo.
   - Calcula: número de palavras, sentenças, sílabas e score.

2. **UI (`User Interface` - Golang)**:
   - Carrega arquivo de texto e processa em tempo real.
   - Retorna o calculo de legibilidade do texto com insight com base na métrica obtida.

## Estrutura do Projeto

```
flesch_score/
├── app/									# UI em golang
├── cli/               		# CLI em C
├── readability/          # Enginedo calculo de flesch score em Rust
├── README.md             # Este documento
```

## Como Rodar

### Requisitos

- Go >= 1.21
- Rust (com cargo) >= 1.70
- Linux (preferencialmente)

### Compilação

1. Compile a biblioteca Rust:

```bash
cd flesch_score/readability
cargo build --release
```

2. Execute o app Go com as bibliotecas:

```bash
cd flesch_score/app
LD_LIBRARY_PATH=.
go build -o app main.go && ./app
```

## Exemplo de Saída

```bash
$ ./app
Texto: A estrutura molecular do DNA foi analisada em laboratório
Flesch Score: 28.7
Label: dificil
Insight: Texto denso. Pode exigir maior atenção ou conhecimento prévio do leitor.
```

## Interoperabilidade entre Linguagens

### Por que usar mais de uma linguagens?

-   **Rust** → Segurança e precisão matemática para cálculo do Flesch Score;
-   **Go** → Agilidade no desenvolvimento da interface + integração de bibliotecas externas.
    
### Comunicação entre elas:

-   Go chama o `.so` de Rust e C++ via `cgo`;
-   O Rust exporta funções com `extern "C"` para compatibilidade; 
-   Dados são passados como `*char` e manipulados como `C.CString`, `C.GoString`, etc.

## Vantagens

- **Alta performance** com Rust para cálculo matemático intensivo.
- **Modularidade**: cada linguagem cumpre uma função específica.
- **Cross-platform-ready**: pode ser usado em sistemas desktop, CLI ou frontend (via WASM).

## Licença

[Portifolio - Junior Paula](https://juniorpaula.com.br)
MIT © 2025 - Junior Paula
