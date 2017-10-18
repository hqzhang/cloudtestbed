#include <time.h>
#include <string.h>
#include "httpservice.h"
#include <stdio.h>
#include <fstream>
#include <sys/types.h>
#include <sys/stat.h>
using namespace std;
#define ENDLINE true
static const char *const weekday[] =
{
    "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"
};

static const char *const month[] =
{
    "Jan", "Feb", "Mar", "Apr", "May", "Jun",
    "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
};


char *Response::httpTime(const time_t *btp, char *buf, int bufsize)
{
	struct tm *tpb;
	if (bufsize < 31)
		return NULL;

	//if (gmtime_r(btp, &tpb) == NULL)

	if ((tpb=gmtime(btp)) == NULL)
		return NULL;
	snprintf(buf, bufsize, "%s, %02d %s %d %02d:%02d:%02d GMT",
			weekday[tpb->tm_wday],
			tpb->tm_mday,
			month[tpb->tm_mon],
			tpb->tm_year + 1900,
			tpb->tm_hour,
			tpb->tm_min,
			tpb->tm_sec);
	return buf;
}

void Response::send(const char * s,bool endLine)
{
	if(s!=NULL)
		headBuffer.append(s);
	if(endLine)
	{
		headBuffer.push_back('\n');
		(*session).send(headBuffer.c_str(),headBuffer.size());
		headBuffer.erase(); //clear headBuffer
	}
}

void Response::sendGeneralHeaders(int code)
{
	char buf[256];
	time_t ct;

	// statue line
	sendStatusLine(code);

	// MIME-Version
	send("MIME-Version: 1.0",ENDLINE);

	// Server current time
	time(&ct);
	send("Date: ");
	send(httpTime(&ct, buf, sizeof(buf)),ENDLINE);

	// Server version
	send("Server: bhttp/2.1",ENDLINE);

	//send Connection
	send("Connection: ");
	send(session->getRequest().getConnection().c_str(),ENDLINE);

	return ;
}    


void Response::sendStatusLine(int code)
{
	char *type,temp[10];
	switch (abs(code))
	{
		case 200:
			type = "OK";
			break;

		case 201:
			type = "Created";
			break;

		case 202:
			type = "Accepted";
			break;

		case 203:
			type = "Provisional Information";
			break;

		case 204:
			type = "No Content";
			break;

		case 206:
			type = "Partial Content";
			break;

		case 300:
			type = "Multiple Choices";
			break;

		case 301:
			type = "Moved Permanently";
			break;

		case 302:
			type = "Moved Temporarily";
			break;

		case 303:
			type = "Method";
			break;

		case 304:
			type = "Not Modified";
			break;

		case 400:
			type = "Bad Request";
			break;

		case 401:
			type = "Unauthorized";
			break;

		case 402:
			type = "Payment Forbidden";
			break;

		case 403:
			type = "Forbidden";
			break;

		case 404:
			type = "Not Found";
			break;

		case 405:
			type = "Method Not Allowed";
			break;

		case 406:
			type = "None Acceptable";
			break;

		case 407:
			type = "Proxy Authentication Required";
			break;

		case 408:
			type = "Request Timeout";
			break;

		case 409:
			type = "Conflict";
			break;

		case 410:
			type = "Gone";
			break;

		case 412:
			type = "Precondition Failed";
			break;

		case 500:
			type = "Internal Server Error";
			break;

		case 501:
			type = "Not Implemented";
			break;

		case 502:
			type = "Bad Gateway";
			break;

		case 503:
			type = "Service Unavailable";
			break;

		case 504:
			type = "Gateway Timeout";
			break;

		default:
			type = "Unknown HTTP Code";
	}

	if((*session).getRequest().getVersion() == "HTTP/1.1")
	{ 
		send("HTTP/1.1 ");
	}
	else
	{
		send("HTTP/1.0 ");
	}
	sprintf(temp,"%d",code);
	send(temp);
	send(" ");
	send(type,ENDLINE);
}

void Response::sendLastModified(time_t ct)
{
    char buf[64];
    
    send("Last-Modified: ");
    send(httpTime(&ct, buf, sizeof(buf)),ENDLINE);
}

void Response::sendContentType(const string& path)
{
	string type;
	string::size_type cp = path.rfind('.');
	if(cp != string::npos)
	{
		type=session->getHTTPServer().getMimeType(path.substr(++cp));
	}else{
		type = HTTPServer::defaultMimeType;
	}
	char buf[64];
	sprintf(buf,"Content-type: %s",type.c_str());
#ifdef DEBUG
	cerr<<buf<<endl;
#endif
	send(buf,ENDLINE);

}


