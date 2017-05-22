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
using System.Windows.Navigation;
using System.Windows.Shapes;
using System.Threading;
using System.Xml.Linq;

namespace Mr.Mind
{
    public partial class MainWindow : Window
    {
        private short colSize;
        private short rowSize;
        public MainWindow()
        {
            InitializeComponent();
            if (System.IO.File.Exists(System.Environment.CurrentDirectory + "/MrMindSave.xml"))
            {
                short cols = 0, rows = 0;
                short currentTurn = 0;
                short currentRowNumber = 0;
                short currentColor = 0;

                short[] code = new short[0];
                short[] codebreakerAIcurGuess = new short[0];

                bool isFirstTurn = false;
                bool codemakerAI = false;
                bool codebreakerAI = false;
                bool showingCode = false;
                bool codebreakerAIfirstTurn = false;

                System.Collections.Generic.List<short[]> board = new System.Collections.Generic.List<short[]>();
                System.Collections.Generic.List<short[]> pegs = new System.Collections.Generic.List<short[]>();

                Src.FileManager.load(ref cols, ref rows, ref currentTurn, ref currentRowNumber, ref currentColor,
                    ref code, ref isFirstTurn, ref codemakerAI, ref codebreakerAI, ref showingCode, ref board, ref pegs, ref codebreakerAIfirstTurn, ref codebreakerAIcurGuess);

                Window win = new PlayWindow(cols, rows, code, board, pegs,
                    currentTurn, currentRowNumber, currentColor, isFirstTurn, codemakerAI, codebreakerAI, showingCode, codebreakerAIfirstTurn, codebreakerAIcurGuess);
                App.Current.MainWindow = win;
                this.Close();
                win.Show();
                return;
            }

            PvP.Click += btnClick;
            PvE.Click += btnClick;
            EvP.Click += btnClick;

            rowSize = 8;
            colSize = 4;
        }

        void btnClick(object sender, RoutedEventArgs e)
        {
            Window win = null;
            switch (((Button)sender).Name)
            {
                case "PvP":
                    win = new PlayWindow(false, false, colSize, rowSize);
                    break;
                case "PvE":
                    win = new PlayWindow(false, true, colSize, rowSize);
                    break;
                case "EvP":
                    win = new PlayWindow(true, false, colSize, rowSize);
                    break;
                default:
                    break;
            }

            if (win == null) { return; }
            App.Current.MainWindow = win;
            this.Close();
            win.Show();
        }
        void colClick(object sender, RoutedEventArgs e)
        {
            switch (((TextBlock)sender).Text)
            {
                case "8":
                    rowBorder.Margin = row8.Margin;
                    rowSize = 8;
                    break;
                case "10":
                    rowBorder.Margin = row10.Margin;
                    rowSize = 10;
                    break;
                case "12":
                    rowBorder.Margin = row12.Margin;
                    rowSize = 12;
                    break;
                default:
                    break;
            }
        }
        void rowClick(object sender, RoutedEventArgs e)
        {
            switch (((TextBlock)sender).Text)
            {
                case "4":
                    colBorder.Margin = col4.Margin;
                    colSize = 4;
                    break;
                case "5":
                    colBorder.Margin = col5.Margin;
                    colSize = 5;
                    break;
                case "6":
                    colBorder.Margin = col6.Margin;
                    colSize = 6;
                    break;
                default:
                    break;
            }
        }
    }
}
