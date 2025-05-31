UXP-monitor agent. Monitor server status , ocsp, to add...

Listen port 2080 for getting tranzaction from uxp-proxy component. 
Example of proxy call to uxp-monitor

POST /store_data HTTP/1.1
Accept-Encoding: gzip, x-gzip, deflate
Content-Length: 808
Content-Type: application/json; charset=UTF-8
Host: 127.0.0.1:2080
Connection: keep-alive
User-Agent: Apache-HttpClient/5.3.1 (Java/17.0.15)

{"records":[{"serviceXRoadInstance":"test1","serviceCode":"clientReg","serviceSecurityServerAddress":"192.168.99.185","requestAttachmentCount":0,"requestOutTs":1748684367108,"serviceSubsystemCode":"MGMT","responseAttachmentCount":0,"clientMemberCode":"00000089","requestType":"SOAP","responseInTs":1748684368473,"messageProtocolVersion":"4.0","messageId":"ae6da682-ccf5-4d65-b13b-7a67b833d131","clientXRoadInstance":"test1","clientMemberClass":"GOV","serviceMemberCode":"00000001","transactionId":"24b6d06c-3e03-11f0-a847-c3ae802a6ac1","securityServerType":"Client","securityServerInternalIp":"192.168.99.203","serviceMemberClass":"GOV","requestInTs":1748684367043,"clientSecurityServerAddress":"192.168.99.203","requestSoapSize":1285,"responseOutTs":1748684368630,"responseSoapSize":1522,"succeeded":true}]}

Listen port 2082, named Status port. Answer 200 if SEG operable.

Listen port 5588, named Admin port. Do not know for which purposes.

In original, monolith system starts as service with helping script /usr/share/uxp/scripts/monitor.sh

output of this script with optin set -x :

