#Linxin Li
#SENG265 Assignment 4
class FileDecoder:
    invalid_punctuations = r"""!"#$%&'()*+-/:;<=>?@[\]^`{|}~"""
    def __init__(self, key, filename, alphabet):
        self.key = key
        self.num_lines = 0
        self.filename = filename
        self.alphabet = alphabet
        self.encrypted = ""
        self.decrypted_string = ""
        self.count = 0

        self.read_all()
        self.decode()
        self.check_decode()

    def set_decrypted_string(self, decrypted_string):
        self.decrypted_string = decrypted_string

    def get_decrypted_string(self):
        return self.decrypted_string

    def read_all(self):
        fp = open(self.filename, "r")
        for line in fp:
            self.encrypted += line
        fp.close()

    def decode(self):
        count = 0
        decrypted_string = ''
        for character in self.encrypted:
            val_input = self.alphabet.find(character)
            val_key = self.alphabet.find(self.key[count % len(self.key)])
            count += 1
            result = (val_input - val_key) % len(self.alphabet)
            decrypted_string += self.alphabet[result]
        self.set_decrypted_string(decrypted_string[:-1])

    def check_decode(self):
        if len(self.decrypted_string.split("\n")) < 2:
            raise DecryptException()
        if "scheduled_departure_minute" not in self.decrypted_string:
            raise DecryptException()
        if "actual_departure_minute" not in self.decrypted_string:
            raise DecryptException()
        length = len(self.decrypted_string.split("\n")[0].split(","))
        for s in self.decrypted_string.split("\n"):
            if (len(s.split(","))) != length:
                raise DecryptException()
        for punctuation in self.invalid_punctuations:
            error = self.decrypted_string.count(punctuation)
            if error > 0:
                raise DecryptException()
        return True

    def __repr__(self):
        return "FileDecoder: FileDecoder(key='{0}', file='{1}')".format(self.key, self.filename)

    def __len__(self):
        return len(self.decrypted_string.split('\n'))

    def __next__(self):
        if self.count < self.num_lines:
            count = self.count
            self.count += 1
            return self.decrypted_string.split('\n')[count]
        else:
            raise StopIteration

    def __iter__(self):
        decrypted_rows = self.decrypted_string.split('\n')
        for l in decrypted_rows:
            yield l.split(",")


class DecryptException(Exception):
    def __init__(self):
        print("Error when decoding, probably wrong key")
    pass