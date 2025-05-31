UXP-ConfClient 
Listen port 5666
    Port used for got command when via web interface add config anchor

        GET /execute HTTP/1.1
        Accept: text/plain, application/json, application/*+json, */*
        Accept-Encoding: gzip, x-gzip, deflate
        Host: localhost:5666
        Connection: keep-alive
        User-Agent: Apache-HttpClient/5.3.1 (Java/17.0.15)

Files adnd directories 

/etc/uxp/services/confclient.conf - load durin startup 
/etc/uxp/services/global.conf - load via confclient.conf
/etc/uxp/conf.d/confclient-logback.xml - load configs via confclient.conf
/etc/uxp/services/local.conf  - load configs via confclient.conf
/usr/share/uxp/jlib/addon/confclient/*.conf - load configs via confclient.conf
/usr/share/uxp/bin/confclient.sh - startup script


Needed mount paths 
    /etc/uxp - for load configs (anchor, app configs) For storing global config
    /usr/share/uxp/ - for load configs
    /var/tmp/uxp - maybe for temperary storing configuration anchor in first start in unconfigured state, allocated 2 MB in RAM FS


Result of work start script 

+ . /etc/uxp/services/confclient.conf
++ . /etc/uxp/services/global.conf
++ JAVA_HOME=/usr/lib/jvm/java-17-openjdk-amd64
++ '[' -z /usr/share/uxp/jlib/addon ']'
++ for addon in ${ADDON_PATH}/confclient/*.conf
++ '[' -e /usr/share/uxp/jlib/addon/confclient/trembita-crypto.conf ']'
++ . /usr/share/uxp/jlib/addon/confclient/trembita-crypto.conf
+++ ADDON_CP=':/usr/share/uxp/jlib/addon/proxy/cipher-jce-provider-1.22.7.jar:/usr/share/uxp/jlib/addon/proxy/ciplus-jce/*'
+++ ADDON_PARAMS=' -Dorg.bytedeco.javacpp.noPointerGC=true -Dorg.bytedeco.javacpp.maxBytes=0 -Dorg.bytedeco.javacpp.maxPhysicalBytes=0 '
+++ export 'LD_PRELOAD= /usr/lib/x86_64-linux-gnu/libtcmalloc.so.4 '
+++ LD_PRELOAD=' /usr/lib/x86_64-linux-gnu/libtcmalloc.so.4 '
+++ export LD_LIBRARY_PATH=:/usr/share/uxp/lib
+++ LD_LIBRARY_PATH=:/usr/share/uxp/lib
++ CP=/usr/share/uxp/jlib/configuration-client.jar
++ CONFCLIENT_PARAMS=' -Dlogback.configurationFile=/etc/uxp/conf.d/confclient-logback.xml'
++ CONFCLIENT_JVM_OPTS=' -Xmx50m -XX:MaxMetaspaceSize=70m '
++ . /etc/uxp/services/local.conf
++ CONFCLIENT_PARAMS=' -Dlogback.configurationFile=/etc/uxp/conf.d/confclient-logback.xml'
+ date -R
Tue, 27 May 2025 19:46:29 +0000
+ 


exec /usr/lib/jvm/java-17-openjdk-amd64/bin/java -Xmx50m -XX:MaxMetaspaceSize=70m -Dlogback.configurationFile=/etc/uxp/conf.d/confclient-logback.xml -XX:+UseG1GC -Xshare:on -Dfile.encoding=UTF-8 -Djava.library.path=/usr/share/uxp/lib/ -cp '/usr/share/uxp/jlib/configuration-client.jar:/usr/share/uxp/jlib/addon/proxy/cipher-jce-provider-1.22.7.jar:/usr/share/uxp/jlib/addon/proxy/ciplus-jce/*' -Dorg.bytedeco.javacpp.noPointerGC=true -Dorg.bytedeco.javacpp.maxBytes=0 -Dorg.bytedeco.javacpp.maxPhysicalBytes=0 ee.cyber.uxp.common.conf.globalconf.ConfigurationClientMain

Container created without ENTRYPOIN or CDM. If you want to start app in Docker, add this ENTRYPOINT
#ENTRYPOINT ["java", \
#            "-Xmx50m", \
#            "-XX:MaxMetaspaceSize=70m", \
#            "-XX:+UseG1GC", \
#            "-Xshare:on", \
#            "-Dfile.encoding=UTF-8", \
#            "-Djava.library.path=/app/lib", \
#            "-Dlogback.configurationFile=/app/confclient-logback.xml", \
#            "-Dorg.bytedeco.javacpp.noPointerGC=true", \
#            "-Dorg.bytedeco.javacpp.maxBytes=0", \
#            "-Duxp.configuration-client.port=5564", \
#            "-Duxp.common.temp-files-path=/etc/uxp/tmp/", \
#            "-Dorg.bytedeco.javacpp.maxPhysicalBytes=0", \
#            "-cp", "/app/configuration-client.jar:/app/jlib/addon/proxy/cipher-jce-provider-1.22.7.jar:/app/jlib/addon/proxy/ciplus-jce/*", \
#            "ee.cyber.uxp.common.conf.globalconf.ConfigurationClientMain"]

For start application in kubernetes, used this construction

  command: ["/opt/java/openjdk/bin/java"]
  args:
    - "-Xmx50m"
    - "-XX:MaxMetaspaceSize=70m"
    - "-XX:+UseG1GC"
    - "-Xshare:on"
    - "-Dorg.bytedeco.javacpp.cachedir=/tmp/java"
    - "-Dfile.encoding=UTF-8"
    - "-Djava.library.path=/app/lib"
    - "-Dlogback.configurationFile=/app/confclient-logback.xml"
    - "-Dorg.bytedeco.javacpp.noPointerGC=true"
    - "-Dorg.bytedeco.javacpp.maxBytes=0"
    - "-Duxp.configuration-client.port=5665"
    - "-Duxp.common.temp-files-path=/etc/uxp/tmp/"
    - "-Dorg.bytedeco.javacpp.maxPhysicalBytes=0"
    - "-cp"
    - "/app/configuration-client.jar:/app/jlib/addon/proxy/cipher-jce-provider-1.22.7.jar:/app/jlib/addon/proxy/ciplus-jce/*"
    - "ee.cyber.uxp.common.conf.globalconf.ConfigurationClientMain"

If you start this container in kubernetes, need only /etc/uxp/ for store globalconf
Container run without shell.
Container works on read only FS, except mounted storages:
/etc/uxp/ - for storing downloaded globalconf
/var/tmp/uxp/ - do not known
/tmp/java/ - for unpacking java classes

