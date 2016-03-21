import random, string

f = open("test.db","w")
a = []
for x in range(0,5000):
	field1 = ''.join(random.choice(['a','b','c','d','e','f']) for i in range(4))
	field2 = ''.join(random.choice(string.ascii_lowercase) for i in range(random.choice([7,8,9,10,11])))
	s = field1 + "\t" + field2 + "\n"
	a.append(field1)
	f.write(s)
a.sort()
wordcount={}

for word in a:
    if word not in wordcount:
        wordcount[word] = 1
    else:
        wordcount[word] += 1

for k,v in wordcount.items():
    print(k, v)
print(len(wordcount))