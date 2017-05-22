using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Shapes;

namespace Mr.Mind
{
    public partial class PlayWindow : Window
    {
        private double rowOffset;
        private double colOffset;
        private double pegRowOffset;

        private Src.Engine engine;

        public PlayWindow(bool CodemakerAI, bool CodebreakerAI, short colSize, short rowSize)
        {
            InitializeComponent();

            engine = new Src.Engine(rowSize, colSize, CodemakerAI, CodebreakerAI);

            BitmapImage ballBit = new BitmapImage();
            ballBit.BeginInit();
            ballBit.UriSource = new Uri("Res/Images/Board/ballHole.png", UriKind.Relative);
            ballBit.EndInit();

            BitmapImage pegBit = new BitmapImage();
            pegBit.BeginInit();
            pegBit.UriSource = new Uri("Res/Images/Board/pegHole.png", UriKind.Relative);
            pegBit.EndInit();

            colOffset = codePopup.Width / colSize;

            printPopup(colSize, ballBit);
            if (CodemakerAI)
            {
                codePopup.Visibility = System.Windows.Visibility.Hidden;
                currentTurn.Text = "Codebreaker's Turn";
            }

            rowOffset = BallGrid.Height / rowSize;
            colOffset = BallGrid.Width / colSize;

            printBoard(rowSize, colSize, ballBit, pegBit);
        }

        /*
         * Called when activating previous instance
         */
        public PlayWindow(short colSize, short rowSize, short[] code, List<short[]> board, List<short[]> pegs, short currentTurn, short currentRowNumber,
            short currentColor, bool isFirstTurn, bool codemakerAI, bool codebreakerAI, bool showingCode, bool codebreakerAIfirstTurn, short[] codebreakerAIcurGuess)
        {
            InitializeComponent();

            engine = new Src.Engine(colSize, rowSize, code, board.ElementAt(currentRowNumber), pegs.ElementAt(currentRowNumber),
                currentTurn, currentRowNumber, currentColor, isFirstTurn, codemakerAI, codebreakerAI, codebreakerAIfirstTurn, codebreakerAIcurGuess);

            BitmapImage ballBit = new BitmapImage();
            ballBit.BeginInit();
            ballBit.UriSource = new Uri("Res/Images/Board/ballHole.png", UriKind.Relative);
            ballBit.EndInit();

            BitmapImage pegBit = new BitmapImage();
            pegBit.BeginInit();
            pegBit.UriSource = new Uri("Res/Images/Board/pegHole.png", UriKind.Relative);
            pegBit.EndInit();

            colOffset = codePopup.Width / colSize;

            printPopupFromFile(colSize, code, ballBit);
            if (codemakerAI)
            {
                codePopup.Visibility = System.Windows.Visibility.Hidden;
                this.currentTurn.Text = "Codebreaker's Turn";
            }

            rowOffset = BallGrid.Height / rowSize;
            colOffset = BallGrid.Width / colSize;

            printBoard(rowSize, colSize, ballBit, pegBit);
            setBoard(rowSize, colSize, board, pegs);
            
            BitmapImage currentBit = new BitmapImage();
            currentBit.BeginInit();
            currentBit.UriSource = new Uri("Res/Images/Balls/" + currentColor.ToString() + ".png", UriKind.Relative);
            currentBit.EndInit();
            this.currentColor.Source = currentBit;

            if (currentTurn == Src.Rules.TURN_CODEBREAKER)
                this.currentTurn.Text = "Codebreaker's Turn";
            else
                this.currentTurn.Text = "Codemaker's Turn";

            System.Diagnostics.Debug.WriteLine("AAAAAAAAAAAAAAAAAAAAAAAA " + showingCode.ToString());

            if (!showingCode)
                codePopup.Visibility = System.Windows.Visibility.Hidden;
        }

        void printPopup(int colSize, BitmapImage bImg)
        {
            for (int x = 0; x < colSize; x++)
            {
                Image codeImg = new Image();
                codeImg.Source = bImg;
                codeImg.Width = colOffset;
                codeImg.Height = colOffset;
                codeImg.MouseLeftButtonDown += onPlaceColorDown;
                codeImg.Name = "_" + x.ToString() + "C" + Src.Rules.SPECIAL_CODEYCOORD.ToString();
                this.RegisterName(codeImg.Name, codeImg);

                Thickness margin = codeImg.Margin;
                margin.Left = x * colOffset;
                margin.Right = PegGrid.Width - codeImg.Width;
                margin.Top = 300;
                margin.Bottom = PegGrid.Height - margin.Top - codeImg.Height;

                codeImg.Margin = margin;
                codePopup.Children.Add(codeImg);
            }
        }