+ . /etc/uxp/services/monitor-agent.conf
++ . /etc/uxp/services/global.conf
+++ JAVA_HOME=/usr/lib/jvm/java-17-openjdk-amd64
+++ PATH=/usr/lib/jvm/java-17-openjdk-amd64/bin:/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin:/snap/bin
+++ ADDON_PATH=/usr/share/uxp/jlib/addon
+++ umask 0027
+++ UXP_PARAMS=' -XX:+UseG1GC -Xshare:on -Dfile.encoding=UTF-8 -Djava.library.path=/usr/share/uxp/lib/ '
++ for addon in ${ADDON_PATH}/monitor/*.conf
++ '[' -e /usr/share/uxp/jlib/addon/monitor/trembita-crypto.conf ']'
++ . /usr/share/uxp/jlib/addon/monitor/trembita-crypto.conf
+++ ADDON_CP=':/usr/share/uxp/jlib/addon/proxy/cipher-jce-provider-1.22.7.jar:/usr/share/uxp/jlib/addon/proxy/ciplus-jce/*'
+++ ADDON_PARAMS=' -Dorg.bytedeco.javacpp.noPointerGC=true -Dorg.bytedeco.javacpp.maxBytes=0 -Dorg.bytedeco.javacpp.maxPhysicalBytes=0 '
+++ export 'LD_PRELOAD= /usr/lib/x86_64-linux-gnu/libtcmalloc.so.4 '
+++ LD_PRELOAD=' /usr/lib/x86_64-linux-gnu/libtcmalloc.so.4 '
+++ export LD_LIBRARY_PATH=:/usr/share/uxp/lib
+++ LD_LIBRARY_PATH=:/usr/share/uxp/lib
++ CP=/usr/share/uxp/jlib/monitoring-proxy-agent.jar:/usr/share/uxp/jlib/signature-xades.jar
++ MONITOR_PARAMS=' -Dlogback.configurationFile=/etc/uxp/conf.d/addons/proxy-monitor-agent-logback.xml '
++ MONITOR_JVM_OPTS=' -Xms50m -Xmx256m -XX:MaxMetaspaceSize=128m '
++ . /etc/uxp/services/local.conf
+ date -R
Sat, 31 May 2025 14:24:58 +0300
+ exec /usr/lib/jvm/java-17-openjdk-amd64/bin/java -Xms50m -Xmx256m -XX:MaxMetaspaceSize=128m -Dlogback.configurationFile=/etc/uxp/conf.d/addons/proxy-monitor-agent-logback.xml -XX:+UseG1GC -Xshare:on -Dfile.encoding=UTF-8 -Djava.library.path=/usr/share/uxp/lib/ -cp '/usr/share/uxp/jlib/monitoring-proxy-agent.jar:/usr/share/uxp/jlib/signature-xades.jar:/usr/share/uxp/jlib/addon/proxy/cipher-jce-provider-1.22.7.jar:/usr/share/uxp/jlib/addon/proxy/ciplus-jce/*' -Dorg.bytedeco.javacpp.noPointerGC=true -Dorg.bytedeco.javacpp.maxBytes=0 -Dorg.bytedeco.javacpp.maxPhysicalBytes=0 ee.cyber.uxp.proxymonitoragent.ProxyMonitorAgentMain

Options readed from java VM by 

jcmd 235013 VM.command_line
235013:
VM Arguments:
jvm_args: -Xms50m -Xmx256m -XX:MaxMetaspaceSize=128m -Dlogback.configurationFile=/etc/uxp/conf.d/addons/proxy-monitor-agent-logback.xml -XX:+UseG1GC -Xshare:on -Dfile.encoding=UTF-8 -Djava.library.path=/usr/share/uxp/lib/ -Dorg.bytedeco.javacpp.noPointerGC=true -Dorg.bytedeco.javacpp.maxBytes=0 -Dorg.bytedeco.javacpp.maxPhysicalBytes=0 
java_command: ee.cyber.uxp.proxymonitoragent.ProxyMonitorAgentMain
java_class_path (initial): /usr/share/uxp/jlib/monitoring-proxy-agent.jar:/usr/share/uxp/jlib/signature-xades.jar:/usr/share/uxp/jlib/addon/proxy/cipher-jce-provider-1.22.7.jar:/usr/share/uxp/jlib/addon/proxy/ciplus-jce/cipherplus-1.0.28-1.5.8-linux-x86_64.jar:/usr/share/uxp/jlib/addon/proxy/ciplus-jce/javacpp-1.5.8.jar:/usr/share/uxp/jlib/addon/proxy/ciplus-jce/pkcs11-wrapper-1.6.9-1.jar:/usr/share/uxp/jlib/addon/proxy/ciplus-jce/ciplus-jce-1.0.24.jar:/usr/share/uxp/jlib/addon/proxy/ciplus-jce/cipherplus-1.0.28-1.5.8.jar
Launcher Type: SUN_STANDARD

jcmd 235013 VM.flags
235013:
-XX:CICompilerCount=3 -XX:CompressedClassSpaceSize=117440512 -XX:ConcGCThreads=1 -XX:G1ConcRefinementThreads=4 -XX:G1EagerReclaimRemSetThreshold=8 -XX:G1HeapRegionSize=1048576 -XX:GCDrainStackTargetSize=64 -XX:InitialHeapSize=52428800 -XX:MarkStackSize=4194304 -XX:MaxHeapSize=268435456 -XX:MaxMetaspaceSize=134217728 -XX:MaxNewSize=160432128 -XX:MinHeapDeltaBytes=1048576 -XX:MinHeapSize=52428800 -XX:NonNMethodCodeHeapSize=5832780 -XX:NonProfiledCodeHeapSize=122912730 -XX:ProfiledCodeHeapSize=122912730 -XX:+RequireSharedSpaces -XX:ReservedCodeCacheSize=251658240 -XX:+SegmentedCodeCache -XX:SoftMaxHeapSize=268435456 -XX:-THPStackMitigation -XX:+UseCompressedClassPointers -XX:+UseCompressedOops -XX:+UseFastUnorderedTimeStamps -XX:+UseG1GC -XX:+UseSharedSpaces 


jcmd 235013 VM.system_properties
235013:
#Sat May 31 15:03:54 EEST 2025
uxp.proxy-monitoring-agent.ignored-network-interfaces=lo
uxp.identity-provider.security-server-client-secret=2DmVrz_VUQUhn3ePNgWm8Ur-TwMK0la_
java.specification.version=17
uxp.common.temp-files-path=/var/tmp/uxp/
sun.jnu.encoding=UTF-8
sun.arch.data.model=64
org.bytedeco.javacpp.noPointerGC=true
org.terracotta.quartz.skipUpdateCheck=true
java.vendor.url=https\://ubuntu.com/
uxp.proxy.internal-cipher-suites=TLS_AES_256_GCM_SHA384,TLS_ECDHE_ECDSA_WITH_AES_256_GCM_SHA384
sun.boot.library.path=/usr/lib/jvm/java-17-openjdk-amd64/lib
sun.java.command=ee.cyber.uxp.proxymonitoragent.ProxyMonitorAgentMain
jdk.debug=release
uxp.status-service.listen-port=2082
uxp.center.allowed-certificate-profiles=ee.cyber.uxp.common.certificateprofile.ua.UaCertificateProfileInfoProvider
java.specification.vendor=Oracle Corporation
uxp.common.license-file=/etc/uxp/license.lic
java.version.date=2025-04-15
java.home=/usr/lib/jvm/java-17-openjdk-amd64
file.separator=/
java.vm.compressedOopsMode=32-bit
line.separator=\n
uxp.message-log.timestamp-provider-round-robin=true
uxp.message-log.archive-interval=0 0 0/1 1/1 * ? *
java.vm.specification.vendor=Oracle Corporation
java.specification.name=Java Platform API Specification
uxp.message-log-s3.address=https\://192.168.99.136\:9000
uxp.proxy.timestamp-verify-signer-chain=true
uxp.proxy.client-jetty-thread-pool-max-size=60
uxp.proxy-monitoring-agent.monitor-agent-conf-file=/etc/uxp/monitor-agent.ini
uxp.op-monitor.max-records-in-payload=10000
sun.management.compiler=HotSpot 64-Bit Tiered Compilers
uxp.op-monitor.clean-interval=0 0 0/12 1/1 * ? *
java.runtime.version=17.0.15+6-Ubuntu-0ubuntu122.04
uxp.common.template-path=/usr/share/uxp/templates/
user.name=uxp
uxp.proxy-monitoring-agent.sending-interval-seconds=180
uxp.common.conf-backup-digest-files-checked=true
uxp.message-log.timestamp-immediately=true
uxp.monitoring-server.opdata-stats-collection-enabled=true
file.encoding=UTF-8
org.bytedeco.javacpp.maxPhysicalBytes=0
uxp.message-log-s3.trusted-certificate=/etc/uxp/ssl/public.crt
uxp.common.configuration-anchor-file=/etc/uxp/configuration-anchor.xml
uxp.op-monitor.health-statistics-period-seconds=600
uxp.proxy-monitoring-agent.admin-port=5588
java.io.tmpdir=/tmp
java.version=17.0.15
uxp.common.device-templates-path=/etc/uxp/device-templates/
uxp.proxy.log-metaservice-signatures=false
uxp.proxy.transport-cipher-suites=TLS_AES_256_GCM_SHA384
uxp.message-log-s3.bucket-name=uxp-messagelog1227
java.vm.specification.name=Java Virtual Machine Specification
org.bytedeco.javacpp.maxBytes=0
native.encoding=UTF-8
java.library.path=/usr/share/uxp/lib/
java.vendor=Ubuntu
java.specification.maintenance.version=1
uxp.common.expiration-warning-threshold-days=32
uxp.proxy.server-jetty-thread-pool-max-size=60
sun.io.unicode.encoding=UnicodeLittle
uxp.proxy.log-monitoring-signatures=false
uxp.proxy-monitoring-agent.params-collecting-interval-seconds=15
uxp.proxy.round-robin-quarantine-time=300000
java.class.path=/usr/share/uxp/jlib/monitoring-proxy-agent.jar\:/usr/share/uxp/jlib/signature-xades.jar\:/usr/share/uxp/jlib/addon/proxy/cipher-jce-provider-1.22.7.jar\:/usr/share/uxp/jlib/addon/proxy/ciplus-jce/cipherplus-1.0.28-1.5.8-linux-x86_64.jar\:/usr/share/uxp/jlib/addon/proxy/ciplus-jce/javacpp-1.5.8.jar\:/usr/share/uxp/jlib/addon/proxy/ciplus-jce/pkcs11-wrapper-1.6.9-1.jar\:/usr/share/uxp/jlib/addon/proxy/ciplus-jce/ciplus-jce-1.0.24.jar\:/usr/share/uxp/jlib/addon/proxy/ciplus-jce/cipherplus-1.0.28-1.5.8.jar
uxp.proxy.batch-signatures-enabled=false
uxp.common.conf-backup-digest-algorithm-id=SHA-512
java.vm.vendor=Ubuntu
uxp.common.digest-chunk-size=262144
user.timezone=Europe/Kyiv
uxp.proxy-monitoring-agent.net-stats-file=/proc/net/dev
java.vm.specification.version=17
os.name=Linux
uxp.proxy-monitoring-agent.zabbix-configurator-client-read-timeout-seconds=300
sun.java.launcher=SUN_STANDARD
user.country=UA
uxp.proxy-status-check.serverproxy-listening-switch-enabled=true
sun.cpu.endian=little
user.home=/var/lib/uxp
user.language=uk
uxp.status-service.allowed-hosts=127.0.0.1
uxp.message-log-s3.access-key=pC9hJZZJZdSdWAOqIgIT
uxp.message-log-s3.secret-key=3daHty2NiBbzwJvZWCZkgTt7SUrR7pfqHN7DNFFZ
uxp.identity-provider.security-server-client-id=pvoqbggvvzpon1r4v55b7z8cu0de18cj
uxp.status-service.listen-address=127.0.0.1
uxp.proxy-monitoring-agent.port=2080
uxp.proxy-monitoring-agent.zabbix-configurator-client-connect-timeout-seconds=30
logback.configurationFile=/etc/uxp/conf.d/addons/proxy-monitor-agent-logback.xml
uxp.common.conf-path=/etc/uxp/
uxp.proxy.signature-timestamp-required=true
uxp.op-monitor.max-stats-records-in-payload=10000
uxp.monitoring-server.opdata-stats-polling-interval-seconds=900
uxp.common.rsa-allowed=false
path.separator=\:
os.version=5.15.0-125-generic
uxp.common.global-conf-path=/etc/uxp/globalconf/
uxp.common.tls-conf-path=/etc/uxp/ssl/
java.runtime.name=OpenJDK Runtime Environment
uxp.message-log.archive-storage-type=s3
uxp.common.pkcs12-provider-name=CiPlusJCE
uxp.op-monitor.records-available-timestamp-offset-seconds=60
java.vm.name=OpenJDK 64-Bit Server VM
uxp.monitoring-server.opdata-stats-period-seconds=300
uxp.proxy-status-check.clientproxy-listening-switch-enabled=true
java.vendor.url.bug=https\://bugs.launchpad.net/ubuntu/+source/openjdk-17
jetty.git.hash=e77516598a07cca826d27fa8a4f7c70e953921a6
uxp.proxy.verify-signing-certificate-qualified=true
user.dir=/
os.arch=amd64
java.vm.info=mixed mode, sharing
java.vm.version=17.0.15+6-Ubuntu-0ubuntu122.04
java.class.version=61.0
uxp.proxy.max-retained-soap-attachment-size-bytes=5242880
uxp.op-monitor.keep-records-for-days=7