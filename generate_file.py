#!/usr/bin/env python
import os
import random
import sys

BYTESIZE = 255

name = sys.argv[1]
count = int(sys.argv[2])
f = open(name, 'wb')
data_to_sort = bytearray([random.randrange(BYTESIZE) for _ in range(count)])
f.write(data_to_sort)
