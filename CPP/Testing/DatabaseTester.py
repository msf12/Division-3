db = open("test.db")
f = open("merge12.db")

s = set([line[0:4] for line in db])
s2 = set([line[0:4] for line in f])

# print(s2)
# print()
print([x for x in s if x not in s2])
print([x for x in s2 if x not in s])
print(s^s2)

# testset = set([])

# for i in range(25,28,2):
# 	f1 = open("temp"+str(i)+".db")

# 	testset = testset | set(line[0:4] for line in f1)

# print(s^testset)