package tddc17;


import aima.core.environment.liuvacuum.*;
import aima.core.agent.Action;
import aima.core.agent.AgentProgram;
import aima.core.agent.Percept;
import aima.core.agent.impl.*;

import java.util.ArrayList;
import java.util.Random;

class MyAgentState
{
	public int[][] world = new int[30][30];
	public int initialized = 0;
	final int UNKNOWN 	= 0;
	final int WALL 		= 1;
	final int CLEAR 	= 2;
	final int DIRT		= 3;
	final int HOME		= 4;
	final int ACTION_NONE 			= 0;
	final int ACTION_MOVE_FORWARD 	= 1;
	final int ACTION_TURN_RIGHT 	= 2;
	final int ACTION_TURN_LEFT 		= 3;
	final int ACTION_SUCK	 		= 4;
	
	public int agent_x_position = 1;
	public int agent_y_position = 1;
	public Action agent_last_action = NoOpAction.NO_OP;
	
	public static final int NORTH = 0;
	public static final int EAST = 1;
	public static final int SOUTH = 2;
	public static final int WEST = 3;
	public int agent_direction = EAST;
	
	MyAgentState()
	{
		for (int i=0; i < world.length; i++)
			for (int j=0; j < world[i].length ; j++)
				world[i][j] = UNKNOWN;
		world[1][1] = HOME;
		agent_last_action = NoOpAction.NO_OP;
	}
	// Based on the last action and the received percept updates the x & y agent position
	public void updatePosition(DynamicPercept p)
	{
		Boolean bump = (Boolean)p.getAttribute("bump");

		if (agent_last_action == LIUVacuumEnvironment.ACTION_MOVE_FORWARD && !bump)
	    {
			switch (agent_direction) {
			case MyAgentState.NORTH:
				agent_y_position--;
				break;
			case MyAgentState.EAST:
				agent_x_position++;
				break;
			case MyAgentState.SOUTH:
				agent_y_position++;
				break;
			case MyAgentState.WEST:
				agent_x_position--;
				break;
			}
	    }
		
	}
	
	public void updateWorld(int x_position, int y_position, int info)
	{
		world[x_position][y_position] = info;
	}
	
	public void printWorldDebug()
	{
		for (int i=0; i < world.length; i++)
		{
			for (int j=0; j < world[i].length ; j++)
			{
				if (i == agent_y_position && j == agent_x_position) {
					System.out.print(" P ");
					continue;
				}
				if (world[j][i]==UNKNOWN)
					System.out.print(" ? ");
				if (world[j][i]==WALL)
					System.out.print(" # ");
				if (world[j][i]==CLEAR)
					System.out.print(" . ");
				if (world[j][i]==DIRT)
					System.out.print(" D ");
				if (world[j][i]==HOME)
					System.out.print(" H ");
			}
			System.out.println("");
		}
	}
	
	public void updateDirection() {
		if (agent_last_action == LIUVacuumEnvironment.ACTION_TURN_LEFT) {
		    agent_direction = ((agent_direction-1) % 4);
		    if (agent_direction < 0)
		    	agent_direction +=4;
		} else if (agent_last_action == LIUVacuumEnvironment.ACTION_TURN_RIGHT) {
			agent_direction = ((agent_direction+1) % 4);
		}
	}
}

class MyAgentProgram implements AgentProgram {

	private int initnialRandomActions = 10;
	private Random random_generator = new Random();
	
	// Here you can define your variables!
	public int iterationCounter = 2000;
	public MyAgentState state = new MyAgentState();
	
	// OWN VARIABLES
	final short GOAL_GOHOME = 0;
	final short GOAL_FINDPERIMETER = 1;
	final short GOAL_SEARCHWORLD = 2;
	final short GOAL_FINISH = 3;
	
	final boolean HAND_LEFT = true;
	final boolean HAND_RIGHT = false;
	
	boolean home = false;
	boolean dirt = false;
	boolean bump = false;
	
	ArrayList<Action> actions = new ArrayList<Action>();
	
