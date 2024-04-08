BLOCK_SIZE = 32

texts = ["test_input/hamlet.txt"]

words_file     = open("test_input/words.txt", "w")
words_file_bin = open("test_input/words_bin.txt", "w")

words_set = set()

for text in texts:

    file = open(text, "r")

    raw = file.read()

    file.close()

    word = ""
    for c in raw:
        if c.isalpha() or c == "'":
            word += c

        elif len(word) != 0:

            words_set.add(word)

            words_file.write(word + "\n")
            words_file_bin.write(word + "\0" * (BLOCK_SIZE - len(word)))
            word = ""


words_file.close()
words_file_bin.close()

print(len(words_set))
