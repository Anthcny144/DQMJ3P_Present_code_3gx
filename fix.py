import sys

if len(sys.argv) != 2:
    print("Wrong arguments:\nUsage: fix.py <codes.txt>")
    quit()

def IsLargeJapanese(Value):
    Value = ord(Value)
    return Value >= 0xFF00 and Value <= 0xFFFF

Chars = []

with open(sys.argv[1], encoding="utf-8") as File:
    while True:
        Char = File.read(1)
        if not Char: break

        if IsLargeJapanese(Char):
            Char = chr(ord(Char) - 0xFEE0)
        Chars.append(Char)

with open("codes.txt", "w+", encoding="utf-8") as File:
    for Char in Chars:
        File.write(Char)