	short currentGoal = GOAL_GOHOME;
	
	// Find perimeter goal variables
	boolean findPerimeterHasMoved = false;
	
	// Search world goal variables
	int currentRow = 1;
	int maxX = -1;
	int maxY = -1;
	int maxYCurrentRow = -1;
	int minYCurrentRow = -1;
	int currentDirection = MyAgentState.EAST;
	int goingToCurrentRowDirection = -1;
	
	// Get around obstacle variables
	Point getTo = Point.NONE;
	Point orig = Point.NONE;
	boolean leftHand = false;
	boolean gettingAroundObstacle = false;
	int getAroundDirection = -1;
	
	// moves the Agent to a random start position
	// uses percepts to update the Agent position - only the position, other percepts are ignored
	// returns a random action
	private Action moveToRandomStartPosition(DynamicPercept percept) {
		int action = random_generator.nextInt(6);
		initnialRandomActions--;
		state.updatePosition(percept);
		if(action==0) {
		    state.agent_direction = ((state.agent_direction-1) % 4);
		    if (state.agent_direction<0) 
		    	state.agent_direction +=4;
		    state.agent_last_action = LIUVacuumEnvironment.ACTION_TURN_LEFT;
			return LIUVacuumEnvironment.ACTION_TURN_LEFT;
		} else if (action==1) {
			state.agent_direction = ((state.agent_direction+1) % 4);
		    state.agent_last_action = LIUVacuumEnvironment.ACTION_TURN_RIGHT;
		    return LIUVacuumEnvironment.ACTION_TURN_RIGHT;
		} 
		state.agent_last_action = LIUVacuumEnvironment.ACTION_MOVE_FORWARD;
		return LIUVacuumEnvironment.ACTION_MOVE_FORWARD;
	}
	
	
	@Override
	public Action execute(Percept percept) {
		
		// DO NOT REMOVE this if condition!!!
    	if (initnialRandomActions>0) {
    		return moveToRandomStartPosition((DynamicPercept) percept);
    	} else if (initnialRandomActions==0) {
    		// process percept for the last step of the initial random actions
    		initnialRandomActions--;
    		state.updatePosition((DynamicPercept) percept);
			System.out.println("Processing percepts after the last execution of moveToRandomStartPosition()");
			state.agent_last_action = LIUVacuumEnvironment.ACTION_SUCK;
	    	return LIUVacuumEnvironment.ACTION_SUCK;
    	}
		
    	// This example agent program will update the internal agent state while only moving forward.
    	// START HERE - code below should be modified!
    	    	
    	System.out.println("x=" + state.agent_x_position);
    	System.out.println("y=" + state.agent_y_position);
    	System.out.println("dir=" + state.agent_direction);
    	
		
	    iterationCounter--;
	    
	    if (iterationCounter==0)
	    	return NoOpAction.NO_OP;

	    DynamicPercept p = (DynamicPercept) percept;
	    bump = (Boolean)p.getAttribute("bump");
	    dirt = (Boolean)p.getAttribute("dirt");
	    home = (Boolean)p.getAttribute("home");
	    System.out.println("percept: " + p);
	    
	    // State update based on the percept value and the last action
	    state.updateDirection();
	    state.updatePosition((DynamicPercept)percept);
	    if (bump) {
			switch (state.agent_direction) {
			case MyAgentState.NORTH:
				state.updateWorld(state.agent_x_position,state.agent_y_position-1,state.WALL);
				break;
			case MyAgentState.EAST:
				state.updateWorld(state.agent_x_position+1,state.agent_y_position,state.WALL);
				break;
			case MyAgentState.SOUTH:
				state.updateWorld(state.agent_x_position,state.agent_y_position+1,state.WALL);
				break;
			case MyAgentState.WEST:
				state.updateWorld(state.agent_x_position-1,state.agent_y_position,state.WALL);
				break;
			}
	    }
	    if (dirt)
	    	state.updateWorld(state.agent_x_position,state.agent_y_position,state.DIRT);
	    else
	    	state.updateWorld(state.agent_x_position,state.agent_y_position,state.CLEAR);
	    
	    state.printWorldDebug();
		
		switch (currentGoal) {
		case GOAL_GOHOME:
			System.out.println("Current Goal: GOAL_GOHOME");
			break;
		case GOAL_FINDPERIMETER:
			System.out.println("Current Goal: GOAL_FINDPERIMETER");
			break;
		case GOAL_SEARCHWORLD:
			System.out.println("Current Goal: GOAL_SEARCHWORLD");
			break;
		case GOAL_FINISH:
			System.out.println("Current Goal: GOAL_FINISH");
			break;
		}
	    
	    if (bump)
	    	actions.clear();
	    
	    if (dirt)
	    	actions.add(LIUVacuumEnvironment.ACTION_SUCK);

		if (actions.isEmpty() && gettingAroundObstacle) {
			if (home && (currentGoal == GOAL_GOHOME || currentGoal == GOAL_FINISH)) {
				gettingAroundObstacle = false;
			} else {
		    	getAroundObstacle();
			}
		}
		
	    // Check if there are still actions to be performed
	    if (!actions.isEmpty()) {
	    	Action a = actions.remove(0);
	    	state.agent_last_action = a;
	    	return a;
	    }

		// If no actions are buffered, check goals for the next action to be performed
		if (currentGoal == GOAL_GOHOME) {
			// Go north to the top wall, then go west
			if (home) {
				System.out.println("===================================================================\nCHANGING GOAL TO GOAL_FINDPERIMETER");
				actions.clear();
				bump = false;
				
				currentGoal = GOAL_FINDPERIMETER;
				turnEast();
			} else {
				goHome();
			}
		}
		if (currentGoal == GOAL_FINDPERIMETER) {
			// Walk around the perimeter, find the max X and Y values
			if (home) {
				if (findPerimeterHasMoved && state.agent_direction == MyAgentState.NORTH) {
					// if home, and has moved at least once
					System.out.println("===================================================================\nCHANGING GOAL TO GOAL_SEARCHWORLD");
					actions.clear();
					bump = false;
					
					currentGoal = GOAL_SEARCHWORLD;
					find_max_x_and_y();
					find_max_and_min_y_current_row();
				} else {
					findPerimeter(); // Should only be called from here once
				}
			} else {
				// If not home, the agent has moved
				// Continue across the perimeter until it gets back home
				findPerimeterHasMoved = true;
				findPerimeter();
			}
		}
		if (currentGoal == GOAL_SEARCHWORLD) {
			// Snake scan the entire area
			System.out.println("maxX: " + maxX + " maxY: " + maxY);
			System.out.println("maxYCurrentRow: " + maxYCurrentRow);
			System.out.println("minYCurrentRow: " + minYCurrentRow);
			
			if (currentRow == maxX - 2 && currentRowClean()) {
				System.out.println("===================================================================\nCHANGING GOAL TO GOAL_FINISH");
				actions.clear();
				bump = false;
				
				currentGoal = GOAL_FINISH;
			} else {
				searchWorld();
			}
		}
		if (currentGoal == GOAL_FINISH) {
			// Same as GOHOME
			if (home) {
				System.out.println("Simulation finished");
		    	actions.add(NoOpAction.NO_OP); // Simulation finished
			} else {
				goHome();
			}
		}
		
		if (actions.isEmpty() && gettingAroundObstacle) {
	    	getAroundObstacle();
		}
		
	    // Check if there are still actions to be performed
	    if (!actions.isEmpty()) {
	    	Action a = actions.remove(0);
	    	state.agent_last_action = a;
	    	return a;
	    } else {
	    	System.out.println("SHOULD NEVER BE REACHED.1");
	    	return NoOpAction.NO_OP;
	    }
	}
	
