#ifndef _HTTPSERVICE_H
#define _HTTPSERVICE_H
#include <common.h>
#include <iostream>
#include <ext/hash_map>
#include <ext/rope>
#include <string>
#include <bthread.h>
#include <socket.h>
#include <stringhash.h>
#include <servicepool.h>
#include <socketport.h>
	
using namespace std;
using namespace bbt;
typedef __gnu_cxx::hash_map<const std::string,std::string,bbt::StringHash> HttpMap;
typedef __gnu_cxx::crope HttpRope;

class HTTPSession;

class HTTPServer;

class Response
{
	private:
		HTTPSession *session;
		string headBuffer;
		void send(const char * s,bool endLine=false);
		void execgi();

	public:

		Response(){};

		void setSession(HTTPSession *session){this->session=session;};
		void sendStateLine();
		char *httpTime(const time_t *btp, char *buf, int bufsize);
		void sendGeneralHeaders(int code);
		void sendStatusLine(int code);
		void sendLastModified(time_t ct);
		void sendContentType(const string& path);
		void sendContentSize(size_t size);
		int httpError(int code,const string& str);
		void sendFooter();
		void process();

};

class Request
{
	private:

		HTTPSession *session;
		HttpRope requestBuf; // getting request from socket
		string method;
		string uri;
		string version;
		string parameter;
		HttpMap httpHeader;
		void parseHead(HttpRope::const_iterator hb,HttpRope::const_iterator he);
		void parseSearchString(const string &str);
		void requestError(const string &s);


	public:

		Request(HTTPSession *s){ session=s; };
		Request(){};
		virtual ~Request();
		void setSession(HTTPSession *s){ session=s; };
		int parseRequest();
		void append(const char* s);
		const string& getMethod() const;
		const string& getURI() const;
		char *URIDecode(char *source, char *dest);
		void URIDecode(string &str);
		const HttpMap &getHeader() const;
		const string& getParameter() const;
		const string& getVersion() const;
		const string& getConnection() const;
};

class HTTPSession :public virtual bbt::SocketPort
{

	public:
		enum SessionState
		{
			SESSION_INITIAL=0,
			SESSION_GETTINGHEAD,
			SESSION_PARSINGHEAD,
			SESSION_KNOWNHEAD,
			SESSION_GETTINGBODY,
			SESSION_PARSINGBODY,
			SESSION_KNOWN
		};
		typedef enum SessionState SessionState;

	protected:
		HTTPServer *httpServer;
		Request request;
		Response response;
		SessionState sessionState;

		Semaphore sessionsem[1];
		void setSessionState(SessionState s){sessionState = s;}
		// clear socket output buffer 

	public:


		static const int	    size_of_buf=1024;

		int send(const char *buf,size_t len){ return writeData(buf,len); };
		// Nothing special to do here, it's all handled
		// by SocketPort 

		virtual ~HTTPSession();

		// When you create a HTTPSession it waits to accept a
		// connection.  This is done by it's own
		HTTPSession(HTTPServer * server,SocketService * svc);

		//
		// This is called by the SocketService thread when it the
		// object has expired.
		//

		virtual void expired();

		//
		// This is called by the SocketService thread when it detects
		// that there is somthing to read on this connection.
		//
		virtual void pending();

		virtual void output();

		virtual void disconnect();

		HTTPServer &getHTTPServer(){return *httpServer;}

		SessionState getSessionState(){return sessionState;}

		Request& getRequest(){return request;}

		Response& getResponse(){return response;}

};


// This is the main application object containing all the
// state for the application.  It uses a SocketService object
// (and thread) to do all the work, however, that object could
// theoretically be use by more than one main application.
//
// It creates a HTTPServer to sit and wait for connections
// from clients.

class CCExec 
{
	public:

		HTTPServer	       *http ;
		Semaphore		      mainsem[1];

		CCExec();

		// These methods defined later.
		virtual ~CCExec();
		int RunApp( char * hn = "bbtsolaris" );

};

//
// HTTPServer simply creates HTTPSession all the time until
// it has an error.  I suspect you could create as many of these
// as the OS could take.
//

class HTTPServer : public virtual TCPSocket, public virtual Thread 
{
	private:
		string root;
		string defaultFile;
		HttpMap mimeTypes;
		ServicePool services;

		void initmime();
		char * replace(char *str,char oldv,char newv);
		char * trim(char * str);

	public:
		static const string defaultMimeType;
		static const string defaultConnection;

		friend class HTTPSession; //for debug
		void run ();

		const string& getRoot()const{ return root;};
		void setRoot( string r="public_html"){ root=r; };
		HTTPServer(InetHostAddress & machine,int port,int service);

		virtual ~HTTPServer();
		const  string& getDefaultFile(){return defaultFile;}
		const string& getMimeType(const string& ext) const ;
		SocketService* getService(){ return services.getService();};
		void freeService(SocketService* s){ services.freeService(s) ;} ;

};

class HTTPSessionException:public Exception
{
	public:
		HTTPSessionException(const std::string &what_arg) : Exception(what_arg){};
};
#endif
