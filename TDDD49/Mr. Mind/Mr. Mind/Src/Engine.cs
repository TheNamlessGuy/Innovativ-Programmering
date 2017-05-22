using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Mr.Mind.Src
{
    class Engine
    {
        private Rules rules;

        public short currentTurn;
        public short currentRowNumber;
        public short currentColor;
        public short maxCols;

        public bool isFirstTurn;

        public short[] code;
        public short[] currentRow;
        public List<short> currentPegs;

        private short maxRows;

        private bool firstTimeShowCode;

        private Codemaker codemaker;
        private Codebreaker codebreaker;

        public Engine(short rows, short columns, bool codemakerAI, bool codebreakerAI)
        {
            FileManager.initSave(columns, rows, codemakerAI, codebreakerAI);
            init(rows, columns, codemakerAI, codebreakerAI);
        }

        /*
         * Called when activating previous instance
         */
        public Engine(short colSize, short rowSize, short[] code, short[] currentRow, short[] currentPegs, short currentTurn, short currentRowNumber,
            short currentColor, bool isFirstTurn, bool codemakerAI, bool codebreakerAI, bool codebreakerAIfirstTurn, short[] codebreakerAIcurGuess)
        {
            init(rowSize, colSize, codemakerAI, codebreakerAI);
            this.code = code;
            this.currentTurn = currentTurn;
            this.currentRowNumber = currentRowNumber;
            this.currentColor = currentColor;
            this.isFirstTurn = isFirstTurn;
            this.currentRow = currentRow;

            for (int whitePegs = 0; whitePegs < currentPegs[0]; whitePegs++)
                this.currentPegs.Add(Rules.PEG_WHITE);
            for (int orangePegs = 0; orangePegs < currentPegs[1]; orangePegs++)
                this.currentPegs.Add(Rules.PEG_ORANGE);

            if (codebreakerAI)
                codebreaker = new AI.CodebreakerAI(this, codebreakerAIfirstTurn, codebreakerAIcurGuess);
        }

        public void init(short rows, short columns, bool codemakerAI, bool codebreakerAI)
        {
            rules = new Rules();

            currentTurn = Rules.TURN_CODEMAKER;
            currentRow = new short[columns];
            code = new short[columns];
            maxRows = rows;
            maxCols = columns;
            currentColor = Rules.COLOR_RED;
            isFirstTurn = true;
            firstTimeShowCode = true;

            currentPegs = new List<short>();

            for (int i = 0; i < currentRow.Length; i++)
                currentRow[i] = Rules.COLOR_NONE;
            for (int i = 0; i < code.Length; i++)
                code[i] = Rules.COLOR_NONE;

            if (codemakerAI)
                codemaker = new AI.CodemakerAI(this);
            else
                codemaker = new Codemaker(this);
            codemaker.firstTurn();

            if (codebreakerAI)
                codebreaker = new AI.CodebreakerAI(this);
            else
                codebreaker = new Codebreaker(this);
        }

        /*
         * Codebreaker
         */
        public bool onColorBoxClick(short color, bool useRules = true)
        {
            clearErrorMessage();
            if (useRules && !rules.canChangeColor(currentTurn, isFirstTurn))
            {
                ((PlayWindow)System.Windows.Application.Current.MainWindow).errorMessage.Text = "Can't pick color!";
                return false;
            }
            currentColor = color;
            System.Windows.Media.Imaging.BitmapImage bImg = new System.Windows.Media.Imaging.BitmapImage();
            bImg.BeginInit();
            bImg.UriSource = new Uri("Res/Images/Balls/" + color.ToString() + ".png", UriKind.Relative);
            bImg.EndInit();
            ((PlayWindow)System.Windows.Application.Current.MainWindow).currentColor.Source = bImg;
            FileManager.currentColorSave(color);
            return true;
        }

        public bool onPlaceColorDown(short x, short y)
        {
            clearErrorMessage();
            if (!rules.canPlaceColor(currentTurn, currentRowNumber, y, isFirstTurn))
            {
                ((PlayWindow)System.Windows.Application.Current.MainWindow).errorMessage.Text = "Can't place color!";
                return false;
            }

            if (y == Rules.SPECIAL_CODEYCOORD)
            {
                code[x] = currentColor;
                ((System.Windows.Controls.Image)((PlayWindow)System.Windows.Application.Current.MainWindow).MainGrid.FindName("_" + x.ToString() + "C" + y.ToString())).Source = ((PlayWindow)System.Windows.Application.Current.MainWindow).currentColor.Source;
            }
            else
            {
                currentRow[x] = currentColor;
                ((System.Windows.Controls.Image)((PlayWindow)System.Windows.Application.Current.MainWindow).MainGrid.FindName("_" + x.ToString() + "C" + y.ToString())).Source = ((PlayWindow)System.Windows.Application.Current.MainWindow).currentColor.Source;
            }
            FileManager.placeBallSave(x, y, currentColor);
            return true;
        }

        /*
         * Codemaker
         */
        public bool onPlacePegDown(short row, short x, short y, short type)
        {
            clearErrorMessage();
            if (!rules.canPlacePeg(currentTurn, row, currentRowNumber))
            {
                ((PlayWindow)System.Windows.Application.Current.MainWindow).errorMessage.Text = "Can't place peg!";
                return false;
            }
            currentPegs.Add(type);
            System.Windows.Media.Imaging.BitmapImage bImg = new System.Windows.Media.Imaging.BitmapImage();
            bImg.BeginInit();
            bImg.UriSource = new Uri("Res/Images/Pegs/" + type.ToString() + ".png", UriKind.Relative);
            bImg.EndInit();

            System.Windows.Controls.Image peg = (System.Windows.Controls.Image)((PlayWindow)System.Windows.Application.Current.MainWindow).MainGrid.FindName("_" + x.ToString() + "P" + y.ToString() + "P" + row.ToString());
            if (((System.Windows.Media.Imaging.BitmapImage)peg.Source).UriSource.OriginalString == "Res/Images/Pegs/0.png")
                currentPegs.Remove(0);
            else if (((System.Windows.Media.Imaging.BitmapImage)peg.Source).UriSource.OriginalString == "Res/Images/Pegs/1.png")
                currentPegs.Remove(1);
            peg.Source = bImg;
            FileManager.placePegSave(row, type);
            return true;
        }

        public bool onShowCode()
        {
            clearErrorMessage();
            if (!rules.canShowCode(currentTurn))
            {
                ((PlayWindow)System.Windows.Application.Current.MainWindow).errorMessage.Text = "Can't show code!";
                return false;
            }

            if (((PlayWindow)System.Windows.Application.Current.MainWindow).codePopup.Visibility == System.Windows.Visibility.Hidden)
                showCode(true);
            else
                showCode(false);
            return true;
        }

        public bool changeColorFirstTurn(short color)
        {
            currentColor = color;
            FileManager.currentColorSave(color);
            return true;
        }

        public bool placeColorDownFirstTurn(short x)
        {
            if (!rules.canPlaceColor(currentTurn, currentRowNumber, Rules.SPECIAL_CODEYCOORD, true))
            {
                return false;
            }
            code[x] = currentColor;
            FileManager.placeBallSave(x, Rules.SPECIAL_CODEYCOORD, currentColor);
            return true;
        }
        /*
         * General
         */

        public void won(short winner)
        {
            if (winner == Rules.WIN_CODEBREAKER)
                ((PlayWindow)System.Windows.Application.Current.MainWindow).popupMessage.Text = "Codebreaker won!";
            else
                ((PlayWindow)System.Windows.Application.Current.MainWindow).popupMessage.Text = "Codemaker won!";
            showCode(true);
            ((PlayWindow)System.Windows.Application.Current.MainWindow).showCode.IsEnabled = false;
            ((PlayWindow)System.Windows.Application.Current.MainWindow).nextTurn.Content = "Main Menu";
            ((PlayWindow)System.Windows.Application.Current.MainWindow).nextTurn.Click += goToMainMenu;
        }

        public void goToMainMenu(object sender, System.Windows.RoutedEventArgs e)
        {
            FileManager.DeleteSave();
            System.Windows.Window win = new MainWindow();
            System.Windows.Application.Current.MainWindow.Close();
            System.Windows.Application.Current.MainWindow = win;
            win.Show();
        }

        public bool onChangeTurnClick()
        {
            clearErrorMessage();
            if (!rules.canChangeTurn(currentTurn, currentRow)) {
                ((PlayWindow)System.Windows.Application.Current.MainWindow).errorMessage.Text = "Can't change turn!";
                return false;
            }

            if (currentTurn == Rules.TURN_CODEMAKER)
            {
                ((PlayWindow)System.Windows.Application.Current.MainWindow).currentTurn.Text = "Codebreaker's turn";
                codebreaker.onChangeTurnClick();
            }
            else
            {
                ((PlayWindow)System.Windows.Application.Current.MainWindow).currentTurn.Text = "Codemaker's turn";
                codemaker.onChangeTurnClick();
            }

            onColorBoxClick(Rules.COLOR_RED, false);
            if (isFirstTurn)
                FileManager.isFirstTurnSave();
            isFirstTurn = false;
            FileManager.turnSave(currentTurn, currentRowNumber);
            return true;
        }

        public bool checkWinningConditions()
        {
            short response = rules.hasWon(currentRowNumber, maxRows, currentPegs, maxCols);
            if (response != Rules.WIN_NONE)
            {
                won(response);
                return true;
            }
            return false;
        }

        public void resetForNextTurn()
        {
            for (int i = 0; i < currentRow.Length; i++)
                currentRow[i] = Rules.COLOR_NONE;
            currentPegs.Clear();
        }
        
        public void showCode(bool show)
        {
            if (show)
            {
                if (firstTimeShowCode)
                {
                    firstTimeShowCode = false;
                    for (int i = 0; i < code.Length; i++)
                    {
                        System.Windows.Media.Imaging.BitmapImage bImg = new System.Windows.Media.Imaging.BitmapImage();
                        bImg.BeginInit();
                        bImg.UriSource = new Uri("Res/Images/Balls/" + code[i].ToString() + ".png", UriKind.Relative);
                        bImg.EndInit();
                        ((System.Windows.Controls.Image)((PlayWindow)System.Windows.Application.Current.MainWindow).MainGrid.FindName("_" + i.ToString() + "C" + Rules.SPECIAL_CODEYCOORD.ToString())).Source = bImg;
                    }
                }
                ((PlayWindow)System.Windows.Application.Current.MainWindow).codePopup.Visibility = System.Windows.Visibility.Visible;
                ((PlayWindow)System.Windows.Application.Current.MainWindow).showCode.Content = "Hide code";
            }
            else
            {
                ((PlayWindow)System.Windows.Application.Current.MainWindow).codePopup.Visibility = System.Windows.Visibility.Hidden;
                ((PlayWindow)System.Windows.Application.Current.MainWindow).showCode.Content = "Show code";
            }
            FileManager.showColorSave(show);
        }

        public void clearErrorMessage()
        {
            ((PlayWindow)System.Windows.Application.Current.MainWindow).errorMessage.Text = "";
        }
    }
}
