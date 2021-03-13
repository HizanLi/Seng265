#Linxin Li
#SENG265 Assignment 4
from cipher import FileDecoder, DecryptException
import string
import re

alphabet = string.ascii_lowercase + string.ascii_uppercase + string.digits + string.punctuation + " \n"

class monthly_delay:
    def __init__(self, month):
        self.month = month
        self.num_data = 0
        self.total_delay = 0
        self.average_delay = float(0.0)

    def get_num_data(self):
        return self.num_data

    def get_average_delay(self):
        return self.average_delay

    def add_date(self, scheduled_departure, actual_departure):
        sd = scheduled_departure[3] * 60 + scheduled_departure[4]
        ad = actual_departure[3] * 60 + actual_departure[4]
        self.total_delay += ad - sd
        self.num_data += 1
        self.cal_average_delay()

    def cal_average_delay(self):
        self.average_delay = float(self.total_delay / self.num_data)

    def __repr__(self):
        return "    Average delay for {0}: {1:.2f}".format(self.month, self.average_delay)


def produce():
    months = ["Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"]
    l = []
    for month in months:
        m = monthly_delay(month)
        l.append(m)
    return l


def main():
    while True:
        filename = input("Please enter the file want to decode: ")
        if is_filename_valid(filename):
            break
        print("Unable to open input file, ", end="")

    while True:
        key = input("Please enter the key: ")
        if is_key_valid_regex(key):
            break
        print("There are some problem with your input format, ", end="")

    while True:
        try:
            file_decoder = FileDecoder(key=key, filename=filename, alphabet=alphabet)
            break
        except DecryptException:
            while True:
                key = input("Please enter the key: ")
                if is_key_valid_regex(key):
                    break
                print("There are some problem with your input format, ", end="")
    #file_decoder = FileDecoder(key="A00!$a", filename="cases/ferry1.out", alphabet=alphabet)

    decrypted_rows = []
    for i in file_decoder:
        decrypted_rows.append(i)
    mon_del = calculate_delay(decrypted_rows)

    print("RESULTS")
    print(file_decoder)
    print("Entries: {}".format(len(file_decoder)))
    for month in mon_del:
        if month.get_num_data() > 0:
            print(month)
    print("END")


def is_key_valid_regex(key):
    if key =="q":
        exit(0)
    num_puns = re.sub('[A-Za-z0-9]', '', key)
    if re.search("^(?=.{2}$)", num_puns) == None:
        return False
    if re.match("^(?=.{6,8}$)(?=(?:.*?[A-Z]))(?=(?:.*?[0-9]){2}).*$", key) == None:
        return False
    return True


def is_filename_valid(filename):
    if filename == "q":
        exit(0)
    try:
        fp = open(filename, "r")
        fp.close()
        return True
    except FileNotFoundError:
        return False


def calculate_delay(decrypted_rows):
    scheduled_departure = []
    actual_departure = []
    mon_del = produce()
    for ele in decrypted_rows[1:]:
        scheduled_departure.append([int(ele[3]), int(ele[4]), int(ele[5]), int(ele[6]), int(ele[7])])
        actual_departure.append([int(ele[8]), int(ele[9]), int(ele[10]), int(ele[11]), int(ele[12])])
    for i in range(len(actual_departure)):
        mon_del[scheduled_departure[i][1] - 1].add_date(scheduled_departure[i], actual_departure[i])
    return mon_del


if __name__ == '__main__':
    main()
