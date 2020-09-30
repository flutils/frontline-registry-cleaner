using System;
using System.Collections;

namespace Cll
{
	///	<summary>
	///	xmlElement is used by xmlReader and xmlWriter for processing.
	///	</summary>
	public class xmlElement
	{
		public string _strName;
		private	ArrayList _arrayAttribNames, _arrayAttribValues;

		public xmlElement()
		{
			_arrayAttribNames = new ArrayList();
			_arrayAttribValues = new ArrayList();
		}

		public xmlElement(string strName)
		{
			_arrayAttribNames = new ArrayList();
			_arrayAttribValues = new ArrayList();

			setName(strName);
		}

		string helperBuildIndent(int nLevel)
		{
			string strSpaces = "";
			
			for	(int i=0;i<nLevel;i++)
				strSpaces +=	" ";

			return strSpaces;
		}

		public void setName(string strName)
		{
			_strName = strName;
		}

		string getName()
		{
			return _strName;
		}
		
		string getAttribName(int nIndex)
		{
			string strAttribName = "";
			if (nIndex>-1 && nIndex<getAttribCount())
				strAttribName = (string)_arrayAttribNames[nIndex];

			return strAttribName;
		}

		string getAttribValue(int nIndex)
		{
			string strAttribValue = "";
			if (nIndex>-1 && nIndex<_arrayAttribValues.Count)
				strAttribValue = (string)_arrayAttribValues[nIndex];

			return strAttribValue;
		}

		int	getAttribCount()
		{
			return (int)_arrayAttribNames.Count;
		}

		bool findAttrib(string strAttribName)
		{
			bool bFound	= false;
			int	i=0;
			int	nSize =	(int) _arrayAttribNames.Count;
			
			while (i<nSize && !bFound)
			{
				bFound = ((string)_arrayAttribNames[i]==strAttribName);
				i++;
			}

			return bFound;
		}

		public void addAttrib(string strAttribName, string strAttribValue)
		{
			bool bFound	= false;
			int	i=0;
			int	nSize =	(int) _arrayAttribNames.Count;
			
			while (i<nSize && !bFound)
			{
				bFound = ((string)_arrayAttribNames[i]==strAttribName);
				i++;
			}
			
			if (bFound)	// already known
			{
				i--;
				_arrayAttribValues[i] = strAttribValue;
			}
			else
			{
				_arrayAttribNames.Add( strAttribName );
				_arrayAttribValues.Add( strAttribValue	);
			}
		}

		public void write(xmlWriter writer, int nDeltaLevel, bool bIndent, bool bEOL)	// for any kind	of open	tag
		{
			writer.setIndentLevel( writer.getIndentLevel()+nDeltaLevel );

			string	s = "";
			if (bIndent)
				s = helperBuildIndent(writer.getIndentLevel());
			
			s += "<";
			s += _strName;
			int	i;
			int	nCount = getAttribCount();
	
			for	(i=0;i<nCount;i++)
			{
				s += " "; // separator between attribute pairs
				s += _arrayAttribNames[i];
				s += "=\"";
				s += _arrayAttribValues[i];
				s += "\"";
			}

			s += ">";
			if (bEOL)
			s += "\r\n"; //	ENDL
			
			writer.writeString(	s );
		}

		public void writeEmpty(xmlWriter writer, int nDeltaLevel, bool bIndent, bool bEOL)
		{
			writer.setIndentLevel( writer.getIndentLevel()+nDeltaLevel );

			string	s = "";
			if (bIndent)
				helperBuildIndent(writer.getIndentLevel());
			
			s += "<";
			s += _strName;
			int	i;
			int	nCount = getAttribCount();
			
			for	(i=0;i<nCount;i++)
			{
				s += " "; // separator between attribute pairs
				s += _arrayAttribNames[i];
				s += "=\"";
				s += _arrayAttribValues[i];
				s += "\"";
			}

			s += "></";
			s += _strName;
			s += ">";

			if (bEOL)
			s += "\r\n"; //	ENDL
			writer.writeString(	s );

			writer.setIndentLevel( writer.getIndentLevel()-nDeltaLevel );
		}

		public void writePInstruction(xmlWriter writer, int nDeltaLevel)
		{
			writer.setIndentLevel( writer.getIndentLevel()+nDeltaLevel );

			string	s = "";
			
			s = helperBuildIndent(writer.getIndentLevel());
			
			s += "<?";
			s += _strName;
			
			int	i;
			int	nCount = getAttribCount();
			
			for	(i=0;i<nCount;i++)
			{
				s += " "; // separator between attribute pairs
				s += _arrayAttribNames[i];
				s += "=\"";
				s += _arrayAttribValues[i];
				s += "\"";
			}

			s += "?>";
			s += "\r\n"; //	ENDL
			
			writer.writeString(	s );
		}

		public void writeClosingTag(xmlWriter writer, int nDeltaLevel, bool bIndent, bool bEOL)
		{
			string	s = "";
			
			if (bIndent)
				s = helperBuildIndent(writer.getIndentLevel());

			s += "</";
			s += _strName;
			s += ">";
			if (bEOL)
			s += "\r\n"; //	ENDL
			writer.writeString(	s );

			writer.setIndentLevel( writer.getIndentLevel()+nDeltaLevel );
		}
	}
}
