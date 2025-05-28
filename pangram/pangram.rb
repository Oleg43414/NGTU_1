# Проверяет, является ли строка панграммой
# @param sentence [String] Входная строка
# @return [Boolean] true если строка содержит все буквы алфавита, иначе false

def is_pangram(sentence)
    ('a'..'z').all? { |c| sentence.downcase.include?(c) }
  end
  
  puts is_pangram("thequickbrownfoxjumpsoverthelazydog") # true
  puts is_pangram("whatdoesthefoxsay") # false