	public void goHome() {
		if (bump) {
			// Walk around the obstacle
			if (state.agent_direction == MyAgentState.WEST) {
				setGetAroundObstacle(HAND_LEFT);
			} else {
				setGetAroundObstacle(HAND_RIGHT);
			}
		} else if (state.agent_y_position != 1) {
			moveNorth();
		} else {
			moveWest();
		}
	}
	
	public void findPerimeter() {
		if (bump) {
			moveRight();
		} else if (hasWallOnLeft()) {
			moveForward();
		} else {
			moveLeft();
		}	
	}
	
	public void searchWorld() {
		System.out.println("CurDir: " + currentDirection + " CurRow: " + currentRow);
		
		// If found currentRow
		if (goingToCurrentRowDirection != -1 && state.agent_y_position == currentRow) {
			goingToCurrentRowDirection = -1;
		}
		
		if (bump) {
			// If bumped while going to current row
			if (goingToCurrentRowDirection != -1) {
				moveToCurrentRow();
			} else if ((currentDirection == MyAgentState.EAST && state.agent_x_position == maxYCurrentRow) ||
					   (currentDirection == MyAgentState.WEST && state.agent_x_position == minYCurrentRow)) {
				// If outer wall, increase currentRow and move down to it. Change direction
				System.out.println("ROW " + currentRow + " FINISHED");
				if (!currentRowClean()) { actions.add(NoOpAction.NO_OP); }
				goingToCurrentRowDirection = currentDirection;
				currentDirection = ((currentDirection == MyAgentState.EAST) ? MyAgentState.WEST : MyAgentState.EAST);
				currentRow++;
				find_max_and_min_y_current_row();
				moveSouth();
			} else {
				if (currentDirection == MyAgentState.EAST)
					setGetAroundObstacle(HAND_LEFT);
				else
					setGetAroundObstacle(HAND_RIGHT);
			}
		} else {
			if (goingToCurrentRowDirection != -1) {
				moveToCurrentRow();
			} else {
				if (currentDirection == MyAgentState.EAST && state.agent_direction != MyAgentState.EAST) {
					turnEast();
				} else if (currentDirection == MyAgentState.WEST && state.agent_direction != MyAgentState.WEST) {
					turnWest();
				} else {
					moveForward();
				}
			}
		}
	}
	
