pub struct TextStats {
  pub sentences: usize,
  pub words: usize,
  pub syllables: usize,
}

/// # Safety
///
/// Esta função assume que `ptr` aponta para uma sequência válida de bytes UTF-8
/// com o tamanho `len`. O ponteiro não pode ser nulo e a região de memória deve esta acessível.
///
/// Se esses condições não forem garantidas, o comportamento será indefinido.
#[unsafe(no_mangle)]
pub unsafe extern "C" fn flesch_score(ptr: *const u8, len: usize) -> f64 {
  let slice = unsafe { std::slice::from_raw_parts(ptr, len)};
  let text = match std::str::from_utf8(slice) {
    Ok(s) => s,
    Err(_) => return 0.0,
  };

  flesch_reading_ease(text)
}

pub fn flesch_reading_ease(text: &str) -> f64 {
  let stats = analize_text(text);
  if stats.sentences == 0 || stats.words == 0 {
    return 0.0;
  }

  let wps = stats.words as f64 / stats.sentences as f64;
  let spw = stats.syllables as f64 / stats.words as f64;

  248.835 - (1.015 * wps) - (84.6 * spw)
}

pub fn analize_text(text: &str) -> TextStats {
  let sentences = count_sentences(text);
  let words = count_words(text);
  let syllables = count_syllables(text);

  TextStats { sentences, words, syllables }
}

fn count_words(text: &str) -> usize {
  text.split_whitespace().count()
}

fn count_sentences(text: &str) -> usize {
  text.matches(&['.', '!', '?'][..]).count().max(1)
}

fn count_syllables(text: &str) -> usize {
  text.split_whitespace()
    .map(count_syllable_in_word)
    .sum()
}

fn count_syllable_in_word(word: &str) -> usize {
  let word = word.to_lowercase();
  let mut count = 0;

  let vowels = "aeiouy";

  let chars: Vec<char> = word.chars().collect();
  let mut prev_was_vowel = false;

  for &c in &chars {
    let is_vowel = vowels.contains(c);
    if is_vowel && !prev_was_vowel {
      count += 1;
    }
    prev_was_vowel = is_vowel;
  }

  if word.ends_with('e') && count > 1 {
    count -= 1;
  }

  count.max(1)
}
