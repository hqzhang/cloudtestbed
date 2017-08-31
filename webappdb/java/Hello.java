import java.util.*; 
class Base {
   Base(){
     System.out.println("enter Base()");
   }
}
interface Shape {
   void run();
}

class Derive extends Base implements Shape {
  
   public Derive() {
     System.out.println("enter Derive()");
   }
   public void run(){
     System.out.println("enter run()");
   }
}


public class Hello {

    public static void main(String[] args) {
        // Prints "Hello, World" to the terminal window.
        // show List 
        ArrayList<String> al=new ArrayList<String>();  
        al.add("Amit");  
        al.add("Kumar");  
        al.add(1,"Sachin");  
        System.out.println("Element at 013nd position: "+al.get(0)+al.get(1)+al.get(2));  
        for(String s:al){  
            System.out.println(s); 
        } 
         // show Set
        System.out.println("list set:");
        Set<String> st=new HashSet<String>();
        st.add("Amit");
        st.add("Kumar");
        //st.add(1,"Sachin");
        //System.out.println("Element at 013nd position: "+st.get(0)+st.get(1)+st.get(2));
        for(String s:st){
            System.out.println(s);
        }
         //show Map
        Map<Integer, String> map = new HashMap<Integer, String>();
        Integer key1   = new Integer(123);
        String  value1 = "value 1";
        Integer key2   = new Integer(456);
        String value2  = "value 2";
        map.put(key1, value1);
        map.put(key2, value2);
	String value1_1 = map.get(key1);

        for(Integer aKey : map.keySet()) {
 	   String aValue = map.get(aKey);
    	   System.out.println("" + aKey + ":" + aValue);
	}
        //show hashtable
        // Creating a Hashtable
        Hashtable<String, String> hashtable = 
              new Hashtable<String, String>();
 
        // Adding Key and Value pairs to Hashtable
        hashtable.put("Key1","Chaitanya");
        hashtable.put("Key2","Ajeet");
        for(String aKey : hashtable.keySet()) {
           String aValue = map.get(aKey);
           System.out.println("" + aKey + ":" + aValue);
        } 
        //show object
        Derive dv=new Derive();
        dv.run();


  }


}
