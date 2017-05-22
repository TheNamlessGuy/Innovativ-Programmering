using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Mr.Mind.Src.AI
{
    class CodebreakerAI: Src.Codebreaker
    {
        private Engine engine;
        private bool firstTime;
        private short[] currentGuess;

        public CodebreakerAI(Engine engine)
        {
            this.engine = engine;
            firstTime = true;
        }

        public CodebreakerAI(Engine engine, bool firstTime, short[] currentGuess)
        {
            this.engine = engine;
            this.firstTime = firstTime;
            this.currentGuess = currentGuess;
        }
        public override void onChangeTurnClick()
        {
            if (engine.checkWinningConditions())
                return;
            engine.currentTurn = Rules.TURN_CODEBREAKER;

            if (firstTime)
            {
                engine.showCode(false);
                engine.isFirstTurn = false;

                firstTime = false;

                currentGuess = new short[engine.maxCols];
                for (short i = 0; i < currentGuess.Length; i++)
                    currentGuess[i] = Rules.COLOR_RED;

                guess();
                FileManager.AIfirstTurnSave();
            }
            else
            {
                engine.currentRowNumber++;
                short whiteCount = 0, orangeCount = 0;
                for (int i = 0; i < engine.currentPegs.Count(); i++)
                    if (engine.currentPegs.ElementAt(i) == Rules.PEG_WHITE)
                        whiteCount++;
                    else
                        orangeCount++;

                for (int i = (whiteCount + orangeCount); i < currentGuess.Length; i++)
                {
                    currentGuess[i]++;
                }

                if (orangeCount > 1)
                {
                    Random rnd = new Random();
                    for (int i = 0; i < (whiteCount + orangeCount); i++)
                    {
                        short newSpot = Convert.ToInt16(rnd.Next(0, (whiteCount + orangeCount - 1)));
                        short temp = currentGuess[newSpot];
                        currentGuess[newSpot] = currentGuess[i];
                        currentGuess[i] = temp;
                    }
                }

                guess();
            }
            engine.onChangeTurnClick();
        }

        public void guess()
        {
            FileManager.AIcurCodeSave(currentGuess);
            for (short i = 0; i < currentGuess.Length; i++)
            {
                engine.onColorBoxClick(currentGuess[i]);
                engine.onPlaceColorDown(i, engine.currentRowNumber);
            }
        }
    }
}
