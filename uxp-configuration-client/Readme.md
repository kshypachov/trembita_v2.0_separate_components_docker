UXP-ConfClient 
Listen port 5666
    Port used for got command when via web interface add config anchor
        

        GET /execute HTTP/1.1
        Accept: text/plain, application/json, application/*+json, */*
        Accept-Encoding: gzip, x-gzip, deflate
        Host: localhost:5666
        Connection: keep-alive
        User-Agent: Apache-HttpClient/5.3.1 (Java/17.0.15)

Files adn directories 

/etc/uxp/services/confclient.conf - load durin startup 
/etc/uxp/services/global.conf - load via confclient.conf
/etc/uxp/conf.d/confclient-logback.xml - load configs via confclient.conf
/etc/uxp/services/local.conf  - load configs via confclient.conf
/usr/share/uxp/jlib/addon/confclient/*.conf - load configs via confclient.conf
/usr/share/uxp/bin/confclient.sh - startup script


Needed mount paths 
    /etc/uxp - for load configs (anchor, app configs) For storing global config
    /usr/share/uxp/ - for load configs 