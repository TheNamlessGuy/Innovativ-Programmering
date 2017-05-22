import ChatApp.*;          // The package containing our stubs. 
import org.omg.CosNaming.*; // HelloServer will use the naming service. 
import org.omg.CosNaming.NamingContextPackage.*; // ..for exceptions. 
import org.omg.CORBA.*;     // All CORBA applications need these classes. 
import org.omg.PortableServer.*;   
import org.omg.PortableServer.POA;

import java.util.ArrayList;

class ClientHolder {
  private ChatCallback ccb;
  private String name;
  private Boolean active;
  private int ID;
  private Boolean playing;
  private char team;
  
  public ClientHolder(ChatCallback ccb, Boolean active, int ID) {
    this.ccb = ccb;
    this.active = active;
    this.ID = ID;
    name = "";
    playing = false;
  }

  public String getName() {
    return name;
  }

  public void setName(String newName) {
    name = newName;
  } 

  public ChatCallback getChatCallback() {
    return ccb;
  }

  public Boolean isActive() {
    return active;
  }
  
  public void setActive(Boolean b) {
    active = b;
  }

  public int getID() {
    return ID;
  }

  public Boolean isPlaying() {
    return playing;
  }

  public void setPlaying(Boolean b) {
    playing = b;
  }

  public char getTeam() {
    return team;
  }

  public void setTeam(char c) {
    team = c;
  }
}

class ChatImpl extends ChatPOA
{
  private ArrayList<ClientHolder> users = new ArrayList<ClientHolder>();
  private char[][] gameboard = new char[8][8];
  private Boolean gameIsRunning = false;
  private ORB orb;

  public void setORB(ORB orb_val) {
    orb = orb_val;
  }

  public void say(String msg, String ID)
  {
    ClientHolder curr = getUser(Integer.parseInt(ID));
    
    if (!curr.isActive()) {
      return;
    }
    msg = curr.getName() + " said: " + msg;
    
    for (ClientHolder ch: users) {
      ch.getChatCallback().callback(msg);
    }
    
    System.out.println(msg);
  }
  
  public void login(String name, String ID) {
    ClientHolder curr = getUser(Integer.parseInt(ID));
    String msg = "Welcome " + name + "!";

    if (curr.isActive()) {
      msg = curr.getName() + " changed name to " + name + "!";
    }

    for (ClientHolder ch: users) {
      if (ch.getName().equals(name)) {
        curr.getChatCallback().loginAck("f", name);
        return;
      }
    }
    
    curr.setActive(true);
    curr.setName(name);
    curr.getChatCallback().loginAck("t", name);

    for (ClientHolder ch: users) {
      ch.getChatCallback().callback(msg);
    }
    System.out.println(name + " logged in");
  }

  public String connect(ChatCallback ccb) {
    int newID = 0;
    
    if (users.size() > 0) {
      //Increase the last users ID with one for the new user
      newID = users.get(users.size() - 1).getID() + 1;
    }
    
    users.add(new ClientHolder(ccb, false, newID));

    System.out.println("New user with ID: " + newID);

    return Integer.toString(newID);
  }

  public String listClients() {
    String list = "List of registered users:";
    
    for (ClientHolder ch: users) {
      if (ch.isActive()) {
        list += "\n" + ch.getName();
      }
    }

    return list;
  }

  public void quit(String ID) {
    int index = -1;
    String name = "";
    for (int i = 0; i < users.size(); ++i) {
      if (Integer.parseInt(ID) == users.get(i).getID()) {
        index = i;
        name = users.get(i).getName();
        break;
      }
    }
    
    if (users.get(index).isActive()) {
      for (ClientHolder ch: users) {
        ch.getChatCallback().callback(name + " left");
      }
    } else {
      name = "User with ID " + ID;
    }

    System.out.println(name + " quitted");
    users.remove(index);
    
    //Check if anyone is still playing
    for (ClientHolder ch: users) {
      if (ch.isPlaying())
        return;
    }

    gameIsRunning = false;
  }

  public void leave(String ID) {
    ClientHolder curr = getUser(Integer.parseInt(ID));
    if (!curr.isActive())
      return;
    
    System.out.println(curr.getName() + " left");
    for (ClientHolder ch: users) {
      ch.getChatCallback().callback(curr.getName() + " left");
    }
    
    System.out.println(curr.getName() + " left");
    curr.setActive(false);
    curr.setName("");
    curr.setPlaying(false);
    
    //Check if anyone is still playing
    for (ClientHolder ch: users) {
      if (ch.isPlaying())
        return;
    }

    gameIsRunning = false;
  }

