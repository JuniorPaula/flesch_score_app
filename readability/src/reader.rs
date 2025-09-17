use std::fs;
use std::path::Path;

pub fn read_text(path: &Path) -> std::io::Result<String> {
  fs::read_to_string(path)
}
