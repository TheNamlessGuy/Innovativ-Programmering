using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Mr.Mind.Src.AI
{
    class CodemakerAI: Mr.Mind.Src.Codemaker
    {
        private Engine engine;

        public CodemakerAI(Engine engine)
        {
            this.engine = engine;
        }

        public override void firstTurn()
        {
            engine.currentTurn = Rules.TURN_CODEMAKER;
            chooseCode();

            engine.changeColorFirstTurn(Rules.COLOR_RED);
            engine.isFirstTurn = false;
            FileManager.isFirstTurnSave();
            engine.currentTurn = Rules.TURN_CODEBREAKER;
            FileManager.turnSave(engine.currentTurn, engine.currentRowNumber);
        }

        /*
         * Called when codemakers turn is the turn to come up next, and it is AI.
         */
        public override void onChangeTurnClick()
        {
            engine.currentTurn = Rules.TURN_CODEMAKER;
            placePegs();
            engine.onChangeTurnClick();
            engine.resetForNextTurn();
        }

        public void chooseCode()
        {
            Random random = new Random();
            for (short i = 0; i < 4; i++)
            {
                short color = (short)random.Next(0, 6); // Generate random number between 0-5, thus color
                engine.changeColorFirstTurn(color);
                engine.placeColorDownFirstTurn(i);
            }
            /*
            engine.changeColorFirstTurn(Rules.COLOR_BLACK);
            engine.placeColorDownFirstTurn(0);
            engine.changeColorFirstTurn(Rules.COLOR_BLACK);
            engine.placeColorDownFirstTurn(1);
            engine.changeColorFirstTurn(Rules.COLOR_BLACK);
            engine.placeColorDownFirstTurn(2);
            engine.changeColorFirstTurn(Rules.COLOR_RED);
            engine.placeColorDownFirstTurn(3);
            */
        }

        public void placePegs()
        {
            short[] codebackup = new short[engine.maxCols];
            for (short i = 0; i < engine.code.Length; i++)
                codebackup[i] = engine.code[i];

            short[] currentRowBackup = new short[engine.maxCols];
            for (short i = 0; i < engine.currentRow.Length; i++)
                currentRowBackup[i] = engine.currentRow[i];

            short x = 0, y = 0;
            for (int i = 0; i < currentRowBackup.Length; i++)
            {
                if (codebackup[i] == currentRowBackup[i])
                {
                    engine.onPlacePegDown(engine.currentRowNumber, x, y, Rules.PEG_WHITE);
                    codebackup[i] = Rules.COLOR_NONE;
                    currentRowBackup[i] = Rules.COLOR_NONE;

                    x++;
                    if (x > 1)
                    {
                        x = 0;
                        y++;
                    }
                }
            }

            for (int i = 0; i < currentRowBackup.Length; i++)
            {
                if (currentRowBackup[i] != Rules.COLOR_NONE && codebackup.Contains(currentRowBackup[i]))
                {
                    engine.onPlacePegDown(engine.currentRowNumber, x, y, Rules.PEG_ORANGE);
                    for (short a = 0; a < codebackup.Length; a++)
                        if (codebackup[a] == engine.currentRow[i])
                        {
                            codebackup[a] = Rules.COLOR_NONE;
                            break;
                        }
                    currentRowBackup[i] = Rules.COLOR_NONE;

                    x++;
                    if (x > 1)
                    {
                        x = 0;
                        y++;
                    }
                }
            }
        }
    }
}
