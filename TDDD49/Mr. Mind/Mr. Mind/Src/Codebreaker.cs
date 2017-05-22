using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Mr.Mind.Src
{
    class Codebreaker
    {
        private Engine engine;

        public Codebreaker() {}
        public Codebreaker(Engine engine)
        {
            this.engine = engine;
        }
        /*
         * Called when codebreakers turn is the turn to come up next.
         */
        public virtual void onChangeTurnClick()
        {
            engine.checkWinningConditions();
            engine.currentTurn = Rules.TURN_CODEBREAKER;

            //om den plussar rownumber när codemaker slagit in koden så startar den på 1
            if (engine.isFirstTurn)
            {
                engine.showCode(false);
                engine.isFirstTurn = false;
            }
            else
                engine.currentRowNumber++; //When codebreaker's turn starts
            engine.resetForNextTurn();
        }
    }
}
