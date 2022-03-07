import java.lang.* ;
import java.io.* ;
import java.net.* ;
import java.util.* ;


public class client
{
   public static void main ( String [] args)
   {

      if (args.length != 2)
      {
         System.out.println("Usage: client <host> <port>");
         System.exit(0);
      }

     try
     {
        // Creat the connections
	// insert code here


	InputStreamReader  is = new InputStreamReader(System.in);
	BufferedReader br = new BufferedReader(is);

        String message;
        boolean done = false;

        while (!done) {
		// complete the code to communicate with the server
                System.out.println("Write something");

                message = br.readLine();

		System.out.println(message);

        }

      }
      catch (Exception e)
      {
         System.err.println("Exception " + e.toString() );
         e.printStackTrace() ;
      }

   }
}

