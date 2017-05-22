import ChatApp.*;          // The package containing our stubs
import org.omg.CosNaming.*; // HelloClient will use the naming service.
import org.omg.CosNaming.NamingContextPackage.*;
import org.omg.CORBA.*;     // All CORBA applications need these classes.
import org.omg.PortableServer.*;   
import org.omg.PortableServer.POA;

import java.util.Scanner;
 
class ChatCallbackImpl extends ChatCallbackPOA
{
  private ORB orb;

  public void setORB(ORB orb_val) {
    orb = orb_val;
  }

  public void callback(String notification)
  {
    System.out.println(notification);
  }

  public void loginAck(String accepted, String name) {
    if (accepted.equals("f")) {
      System.out.println("ERROR: '" + name + "' is already an active user!");
    }
  }
}

public class ChatClient
{
  static Chat chatImpl;

  public static void join(String[] userInputSplit, String myID) {
    if (userInputSplit.length != 2) {
      System.out.println("ERROR: Usage => join [name]");
      return;
    }
    chatImpl.login(userInputSplit[1], myID);
  }

  public static void post(String[] userInputSplit, String myID) {
    String msg = "";
    for (int i = 1; i < userInputSplit.length; ++i) {
      msg += userInputSplit[i] + " ";
    }
    chatImpl.say(msg, myID);
  }

  public static void list() {
    System.out.println(chatImpl.listClients());
  }

  public static void othelloJoin(String[] userInputSplit, String myID) {
    if (userInputSplit.length != 2 &&
        (userInputSplit[1] != "x" || userInputSplit[1] != "o")) {
      System.out.println("ERROR: Usage => Othello [color], where [color] is 'x' or 'o'");
      return;
    }
    chatImpl.othelloJoin(myID, userInputSplit[1]);
  }

  public static void othelloMove(String[] userInputSplit, String myID) {
    chatImpl.othelloMove(myID, userInputSplit[1]);
  }
    
  public static void main(String args[])
  {
    try {
      // create and initialize the ORB
      ORB orb = ORB.init(args, null);

      // create servant (impl) and register it with the ORB
      ChatCallbackImpl chatCallbackImpl = new ChatCallbackImpl();
      chatCallbackImpl.setORB(orb);

      // get reference to RootPOA and activate the POAManager
      POA rootpoa = 
        POAHelper.narrow(orb.resolve_initial_references("RootPOA"));
      rootpoa.the_POAManager().activate();
	    
      // get the root naming context 
      org.omg.CORBA.Object objRef = 
        orb.resolve_initial_references("NameService");
      NamingContextExt ncRef = NamingContextExtHelper.narrow(objRef);
	    
      // resolve the object reference in naming
      String name = "Chat";
      chatImpl = ChatHelper.narrow(ncRef.resolve_str(name));
	    
      // obtain callback reference for registration w/ server
      org.omg.CORBA.Object ref = 
        rootpoa.servant_to_reference(chatCallbackImpl);
      ChatCallback cref = ChatCallbackHelper.narrow(ref);
	    
      // Application code goes below
      Scanner scanner = new Scanner(System.in);
      String myID = chatImpl.connect(cref);
      String userInput = "";
      
      while (!userInput.equals("quit")) {
        userInput = scanner.nextLine();
        String[] userInputSplit = userInput.split(" ");

        switch(userInputSplit[0].toLowerCase()) {
          case "join":
            join(userInputSplit, myID);
            break;
          case "post":
            post(userInputSplit, myID);
            break;
          case "leave":
            chatImpl.leave(myID);
            System.out.println("Goodbye!");
            break;
          case "list":
            list();
            break;
          case "othello":
            othelloJoin(userInputSplit, myID);
            break;
          case "piece":
            othelloMove(userInputSplit, myID);
            break;
          case "exit":
            chatImpl.othelloLeave(myID);
            break;
          case "reset":
            chatImpl.resetBoard(myID);
            break;
          default:
            break;
        }
      }
      scanner.close();
      chatImpl.quit(myID);
	    
    } catch(Exception e){
      System.out.println("ERROR : " + e);
      e.printStackTrace(System.out);
    }
  }
}
