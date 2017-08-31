class Base {
   Base(){
     System.out.println("enter Base()");
   }
   void play(){
     System.out.println("play() in base");
   }
}

interface Shape {
   void run();
}

class Derive extends Base implements Shape {
   Derive() {
     System.out.println("enter Derive()");
   }
   public void run(){
     System.out.println("enter run()");
   }
   public void play(){
     System.out.println("play() in derive");
   }
   public static void main(String args[]){
   	Derive dv=new Derive();
   	dv.run();
   	dv.play();
   }
}
/*

class Derive extends Base implements Shape {  
    public void run(){System.out.println("enter run()Hello");}  
  
    public static void main(String args[]){  
       Derive obj = new Derive();  
       obj.run();  
 }  
}  
*/
