using System;
using System.IO;
using System.Windows.Forms;

namespace Cll
{
	/// <summary>
	/// Summary description for xmlWriter.
	/// </summary>
	public class xmlWriter
	{
		StreamWriter _sw = null;
		bool _bFileOpen = false, _bPrototypeWritten;
		string _strFilename;
		int _nLevel;

		public xmlWriter()
		{
			init();
		}

		public void setIndentLevel(int n) 
		{ 
			_nLevel = n; 
		}
		
		public int getIndentLevel() 
		{ 
			return _nLevel; 
		}
		
		public string getFilename() 
		{ 
			return _strFilename; 
		}

		public bool mustBeClosed() 
		{ 
			return _bFileOpen; 
		}

		void init()
		{
			setIndentLevel(0);
			_bFileOpen = false;
			_bPrototypeWritten = false;
		}

		public bool open(string strFilename)
		{
			if (_bFileOpen)
				return true;

			_strFilename = strFilename;

			return true;
		}

		public bool writeString(string strData)
		{
			if (!_bFileOpen) // open file
			{
				try
				{
					_sw = File.CreateText(_strFilename);
					_bFileOpen = true;
				}
				catch (Exception) 
				{
					//MessageBox.Show (e.ToString(), "Office Recovery Manager", 
					//	MessageBoxButtons.OK, MessageBoxIcon.Exclamation);

					_bFileOpen = false;
				}
			}
			
			if (!_bFileOpen)
				return false;

			if (!_bPrototypeWritten)
			{
				_bPrototypeWritten = true; // make sure to set this flag to true before we call a reentrant method such like .WritePInstruction

				// write XML prototype, once and only once
				//
				int nOldIndent = getIndentLevel();
				setIndentLevel(0);

				xmlElement xml = new xmlElement("xml");
				xml.addAttrib( "version", "1.0" );
				xml.addAttrib( "encoding", "UTF-8" );
				xml.writePInstruction(this,0);
					
				setIndentLevel(nOldIndent);
			}

			// actual write
			_sw.Write(strData);

			return true;
		}

		public bool close()
		{
			if (_sw!=null && _bFileOpen)
			{
				_sw.Close();
			}
			
			init();

			return true;
		}
	}
}
