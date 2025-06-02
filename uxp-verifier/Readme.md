UXP-Verifier
Listen port 8086 - api port
Need connection to identity provider for check authentication token

Need access to directory /var/lib/uxp/messagelog/ for read proxy stored transactions 
Need access to s3 (if configured) for show transactions from s3 storage.
Need access to /etc/uxp/conf.d/proxy.ini - do not started without parameters from this file
Need access to /etc/uxp/db.properties - do not start without connection to db.

+ . /etc/uxp/services/verifier-rest-api.conf
++ . /etc/uxp/services/global.conf
++ for addon in ${ADDON_PATH}/verifier-rest-api/*.conf
++ '[' -e /usr/share/uxp/jlib/addon/verifier-rest-api/certprofile-trembita-springboot.conf ']'
++ . /usr/share/uxp/jlib/addon/verifier-rest-api/certprofile-trembita-springboot.conf
+++ ADDON_CP=,/usr/share/uxp/jlib/addon/certprofile-trembita.jar
++ for addon in ${ADDON_PATH}/verifier-rest-api/*.conf
++ '[' -e /usr/share/uxp/jlib/addon/verifier-rest-api/trembita-crypto-springboot.conf ']'
++ . /usr/share/uxp/jlib/addon/verifier-rest-api/trembita-crypto-springboot.conf
+++ ADDON_CP=',/usr/share/uxp/jlib/addon/certprofile-trembita.jar,/usr/share/uxp/jlib/addon/proxy/cipher-jce-provider-1.22.7.jar,/usr/share/uxp/jlib/addon/proxy/ciplus-jce/*'
+++ ADDON_PARAMS=' -Dorg.bytedeco.javacpp.noPointerGC=true -Dorg.bytedeco.javacpp.maxBytes=0 -Dorg.bytedeco.javacpp.maxPhysicalBytes=0 '
+++ export LD_LIBRARY_PATH=:/usr/share/uxp/lib
+++ LD_LIBRARY_PATH=:/usr/share/uxp/lib
++ CP=/usr/share/uxp/jlib/verifier-rest-api.jar
++ BOOT_CP=/usr/share/uxp/jlib/signature-xades.jar
++ VERIFIER_REST_API_PARAMS=' -Dserver.port=8086 -Dlogging.config=/etc/uxp/conf.d/verifier-rest-api-logback.xml'
++ VERIFIER_REST_API_JVM_OPTS=' -Xmx128m -XX:MaxMetaspaceSize=128m'
++ . /etc/uxp/services/local.conf
+ date -R
Sun, 01 Jun 2025 19:14:49 +0300
+ exec /usr/lib/jvm/java-17-openjdk-amd64/bin/java -Xmx128m -XX:MaxMetaspaceSize=128m -Dserver.port=8086 -Dlogging.config=/etc/uxp/conf.d/verifier-rest-api-logback.xml -XX:+UseG1GC -Xshare:on -Dfile.encoding=UTF-8 -Djava.library.path=/usr/share/uxp/lib/ -cp /usr/share/uxp/jlib/verifier-rest-api.jar '-Dloader.path=/usr/share/uxp/jlib/signature-xades.jar,,/usr/share/uxp/jlib/addon/certprofile-trembita.jar,/usr/share/uxp/jlib/addon/proxy/cipher-jce-provider-1.22.7.jar,/usr/share/uxp/jlib/addon/proxy/ciplus-jce/*' -Dorg.bytedeco.javacpp.noPointerGC=true -Dorg.bytedeco.javacpp.maxBytes=0 -Dorg.bytedeco.javacpp.maxPhysicalBytes=0 org.springframework.boot.loader.PropertiesLauncher


jcmd 596076  VM.command_line
596076:
VM Arguments:
jvm_args: -Xmx128m -XX:MaxMetaspaceSize=128m -Dserver.port=8086 -Dlogging.config=/etc/uxp/conf.d/verifier-rest-api-logback.xml -XX:+UseG1GC -Xshare:on -Dfile.encoding=UTF-8 -Djava.library.path=/usr/share/uxp/lib/ -Dloader.path=/usr/share/uxp/jlib/signature-xades.jar,,/usr/share/uxp/jlib/addon/certprofile-trembita.jar,/usr/share/uxp/jlib/addon/proxy/cipher-jce-provider-1.22.7.jar,/usr/share/uxp/jlib/addon/proxy/ciplus-jce/* -Dorg.bytedeco.javacpp.noPointerGC=true -Dorg.bytedeco.javacpp.maxBytes=0 -Dorg.bytedeco.javacpp.maxPhysicalBytes=0 
java_command: org.springframework.boot.loader.PropertiesLauncher
java_class_path (initial): /usr/share/uxp/jlib/verifier-rest-api.jar
Launcher Type: SUN_STANDARD


jcmd 596076 VM.system_properties
596076:
#Sun Jun 01 19:19:59 EEST 2025
uxp.identity-provider.security-server-client-secret=2DmVrz_VUQUhn3ePNgWm8Ur-TwMK0la_
java.specification.version=17
uxp.common.temp-files-path=/var/tmp/uxp/
uxp.proxy.openapi-download-read-timeout=5000
uxp.proxy.ocsp-responder-client-read-timeout=30000
sun.arch.data.model=64
uxp.proxy.server-port=5500
org.bytedeco.javacpp.noPointerGC=true
java.vendor.url=https\://ubuntu.com/
uxp.identity-provider.oauth2-issuer-location=
uxp.op-monitor-buffer.httpclient-read-timeout=60000
uxp.pkcs11.signing-session-pool-wait-time-seconds=10
uxp.identity-provider.bcrypt..log-rounds=10
uxp.proxy.software-token-batch-signatures=false
uxp.message-log.timestamper-batch-size=10000
sun.boot.library.path=/usr/lib/jvm/java-17-openjdk-amd64/lib
sun.java.command=org.springframework.boot.loader.PropertiesLauncher
uxp.proxy.log-signatures=true
jdk.debug=release
uxp.status-service.listen-port=2082
java.specification.vendor=Oracle Corporation
uxp.common.license-file=/etc/uxp/license.lic
java.version.date=2025-04-15
java.home=/usr/lib/jvm/java-17-openjdk-amd64
logging.config=/etc/uxp/conf.d/verifier-rest-api-logback.xml
uxp.proxy.server-connection-accept-rate-limit-period=1
uxp.message-log.timestamp-provider-round-robin=true
uxp.proxy.server-stapling-ocsp-cache-lifetime=300
java.specification.name=Java Platform API Specification
java.vm.specification.vendor=Oracle Corporation
uxp.proxy.wsdl-download-connect-timeout=10000
uxp.anti-dos.max-cpu-load=1.1
uxp.proxy.client-jetty-thread-pool-max-size=60
sun.management.compiler=HotSpot 64-Bit Tiered Compilers
uxp.op-monitor.clean-interval=0 0 0/12 1/1 * ? *
java.runtime.version=17.0.15+6-Ubuntu-0ubuntu122.04
uxp.common.template-path=/usr/share/uxp/templates/
uxp.proxy.server-listen-address=0.0.0.0
uxp.common.conf-backup-digest-files-checked=true
uxp.identity-provider.public-client-access-token-time-to-live=180
uxp.monitoring-server.opdata-stats-collection-enabled=true
file.encoding=UTF-8
org.bytedeco.javacpp.maxPhysicalBytes=0
uxp.message-log-s3.trusted-certificate=/etc/uxp/ssl/public.crt
server.port=8086
uxp.common.configuration-anchor-file=/etc/uxp/configuration-anchor.xml
uxp.proxy.connector-host=0.0.0.0
uxp.proxy.client-httpclient-read-timeout=300000
uxp.op-monitor.health-statistics-period-seconds=600
uxp.proxy-monitoring-agent.admin-port=5588
java.io.tmpdir=/tmp
java.version=17.0.15
uxp.common.device-templates-path=/etc/uxp/device-templates/
uxp.proxy.server-connector-socket-buffer-size=16384
uxp.message-log-s3.bucket-name=uxp-messagelog1227
java.vm.specification.name=Java Virtual Machine Specification
uxp.proxy.software-token-key-dir=/etc/uxp/signer/
CONSOLE_LOG_CHARSET=UTF-8
native.encoding=UTF-8
java.library.path=/usr/share/uxp/lib/
java.vendor=Ubuntu
java.specification.maintenance.version=1
sun.io.unicode.encoding=UnicodeLittle
uxp.proxy-monitoring-agent.params-collecting-interval-seconds=15
uxp.proxy.client-httpclient-connect-timeout=30000
uxp.message-log.archiver-batch-size=100
uxp.proxy.round-robin-quarantine-time=300000
uxp.op-monitor-buffer.sending-interval-seconds=5
uxp.proxy.batch-signatures-enabled=false
uxp.op-monitor-buffer.max-records-in-message=500
uxp.common.digest-chunk-size=262144
uxp.message-log.archive-transfer-command=
user.timezone=Europe/Kyiv
org.jboss.logging.provider=slf4j
java.vm.specification.version=17
os.name=Linux
user.home=/var/lib/uxp
uxp.message-log-s3.secret-key=3daHty2NiBbzwJvZWCZkgTt7SUrR7pfqHN7DNFFZ
uxp.identity-provider.security-server-client-id=pvoqbggvvzpon1r4v55b7z8cu0de18cj
uxp.message-log.metadata-record-cleanup-operation=DELETE
uxp.proxy.database-properties=/etc/uxp/db.properties
uxp.proxy-monitoring-agent.port=2080
FILE_LOG_CHARSET=UTF-8
java.awt.headless=true
uxp.proxy.serverconf-reload-interval-seconds=60
aws.java.v1.disableDeprecationAnnouncement=true
uxp.common.conf-path=/etc/uxp/
uxp.proxy.signature-timestamp-required=true
uxp.common.rsa-allowed=false
uxp.proxy.additional-forbidden-rest-http-headers=
uxp.anti-dos.max-heap-usage=1.1
uxp.message-log.metadata-cleaner-batch-size=1000
path.separator=\:
os.version=5.15.0-125-generic
uxp.common.tls-conf-path=/etc/uxp/ssl/
uxp.message-log-s3.request-timeout=0
uxp.proxy.timestamper-httpclient-read-timeout=300000
uxp.message-log.archive-storage-type=s3
java.vm.name=OpenJDK 64-Bit Server VM
uxp.proxy.timestamper-httpclient-idle-connection-eviction-period=1
uxp.proxy.ocsp-cache-path=/var/cache/uxp
uxp.monitoring-server.opdata-stats-period-seconds=300
uxp.monitoring-service.httpclient-read-timeout=60000
uxp.proxy-status-check.clientproxy-listening-switch-enabled=true
uxp.proxy.verify-signing-certificate-qualified=true
os.arch=amd64
uxp.proxy.ocsp-usage-safety-offset=2
uxp.proxy.ocsp-responder-client-connect-timeout=20000
uxp.anti-dos.enabled=true
java.vm.info=mixed mode, sharing
uxp.anti-dos.min-free-file-handles=100
java.class.version=61.0
uxp.proxy.max-retained-soap-attachment-size-bytes=5242880
uxp.proxy.digest-algorithm-id=SHA-512
uxp.op-monitor.keep-records-for-days=7
uxp.proxy-monitoring-agent.ignored-network-interfaces=lo
uxp.proxy.max-retained-soap-message-size-bytes=5242880
uxp.proxy.client-httpclient-target-selection-strategy=round-robin
sun.jnu.encoding=UTF-8
uxp.identity-provider.database-properties=/etc/uxp/db.properties
uxp.message-log.metadata-persistence-unit=messagelog-metadata
uxp.proxy.csr-signature-digest-algorithm-id=SHA-256
uxp.message-log.archiver-admin-port=5765
uxp.op-monitor-buffer.size=20000
uxp.message-log.timestamp-certificate-chain-request=true
uxp.proxy.internal-cipher-suites=TLS_AES_256_GCM_SHA384,TLS_ECDHE_ECDSA_WITH_AES_256_GCM_SHA384
uxp.anti-dos.max-parallel-connections=5000
uxp.center.allowed-certificate-profiles=ee.cyber.uxp.common.certificateprofile.ua.UaCertificateProfileInfoProvider
uxp.proxy-status-check.interval-seconds=15
uxp.identity-provider.public-client-id=uxp-ss-ui
uxp.proxy.client-httpclient-socket-buffer-size=16384
file.separator=/
java.vm.compressedOopsMode=32-bit
uxp.message-log.acceptable-timestamp-failure-period=14400
line.separator=\n
uxp.message-log.archive-interval=0 0 0/1 1/1 * ? *
uxp.message-log-s3.address=https\://192.168.99.136\:9000
uxp.proxy.timestamp-verify-signer-chain=true
uxp.proxy-monitoring-agent.monitor-agent-conf-file=/etc/uxp/monitor-agent.ini
uxp.op-monitor.max-records-in-payload=10000
uxp.identity-provider.oauth2-introspect-uri=http\://localhost\:8087/auth-api/v1/oauth2/introspect
java.protocol.handler.pkgs=org.springframework.boot.loader
user.name=uxp
uxp.proxy-monitoring-agent.sending-interval-seconds=180
uxp.identity-provider.login-lockout-time-duration=15
uxp.message-log.timestamp-immediately=true
uxp.proxy.server-httpclient-socket-buffer-size=16384
uxp.op-monitor-buffer.httpclient-connect-timeout=30000
uxp.message-log-s3.region=us-west-1
uxp.proxy.server-httpclient-connect-timeout=30000
uxp.proxy.log-metaservice-signatures=false
uxp.proxy.transport-cipher-suites=TLS_AES_256_GCM_SHA384
PID=596076
org.bytedeco.javacpp.maxBytes=0
loader.path=/usr/share/uxp/jlib/signature-xades.jar,,/usr/share/uxp/jlib/addon/certprofile-trembita.jar,/usr/share/uxp/jlib/addon/proxy/cipher-jce-provider-1.22.7.jar,/usr/share/uxp/jlib/addon/proxy/ciplus-jce/*
uxp.proxy.server-listen-port=5500
uxp.message-log.metadata-record-extract-path=/var/lib/uxp/messagelog/old-metadata
uxp.proxy.cert-reg-signature-digest-algorithm-id=SHA-512
uxp.common.expiration-warning-threshold-days=32
uxp.proxy.server-jetty-thread-pool-max-size=60
uxp.message-log.timestamper-client-read-timeout=5000
uxp.proxy.log-monitoring-signatures=false
uxp.message-log.storage-path=/var/lib/uxp/messagelog
uxp.message-log.hash-algo-id=SHA-512
uxp.proxy.server-httpclient-idle-connection-eviction-period=1
java.class.path=/usr/share/uxp/jlib/verifier-rest-api.jar
uxp.common.conf-backup-digest-algorithm-id=SHA-512
uxp.message-log-s3.storage-class=
uxp.message-log.archive-path=/var/lib/uxp/messagelog/archive
java.vm.vendor=Ubuntu
uxp.proxy-monitoring-agent.net-stats-file=/proc/net/dev
uxp.proxy.client-connector-socket-buffer-size=16384
uxp.monitoring-service.httpclient-connect-timeout=30000
uxp.proxy-monitoring-agent.zabbix-configurator-client-read-timeout-seconds=300
sun.java.launcher=SUN_STANDARD
user.country=UA
uxp.proxy.server-connection-accept-rate-limit=0
uxp.identity-provider.login-max-failed-attempts=5
uxp.proxy-status-check.serverproxy-listening-switch-enabled=true
sun.cpu.endian=little
user.language=uk
uxp.status-service.allowed-hosts=127.0.0.1
uxp.identity-provider.public-client-redirect-uris=
uxp.message-log-s3.access-key=pC9hJZZJZdSdWAOqIgIT
uxp.proxy.client-http-port=80
uxp.message-log.metadata-record-lifetime-minutes=1576800
uxp.status-service.listen-address=127.0.0.1
uxp.proxy-monitoring-agent.zabbix-configurator-client-connect-timeout-seconds=30
uxp.proxy.max-retained-rest-payload-size-bytes=5242880
uxp.op-monitor.max-stats-records-in-payload=10000
uxp.proxy.client-httpclient-idle-connection-eviction-period=1
uxp.message-log.timestamper-client-connect-timeout=10000
uxp.monitoring-server.opdata-stats-polling-interval-seconds=900
uxp.proxy.wsdl-download-read-timeout=5000
uxp.proxy.server-httpclient-read-timeout=10000
uxp.common.global-conf-path=/etc/uxp/globalconf/
java.runtime.name=OpenJDK Runtime Environment
uxp.common.pkcs12-provider-name=CiPlusJCE
uxp.op-monitor.records-available-timestamp-offset-seconds=60
uxp.proxy.timestamper-httpclient-connect-timeout=30000
java.vendor.url.bug=https\://bugs.launchpad.net/ubuntu/+source/openjdk-17
jetty.git.hash=e77516598a07cca826d27fa8a4f7c70e953921a6
uxp.proxy.client-https-port=443
user.dir=/
uxp.message-log.temp-files-path=/var/lib/uxp/messagelog/tmp
uxp.identity-provider.hostname=
uxp.proxy.openapi-download-connect-timeout=10000
java.vm.version=17.0.15+6-Ubuntu-0ubuntu122.04
uxp.message-log.metadata-cleanup-interval=0 0 0 * * ? *

jcmd 596076 VM.flags
596076:
-XX:CICompilerCount=3 -XX:CompressedClassSpaceSize=117440512 -XX:ConcGCThreads=1 -XX:G1ConcRefinementThreads=4 -XX:G1EagerReclaimRemSetThreshold=8 -XX:G1HeapRegionSize=1048576 -XX:GCDrainStackTargetSize=64 -XX:InitialHeapSize=65011712 -XX:MarkStackSize=4194304 -XX:MaxHeapSize=134217728 -XX:MaxMetaspaceSize=134217728 -XX:MaxNewSize=79691776 -XX:MinHeapDeltaBytes=1048576 -XX:MinHeapSize=8388608 -XX:NonNMethodCodeHeapSize=5832780 -XX:NonProfiledCodeHeapSize=122912730 -XX:ProfiledCodeHeapSize=122912730 -XX:+RequireSharedSpaces -XX:ReservedCodeCacheSize=251658240 -XX:+SegmentedCodeCache -XX:SoftMaxHeapSize=134217728 -XX:-THPStackMitigation -XX:+UseCompressedClassPointers -XX:+UseCompressedOops -XX:+UseFastUnorderedTimeStamps -XX:+UseG1GC -XX:+UseSharedSpaces


