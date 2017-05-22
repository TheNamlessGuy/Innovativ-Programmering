using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Collections.Generic;
using System.Xml.Linq;

namespace Mr.Mind.Src
{
    class FileManager
    {
        public static void initSave(short cols, short rows, bool codemakerAI, bool codebreakerAI)
        {
            XDocument xDoc = new XDocument();
            XElement rootElement = new XElement("Save");

            // Set board
            XElement xBoard = new XElement("Board");
            XElement temp = new XElement("maxRows");
            temp.Value = rows.ToString();
            xBoard.Add(temp);

            temp = new XElement("maxCols");
            temp.Value = cols.ToString();
            xBoard.Add(temp);

            XElement xCode = new XElement("code");
            for (short code = 0; code < cols; code++)
            {
                XElement currentCode = new XElement("col" + code.ToString());
                currentCode.Value = "-1";
                xCode.Add(currentCode);
            }
            xBoard.Add(xCode);

            for (short r = 0; r < rows; r++)
            {
                XElement currentRow = new XElement("row" + r.ToString());
                for (short c = 0; c < cols; c++)
                {
                    XElement currentCol = new XElement("col" + c.ToString());
                    currentCol.Value = "-1";
                    currentRow.Add(currentCol);
                }
                XElement pegTemp = new XElement("whitepegs");
                pegTemp.Value = "0";
                currentRow.Add(pegTemp);

                pegTemp = new XElement("orangepegs");
                pegTemp.Value = "0";
                currentRow.Add(pegTemp);

                xBoard.Add(currentRow);
            }

            // Set engine
            XElement xEngine = new XElement("Engine");
            temp = new XElement("turn");
            temp.Value = "0";
            xEngine.Add(temp);

            temp = new XElement("currentRowNumber");
            temp.Value = "0";
            xEngine.Add(temp);

            temp = new XElement("currentColor");
            temp.Value = "0";
            xEngine.Add(temp);

            temp = new XElement("isFirstTurn");
            temp.Value = "true";
            xEngine.Add(temp);

            temp = new XElement("codemakerAI");
            temp.Value = codemakerAI.ToString();
            xEngine.Add(temp);

            temp = new XElement("codebreakerAI");
            temp.Value = codebreakerAI.ToString();
            xEngine.Add(temp);

            temp = new XElement("showingCode");
            temp.Value = "True";
            xEngine.Add(temp);

            temp = new XElement("codebreakerAIData");
            XElement firstTurn = new XElement("firstTurn");
            if (codebreakerAI)
                firstTurn.Value = "True";
            else
                firstTurn.Value = "False";
            XElement curCode = new XElement("curCode");
            for (short c = 0; c < cols; c++)
            {
                XElement cTemp = new XElement("col" + c.ToString());
                if (codebreakerAI)
                    cTemp.Value = "0";
                else
                    cTemp.Value = "-1";
                curCode.Add(cTemp);
            }
            temp.Add(firstTurn);
            temp.Add(curCode);
            xEngine.Add(temp);

            // Set rootElement
            rootElement.Add(xBoard);
            rootElement.Add(xEngine);

            // Save xDoc
            xDoc.Add(rootElement);
            xDoc.Save(System.Environment.CurrentDirectory + "/MrMindSave.xml");
        }
        public static void turnSave(short currentTurn, short currentRowNumber)
        {
            XDocument xDoc = XDocument.Load(System.Environment.CurrentDirectory + "/MrMindSave.xml");
            xDoc.Root.Element("Engine").Element("turn").Value = currentTurn.ToString();
            xDoc.Root.Element("Engine").Element("currentRowNumber").Value = currentRowNumber.ToString();
            xDoc.Save(System.Environment.CurrentDirectory + "/MrMindSave.xml");
        }
        public static void placeBallSave(short x, short y, short color)
        {
            XDocument xDoc = XDocument.Load(System.Environment.CurrentDirectory + "/MrMindSave.xml");
            XElement row = xDoc.Root.Element("Board");
            if (y == Rules.SPECIAL_CODEYCOORD)
                row = row.Element("code");
            else
                row = row.Element("row" + y.ToString());
            XElement col = row.Element("col" + x.ToString());
            col.Value = color.ToString();
            xDoc.Save(System.Environment.CurrentDirectory + "/MrMindSave.xml");
        }
        public static void currentColorSave(short currentColor)
        {
            XDocument xDoc = XDocument.Load(System.Environment.CurrentDirectory + "/MrMindSave.xml");
            xDoc.Root.Element("Engine").Element("currentColor").Value = currentColor.ToString();
            xDoc.Save(System.Environment.CurrentDirectory + "/MrMindSave.xml");
        }
        public static void isFirstTurnSave()
        {
            XDocument xDoc = XDocument.Load(System.Environment.CurrentDirectory + "/MrMindSave.xml");
            xDoc.Root.Element("Engine").Element("isFirstTurn").Value = "False";
            xDoc.Save(System.Environment.CurrentDirectory + "/MrMindSave.xml");
        }
        public static void placePegSave(short row, short color)
        {
            XDocument xDoc = XDocument.Load(System.Environment.CurrentDirectory + "/MrMindSave.xml");
            XElement xPeg = xDoc.Root.Element("Board").Element("row" + row.ToString());
            if (color == Rules.PEG_WHITE)
                xPeg = xPeg.Element("whitepegs");
            else
                xPeg = xPeg.Element("orangepegs");

            short value = Convert.ToInt16(xPeg.Value);
            value++;
            xPeg.Value = value.ToString();

            xDoc.Save(System.Environment.CurrentDirectory + "/MrMindSave.xml");
        }
        public static void showColorSave(bool showingCode)
        {
            XDocument xDoc = XDocument.Load(System.Environment.CurrentDirectory + "/MrMindSave.xml");
            xDoc.Root.Element("Engine").Element("showingCode").Value = showingCode.ToString();
            xDoc.Save(System.Environment.CurrentDirectory + "/MrMindSave.xml");
        }
        public static void AIfirstTurnSave()
        {
            XDocument xDoc = XDocument.Load(System.Environment.CurrentDirectory + "/MrMindSave.xml");
            xDoc.Root.Element("Engine").Element("codebreakerAIData").Element("firstTurn").Value = "False";
            xDoc.Save(System.Environment.CurrentDirectory + "/MrMindSave.xml");
        }
        public static void AIcurCodeSave(short[] curCode)
        {
            XDocument xDoc = XDocument.Load(System.Environment.CurrentDirectory + "/MrMindSave.xml");
            XElement xCurCode = xDoc.Root.Element("Engine").Element("codebreakerAIData").Element("curCode");
            for (short i = 0; i < curCode.Length; i++)
            {
                XElement cTemp = xCurCode.Element("col" + i.ToString());
                cTemp.Value = curCode[i].ToString();
            }
            xDoc.Save(System.Environment.CurrentDirectory + "/MrMindSave.xml");
        }
        public static void load(ref short cols, ref short rows, ref short currentTurn, ref short currentRowNumber, ref short currentColor,
                    ref short[] code, ref bool isFirstTurn, ref bool codemakerAI, ref bool codebreakerAI, ref bool showingCode, ref List<short[]> board, ref List<short[]> pegs,
                    ref bool codebreakerAIfirstTurn, ref short[] codebreakerAIcurGuess)
        {
            XDocument xDoc = XDocument.Load(System.Environment.CurrentDirectory + "/MrMindSave.xml");
            XElement xBoard = xDoc.Root.Element("Board");

            // Get max cols and rows
            cols = Convert.ToInt16(xBoard.Element("maxCols").Value);
            rows = Convert.ToInt16(xBoard.Element("maxRows").Value);

            // Get the code
            code = new short[cols];
            XElement xCode = xBoard.Element("code");
            for (short c = 0; c < cols; c++)
            {
                XElement codeRow = xCode.Element("col" + c.ToString());
                code[c] = Convert.ToInt16(codeRow.Value);
            }

            // Get the board
            for (short r = 0; r < rows; r++)
            {
                XElement currentRow = xBoard.Element("row" + r.ToString());
                short[] temp = new short[cols];
                for (short c = 0; c < cols; c++)
                {
                    XElement currentCol = currentRow.Element("col" + c.ToString());
                    temp[c] = Convert.ToInt16(currentCol.Value);
                }
                board.Add(temp);

                temp = new short[2];
                XElement xPegs = currentRow.Element("whitepegs");
                temp[0] = Convert.ToInt16(xPegs.Value);

                xPegs = currentRow.Element("orangepegs");
                temp[1] = Convert.ToInt16(xPegs.Value);

                pegs.Add(temp);
            }
            // Get engine variables
            XElement xEngine = xDoc.Root.Element("Engine");
            currentTurn = Convert.ToInt16(xEngine.Element("turn").Value);
            currentRowNumber = Convert.ToInt16(xEngine.Element("currentRowNumber").Value);
            currentColor = Convert.ToInt16(xEngine.Element("currentColor").Value);
            isFirstTurn = Convert.ToBoolean(xEngine.Element("isFirstTurn").Value);
            codemakerAI = Convert.ToBoolean(xEngine.Element("codemakerAI").Value);
            codebreakerAI = Convert.ToBoolean(xEngine.Element("codebreakerAI").Value);
            showingCode = Convert.ToBoolean(xEngine.Element("showingCode").Value);

            XElement xAI = xEngine.Element("codebreakerAIData");
            codebreakerAIfirstTurn = Convert.ToBoolean(xAI.Element("firstTurn").Value);

            codebreakerAIcurGuess = new short[cols];
            for (short c = 0; c < cols; c++)
                codebreakerAIcurGuess[c] = Convert.ToInt16(xAI.Element("curCode").Element("col" + c.ToString()).Value);
        }

        public static void DeleteSave()
        {
            System.IO.File.Delete(System.Environment.CurrentDirectory + "/MrMindSave.xml");
        }
    }
}
