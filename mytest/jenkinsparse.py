#!/usr/bin/python

import json
from pprint import pprint

with open('myjson') as data_file:    
    data = json.load(data_file)

images=[]
dirs=["mydir","yourdir", "hisdir"]
items=["added", "modified", "removed"]
file="myjson"
ver=data["ref"]


for dir in dirs:

  print("first loop");  print(dir)
  for item in items:
    print("second loop"); print(item)
    result=data["commit_head"][item]
    
    print(result)
    for var in result:
      flag=""
      print("third loop:")
      var=var.encode("utf-8")
      print("compare:\n"); print(var);print(dir );print("######")
      
      if var.find(dir) != -1:
         print("I add sth!!!")
         images.append(dir)
         flag="a"
         break
    if flag != "":
       break
pprint("final res:")
pprint("version:"+str(ver) )
pprint("images:"+str(images) )
              


