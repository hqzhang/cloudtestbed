#include "httpservice.h"
#include <strings.h>
#include <cstring>
#include <algorithm>
using namespace std;
void Request::URIDecode(string &str)
{
	char *source=strdup(str.c_str());
	source=URIDecode(source,NULL);
	str = string(source);

}
char *Request::URIDecode( char * source, char *dest)
{
	char *ret;

	if(!dest)
		dest = source;

	ret = dest;

	while(*source)
	{
		switch(*source)
		{
			case '+':
				*(dest++) = ' ';
				break;
			case '%':
				dest[0] = source[1];
				dest[1] = source[2];
				dest[2] = 0;
				*(dest++) = (char)strtol(dest, NULL, 16);
				source += 2;
				break;
			default:
				*(dest++) = *source;
		}
		++source;
	}
	*(dest++) = 0;
	return ret;
}
/*
char * Request::Readline(char *buf, size_t buflen,int *next)
{
	bool crlf = false;
	bool nl = false;
	size_t nstat=buflen;
	if(*next<0||*next>buflen-1) return NULL;
	int c;
	int head;

	c = *next;
	head=c;
	while(c < buflen && !nl)
	{
		if(buf[c] == '\r' && c < buflen - 1)
		{
			if(buf[c + 1] == '\n')
			{
				c+= 2;
				nl = true;
				crlf = true;
				break;
			}
		}
		if(buf[c] == '\n')
		{
			++c;
			nl = true;
			break;
		}
		++c;
	}
		
	*next=c;
	if(crlf)
		--c;
	//if n1==false , the last part isn't a whole line.
	// isn't there any problem?
	buf[c - 1] = '\0';

	return buf+head;
}
*/
void Request::parseHead(HttpRope::const_iterator hb,HttpRope::const_iterator he)
{
	HttpRope::const_iterator hpos,hline,sb;
	string name,value;
	string token = " \t";
	char newline = '\n';

	for(;;)
	{
		hline=find(hb,he,newline);
		if(hline == he)
			break;
		sb = hb;  // begin of this line
		hb = hline+1;  // hb is start of next line

		hpos = find(sb,hline,':');
		if(hpos == hline) //ignor illegal line
			continue;

		name=string(requestBuf.substr(sb,hpos).c_str());

		++hpos;


		value=string(requestBuf.substr(hpos,hline).c_str());

		// strap string leading and ending char: ' ' '\t'
		string::size_type cb,ce;
		cb=value.find_first_not_of(token);
		ce=value.find_last_not_of(token);
		if(cb != ce)
		{
			value=value.substr(cb,ce-cb+1);
		}else
		{
			value.erase(); //empty container
		}


		httpHeader.insert(pair<const string,string>(name,value));
	}

}

int Request::parseRequest()
{
	HttpRope::const_iterator istart,iend,ipos,ifpos;
	istart=requestBuf.begin();
	iend=requestBuf.end();

	
#ifdef DEBUG
	cerr<<"received Request Head is:\n"<<requestBuf.c_str()<<endl;
#endif
	
	string token(" \t");
	string::iterator tb = token.begin();
	string::iterator te = token.end();

	//get first line 
	ipos = find(istart,iend,'\n');
	if (ipos == iend)
	{
		requestError("Request Error:line Error\n");
		return -1;
	}


	ifpos=find_first_of(istart,ipos,tb,te); 
	if(ifpos == ipos )
	{
		requestError("Request Error:Method Error\n");
		return -1;
	}

	method = string(requestBuf.substr(istart,ifpos).c_str());

	istart= ++ifpos;
	ifpos=find_first_of(istart,ipos,tb,te);
	if(ifpos == ipos )
	{
		requestError("Request Error\n");
		return -1;
	}

	uri = string(requestBuf.substr(istart,ifpos).c_str());
	version = string(requestBuf.substr(++ifpos,ipos).c_str());


	parseHead(++ipos,iend);

	// Get real url and parse search string follow the URL 

	URIDecode(uri); 
	string::size_type cp = uri.find('?');
	if (cp != string::npos)
	{
		parseSearchString(uri.substr(cp));
		uri=uri.substr(0,cp);
	}

	requestBuf = HttpRope(); //empty rope
	return 0;
}

void Request::append(const char* s)
{
	requestBuf.append(s);
}

void Request::parseSearchString(const string& str)
{ 
	//implement later
	parameter=str;
}

const string& Request::getMethod() const
{
	return method;
}

const string& Request::getURI() const
{
	return uri;
}


const HttpMap& Request::getHeader() const
{
	return httpHeader;
}
const string& Request::getConnection()const
{
	HttpMap::const_iterator it;
	/*	for(it=httpheader.begin();it!=httpheader.end();it++) 
		if(!strcasecmp("Connection",it->first)) return it->second;
		return it->second; */
	it=httpHeader.find("Connection");
	if(it!=httpHeader.end()) return it->second;
	return HTTPServer::defaultConnection;
}

const string& Request::getParameter()const
{
	return parameter;
}

const string& Request::getVersion() const
{
	return version;
}

Request::~Request()
{
}

void Request::requestError(const string &s)
{
	switch(Thread::getException())
	{
		case Thread::throwObject:
			throw this;
		case Thread::throwException:
			throw HTTPSessionException(s);
		case Thread::throwNothing:
			break;
	}
}