	public void moveToCurrentRow() {
		System.out.println("moveToCurrentRow()");

		if (goingToCurrentRowDirection == MyAgentState.EAST) {
			// Use left hand
			if (bump) {
				moveRight();
			} else if (hasWallOnLeft()) {
				moveForward();
			} else {
				moveLeft();
			}	
		} else {
			// Use right hand
			if (bump) {
				moveLeft();
			} else if (hasWallOnRight()) {
				moveForward();
			} else {
				moveRight();
			}	
		}
	}
	
	public void find_max_x_and_y() {
		for (int i = 2; i < 29; i++) {
			for (int j = 2; j < 29; j++) {
				// Check if the block you are on is a wall, and if the block after it is unknown
				// if it is unknown, it means it is out of bounds since the agent walked the perimeter before this function is called
				// Check on all rows and columns, since there might be protrusions in the wall attached to the outer one
				if (state.world[i][j] == state.WALL && state.world[i + 1][j] == state.UNKNOWN && i > maxX) {
					maxX = i;
				}
				
				if (state.world[j][i] == state.WALL && state.world[j][i + 1] == state.UNKNOWN && i > maxY) {
					maxY = i;
				}
			}
		}
	}
	
	public void find_max_and_min_y_current_row() {
		maxYCurrentRow = maxY - 1;
		minYCurrentRow = 1;
		
		for (int i = maxY - 1; i > 0; i--) {
			if (state.world[i][currentRow] != state.WALL && state.world[i][currentRow] != state.UNKNOWN) {
				maxYCurrentRow = i;
				break;
			}
		}
		
		for (int i = 1; i < maxYCurrentRow; i++) {
			if (state.world[i][currentRow] != state.WALL && state.world[i][currentRow] != state.UNKNOWN) {
				minYCurrentRow = i;
				break;
			}
		}
	}
	
