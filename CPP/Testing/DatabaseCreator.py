import random, string

f = open("test.db","w")
a = []
db = []

for x in range(1,200):
	field1 = ''.join(random.choice(['a','b','c','d','e']) for i in range(random.randint(3,5)))
	for i in range(random.randint(1,10)):
		field2 = ''.join(random.choice(string.ascii_lowercase) for i in range(random.randint(7,10)))
		for j in range(random.randint(8,15)):
			field3 = ''.join(random.choice(string.ascii_lowercase) for i in range(random.randint(5,20)))
			path = '/'.join(''.join(random.choice(string.ascii_lowercase) for i in range(random.randint(5,20))) for j in range(random.randint(1,3))) + "/" + field3
			s = field1 + "\t" + field2 + "\t" + field3 + "\t" + path
			db.append(s)
		a.append(field1)

# field1 = ''.join(random.choice(['a','b','c','d','e']) for i in range(4))
# field2 = ''.join(random.choice(string.ascii_lowercase) for i in range(random.choice([7,8,9,10,11])))
# s = field1 + "\t" + field2
# a.append(field1)
# db.append(s)

random.shuffle(db)

for line in db:
	f.write(line + "\n")

# a.sort()
# wordcount={}

# for word in a:
#     if word not in wordcount:
#         wordcount[word] = 1
#     else:
#         wordcount[word] += 1

# for k,v in wordcount.items():
#     print(k, v)
# print(len(wordcount))