#include <iostream>
 
using namespace std;

// Base class
class Shape  {
   public:
      void setWidth(int w) {
         width = w;
      }
		
      void setHeight(int h) {
         height = h;
      }
      virtual int getArea() {
         cout<<"getArea()in shape"<<endl;
         return (width * height);
      }
		
   protected:
      int width;
      int height;
};

// Derived class
class Rectangle: public Shape {
   public:
      int getArea() { 
         cout<<"getArea()in rectangle"<<endl;
         return (width * height); 
      }
};

int main(void) {
   Shape *Rect=new Shape();
   Rect->setWidth(5);
   Rect->setHeight(7);
   cout << "Total area: " << Rect->getArea() << endl;
   
   Rect = new Rectangle();
   Rect->setWidth(5);
   Rect->setHeight(7);

   // Print the area of the object.
   cout << "Total area: " << Rect->getArea() << endl;

   return 0;
}
