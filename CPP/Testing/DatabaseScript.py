def sortDatabase(database,delim="\t",conn=","):
	db = open(database)
	numFields = len(db.readline().split("\t"))
	print(numFields)
	db.seek(0)

	for line in db:
		

sortDatabase("test.db")