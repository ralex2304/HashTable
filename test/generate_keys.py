import random

BLOCK_SIZE = 32
N = 10000000

words_dict_file = open("test_input/english_words_dict.txt", "r")
words_dict = words_dict_file.read().split("\n")[:-2]

words_dict_file.close()

words_file = open("test_input/words.txt", "r")
words = words_file.read().split("\n")[:-2]
words_file.close()

keys_file     = open("test_input/keys.txt", "w")
keys_bin_file = open("test_input/keys_bin.txt", "w")

for i in range(N):
    out = ""

    if random.randint(0, 1):
        out = random.choice(words)
    else:
        out = random.choice(words_dict)
        while len(out) >= BLOCK_SIZE:
            out = random.choice(words_dict)

        if random.randint(0, 1):
            out = out[0].upper() + out[1:]

    keys_bin_file.write(out + "\0" * (BLOCK_SIZE - len(out)))
    keys_file.write(out + "\n")

keys_bin_file.write("\0")

keys_file.close()
keys_bin_file.close()
