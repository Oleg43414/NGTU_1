def is_pangram(sentence: str) -> bool:
    """
    Проверяет, является ли строка панграммой (содержит все буквы английского алфавита).
    
    Args:
        sentence: Входная строка для проверки
        
    Returns:
        bool: True если строка содержит все буквы алфавита, иначе False
    """
    
    alphabet = set('abcdefghijklmnopqrstuvwxyz')
    return alphabet.issubset(sentence.lower())

# Примеры использования
print(is_pangram("thequickbrownfoxjumpsoverthelazydog"))  # True
print(is_pangram("whatdoesthefoxsay"))  # False