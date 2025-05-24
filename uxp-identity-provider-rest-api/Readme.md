UXP-identity-provider-rest-api
    Listen port 0.0.0.0 8087 
    Port use for provide authentication OAuth2 API.
    In original producat all URI with prefix /auth-api , are redirect to port 8087.
    Full api specification you can  find in swagger on security server.

Files and directories

/etc/uxp/services/identity-provider-rest-api.conf - load durin startup 
/etc/uxp/services/global.conf - load via identity-provider-rest-api.conf
/etc/uxp/conf.d/identity-provider-rest-api-logback.xml - load via identity-provider-rest-api.conf
/etc/uxp/services/local.conf - load via identity-provider-rest-api.conf
/usr/share/uxp/bin/identity-provider-rest-api.sh - startup script
/etc/uxp/conf.d/identity-provider.ini - configuration script

CP="/usr/share/uxp/jlib/identity-provider-rest-api.jar"