	public boolean currentRowClean() {
		for (int i = minYCurrentRow; i < maxYCurrentRow; i++) {
			if (state.world[i][currentRow] == state.UNKNOWN || state.world[i][currentRow] == state.DIRT) {
				return false;
			}
		}
		return true;
	}
	
	public void getGetToPoint(int direction, int x, int y) {
		switch (direction) {
		case MyAgentState.NORTH:
			for (int i = y - 1; i > 0; i--) {
				if (state.world[x][i] != state.WALL) {
					getTo = new Point(x, i);
					return;
				}
			}
			// If hit northern wall
			getTo = new Point(x, -1);
			break;
		case MyAgentState.EAST:
			for (int i = x + 1; i < maxX; i++) {
				if (state.world[i][y] != state.WALL) {
					getTo = new Point(i, y);
					return;
				}
			}
			// If hit eastern wall
			getTo = new Point(-1, y);
			break;
		case MyAgentState.SOUTH:
			for (int i = y + 1; i < maxY; i++) {
				if (state.world[x][i] != state.WALL) {
					getTo = new Point(x, i);
					return;
				}
			}
			// If hit southern wall
			getTo = new Point(x, -1);
			break;
		case MyAgentState.WEST:
			for (int i = x - 1; i > 0; i--) {
				if (state.world[i][y] != state.WALL) {
					getTo = new Point(i, y);
					return;
				}
			}
			// If hit western wall
			getTo = new Point(-1, y);
			break;
		}
	}
	
	public void setGetAroundObstacle(boolean hand) {
		System.out.println("setGetAroundObstacle(" + state.agent_direction + ")");
		
		actions.clear();
		bump = false;
		gettingAroundObstacle = true;
		getAroundDirection = state.agent_direction;
		
		leftHand = hand;
		
		orig = new Point(state.agent_x_position, state.agent_y_position);
		
		getGetToPoint(state.agent_direction, state.agent_x_position, state.agent_y_position);
		System.out.println("getTo: " + getTo.x + " " + getTo.y);
		
		if (leftHand) {
			// Start with the agents left side facing the wall it's trying to pass
			switch(state.agent_direction) {
			case MyAgentState.NORTH:
				turnEast();
				break;
			case MyAgentState.EAST:
				turnSouth();
				break;
			case MyAgentState.SOUTH:
				turnWest();
				break;
			case MyAgentState.WEST:
				turnNorth();
				break;
			}
		} else {
			// Start with the agents right side facing the wall it's trying to pass
			switch(state.agent_direction) {
			case MyAgentState.NORTH:
				turnWest();
				break;
			case MyAgentState.EAST:
				turnNorth();
				break;
			case MyAgentState.SOUTH:
				turnEast();
				break;
			case MyAgentState.WEST:
				turnSouth();
				break;
			}
		}
	}
	