        void printPopupFromFile(int colSize, short[] code, BitmapImage ballBit)
        {
            for (int x = 0; x < colSize; x++)
            {
                if (code[x] != -1)
                {
                    ballBit = new BitmapImage();
                    ballBit.BeginInit();
                    ballBit.UriSource = new Uri("Res/Images/Balls/" + code[x].ToString() + ".png", UriKind.Relative);
                    ballBit.EndInit();
                }

                Image codeImg = new Image();
                codeImg.Source = ballBit;
                codeImg.Width = colOffset;
                codeImg.Height = colOffset;
                codeImg.MouseLeftButtonDown += onPlaceColorDown;
                codeImg.Name = "_" + x.ToString() + "C" + Src.Rules.SPECIAL_CODEYCOORD.ToString();
                this.RegisterName(codeImg.Name, codeImg);

                Thickness margin = codeImg.Margin;
                margin.Left = x * colOffset;
                margin.Right = PegGrid.Width - codeImg.Width;
                margin.Top = 300;
                margin.Bottom = PegGrid.Height - margin.Top - codeImg.Height;

                codeImg.Margin = margin;
                codePopup.Children.Add(codeImg);
            }
        }

        void printBoard(int rowSize, int colSize, BitmapImage ballBit, BitmapImage pegBit)
        {
            int pegRowAmount = (int)Math.Ceiling(colSize / 2.0);
   
            double marginTopOffset = 0;
            for (int i = 0; i < rowSize; i++)
            {
                double marginLeftOffset = 0;
                for (int j = 0; j <= colSize; j++)
                {
                    // Print peg holes
                    if (j == colSize)
                    {
                        pegRowOffset = 20;
                        int leftToAdd = colSize;
                        double pegTopOffset = 0;

                        for (int x = 0; x < pegRowAmount; x++)
                        {
                            Image pegImg1 = new Image();
                            pegImg1.Source = pegBit;
                            pegImg1.Width = 20;
                            pegImg1.Height = 20;
                            pegImg1.Name = "_0P" + Convert.ToString(x) + "P" + Convert.ToString(i);
                            this.RegisterName(pegImg1.Name, pegImg1); //nödvändigt??
                            pegImg1.MouseLeftButtonDown += onPlacePegDown;
                            pegImg1.MouseRightButtonDown += onPlacePegDown;

                            Thickness firstMargin = pegImg1.Margin;
                            firstMargin.Left = 0;
                            firstMargin.Right = PegGrid.Width - pegImg1.Width;
                            firstMargin.Top = marginTopOffset + pegTopOffset;
                            firstMargin.Bottom = PegGrid.Height - firstMargin.Top - pegImg1.Height;

                            pegImg1.Margin = firstMargin;
                            PegGrid.Children.Add(pegImg1);

                            if (--leftToAdd == 0)
                                continue;

                            Image pegImg2 = new Image();
                            pegImg2.Source = pegBit;
                            pegImg2.Width = 20;
                            pegImg2.Height = 20;
                            pegImg2.Name = "_1P" + Convert.ToString(x) + "P" + Convert.ToString(i);
                            this.RegisterName(pegImg2.Name, pegImg2); //nödvändigt??
                            pegImg2.MouseLeftButtonDown += onPlacePegDown;
                            pegImg2.MouseRightButtonDown += onPlacePegDown;

                            Thickness secondMargin = pegImg2.Margin;
                            secondMargin.Left = pegImg1.Width;
                            secondMargin.Right = PegGrid.Width - secondMargin.Left - pegImg2.Width;
                            secondMargin.Top = marginTopOffset + pegTopOffset;
                            secondMargin.Bottom = PegGrid.Height - secondMargin.Top - pegImg2.Height;

                            pegImg2.Margin = secondMargin;
                            PegGrid.Children.Add(pegImg2);

                            leftToAdd--;
                            pegTopOffset += pegRowOffset;
                        }
                    }
                    // Print ball holes
                    else
                    {
                        Image img = new Image();
                        img.Source = ballBit;
                        img.Width = colOffset;
                        img.Height = rowOffset;
                        img.MouseLeftButtonDown += onPlaceColorDown;
                        img.Name = "_" + Convert.ToString(j) + "C" + Convert.ToString(i);
                        this.RegisterName(img.Name, img); //nödvändigt??
                        // dra background oxå

                        Thickness margin = img.Margin;
                        margin.Left = marginLeftOffset;
                        margin.Right = ((BallGrid.Width - colOffset) - marginLeftOffset);
                        margin.Top = marginTopOffset;
                        margin.Bottom = ((BallGrid.Height - rowOffset) - marginTopOffset);

                        img.Margin = margin;

                        BallGrid.Children.Add(img);

                        marginLeftOffset += colOffset;
                    }
                }
                marginTopOffset += rowOffset;
            }
        }

