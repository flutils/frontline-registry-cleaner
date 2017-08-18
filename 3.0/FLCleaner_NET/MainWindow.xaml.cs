using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Forms;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace FLCleaner_NET
{
    // Interaction logic for MainWindow.xaml
    // This creates the GUI and should call the Engine to get all required data
    public partial class MainWindow : Window
    {

    ///////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////

        // Defs //
        [System.Runtime.InteropServices.DllImport("Gdi32.dll", EntryPoint = "CreateRoundRectRgn")]
        private static extern IntPtr CreateRoundRectRgn
        (
            int nLeftRect,      // x-coordinate of upper-left corner
            int nTopRect,       // y-coordinate of upper-left corner
            int nRightRect,     // x-coordinate of lower-right corner
            int nBottomRect,    // y-coordinate of lower-right corner
            int nWidthEllipse,  // height of ellipse
            int nHeightEllipse  // width of ellipse
        );

    ///////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////

        // App //
        public MainWindow()
        {

            // Init app
            InitializeComponent();

            // Events
            //this.MouseDown += new MouseButtonEventHandler(MainWindow_Drag); // Window Drag

            // Border Radius 
            // https://stackoverflow.com/a/18822204/1143732
            //this.Region = System.Drawing.Region.FromHrgn(CreateRoundRectRgn(0, 0, Convert.ToInt32(Width), Convert.ToInt32(Height), 20, 20));

        }

    ///////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////

        private void MainWindow_Paint(object sender, PaintEventArgs e)
        {
            //e.Graphics.DrawRectangle(new Pen(Black, 1), this.DisplayRectangle);
        }

    ///////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////
    // Extras //
    ///////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////

        // Draggable Form
        // https://stackoverflow.com/a/1592899/1143732
        public const int WM_NCLBUTTONDOWN = 0xA1;
        public const int HT_CAPTION = 0x2;

        [System.Runtime.InteropServices.DllImportAttribute("user32.dll")]
        public static extern int SendMessage(IntPtr hWnd, int Msg, int wParam, int lParam);
        [System.Runtime.InteropServices.DllImportAttribute("user32.dll")]
        public static extern bool ReleaseCapture();

        // Mousedown Event
        private void MainWindow_Drag(object sender, System.Windows.Forms.MouseEventArgs e)
        {
            if (e.Button == MouseButtons.Left)
            {
                ReleaseCapture();
                //SendMessage(Handle, WM_NCLBUTTONDOWN, HT_CAPTION, 0);
            }
        }

    ///////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////

    }
}