	public void getAroundObstacle() {
		System.out.println("getAroundObstacle(" + getAroundDirection + ")");
		System.out.println("curY: " + state.agent_y_position + " getTo.y: " + getTo.y + " orig.y: " + orig.y);
		System.out.println("curx: " + state.agent_x_position + " getTo.x: " + getTo.x + " orig.x: " + orig.x);
		
		// If the agent has moved, check if the agent got passed the obstacle
		if (orig.x != state.agent_x_position || orig.y != state.agent_y_position) {
			System.out.println("Agent has moved");
			
			switch(getAroundDirection) {
			case MyAgentState.NORTH:
				if ((state.agent_y_position == 1 && getTo.x == -1) ||
					(state.agent_y_position <= orig.y && state.agent_x_position == getTo.x && state.agent_y_position == getTo.y)) {
					// If got past obstacle, or found upper wall
					gettingAroundObstacle = false;
					turnNorth();
					return;
				}
				break;
			case MyAgentState.EAST:
				if ((state.agent_x_position == maxX && getTo.y == -1) ||
				    (state.agent_x_position >= orig.x && state.agent_y_position == getTo.y && state.agent_x_position == getTo.x)) {
					// If got past obstacle, or found right wall
					gettingAroundObstacle = false;
					turnEast();
					return;
				}
				break;
			case MyAgentState.SOUTH:
				if ((state.agent_y_position == maxY && getTo.x == -1) ||
				    (state.agent_y_position >= orig.y && state.agent_x_position == getTo.x && state.agent_y_position == getTo.y)) {
					// If got past obstacle, or found lower wall
					gettingAroundObstacle = false;
					turnSouth();
					return;
				}
				break;
			case MyAgentState.WEST:
				if ((state.agent_x_position == 1 && getTo.y == -1) ||
				    (state.agent_x_position <= orig.x && state.agent_y_position == getTo.y && state.agent_x_position == getTo.x)) {
					// If got past obstacle, or found left wall
					gettingAroundObstacle = false;
					turnWest();
					return;
				}
				break;
			}
		} else {
			System.out.println("Agent has not moved!");
		}
		
		System.out.println("Exit location not found");
		
		if (bump) {
			System.out.println("BUMP!");
			// If getTo is unreachable, find the next available spot
			if (isUnobtainable(getTo)) {
				getGetToPoint(getAroundDirection, orig.x, orig.y);
			}
			
			// If bump location was getTo, change getTo to next block
			switch (state.agent_direction) {
			case MyAgentState.NORTH:
				if (state.agent_x_position == getTo.x && state.agent_y_position - 1 == getTo.y) {
					getGetToPoint(getAroundDirection, orig.x, orig.y);
				}
				break;
			case MyAgentState.EAST:
				if (state.agent_x_position + 1 == getTo.x && state.agent_y_position == getTo.y) {
					getGetToPoint(getAroundDirection, orig.x, orig.y);
				}
				break;
			case MyAgentState.SOUTH:
				if (state.agent_x_position == getTo.x && state.agent_y_position + 1 == getTo.y) {
					getGetToPoint(getAroundDirection, orig.x, orig.y);
				}
				break;
			case MyAgentState.WEST:
				if (state.agent_x_position - 1 == getTo.x && state.agent_y_position == getTo.y) {
					getGetToPoint(getAroundDirection, orig.x, orig.y);
				}
				break;
			}
			
			// Avoid bumped obstacle
			if (leftHand) {
				moveRight();
			} else {
				moveLeft();
			}
		} else if (leftHand && !hasWallOnLeft()) {
			moveLeft();
		} else if (!leftHand && !hasWallOnRight()) {
			moveRight();
		} else {
			moveForward();
		}
	}
	
	// Checks if the point is unobtainable, and marks it as a wall if it is
	// Could be improved to use pattern recognition and catch n number of enclosed "open" spaces (Euler Characteristic?)
	public boolean isUnobtainable(Point p) {
		if (p.x == -1 || p.y == -1) { return false; }
		
		if ((state.world[p.x - 1][p.y] == state.WALL || p.x - 1 == 0) &&
			(state.world[p.x + 1][p.y] == state.WALL || p.x + 1 == maxX) &&
			(state.world[p.x][p.y - 1] == state.WALL || p.y - 1 == 0) &&
			(state.world[p.x][p.y + 1] == state.WALL || p.y + 1 == maxY)) {
			
			state.updateWorld(p.x, p.y, state.WALL);
			return true;
		}
		
		return false;
	}
	
	public boolean hasWallOnLeft() {
		switch(state.agent_direction) {
		case MyAgentState.NORTH:
			if (state.world[state.agent_x_position - 1][state.agent_y_position] == state.WALL)
				return true;
			break;
		case MyAgentState.EAST:
			if (state.world[state.agent_x_position][state.agent_y_position - 1] == state.WALL)
				return true;
			break;
		case MyAgentState.SOUTH:
			if (state.world[state.agent_x_position + 1][state.agent_y_position] == state.WALL)
				return true;
			break;
		case MyAgentState.WEST:
			if (state.world[state.agent_x_position][state.agent_y_position + 1] == state.WALL)
				return true;
			break;
		}
		return false;
	}
	