  public void othelloJoin(String ID, String team) {
    ClientHolder curr = getUser(Integer.parseInt(ID));

    if (curr.isPlaying() || !curr.isActive())
      return;

    curr.setTeam(team.charAt(0));
    curr.setPlaying(true);

    System.out.println(curr.getName() + " joins team " + team);
    for (ClientHolder ch: users) {
      ch.getChatCallback().callback(curr.getName() + " joins team " + team);
    }

    if (!gameIsRunning) {
      gameIsRunning = true;
      resetBoard(ID);
    } else {
      curr.getChatCallback().callback(getGameboard());
    }
  }

  public void othelloMove(String ID, String position) {
    ClientHolder curr = getUser(Integer.parseInt(ID));
    System.out.println(curr.getName() + " othelloMove: " + position);
    int[] realpos = new int[2];
    
    switch(position.toUpperCase().charAt(0)) {
      case 'A':
        realpos[1] = 0;
        break;
      case 'B':
        realpos[1] = 1;
        break;
      case 'C':
        realpos[1] = 2;
        break;
      case 'D':
        realpos[1] = 3;
        break;
      case 'E':
        realpos[1] = 4;
        break;
      case 'F':
        realpos[1] = 5;
        break;
      case 'G':
        realpos[1] = 6;
        break;
      case 'H':
        realpos[1] = 7;
        break;
      default:
        return;
    }

    realpos[0] = Character.getNumericValue(position.charAt(1)) - 1;

    if (gameboard[realpos[0]][realpos[1]] != '-') {
      System.out.println("Illegal move");
      return;
    }

    Boolean legitMove = false;
    
    if (realpos[0] <= 5) {
      //Check to the right
      if (gameboard[realpos[0] + 2][realpos[1]] == curr.getTeam() &&
          gameboard[realpos[0] + 1][realpos[1]] != '-' &&
          gameboard[realpos[0] + 1][realpos[1]] != curr.getTeam()) {
        //Legit move
        legitMove = true;
        gameboard[realpos[0]][realpos[1]] = curr.getTeam();
        gameboard[realpos[0] + 1][realpos[1]] = curr.getTeam();
      }
      if (realpos[1] >= 2) {
        System.out.println("Checking northeast");
        //Check northeast
        if (gameboard[realpos[0] + 2][realpos[1] - 2] == curr.getTeam() && 
            gameboard[realpos[0] + 1][realpos[1] - 1] != '-' &&
            gameboard[realpos[0] + 1][realpos[1] - 1] != curr.getTeam()) {
          //Legit move
          legitMove = true;
          gameboard[realpos[0]][realpos[1]] = curr.getTeam();
          gameboard[realpos[0] + 1][realpos[1] - 1] = curr.getTeam();
        }
      }
      if (realpos[1] <= 5) {
        System.out.println("Checking southeast");
        //Check southeast
        if (gameboard[realpos[0] + 2][realpos[1] + 2] == curr.getTeam() &&
            gameboard[realpos[0] + 1][realpos[1] + 1] != '-' &&
            gameboard[realpos[0] + 1][realpos[1] + 1] != curr.getTeam()) {
          //Legit move
          legitMove = true;
          gameboard[realpos[0]][realpos[1]] = curr.getTeam();
          gameboard[realpos[0] + 1][realpos[1] + 1] = curr.getTeam();
        }
      }
    }

    if (realpos[0] >= 2) {
      System.out.println("Checking west");
      //Check to the left
      if (gameboard[realpos[0] - 2][realpos[1]] == curr.getTeam() &&
          gameboard[realpos[0] - 1][realpos[1]] != '-' &&
          gameboard[realpos[0] - 1][realpos[1]] != curr.getTeam()) {
        //Legit move
        legitMove = true;
        gameboard[realpos[0]][realpos[1]] = curr.getTeam();
        gameboard[realpos[0] - 1][realpos[1]] = curr.getTeam();
      }
      if (realpos[1] >= 2) {
        System.out.println("Checking northwest");
        //Check northwest
        if (gameboard[realpos[0] - 2][realpos[1] - 2] == curr.getTeam() &&
            gameboard[realpos[0] - 1][realpos[1] - 1] != '-' &&
            gameboard[realpos[0] - 1][realpos[1] - 1] != curr.getTeam()) {
          //Legit move
          legitMove = true;
          gameboard[realpos[0]][realpos[1]] = curr.getTeam();
          gameboard[realpos[0] - 1][realpos[1] - 1] = curr.getTeam();
        }
      }
      if (realpos[1] <= 5) {
        System.out.println("Checking southwest");
        //Check southwest
        if (gameboard[realpos[0] - 2][realpos[1] + 2] == curr.getTeam() &&
            gameboard[realpos[0] - 1][realpos[1] + 1] != '-' &&
            gameboard[realpos[0] - 1][realpos[1] + 1] != curr.getTeam()) {
          //Legit move
          legitMove = true;
          gameboard[realpos[0]][realpos[1]] = curr.getTeam();
          gameboard[realpos[0] - 1][realpos[1] + 1] = curr.getTeam();
        }
      }
    }

    if (realpos[1] >= 2) {
      System.out.println("Checking north");
      //Check north
      if (gameboard[realpos[0]][realpos[1] - 2] == curr.getTeam() &&
          gameboard[realpos[0]][realpos[1] - 1] != '-' &&
          gameboard[realpos[0]][realpos[1] - 1] != curr.getTeam()) {
        //Legit move
        legitMove = true;
        gameboard[realpos[0]][realpos[1]] = curr.getTeam();
        gameboard[realpos[0]][realpos[1] - 1] = curr.getTeam();
      }
    }

    if (realpos[1] <= 5) {
      System.out.println("Checking south");
      //Check south
      if (gameboard[realpos[0]][realpos[1] + 2] == curr.getTeam() &&
          gameboard[realpos[0]][realpos[1] + 1] != '-' &&
          gameboard[realpos[0]][realpos[1] + 1] != curr.getTeam()) {
        //Legit move
        legitMove = true;
        gameboard[realpos[0]][realpos[1]] = curr.getTeam();
        gameboard[realpos[0]][realpos[1] + 1] = curr.getTeam();
      }
    }
    
    if (!legitMove)
      return;

    System.out.println(curr.getName() + " placed an " + curr.getTeam() + " on position " + position);
    for (ClientHolder ch: users) {
      ch.getChatCallback().callback(curr.getName() + " placed an " + curr.getTeam() + " on position " + position);
    }
    sendGameboard();
  }

