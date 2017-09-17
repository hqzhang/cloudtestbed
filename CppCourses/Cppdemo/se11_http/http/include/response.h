#include "httpservice.h"
#include <time.h>
class Response{
	HTTPSessison session;
	
	void send(char * s);
public:
	Response(HTTPSession session){this.seesion=session;};
	void sendstateline();
	// using HTTPSession Send to send data
	char *httptime(const time_t *btp, char *buf, int bufsize);

	void sendheaders(int code,char *type);


	void sendnl();

	void sendstatusline(int code);

	void sendlastmodified(time_t ct);

	int httperror(int code,char * str);
	       
	void sendfooter();

	void prosess();


}

