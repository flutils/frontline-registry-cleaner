using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using MCleanEngine;
using System.Diagnostics;
using System.Threading;
using System.Runtime.InteropServices;

namespace RegGenie
{
    public partial class Test : Form
    {

        ManagedCleanEngine ce = new ManagedCleanEngine();
        String strResult = String.Empty;
        //int _previousSannerID = 0;
        List<CEScannerID> ScannerList = new List<CEScannerID>();

        public Test()
        {
            InitializeComponent();
            panel1.OverlayLeadingSize = 4;
            panel1.Value = 0;
            panel1.BackgroundTrailingSize = 5;
            ManagedCleanEngine.CENotifierItemFound += new CENotificationItemFound(ItemFound);
            ManagedCleanEngine.CEScanFinished += new CEScannerFinished(ScannerFinishedProcessing);
            //ScannerList.Add(CEScannerID.COMOBJECT_SCANNER_ID); 
            //ScannerList.Add(CEScannerID.APPLICATIONS_SCANNER_ID);
            //ScannerList.Add(CEScannerID.APPPATH_SCANNER_ID);
            //ScannerList.Add(CEScannerID.BANDS_SCANNER_ID);
            //ScannerList.Add(CEScannerID.BHO_SCANNER_ID);
            //ScannerList.Add(CEScannerID.BROKENSHCUTS_SCANNER_ID);
            //ScannerList.Add(CEScannerID.CLIENTS_SCANNER_ID);
            ScannerList.Add(CEScannerID.RECYCLEBIN_SCANNER_ID);

        }
        
        private void InsertItem(string text, int itemID, int scannerID)
        {
            
            
            var lvi = this.listView1.Items.Add(text);
            lvi.SubItems.Add(itemID.ToString());
        }

        public delegate void SimpleDelegate(string str, int item_id, int scanner_id);

        Random rd = new Random(2);
        private void ItemFound(String str, int itemID, int scannerID)
        {

            if(this.listView1.InvokeRequired)
            {
                var ind = ScannerList.IndexOf(ScannerHelpers.ScannerAsociation(scannerID))+1;
                var limitation = ind * 100 / ScannerList.Count;
                if (panel1.Value + 100 / ScannerList.Count > limitation)
                {
                    var inc = rd.Next(3) + 1;
                    panel1.Value -= inc;
                    panel1.MarqueeUpdate();

                }
                else
                {
                    panel1.Value += 1;
                    panel1.MarqueeUpdate();
                }

               SimpleDelegate del = new SimpleDelegate(InsertItem);
               this.Invoke(del, new object[] { str, itemID, scannerID });
            }
            else
            {
                //listView1.Items.Add(str);
            }
            
        }

        private void ScannerFinishedProcessing(int scannerID)
        {
            if (panel1.Value + 100 / ScannerList.Count < 100)
                panel1.Value += 100 / ScannerList.Count;
            else
                panel1.Value = 99;
            panel1.MarqueeUpdate();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            this.panel1.MarqueeWidth = (int)(100 / ScannerList.Count);
            ce.EnableAllScanners(false);
            foreach (var scanner in ScannerList)
            {
                ce.EnableScanner((int)scanner, true);
               
            }
            ce.Start();
        }

        private void Test_FormClosing(object sender, FormClosingEventArgs e)
        {
            Process.GetCurrentProcess().Kill();
        }

        private void button2_Click(object sender, EventArgs e)
        {
            panel1.MarqueeUpdate();
        }

        private void button3_Click(object sender, EventArgs e)
        {
            listView1.Items.Add(strResult);
        }

        private void button4_Click(object sender, EventArgs e)
        {
            ce.Stop();
            ce.Dispose();
            ce = null;
        }

        private void button5_Click(object sender, EventArgs e)
        {
            //ce.FixItem(0);
            MessageBox.Show(GetCount().ToString());
        }
        [DllImport("shell32.dll")]
        static extern int SHQueryRecycleBin(string pszRootPath, ref SHQUERYRBINFO
           pSHQueryRBInfo);

        [StructLayout(LayoutKind.Sequential, Pack = 4)]
        //[StructLayout(LayoutKind.Explicit, Size = 20)]
        public struct SHQUERYRBINFO
        {
            public int cbSize;
            public long i64Size;
            public long i64NumItems;
            //[FieldOffset(0)]
            //public int cbSize;
            //[FieldOffset(4)]
            //public long i64Size;
            //[FieldOffset(12)]
            //public long i64NumItems;
        }
        public static int GetCount()
        {
            SHQUERYRBINFO sqrbi = new SHQUERYRBINFO();
            sqrbi.cbSize = Marshal.SizeOf(typeof(SHQUERYRBINFO));
            int hresult = SHQueryRecycleBin(string.Empty, ref sqrbi);
            return (int)sqrbi.i64NumItems;
        }
    }
}
