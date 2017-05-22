using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Mr.Mind.Src
{
    class Codemaker
    {
        private Engine engine;

        public Codemaker() {}
        public Codemaker(Engine engine)
        {
            this.engine = engine;
        }
        /*
         * Called when codemakers turn is the turn to come up next.
         */
        public virtual void onChangeTurnClick()
        {
            engine.currentTurn = Rules.TURN_CODEMAKER;
            engine.resetForNextTurn();
        }

        public virtual void firstTurn() {}
    }
}
