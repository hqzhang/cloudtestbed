#!/usr/bin/python
import time;

def myfoo(a,b):
   print "Hello, Python!",a,b
class Person:
   def __init__(age,sex):
      self.age = name
      self.sex = sex 
class Employee(Person):
   'Common base class for all employees'
   empCount = 0

   def __init__(self, name, salary):
      self.name = name
      self.salary = salary
      Employee.empCount += 1
   
   def displayCount(self):
     print "Total Employee %d" % Employee.empCount

   def displayEmployee(self):
      print "Name : ", self.name,  ", Salary: ", self.salary

"This would create first object of Employee class"
emp1 = Employee("Zara", 2000)
"This would create second object of Employee class"
emp2 = Employee("Manni", 5000)
emp1.displayEmployee()
emp2.displayEmployee()
print "Total Employee %d" % Employee.empCount
myfoo(2,3)

var = 100
if  var  == 100 : 
   print "Value of expression is 100"
if var:
   print "if var: expect"
 #show list and loop
fruits = ['banana', 'apple']
tuples = ('banana', 'apple')
dicts = {'Name': 'Zara', 'Age': 7, 'Class': 'First'}
for fruit in fruits:        # Second Example
   print 'Current fruit :', fruit

 #show time
ticks = time.localtime(time.time())
print "ticks from 1970:", ticks
 #show input
str = raw_input("Enter your input: ");
print "Received input is : ", str

# Open a file
fo = open("foo.txt", "wbr+")
print "Name of the file: ", fo.name
fo.write( "Python is a great language.\nYeah its great!!\n");
fo.close()
fo = open("foo.txt", "r+")
str = fo.read(10);
print "Read String is : ", str
# Close opend file
fo.close()
