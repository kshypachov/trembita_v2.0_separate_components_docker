UXP-Message-log-archiver (part of uxp-proxy)
Listen port 5765

 GET /execute push up executing process without scheduler



Files and directories 

/etc/uxp/services/messagelog-archiver.conf - load durin startup 
/etc/uxp/services/global.conf - load via messagelog-archiver.conf
/etc/uxp/conf.d/messagelog-archiver-logback.xml - load configs via confclient.conf
/etc/uxp/services/local.conf  - load configs via confclient.conf
/usr/share/uxp/jlib/addon/messagelog-archiver/*.conf - load configs via confclient.conf
/usr/share/uxp/bin/confclient.sh - startup script

CP="/usr/share/uxp/jlib/messagelog-archiver.jar:/usr/share/uxp/jlib/signature-xades.jar"

Needed mount paths 
    /etc/uxp - for load configs (anchor, app configs) For storing global config
    /usr/share/uxp/ - for load configs 
    /var/lib/uxp/ - for reading transaction information, stored in fs (timestamps, signatures, etc)