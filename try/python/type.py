import random

def main():
    while True:
        question = "".join([generate_char(ONLY_NUM) for i in range(20)])
        print(question)
        answer = input()
        if (question != answer):
            print("错了")

ONLY_NUM = 1
def generate_char(which=None):
    chars = ['0', '1','2', '3', '4', '5', '6', '7', '8', '9', '!', '@', '#', '$', '%', '^', '&', '*', '(', ')']
    if which == ONLY_NUM:
        rand = random.randint(0, 9)
    else:
        rand = random.randint(0, len(chars) - 1)
        
    return chars[rand]




if __name__ == '__main__':
    main()
