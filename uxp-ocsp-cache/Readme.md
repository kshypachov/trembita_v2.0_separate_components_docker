UXP-OCSP-cache (part of uxp-proxy)
Listen port 5766 - admin port
 GET /execute push up executing process without scheduler
Listen port 5577 - ocsp responder



Files and directories 

/etc/uxp/services/ocsp-cache.conf - load durin startup 
/etc/uxp/services/global.conf - load via ocsp-cache.conf
/etc/uxp/conf.d/ocsp-cache-logback.xml - load configs via ocsp-cache.conf
/etc/uxp/services/local.conf  - load configs via csp-cache.conf
/usr/share/uxp/jlib/addon/ocsp-cache/*.conf - load configs via ocsp-cache.conf
/usr/share/uxp/bin/ocsp-cache.sh - startup script
    Config file 
    /etc/uxp/conf.d/ocsp-cache.ini

CP="/usr/share/uxp/jlib/ocsp-cache.jar:/usr/share/uxp/jlib/signature-xades.jar"

Needed mount paths 
    /etc/uxp - for load configs (anchor, app configs) For storing global config
    /usr/share/uxp/ - for load configs