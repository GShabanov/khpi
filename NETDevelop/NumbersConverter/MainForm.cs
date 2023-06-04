using System;
using System.Linq;
using System.Numerics;
using System.Windows.Forms;

namespace NumbersConverter
{

    public partial class MainForm : Form
    {

        private static char[] m_allowedCharsBin = { '0', '1', '\b' };
        private static char[] m_allowedCharsOct = { '0', '1', '2', '3', '4', '5', '6', '7', '\b' };
        private static char[] m_allowedCharsDec = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '\b' };
        private static char[] m_allowedCharsHex = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f', '\b' };

        //
        // Masks for turning on/off keys depending on the selected number system.
        //
        private Button[] m_BinButtonEnableMask;
        private Button[] m_OctButtonEnableMask;
        private Button[] m_DecButtonEnableMask;
        private Button[] m_HexButtonEnableMask;

        private enum TypeSelected
        {
            DecSelected = 0,
            HexSelected,
            OctSelected,
            BinSelected
        }

        ///
        /// <summary>
        ///     Number radix information
        /// </summary>
        /// 
        struct NumberTypeInfo
        {
            public TypeSelected Type     { get; }
            public int          Radix    { get; }

            public int          Grouping { get; }

            public char[]       AllowedChars { get; }

            public Button[]     ButtonEnableMask { get; }

            public NumberTypeInfo(TypeSelected type, ref char[] allowedChars, ref Button[] EnableMask, int radix, int grouping = 3) 
            {
                Type = type;
                Radix = radix;
                Grouping = grouping;
                AllowedChars = allowedChars;
                ButtonEnableMask = EnableMask;
            }
        }

        private BigInteger m_CorveribleNumber;


        public MainForm()
        {
            InitializeComponent();

            m_BinButtonEnableMask = new Button[]
            {
                button0,
                button1,
            };

            m_OctButtonEnableMask = new Button[]
            {
                button0, button1, button2, button3,
                button4, button5, button6, button7,
            };

            m_DecButtonEnableMask = new Button[]
            {
                button0, button1, button2, button3,
                button4, button5, button6, button7,
                button8, button9
            };

            m_HexButtonEnableMask = new Button[]
            {
                button0, button1, button2, button3,
                button4, button5, button6, button7,
                button8, button9, buttonA, buttonB,
                buttonC, buttonD, buttonE, buttonF,
            };

            m_CorveribleNumber = new BigInteger(0);

            NumberTypeInfo a = new NumberTypeInfo(TypeSelected.DecSelected, ref m_allowedCharsDec, ref m_DecButtonEnableMask, 10, 3);

            char[] firstTextualized = NumberToChars(ref m_CorveribleNumber, ref a);

            this.numbersControlFirst.SetInput = firstTextualized;
            this.numbersControlSecond.SetInput = firstTextualized;


            this.numbersControlFirst.Focus();
            this.numbersControlFirst.Select();
        }

        ///
        /// <summary>
        ///     OnInput events for two XNumber panes
        /// </summary> 
        /// <returns>
        /// </returns>
        ///

        private void UpdateNumbersForm()
        {
            NumberTypeInfo firstFormNumberType = GetFirstType();

            char[] firstTextualized = NumberToChars(ref m_CorveribleNumber, ref firstFormNumberType);

            this.numbersControlFirst.SetInput = firstTextualized;
            this.numbersControlFirst.SetAllowedChars = firstFormNumberType.AllowedChars;


            NumberTypeInfo secondFormNumberType = GetSecondType();

            char[] secondTextualized = NumberToChars(ref m_CorveribleNumber, ref secondFormNumberType);

            this.numbersControlSecond.SetInput = secondTextualized;
            this.numbersControlSecond.SetAllowedChars = secondFormNumberType.AllowedChars;

            for (int i = 0; i < m_HexButtonEnableMask.Length; i++) 
            {
                if (i > (firstFormNumberType.ButtonEnableMask.Length - 1) ||
                    firstFormNumberType.ButtonEnableMask[i] != m_HexButtonEnableMask[i]) 
                {
                    m_HexButtonEnableMask[i].Enabled = false;
                }
                else
                {
                    m_HexButtonEnableMask[i].Enabled = true;
                }
            }

        }

        private void First_OnNewCharacter(object sender, Char character) 
        {

            //
            // use first type as base to convert
            //
            NumberTypeInfo firstFormNumberType = GetFirstType();

            m_CorveribleNumber *= firstFormNumberType.Radix;

            if (character >= 'a' && character < 'g') 
            {
                m_CorveribleNumber += ((int)(character - 'a') + 10);

            }
            else 
            {
                m_CorveribleNumber += (int)(character - '0');
            }


            UpdateNumbersForm();
        }

        private void Second_OnNewCharacter(object sender, Char character)
        {
            //
            // use first type as base to convert
            //
            NumberTypeInfo secondFormNumberType = GetSecondType();

            m_CorveribleNumber *= secondFormNumberType.Radix;

            if (character >= 'a' && character < 'g')
            {
                m_CorveribleNumber += ((int)(character - 'a') + 10);

            }
            else
            {
                m_CorveribleNumber += (int)(character - '0');
            }

            UpdateNumbersForm();

        }

        private void First_OnDelCharacter(object sender, Char character)
        {

            //
            // use first type as base to convert
            //
            NumberTypeInfo firstFormNumberType = GetFirstType();

            m_CorveribleNumber /= firstFormNumberType.Radix;


            UpdateNumbersForm();
        }

