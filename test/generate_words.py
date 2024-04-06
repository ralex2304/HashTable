
texts = ["test_input/hamlet.txt"]

words = ""

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

            words += word + "\n"
            word = ""

file = open("test_input/words.txt", "w")

file.write(words)

file.close()

print(len(words_set))