void Response::sendContentSize(size_t size)
{
	char buf[64];
	snprintf(buf,sizeof(buf),"%d",size);
	send("Content-length: ");
	send(buf,ENDLINE);
}


int Response::httpError(int code,const string& value)
{
	char *type, buf[256];
	char *servername=NULL;

	// http version less than 1.0, don't receive head
	if ((*session).getRequest().getHeader().empty())
	{
		sendGeneralHeaders(code);
		send("Content-Type: text/html",ENDLINE);
		send(NULL,ENDLINE); //send NULL LINE
	}

	if ((*session).getRequest().getMethod()== "HEAD") 
		return code;
	/*	outputs standard errors, including status lines, and body text. */
	send("<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 3.2 Final//EN\"> \
			<HTML><HEAD><TITLE>Error ");
	snprintf(buf,sizeof(buf),"%d%s",code , "</TITLE></HEAD><BODY>");
	send(buf);
	switch (code)
	{
		case 404: send("<H1>Not Found</H1>The requested URL ");
				  send(value.c_str());
				  send(" was not found on this server."); 
				  break;
		case 400: send("<H1>Bad Request</H1>Your browser sent a request that this server \
						  could not understand.<P>Client sent HTTP/1.1 request without hostname \
						  (see RFC2068 section 9, and 14.23)");
				  break;
		case 501: send("<H1>Method Not Implemented</H1>");
				  snprintf(buf,sizeof(buf),"%s%s%s%s", value.c_str()," not supported.<P>Invalid \
						  method in request ",value.c_str(), ".");
				  send(buf);
				  break;
		case 505:send("<H1>HTTP Version Not Supported</H1>Your browser uses an HTTP \
						 version this server cannot serve. Please upgrade your browser to a version \
						 that uses at least HTTP/1.0.");
				 break;
		case 601: send("<H1>Protocol Not Implemented</H1>Your browser sent a request \
						  using a protocol that is not implemented. <P>Client used protocol ");
				  snprintf(buf,sizeof(buf),"%s%s",value.c_str(), ".");
				  send(buf);
defautl:send("<H1> Connection refused </H1>");
	}

	sendFooter(); 
	send("</BODY></HTML>",ENDLINE);


}

void Response::sendFooter()
{
	//implement later
}

void Response::process()
{

	struct stat st;
	bool cgi=false;
	string path=session->getHTTPServer().getRoot();
	int len;
	ifstream resource;
	const string& method=session->getRequest().getMethod();

	char buf[1024];
	path.append(session->getRequest().getURI());

	if (path[path.size()-1] == '/')
		path.append(session->getHTTPServer().getDefaultFile()); 

	session->setCompletion(true);  // block write
	if(method=="GET")
	{
		//todo get method
		if (stat(path.c_str(), &st) == -1) 
		{
			httpError(404,path);
		}
		else
		{
			if ((st.st_mode & S_IFMT) == S_IFDIR)
				path.append( session->getHTTPServer().getDefaultFile());
			if ((st.st_mode & S_IXUSR) ||
					(st.st_mode & S_IXGRP) ||
					(st.st_mode & S_IXOTH)    )
				cgi = true;
			if (!cgi)
			{

				resource.open(path.c_str());
				if (!resource.is_open())
					httpError(404,path);
				else
				{
					sendGeneralHeaders(200);

					// send file header
					sendLastModified(st.st_mtime);
					sendContentType(path);
					sendContentSize(st.st_size);
					send(NULL,ENDLINE);  // finish head

					int lens=0;
					while (resource.read(buf,sizeof(buf)))
					{
						lens+=session->send(buf, sizeof(buf));
					}
					//send last one
					lens+=session->send(buf,resource.gcount());
#ifdef DEBUG
					cerr<<"send "<<lens<<" bytes"<<endl;
#endif //DEBUG
					resource.close();
				}
			}
			else
			{
				execgi();
			}
		}

	}
	else if(method == "HEAD")
	{
		//do head method
		if (stat(path.c_str(), &st) == -1) 
		{
			httpError(404,path);
		}
		else
		{
			sendGeneralHeaders(200);
			sendLastModified(st.st_mtime);
			sendContentType(path);
			sendContentSize(st.st_size);
			send(NULL,ENDLINE); 
		}
	}
	else if(method == "POST")
	{
		//do post method
		if (stat(path.c_str(), &st) == -1) {
			httpError(404,path);
		}
		else
		{
			if ((st.st_mode & S_IXUSR) ||
					(st.st_mode & S_IXGRP) ||
					(st.st_mode & S_IXOTH)    )
				execgi();
			else
			{
				httpError(403,path);
			}	
		}


	}
	else if(method == "TRACE")
	{
		//do trace method
	}
	else httpError(501,method);
}

void Response::execgi()
{

	// todo execute cgi
}
