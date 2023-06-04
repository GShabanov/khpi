using System;
using System.Drawing;
using System.Windows.Forms;

namespace XNumber
{

    public partial class XNumberControl : UserControl
    {
        private static char[] defAllowedChars = { '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', 'a', 'b', 'c', 'd', 'e', 'f', '\b' };

        private char[] m_allowedChars;
        private Char[] m_InputArray;
        bool m_focused = false;


        public XNumberControl()
        {
            InitializeComponent();

            m_InputArray = new char[] { '0' };
            m_allowedChars = defAllowedChars;
        }

        public delegate void OnNewCharacterEvent(object sender, Char character);
        public delegate void OnDelCharacterEvent(object sender, Char character);

        public event OnNewCharacterEvent OnNewCharacter;
        public event OnDelCharacterEvent OnDelCharacter;

        public char[] SetInput { 

            set {

                m_InputArray = value;
                this.Invalidate(true);
            } 
        }
        public char[] SetAllowedChars
        {
            set
            {
                m_allowedChars = value;
            }
        }
        //
        // dispatch paint for our control
        //
        protected override void OnPaint(PaintEventArgs e)
        {
            //
            // call overrieded virtual member
            //
            base.OnPaint(e);

            if (m_focused)
            {
                Pen focusPen = new Pen(Color.FromArgb(0x10, 0x10, 0xFF));

                Rectangle focusRect = this.ClientRectangle;
                focusRect.Inflate(-2, -2);


                e.Graphics.DrawRectangle(focusPen, focusRect);
            }

            
            Rectangle clientRect = this.ClientRectangle;

            Pen drawPen = new Pen(Color.FromArgb(0x10, 0x10, 0x10));
            Brush drawBrush = new SolidBrush(Color.FromArgb(0x10, 0x80, 0x10));
            Font drawFont = new Font("Arial", 30);

            Point[]  poligon = new Point[4];

            //
            // prepare poligon for our number
            //
            
            poligon[0].X = clientRect.Right - 35;
            poligon[0].Y = 5;

            poligon[1].X = clientRect.Right - 5;
            poligon[1].Y = 5;

            poligon[2].X = clientRect.Right - 5;
            poligon[2].Y = clientRect.Height - 5;

            poligon[3].X = clientRect.Right - 35;
            poligon[3].Y = clientRect.Height - 5;


            for (int i = 0; i < m_InputArray.Length; i++)
            {

                int stepIncrement = 0;

                if (m_InputArray[i] != '\'')
                {
                    e.Graphics.DrawPolygon(drawPen, poligon);

                    string _out = m_InputArray[i].ToString();

                    e.Graphics.DrawString(_out, drawFont, drawBrush, poligon[0].X - 5, poligon[0].Y);

                    stepIncrement = 32;

                }
                else 
                {
                    stepIncrement = 5;
                }

                for (int j = 0; j < 4; j++)
                {
                    poligon[j].X -= stepIncrement;
                }
            }

        }

        protected override void OnGotFocus(EventArgs e)
        {
            base.OnGotFocus(e);

            m_focused = true;
            this.Invalidate(true);
        }
        protected override void OnLostFocus(EventArgs e)
        {
            base.OnLostFocus(e);

            m_focused = false;
            this.Invalidate(true);
        }

        protected override void OnKeyPress(KeyPressEventArgs e)
        {
            base.OnKeyPress(e);


            e.Handled = true;


            for (int i = 0; i < m_allowedChars.Length; i++)
            {
                if (e.KeyChar == m_allowedChars[i])
                {

                    //
                    // backspace
                    //
                    if (e.KeyChar == '\b')
                    {
                        OnDelCharacter?.Invoke(this, e.KeyChar);
                    }
                    else
                    {
                        OnNewCharacter?.Invoke(this, e.KeyChar);
                    }

                    break;
                }
            }

            //e.KeyChar

        }

    }
}
