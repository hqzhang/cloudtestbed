
all:
	script/build.sh
	./install.sh
	
arm64:
	script/buildarm.sh


clean:
	script/clean.sh

install:
	mkdir -p /etc/canal
	mkdir -p /etc/canal/config
#	mkdir -p /etc/canal/cbb
	cp config/config.json /etc/canal/config
	cp -r templates /etc/canal
	cp -r tls /etc/canal
	
	#cp appChain /etc/canal -fr
	
uninstall:
	rm -fr /sbin/canal-agent
	rm -fr /sbin/canal-controller
	rm -fr /etc/canal

test:
