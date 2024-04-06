import random

N = 10000

words_dict_file = open("test_input/english_words_dict.txt", "r")
words_dict = words_dict_file.read().split("\n")
words_dict_file.close()

words_file = open("words.txt", "r")
words = words_file.read().split("\n")
words_file.close()

keys = ""

for i in range(N):
    if random.randint(0, 1):
        keys += random.choice(words) + "\n"
    else:

        if random.randint(0, 1):
            keys += random.choice(words_dict) + "\n"
        else:
            word = random.choice(words_dict)
            keys += word[0].upper() + word[1:] + "\n"

keys_file = open("test_input/keys.txt", "w")
keys_file.write(keys)
keys_file.close()
