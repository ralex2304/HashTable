import random

N = 1000000

words_dict_file = open("test_input/english_words_dict.txt", "r")
words_dict = words_dict_file.read().split("\n")[:-2]

words_dict_file.close()

words_file = open("test_input/words.txt", "r")
words = words_file.read().split("\n")[:-2]
words_file.close()

keys_file = open("test_input/keys.txt", "w")

for i in range(N):
    if random.randint(0, 1):
        keys_file.write(random.choice(words) + "\n")
    else:

        if random.randint(0, 1):
            keys_file.write(random.choice(words_dict) + "\n")
        else:
            word = random.choice(words_dict)
            keys_file.write(word[0].upper() + word[1:] + "\n")

keys_file.close()
