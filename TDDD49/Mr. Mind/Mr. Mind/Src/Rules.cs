using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Mr.Mind.Src
{
    class Rules
    {
        public static short COLOR_NONE = -1;
        public static short COLOR_RED = 0;
        public static short COLOR_GREEN = 1;
        public static short COLOR_BLUE = 2;
        public static short COLOR_YELLOW = 3;
        public static short COLOR_PURPLE = 4;
        public static short COLOR_BLACK = 5;

        public static short TURN_CODEMAKER = 0;
        public static short TURN_CODEBREAKER = 1;

        public static short PEG_NONE = -1; // Needed?
        public static short PEG_WHITE = 0; // Right color, right place
        public static short PEG_ORANGE = 1; // Right color, wrong place

        public static short SPECIAL_CODEYCOORD = 13;

        public static short WIN_NONE = 0;
        public static short WIN_CODEMAKER = 1;
        public static short WIN_CODEBREAKER = 2;

        // Constructor
        public Rules() {}
        // "Can" functions
        public bool canChangeColor(short currentTurn, bool isFirstTurn)
        {
            if (currentTurn == Rules.TURN_CODEBREAKER ||
                (currentTurn == Rules.TURN_CODEMAKER && isFirstTurn))
                return true;
            return false;
        }

        public bool canPlaceColor(short currentTurn, short currentRowNumber, short y, bool isFirstTurn)
        {
            if (currentTurn == Rules.TURN_CODEBREAKER &&
                currentRowNumber == y) return true;
            else if (currentTurn == Rules.TURN_CODEMAKER && isFirstTurn) return true;
            return false;
        }

        public bool canChangeTurn(short currentTurn, short[] currentRow)
        {
            if (currentTurn == Rules.TURN_CODEBREAKER &&
                currentRow.Contains(Rules.COLOR_NONE))
                    return false;
            return true;
        }

        public bool canPlacePeg(short currentTurn, short row, short currentRowNumber)
        {
            if (currentTurn == Rules.TURN_CODEMAKER &&
                currentRowNumber == row) return true;
            return false;
        }
        public bool canShowCode(short currentTurn)
        {
            if (currentTurn == Rules.TURN_CODEMAKER) return true;
            return false;
        }

        public short hasWon(short currentRowNumber, short maxRows, List<short> currentPegs, short maxCols)
        {
            short whiteAmount = 0;
            foreach (short peg in currentPegs)
                if (peg == Rules.PEG_WHITE)
                    whiteAmount++;

            if (whiteAmount == maxCols) return Rules.WIN_CODEBREAKER;
            if (currentRowNumber == maxRows - 1) return Rules.WIN_CODEMAKER;
            return Rules.WIN_NONE;
        }
    }
}