        private void Second_OnDelCharacter(object sender, Char character)
        {
            //
            // use first type as base to convert
            //
            NumberTypeInfo secondFormNumberType = GetSecondType();

            m_CorveribleNumber /= secondFormNumberType.Radix;

            UpdateNumbersForm();

        }

        private void radioButtons_CheckedChanged(Object sender, EventArgs e)
        {
            UpdateNumbersForm();

        }


        ///
        /// <summary>
        ///     convert BigNumber type to chars array
        /// </summary> 
        /// <returns>
        ///      char[]  - array
        /// </returns>
        ///
        private char[] NumberToChars(ref BigInteger number, ref NumberTypeInfo typeInfo) 
        {
            BigInteger temp = number;

            char[] _output = new char[0];

            if (number == 0) 
            {
                _output = _output.Append('0').ToArray();
            }
            else 
            {

                int n = 0;

                while (temp != 0)
                {

                    int numb = (int)(temp % typeInfo.Radix);

                    char ch = '\'';

                    if (numb > 9)
                    {
                        ch = (char)((numb - 10) + 'A');
                    }
                    else
                    {
                        ch = (char)(numb + '0');
                    }

                    _output = _output.Append(ch).ToArray();

                    temp /= typeInfo.Radix;

                    n++;

                    if (n == typeInfo.Grouping) 
                    {
                        ch = '\'';
                        _output = _output.Append(ch).ToArray();
                        n = 0;
                    }
                }
            }

            return _output;
        }

        ///
        /// <summary>
        ///     Get number type info for the first group box
        /// </summary> 
        /// <returns>
        ///      NumberTypeInfo  - type information
        /// </returns>
        ///

        private NumberTypeInfo GetFirstType()
        {
            NumberTypeInfo local = new NumberTypeInfo(TypeSelected.DecSelected, ref m_allowedCharsDec, ref m_DecButtonEnableMask, 10, 3);

            if (this.radioHex1.Checked)
            {
                local = new NumberTypeInfo(TypeSelected.HexSelected, ref m_allowedCharsHex, ref m_HexButtonEnableMask, 16, 4);
            }

            if (this.radioOct1.Checked)
            {
                local = new NumberTypeInfo(TypeSelected.OctSelected, ref m_allowedCharsOct, ref m_OctButtonEnableMask, 8, 3);
            }

            if (this.radioBin1.Checked)
            {
                local = new NumberTypeInfo(TypeSelected.BinSelected, ref m_allowedCharsBin, ref m_BinButtonEnableMask, 2, 4);
            }

            return local;
        }

        ///
        /// <summary>
        ///     Get number type info for the second group box
        /// </summary> 
        /// <returns>
        ///      NumberTypeInfo  - type information
        /// </returns>
        ///
        private NumberTypeInfo GetSecondType()
        {
            NumberTypeInfo local = new NumberTypeInfo(TypeSelected.DecSelected, ref m_allowedCharsDec, ref m_DecButtonEnableMask, 10, 3);

            if (this.radioHex2.Checked)
            {
                local = new NumberTypeInfo(TypeSelected.HexSelected, ref m_allowedCharsHex, ref m_HexButtonEnableMask, 16, 4);
            }

            if (this.radioOct2.Checked)
            {
                local = new NumberTypeInfo(TypeSelected.OctSelected, ref m_allowedCharsOct, ref m_OctButtonEnableMask, 8, 3);
            }

            if (this.radioBin2.Checked)
            {
                local = new NumberTypeInfo(TypeSelected.BinSelected, ref m_allowedCharsBin, ref m_BinButtonEnableMask, 2, 4);
            }

            return local;
        }


        private void AppendNumber(int number) 
        {
            NumberTypeInfo firstFormNumberType = GetFirstType();

            m_CorveribleNumber *= firstFormNumberType.Radix;
            m_CorveribleNumber += number;

            UpdateNumbersForm();
        }

        private void buttonReset_Click(object sender, EventArgs e)
        {
            m_CorveribleNumber = 0;
            UpdateNumbersForm();
        }

        private void button0_Click(object sender, EventArgs e)
        {
            AppendNumber(0);
        }

        private void button1_Click(object sender, EventArgs e)
        {
            AppendNumber(1);
        }

        private void button2_Click(object sender, EventArgs e)
        {
            AppendNumber(2);
        }

        private void button3_Click(object sender, EventArgs e)
        {
            AppendNumber(3);
        }

        private void button4_Click(object sender, EventArgs e)
        {
            AppendNumber(4);
        }

        private void button5_Click(object sender, EventArgs e)
        {
            AppendNumber(5);
        }

        private void button6_Click(object sender, EventArgs e)
        {
            AppendNumber(6);
        }

        private void button7_Click(object sender, EventArgs e)
        {
            AppendNumber(7);
        }

        private void button8_Click(object sender, EventArgs e)
        {
            AppendNumber(8);
        }

        private void button9_Click(object sender, EventArgs e)
        {
            AppendNumber(9);
        }

        private void buttonA_Click(object sender, EventArgs e)
        {
            AppendNumber(10);
        }

        private void buttonB_Click(object sender, EventArgs e)
        {
            AppendNumber(11);
        }

        private void buttonC_Click(object sender, EventArgs e)
        {
            AppendNumber(12);
        }

        private void buttonD_Click(object sender, EventArgs e)
        {
            AppendNumber(13);
        }

        private void buttonE_Click(object sender, EventArgs e)
        {
            AppendNumber(14);
        }

        private void buttonF_Click(object sender, EventArgs e)
        {
            AppendNumber(15);
        }
    }
}
