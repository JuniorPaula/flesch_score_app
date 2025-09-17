use::readability::{flesch_reading_ease};

fn main() {
  let text = "O gato roeu a roupa do rei de roma. Mas ele não gostou! Sera?";

  let score = flesch_reading_ease(text);

  println!("Flesch Score: {:.2}", score);
}
