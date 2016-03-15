data = open("output.txt").readlines()

print "unsigned char zero[BLOCK * AMOUNT] = \n{"
for i in xrange(512):
	#line = data[i * 7 + 5].split()
	line = data[i * 7 + 3].split()
	for x in line:
		print "0x%s,"%(x),
	print
print "};"