mod reader;

use::readability::{flesch_reading_ease, analize_text};
use std::path::Path;

fn main() {
  let path = std::env::args().nth(1).expect("[ERROR] Informe o caminho do arquivo.");
  let path = Path::new(&path);

  let content = if path.extension().map_or(false, |ext| ext == "txt") {
    reader::read_text(path).expect("[ERROR] Não foi possível ler o arquivo.")
  } else {
    panic!("[ERROR] Formato do arquivo inválido.");
  };
  

  let score = flesch_reading_ease(&content);
  let stats = analize_text(&content);

  println!("Flesch Score: {:.2}", score);

  println!("Estatísticas: ");
  println!("Palavras: {}", stats.words);
  println!("Sentenças: {}", stats.sentences);
  println!("Sílabas: {}", stats.syllables);
}
