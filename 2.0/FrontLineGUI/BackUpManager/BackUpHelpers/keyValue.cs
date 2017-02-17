using System;

namespace Cll
{
	/// <summary>
	/// Summary description for keyValue.
	/// </summary>
	public class keyValue
	{
		string _strName = "";
		string _strValue = "";
		int _nType = 0;

		public keyValue()
		{
		}

		public void setKeyValue(string strName, string strValue, int nType)
		{
			_strName = strName;
			_strValue = strValue;
			_nType = nType;
		}

		public string getName() 
		{ 
			return _strName; 
		}
		
		public string getValue() 
		{ 
			return _strValue; 
		}
		
		public int getType() 
		{ 
			return _nType; 
		}
	}
}
