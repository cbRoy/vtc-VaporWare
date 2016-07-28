#!/usr/bin/env python

print 'Draw level with 1s and 0s'
print 'Max width is 8'
print 'Stop drawing with a newline'
print

sentinel = ''
lines = '\n'.join(iter(raw_input, sentinel))
width = lines.index('\n')
height = lines.count('\n') + 1
bricks = lines.count('1')
layout = "{ "


for line in lines.split('\n'):
	
	layout += hex(int(line,2)) + ", "

layout = layout[:-2]
layout += " }"

print "struct levelDesc LEVEL = {"
print "\t.width = {},".format(width)
print "\t.height = {},".format(height)
print "\t.bricks = {},".format(bricks)
print "\t.layout = {}".format(layout)
for line in lines.split('\n'):
	print "\t// ", line.replace('0',' ')
print "};"