  public void othelloLeave(String ID) {
    ClientHolder curr = getUser(Integer.parseInt(ID));
    curr.setPlaying(false);

    System.out.println(curr.getName() + " leaves the game");
    for (ClientHolder ch: users) {
      ch.getChatCallback().callback(curr.getName() + " leaves the game");
    }
    
    //Check if anyone is still playing
    for (ClientHolder ch: users) {
      if (ch.isPlaying())
        return;
    }

    gameIsRunning = false;
  }

  public String getGameboard() {
    String gameboardStr = "  A B C D E F G H\n";
    for (int i = 0; i < gameboard.length; ++i) {
      gameboardStr += Integer.toString(i + 1) + " ";
      
      for (int j = 0; j < gameboard[i].length; ++j) {
        gameboardStr += gameboard[i][j] + " ";
      }
      gameboardStr += "\n";
    }
    return gameboardStr;
  }

  public void sendGameboard() {
    String gameboardStr = getGameboard();

    for (ClientHolder ch: users) {
      ch.getChatCallback().callback(gameboardStr);
    }
  }

  public void resetBoard (String ID) {
    ClientHolder curr = getUser(Integer.parseInt(ID));
    if (!curr.isActive())
      return;

    System.out.println(curr.getName() + " reset the board");
    for (ClientHolder ch: users) {
      ch.getChatCallback().callback(curr.getName() + " reset the board");
    }

    for (int i = 0; i < gameboard.length; ++i) {
      for (int j = 0; j < gameboard[i].length; ++j) {
        gameboard[i][j] = '-';
      }
    }
    gameboard[3][3] = 'x';
    gameboard[4][3] = 'o';
    gameboard[3][4] = 'o';
    gameboard[4][4] = 'x';
    sendGameboard();
  }

  public ClientHolder getUser(int id) {
    for (ClientHolder ch: users) {
      if (ch.getID() == id) {
        return ch;
      }
    }
    return null;
  }
  
}

public class ChatServer 
{
  public static void main(String args[]) 
  {
    try { 
      // create and initialize the ORB
      ORB orb = ORB.init(args, null); 

      // create servant (impl) and register it with the ORB
      ChatImpl chatImpl = new ChatImpl();
      chatImpl.setORB(orb); 

      // get reference to rootpoa & activate the POAManager
      POA rootpoa = 
        POAHelper.narrow(orb.resolve_initial_references("RootPOA"));  
      rootpoa.the_POAManager().activate(); 

      // get the root naming context
      org.omg.CORBA.Object objRef = 
        orb.resolve_initial_references("NameService");
      NamingContextExt ncRef = NamingContextExtHelper.narrow(objRef);

      // obtain object reference from the servant (impl)
      org.omg.CORBA.Object ref = 
        rootpoa.servant_to_reference(chatImpl);
      Chat cref = ChatHelper.narrow(ref);

      // bind the object reference in naming
      String name = "Chat";
      NameComponent path[] = ncRef.to_name(name);
      ncRef.rebind(path, cref);

      // Application code goes below
      System.out.println("ChatServer ready and waiting ...");
	    
      // wait for invocations from clients
      orb.run();
    }
	    
    catch(Exception e) {
      System.err.println("ERROR : " + e);
      e.printStackTrace(System.out);
    }

    System.out.println("ChatServer Exiting ...");
  }

}
