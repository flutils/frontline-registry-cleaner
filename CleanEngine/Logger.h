#pragma once

#include <AtlStr.h>

class MessagesLogger
{
  public:
  
  static void Log(LPCTSTR lpcszString)
  {
	  MessagesLogger::Instance().LogMessage(lpcszString);
  }

  private:
  MessagesLogger()
  {
	  //m_hLogFile = CreateFile( "Log.txt", GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	  //if( INVALID_HANDLE_VALUE == m_hLogFile )
	  {
		m_hLogFile = NULL;
	  }
  }

  ~MessagesLogger()
  {
	  if(m_hLogFile)
		  CloseHandle(m_hLogFile);
  }

  static MessagesLogger& Instance()
  {
       static MessagesLogger lg;
	   return lg;
  }

  void LogMessage(LPCTSTR lpcszString)
  {
	  if(m_hLogFile)
	  {
	    CAtlString strLogMessage(lpcszString);
	    strLogMessage+=_T("\r\n");
	    DWORD res(0);
	    WriteFile(m_hLogFile, strLogMessage.GetString(), strLogMessage.GetLength(), &res, NULL);
	  }
  }

 

  HANDLE m_hLogFile;
};

#ifdef _DEBUG

#define LOG_TEXT( Descr ) { \
 MessagesLogger::Log(Descr); }



#define LOG_TEXT2( DescrPart1, DescrPart2 ) { \
 CAtlString strLog(DescrPart1); \
 strLog+=DescrPart2; \
 MessagesLogger::Log(strLog); }




#define LOG_TEXT3( DescrPart1, DescrPart2, DescrPart3 ) { \
 CAtlString strLog(DescrPart1); \
 strLog+=DescrPart2; \
 strLog+=DescrPart3; \
 MessagesLogger::Log(strLog); }

#else

#define LOG_TEXT( Descr ) ;
#define LOG_TEXT2( DescrPart1, DescrPart2 ) ;
#define LOG_TEXT3(DescrPart1, DescrPart2, DescrPart3) ;

#endif