	public boolean hasWallOnRight() {
		switch(state.agent_direction) {
		case MyAgentState.NORTH:
			if (state.world[state.agent_x_position + 1][state.agent_y_position] == state.WALL)
				return true;
			break;
		case MyAgentState.EAST:
			if (state.world[state.agent_x_position][state.agent_y_position + 1] == state.WALL)
				return true;
			break;
		case MyAgentState.SOUTH:
			if (state.world[state.agent_x_position - 1][state.agent_y_position] == state.WALL)
				return true;
			break;
		case MyAgentState.WEST:
			if (state.world[state.agent_x_position][state.agent_y_position - 1] == state.WALL)
				return true;
			break;
		}
		
		return false;
	}
	
	public void turnNorth() {
		switch(state.agent_direction) {
		case MyAgentState.EAST:
			actions.add(LIUVacuumEnvironment.ACTION_TURN_LEFT);
			break;
		case MyAgentState.SOUTH:
			actions.add(LIUVacuumEnvironment.ACTION_TURN_LEFT);
			actions.add(LIUVacuumEnvironment.ACTION_TURN_LEFT);
			break;
		case MyAgentState.WEST:
			actions.add(LIUVacuumEnvironment.ACTION_TURN_RIGHT);
			break;
		}
	}
	
	public void turnEast() {
		switch(state.agent_direction) {
		case MyAgentState.NORTH:
			actions.add(LIUVacuumEnvironment.ACTION_TURN_RIGHT);
			break;
		case MyAgentState.SOUTH:
			actions.add(LIUVacuumEnvironment.ACTION_TURN_LEFT);
			break;
		case MyAgentState.WEST:
			actions.add(LIUVacuumEnvironment.ACTION_TURN_LEFT);
			actions.add(LIUVacuumEnvironment.ACTION_TURN_LEFT);
			break;
		}
	}
	
	public void turnSouth() {
		switch(state.agent_direction) {
		case MyAgentState.NORTH:
			actions.add(LIUVacuumEnvironment.ACTION_TURN_LEFT);
			actions.add(LIUVacuumEnvironment.ACTION_TURN_LEFT);
			break;
		case MyAgentState.EAST:
			actions.add(LIUVacuumEnvironment.ACTION_TURN_RIGHT);
			break;
		case MyAgentState.WEST:
			actions.add(LIUVacuumEnvironment.ACTION_TURN_LEFT);
			break;
		}
	}
	
	public void turnWest() {
		switch(state.agent_direction) {
		case MyAgentState.NORTH:
			actions.add(LIUVacuumEnvironment.ACTION_TURN_LEFT);
			break;
		case MyAgentState.EAST:
			actions.add(LIUVacuumEnvironment.ACTION_TURN_LEFT);
			actions.add(LIUVacuumEnvironment.ACTION_TURN_LEFT);
			break;
		case MyAgentState.SOUTH:
			actions.add(LIUVacuumEnvironment.ACTION_TURN_RIGHT);
			break;
		}
	}
	
	public void moveForward() {
		actions.add(LIUVacuumEnvironment.ACTION_MOVE_FORWARD);
	}
	
	public void moveNorth() {
		turnNorth();
		moveForward();
	}
	
	public void moveEast() {
		turnEast();
		moveForward();
	}
	
	public void moveSouth() {
		turnSouth();
		moveForward();
	}
	
	public void moveWest() {
		turnWest();
		moveForward();
	}
	
	public void moveRight() {
		actions.add(LIUVacuumEnvironment.ACTION_TURN_RIGHT);
		moveForward();
	}
	
	public void moveLeft() {
		actions.add(LIUVacuumEnvironment.ACTION_TURN_LEFT);
		moveForward();
	}
}

class Point {
	public static final Point NONE = new Point(-1, -1);
	
	public int x;
	public int y;
	
	Point(int x_i, int y_i) {
		x = x_i;
		y = y_i;
	}
}


public class MyVacuumAgent extends AbstractAgent {
    public MyVacuumAgent() {
    	super(new MyAgentProgram());
	}
}