        void setBoard(short rowSize, short colSize, List<short[]> board, List<short[]> pegs)
        {
            int pegRowAmount = (int)Math.Ceiling(colSize / 2.0);
            for (short y = 0; y < rowSize; y++)
            {
                // Draw balls
                for (short x = 0; x < colSize; x++)
                {
                    if (board.ElementAt(y)[x] == -1) continue;
                    BitmapImage bImg = new BitmapImage();
                    bImg.BeginInit();
                    bImg.UriSource = new Uri("Res/Images/Balls/" + board.ElementAt(y)[x].ToString() + ".png", UriKind.Relative);
                    bImg.EndInit();

                    ((Image)MainGrid.FindName("_" + x.ToString() + "C" + y.ToString())).Source = bImg;
                }
                // Draw pegs
                short tempWhiteAmount = pegs.ElementAt(y)[0];
                short tempOrangeAmount = pegs.ElementAt(y)[1];
                for (int pegRow = 0; pegRow < pegRowAmount; pegRow++)
                {
                    if (tempWhiteAmount == 0 && tempOrangeAmount == 0) break;
                    // Peg One
                    Image pegImg = ((Image) MainGrid.FindName("_0P" + pegRow.ToString() + "P" + y.ToString()));

                    BitmapImage bImg = new BitmapImage();
                    bImg.BeginInit();
                    if (tempWhiteAmount > 0)
                    {
                        bImg.UriSource = new Uri("Res/Images/Pegs/0.png", UriKind.Relative);
                        tempWhiteAmount--;
                    }
                    else
                    {
                        bImg.UriSource = new Uri("Res/Images/Pegs/1.png", UriKind.Relative);
                        tempOrangeAmount--;
                    }
                    bImg.EndInit();

                    pegImg.Source = bImg;

                    if (tempWhiteAmount == 0 && tempOrangeAmount == 0) break;
                    // Peg Two
                    pegImg = ((Image)MainGrid.FindName("_1P" + pegRow.ToString() + "P" + y.ToString()));
                    bImg = new BitmapImage();
                    bImg.BeginInit();
                    if (tempWhiteAmount > 0)
                    {
                        bImg.UriSource = new Uri("Res/Images/Pegs/0.png", UriKind.Relative);
                        tempWhiteAmount--;
                    }
                    else
                    {
                        bImg.UriSource = new Uri("Res/Images/Pegs/1.png", UriKind.Relative);
                        tempOrangeAmount--;
                    }
                    bImg.EndInit();
                    pegImg.Source = bImg;
                }
            }
        }

        short colorNameToShort(string name)
        {
            switch (name)
            {
                case "Red":
                    return Src.Rules.COLOR_RED;

                case "Green":
                    return Src.Rules.COLOR_GREEN;

                case "Blue":
                    return Src.Rules.COLOR_BLUE;

                case "Yellow":
                    return Src.Rules.COLOR_YELLOW;

                case "Purple":
                    return Src.Rules.COLOR_PURPLE;

                case "Black":
                    return Src.Rules.COLOR_BLACK;

                default:
                    return Src.Rules.COLOR_NONE;
            }
        }

        void onColorBoxClick(object sender, RoutedEventArgs e)
        {
            short color = colorNameToShort(((Image)sender).Name);
            engine.onColorBoxClick(color);
        }

        void onPlaceColorDown(object sender, RoutedEventArgs e)
        {
            Image img = (Image)sender;

            short x = Convert.ToInt16(Math.Floor(img.Margin.Left / colOffset));
            short y = Convert.ToInt16(img.Margin.Top / rowOffset);

            if (codePopup.Visibility == System.Windows.Visibility.Visible)
                y = Src.Rules.SPECIAL_CODEYCOORD;

            engine.onPlaceColorDown(x, y);
        }

        void onPlacePegDown(object sender, RoutedEventArgs e)
        {
            Image img = (Image)sender;

            short row = (short)(img.Margin.Top / rowOffset);
            short x = (short)(img.Margin.Left / 20);
            short y = (short)((img.Margin.Top - row * rowOffset) / pegRowOffset);

            MouseEventArgs me = (MouseEventArgs)e;

            short type;
            if (me.LeftButton == MouseButtonState.Pressed)
                type = Src.Rules.PEG_WHITE;
            else if (me.RightButton == MouseButtonState.Pressed)
                type = Src.Rules.PEG_ORANGE;
            else
                type = Src.Rules.PEG_NONE;

            engine.onPlacePegDown(row, x, y, type);
        }

        void onShowCodeClick(object sender, RoutedEventArgs e)
        {
            engine.onShowCode();
        }

        void onChangeTurnClick(object sender, RoutedEventArgs e)
        {
            engine.onChangeTurnClick();
        }
    }
}
