# UXP Proxy

The `uxp-proxy` service is the core component of the system, responsible for secure data exchange within the Trembita network. This service handles:

- All inbound and outbound SOAP and REST calls;
- Message encryption and signing;
- Signature verification;
- Interaction with external secure exchange gateways.

## Purpose

The `uxp-proxy` container provides TLS-based communication with other Trembita instances:

- `127.0.0.1:5566` — internal **admin control port** for executing management commands (e.g., triggering configuration reload via `GET /execute`).
- `:80` — internal unsecure API endpoint;
- `:443` — internal secure API endpoint (TLS 1.2 / 1.3);
- `:5500` — external secure API endpoint (TLS 1.3) used for incoming connections from other proxies.

## Container Structure

The image is based on `eclipse-temurin:17-jdk-jammy` and includes:

- Main JARs: `proxy.jar`, `signature-xades.jar`;
- Add-ons:
  - `certprofile-trembita.jar`
  - `monitoring-1.22.7.jar`
  - `metaservice-1.22.7.jar`
  - `cipher-jce-provider-1.22.7.jar`
  - JCE drivers (Gryada-301, Cipher-HSM)
- Utilities:
  - `token-initializer` — initializes software tokens;
  - `token_login` — logs into hardware tokens;
  - `trembita-healthcheck` — verifies proxy availability.

## Features

- Supports operation in `read-only` filesystem mode.
  - Requires temporary writable mount for extracting `.so` libraries (approx. 70MB).
- Most configuration is delivered via `.ini` files.
  - Only a minimal subset is passed via environment variables.
- Supports external HSMs via PKCS#11 (e.g., Gryada-301, Cipher-HSM).
- Uses `authbind` to allow binding to ports 80 and 443 as non-root user `uxp`.

## Required Mounts

| Container Path                            | Purpose                                                     |
|-------------------------------------------|-------------------------------------------------------------|
| `/etc/uxp/db.properties`                  | Configuration file for access to DB                         |
| `/etc/uxp/license`                        | License file                                                |
| `/etc/uxp/signer/`                        | Directory for software token data                           |
| `/etc/uxp/globalconf/`                    | Global configuration synced via `uxp-configuration-client`  |
| `/var/lib/uxp/messagelog/`                | Transaction logs before being archived by `uxp-message-log` |
| `/usr/share/uxp/lib/osplm.ini` (optional) | HSM config for Gryada-301                                   |

## Optional Environment Variables

| Variable               | Description                                           |
|------------------------|-------------------------------------------------------|
| `PKCS11_PROXY_SOCKET`  | Address and port of the HSM (e.g., `tcp://ip:port`)   |

## Key Dependencies

- `authbind` — allows binding to privileged ports;
- `libgomp1` — required by some crypto drivers;
- `libcihsm.so`, `NCMGryada301PKCS11Libs-Linux` — HSM drivers included in the image.

---

## 🚀 Startup Command

In legacy setups, the Java application is launched with:

```bash
authbind /usr/lib/jvm/java-17-openjdk-amd64/bin/java \
  -Xms150m \
  -Xmx512m \
  -XX:MaxMetaspaceSize=256m \
  -XX:+UseG1GC \
  -Xshare:on \
  -Dfile.encoding=UTF-8 \
  -Djava.library.path=/usr/share/uxp/lib/ \
  -Dlogback.configurationFile=/etc/uxp/conf.d/proxy-logback.xml \
  -Duxp.proxy.clientHandlers=ee.cyber.uxp.proxy.clientproxy.MetadataHandler \
  -Duxp.proxy.serverServiceHandlers=ee.cyber.uxp.proxy.serverproxy.MetadataServiceSOAPHandlerImpl,ee.cyber.uxp.proxy.serverproxy.MonitoringServiceHandlerImpl \
  -Duxp.proxy.serverRestApiHandlers=ee.cyber.uxp.proxy.serverproxy.MetadataServiceRESTHandlerImpl \
  -Dorg.bytedeco.javacpp.noPointerGC=true \
  -Dorg.bytedeco.javacpp.maxBytes=0 \
  -Dorg.bytedeco.javacpp.maxPhysicalBytes=0 \
  -cp "/usr/share/uxp/jlib/proxy.jar:/usr/share/uxp/jlib/signature-xades.jar:/usr/share/uxp/jlib/addon/certprofile-trembita.jar:/usr/share/uxp/jlib/addon/proxy/metaservice-1.22.7.jar:/usr/share/uxp/jlib/addon/proxy/monitoring-1.22.7.jar:/usr/share/uxp/jlib/addon/proxy/cipher-jce-provider-1.22.7.jar:/usr/share/uxp/jlib/addon/proxy/ciplus-jce/*" \
  ee.cyber.uxp.proxy.ProxyMain
```

---
## ⚡ Runtime JVM Options (jcmd)

<details>
<summary>Click to expand output of <code>jcmd &lt;pid&gt; VM.command_line</code></summary>

```txt
VM Arguments:
jvm_args: -Xms150m -Xmx512m -XX:MaxMetaspaceSize=256m -Dlogback.configurationFile=/etc/uxp/conf.d/proxy-logback.xml -Duxp.proxy.clientHandlers=ee.cyber.uxp.proxy.clientproxy.MetadataHandler -Duxp.proxy.serverServiceHandlers=ee.cyber.uxp.proxy.serverproxy.MetadataServiceSOAPHandlerImpl,ee.cyber.uxp.proxy.serverproxy.MonitoringServiceHandlerImpl -Duxp.proxy.serverRestApiHandlers=ee.cyber.uxp.proxy.serverproxy.MetadataServiceRESTHandlerImpl -XX:+UseG1GC -Xshare:on -Dfile.encoding=UTF-8 -Djava.library.path=/usr/share/uxp/lib/ -Dorg.bytedeco.javacpp.noPointerGC=true -Dorg.bytedeco.javacpp.maxBytes=0 -Dorg.bytedeco.javacpp.maxPhysicalBytes=0 
java_command: ee.cyber.uxp.proxy.ProxyMain
java_class_path (initial): /usr/share/uxp/jlib/proxy.jar:/usr/share/uxp/jlib/signature-xades.jar:/usr/share/uxp/jlib/addon/certprofile-trembita.jar:/usr/share/uxp/jlib/addon/proxy/metaservice-1.22.7.jar:/usr/share/uxp/jlib/addon/proxy/monitoring-1.22.7.jar:/usr/share/uxp/jlib/addon/proxy/cipher-jce-provider-1.22.7.jar:/usr/share/uxp/jlib/addon/proxy/ciplus-jce/cipherplus-1.0.28-1.5.8-linux-x86_64.jar:/usr/share/uxp/jlib/addon/proxy/ciplus-jce/javacpp-1.5.8.jar:/usr/share/uxp/jlib/addon/proxy/ciplus-jce/pkcs11-wrapper-1.6.9-1.jar:/usr/share/uxp/jlib/addon/proxy/ciplus-jce/ciplus-jce-1.0.24.jar:/usr/share/uxp/jlib/addon/proxy/ciplus-jce/cipherplus-1.0.28-1.5.8.jar
Launcher Type: SUN_STANDARD
```
</details>

<details>
<summary>Click to expand output of <code>jcmd &lt;pid&gt; VM.system_properties</code></summary>

```txt
uxp.proxy-monitoring-agent.ignored-network-interfaces=lo
uxp.proxy.max-retained-soap-message-size-bytes=5242880
java.specification.version=17
uxp.identity-provider.security-server-client-secret=2DmVrz_VUQUhn3ePNgWm8Ur-TwMK0la_
uxp.common.temp-files-path=/var/tmp/uxp/
uxp.proxy.client-httpclient-target-selection-strategy=round-robin
sun.jnu.encoding=UTF-8
uxp.proxy.ocsp-responder-client-read-timeout=30000
uxp.proxy.openapi-download-read-timeout=5000
uxp.proxy.csr-signature-digest-algorithm-id=SHA-256
sun.arch.data.model=64
jdk.tls.stapling.cacheLifetime=300
uxp.proxy.server-port=5500
org.bytedeco.javacpp.noPointerGC=true
java.vendor.url=https\://ubuntu.com/
uxp.op-monitor-buffer.size=20000
org.terracotta.quartz.skipUpdateCheck=true
uxp.pkcs11.signing-session-pool-wait-time-seconds=10
uxp.op-monitor-buffer.httpclient-read-timeout=60000
uxp.proxy.software-token-batch-signatures=false
uxp.proxy.internal-cipher-suites=TLS_AES_256_GCM_SHA384,TLS_ECDHE_ECDSA_WITH_AES_256_GCM_SHA384
sun.boot.library.path=/usr/lib/jvm/java-17-openjdk-amd64/lib
sun.java.command=ee.cyber.uxp.proxy.ProxyMain
uxp.proxy.log-signatures=true
jdk.debug=release
uxp.anti-dos.max-parallel-connections=5000
uxp.status-service.listen-port=2082
uxp.center.allowed-certificate-profiles=ee.cyber.uxp.common.certificateprofile.ua.UaCertificateProfileInfoProvider
uxp.proxy-status-check.interval-seconds=15
java.specification.vendor=Oracle Corporation
uxp.common.license-file=/etc/uxp/license.lic
java.version.date=2025-04-15
java.home=/usr/lib/jvm/java-17-openjdk-amd64
jdk.tls.server.enableStatusRequestExtension=true
uxp.proxy.client-httpclient-socket-buffer-size=16384
file.separator=/
uxp.proxy.server-connection-accept-rate-limit-period=1
java.vm.compressedOopsMode=32-bit
line.separator=\n
uxp.message-log.timestamp-provider-round-robin=true
uxp.message-log.archive-interval=0 0 0/1 1/1 * ? *
java.vm.specification.vendor=Oracle Corporation
java.specification.name=Java Platform API Specification
uxp.proxy.server-stapling-ocsp-cache-lifetime=300
uxp.message-log-s3.address=https\://192.168.99.136\:9000
uxp.proxy.wsdl-download-connect-timeout=10000
uxp.proxy.timestamp-verify-signer-chain=true
uxp.anti-dos.max-cpu-load=1.1
uxp.proxy.client-jetty-thread-pool-max-size=60
uxp.proxy-monitoring-agent.monitor-agent-conf-file=/etc/uxp/monitor-agent.ini
uxp.op-monitor.max-records-in-payload=10000
sun.management.compiler=HotSpot 64-Bit Tiered Compilers
uxp.op-monitor.clean-interval=0 0 0/12 1/1 * ? *
java.runtime.version=17.0.15+6-Ubuntu-0ubuntu122.04
uxp.common.template-path=/usr/share/uxp/templates/
user.name=uxp
uxp.proxy.server-listen-address=0.0.0.0
uxp.proxy-monitoring-agent.sending-interval-seconds=180
uxp.common.conf-backup-digest-files-checked=true
uxp.message-log.timestamp-immediately=true
uxp.monitoring-server.opdata-stats-collection-enabled=true
uxp.proxy.server-httpclient-socket-buffer-size=16384
uxp.op-monitor-buffer.httpclient-connect-timeout=30000
uxp.proxy.serverServiceHandlers=ee.cyber.uxp.proxy.serverproxy.MetadataServiceSOAPHandlerImpl,ee.cyber.uxp.proxy.serverproxy.MonitoringServiceHandlerImpl
file.encoding=UTF-8
org.bytedeco.javacpp.maxPhysicalBytes=0
uxp.message-log-s3.trusted-certificate=/etc/uxp/ssl/public.crt
uxp.common.configuration-anchor-file=/etc/uxp/configuration-anchor.xml
uxp.proxy.connector-host=0.0.0.0
uxp.proxy.client-httpclient-read-timeout=300000
uxp.op-monitor.health-statistics-period-seconds=600
uxp.proxy-monitoring-agent.admin-port=5588
java.io.tmpdir=/tmp
java.version=17.0.15
uxp.common.device-templates-path=/etc/uxp/device-templates/
uxp.proxy.server-connector-socket-buffer-size=16384
uxp.proxy.server-httpclient-connect-timeout=30000
uxp.proxy.log-metaservice-signatures=false
uxp.proxy.transport-cipher-suites=TLS_AES_256_GCM_SHA384
uxp.message-log-s3.bucket-name=uxp-messagelog1227
java.vm.specification.name=Java Virtual Machine Specification
uxp.proxy.software-token-key-dir=/etc/uxp/signer/
org.bytedeco.javacpp.maxBytes=0
native.encoding=UTF-8
uxp.proxy.server-listen-port=5500
java.library.path=/usr/share/uxp/lib/
uxp.proxy.cert-reg-signature-digest-algorithm-id=SHA-512
java.vendor=Ubuntu
java.specification.maintenance.version=1
uxp.common.expiration-warning-threshold-days=32
uxp.proxy.server-jetty-thread-pool-max-size=60
jdk.tls.stapling.responderOverride=true
sun.io.unicode.encoding=UnicodeLittle
uxp.proxy.clientHandlers=ee.cyber.uxp.proxy.clientproxy.MetadataHandler
uxp.proxy.log-monitoring-signatures=false
uxp.proxy-monitoring-agent.params-collecting-interval-seconds=15
jdk.tls.stapling.responderURI=http\://127.0.0.1\:5577/ocsp
uxp.proxy.client-httpclient-connect-timeout=30000
uxp.proxy.server-httpclient-idle-connection-eviction-period=1
uxp.proxy.round-robin-quarantine-time=300000
java.class.path=/usr/share/uxp/jlib/proxy.jar\:/usr/share/uxp/jlib/signature-xades.jar\:/usr/share/uxp/jlib/addon/certprofile-trembita.jar\:/usr/share/uxp/jlib/addon/proxy/metaservice-1.22.7.jar\:/usr/share/uxp/jlib/addon/proxy/monitoring-1.22.7.jar\:/usr/share/uxp/jlib/addon/proxy/cipher-jce-provider-1.22.7.jar\:/usr/share/uxp/jlib/addon/proxy/ciplus-jce/cipherplus-1.0.28-1.5.8-linux-x86_64.jar\:/usr/share/uxp/jlib/addon/proxy/ciplus-jce/javacpp-1.5.8.jar\:/usr/share/uxp/jlib/addon/proxy/ciplus-jce/pkcs11-wrapper-1.6.9-1.jar\:/usr/share/uxp/jlib/addon/proxy/ciplus-jce/ciplus-jce-1.0.24.jar\:/usr/share/uxp/jlib/addon/proxy/ciplus-jce/cipherplus-1.0.28-1.5.8.jar
uxp.proxy.batch-signatures-enabled=false
uxp.op-monitor-buffer.sending-interval-seconds=5
uxp.common.conf-backup-digest-algorithm-id=SHA-512
java.vm.vendor=Ubuntu
uxp.op-monitor-buffer.max-records-in-message=500
uxp.common.digest-chunk-size=262144
user.timezone=Europe/Kyiv
uxp.proxy-monitoring-agent.net-stats-file=/proc/net/dev
java.vm.specification.version=17
os.name=Linux
uxp.proxy.client-connector-socket-buffer-size=16384
uxp.monitoring-service.httpclient-connect-timeout=30000
uxp.proxy-monitoring-agent.zabbix-configurator-client-read-timeout-seconds=300
sun.java.launcher=SUN_STANDARD
user.country=UA
uxp.proxy.server-connection-accept-rate-limit=0
uxp.proxy-status-check.serverproxy-listening-switch-enabled=true
sun.cpu.endian=little
user.home=/var/lib/uxp
user.language=uk
uxp.status-service.allowed-hosts=127.0.0.1
uxp.message-log-s3.access-key=pC9hJZZJZdSdWAOqIgIT
uxp.message-log-s3.secret-key=3daHty2NiBbzwJvZWCZkgTt7SUrR7pfqHN7DNFFZ
uxp.proxy.client-http-port=80
uxp.identity-provider.security-server-client-id=pvoqbggvvzpon1r4v55b7z8cu0de18cj
uxp.proxy.database-properties=/etc/uxp/db.properties
uxp.status-service.listen-address=127.0.0.1
uxp.proxy-monitoring-agent.port=2080
uxp.proxy-monitoring-agent.zabbix-configurator-client-connect-timeout-seconds=30
uxp.proxy.max-retained-rest-payload-size-bytes=5242880
uxp.proxy.serverconf-reload-interval-seconds=60
logback.configurationFile=/etc/uxp/conf.d/proxy-logback.xml
uxp.common.conf-path=/etc/uxp/
uxp.proxy.signature-timestamp-required=true
uxp.op-monitor.max-stats-records-in-payload=10000
uxp.proxy.client-httpclient-idle-connection-eviction-period=1
uxp.monitoring-server.opdata-stats-polling-interval-seconds=900
uxp.common.rsa-allowed=false
uxp.proxy.additional-forbidden-rest-http-headers=
jdk.tls.client.enableStatusRequestExtension=true
uxp.anti-dos.max-heap-usage=1.1
uxp.proxy.server-httpclient-read-timeout=10000
uxp.proxy.wsdl-download-read-timeout=5000
path.separator=\:
os.version=5.15.0-125-generic
uxp.common.global-conf-path=/etc/uxp/globalconf/
uxp.common.tls-conf-path=/etc/uxp/ssl/
java.runtime.name=OpenJDK Runtime Environment
uxp.proxy.timestamper-httpclient-read-timeout=300000
uxp.message-log.archive-storage-type=s3
uxp.common.pkcs12-provider-name=CiPlusJCE
uxp.op-monitor.records-available-timestamp-offset-seconds=60
java.vm.name=OpenJDK 64-Bit Server VM
uxp.proxy.ocsp-cache-path=/var/cache/uxp
uxp.proxy.timestamper-httpclient-idle-connection-eviction-period=1
uxp.proxy.timestamper-httpclient-connect-timeout=30000
uxp.monitoring-server.opdata-stats-period-seconds=300
uxp.monitoring-service.httpclient-read-timeout=60000
uxp.proxy-status-check.clientproxy-listening-switch-enabled=true
java.vendor.url.bug=https\://bugs.launchpad.net/ubuntu/+source/openjdk-17
jetty.git.hash=e77516598a07cca826d27fa8a4f7c70e953921a6
uxp.proxy.verify-signing-certificate-qualified=true
uxp.proxy.client-https-port=443
user.dir=/
os.arch=amd64
uxp.proxy.serverRestApiHandlers=ee.cyber.uxp.proxy.serverproxy.MetadataServiceRESTHandlerImpl
uxp.proxy.ocsp-usage-safety-offset=2
uxp.proxy.ocsp-responder-client-connect-timeout=20000
uxp.proxy.openapi-download-connect-timeout=10000
uxp.anti-dos.enabled=true
java.vm.info=mixed mode, sharing
java.vm.version=17.0.15+6-Ubuntu-0ubuntu122.04
uxp.anti-dos.min-free-file-handles=100
java.class.version=61.0
uxp.proxy.max-retained-soap-attachment-size-bytes=5242880
uxp.proxy.digest-algorithm-id=SHA-512
uxp.op-monitor.keep-records-for-days=7
```
</details>

<details>
<summary>Click to expand output of <code>jcmd &lt;pid&gt; VM.flags</code></summary>

```txt
-XX:CICompilerCount=3 
-XX:CompressedClassSpaceSize=218103808 
-XX:ConcGCThreads=1 
-XX:G1ConcRefinementThreads=4 
-XX:G1EagerReclaimRemSetThreshold=8 
-XX:G1HeapRegionSize=1048576 
-XX:GCDrainStackTargetSize=64 
-XX:InitialHeapSize=157286400 
-XX:MarkStackSize=4194304 
-XX:MaxHeapSize=536870912 
-XX:MaxMetaspaceSize=268435456 
-XX:MaxNewSize=321912832 
-XX:MinHeapDeltaBytes=1048576 
-XX:MinHeapSize=157286400 
-XX:NonNMethodCodeHeapSize=5832780 
-XX:NonProfiledCodeHeapSize=122912730 
-XX:ProfiledCodeHeapSize=122912730 
-XX:+RequireSharedSpaces 
-XX:ReservedCodeCacheSize=251658240 
-XX:+SegmentedCodeCache 
-XX:SoftMaxHeapSize=536870912 
-XX:-THPStackMitigation 
-XX:+UseCompressedClassPointers 
-XX:+UseCompressedOops 
-XX:+UseFastUnorderedTimeStamps 
-XX:+UseG1GC 
-XX:+UseSharedSpaces
```
</details>

---

## Startup Script Output
Below is an example of the full startup script output during container initialization, showing how the environment is assembled and parameters are passed:

```bash
+ . /etc/uxp/services/proxy.conf
++ . /etc/uxp/services/global.conf
+++ JAVA_HOME=/usr/lib/jvm/java-17-openjdk-amd64
+++ PATH=/usr/lib/jvm/java-17-openjdk-amd64/bin:/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin:/snap/bin
+++ ADDON_PATH=/usr/share/uxp/jlib/addon
+++ umask 0027
+++ UXP_PARAMS=' -XX:+UseG1GC -Xshare:on -Dfile.encoding=UTF-8 -Djava.library.path=/usr/share/uxp/lib/ '
++ PROXY_PARAMS=
++ CLIENT_HANDLERS=
++ SERVICE_HANDLERS=
++ REST_API_HANDLERS=
++ for addon in ${ADDON_PATH}/proxy/*.conf
++ '[' -e /usr/share/uxp/jlib/addon/proxy/certprofile-trembita.conf ']'
++ . /usr/share/uxp/jlib/addon/proxy/certprofile-trembita.conf
+++ ADDON_CP=:/usr/share/uxp/jlib/addon/certprofile-trembita.jar
++ for addon in ${ADDON_PATH}/proxy/*.conf
++ '[' -e /usr/share/uxp/jlib/addon/proxy/metaservices.conf ']'
++ . /usr/share/uxp/jlib/addon/proxy/metaservices.conf
+++ ADDON_CP=:/usr/share/uxp/jlib/addon/certprofile-trembita.jar:/usr/share/uxp/jlib/addon/proxy/metaservice-1.22.7.jar
+++ SERVICE_HANDLERS=,ee.cyber.uxp.proxy.serverproxy.MetadataServiceSOAPHandlerImpl
+++ REST_API_HANDLERS=,ee.cyber.uxp.proxy.serverproxy.MetadataServiceRESTHandlerImpl
+++ CLIENT_HANDLERS=,ee.cyber.uxp.proxy.clientproxy.MetadataHandler
++ for addon in ${ADDON_PATH}/proxy/*.conf
++ '[' -e /usr/share/uxp/jlib/addon/proxy/monitor.conf ']'
++ . /usr/share/uxp/jlib/addon/proxy/monitor.conf
+++ ADDON_CP=:/usr/share/uxp/jlib/addon/certprofile-trembita.jar:/usr/share/uxp/jlib/addon/proxy/metaservice-1.22.7.jar:/usr/share/uxp/jlib/addon/proxy/monitoring-1.22.7.jar
+++ SERVICE_HANDLERS=,ee.cyber.uxp.proxy.serverproxy.MetadataServiceSOAPHandlerImpl,ee.cyber.uxp.proxy.serverproxy.MonitoringServiceHandlerImpl
++ for addon in ${ADDON_PATH}/proxy/*.conf
++ '[' -e /usr/share/uxp/jlib/addon/proxy/trembita-crypto.conf ']'
++ . /usr/share/uxp/jlib/addon/proxy/trembita-crypto.conf
+++ ADDON_CP=':/usr/share/uxp/jlib/addon/certprofile-trembita.jar:/usr/share/uxp/jlib/addon/proxy/metaservice-1.22.7.jar:/usr/share/uxp/jlib/addon/proxy/monitoring-1.22.7.jar:/usr/share/uxp/jlib/addon/proxy/cipher-jce-provider-1.22.7.jar:/usr/share/uxp/jlib/addon/proxy/ciplus-jce/*'
+++ ADDON_PARAMS=' -Dorg.bytedeco.javacpp.noPointerGC=true -Dorg.bytedeco.javacpp.maxBytes=0 -Dorg.bytedeco.javacpp.maxPhysicalBytes=0 '
+++ export 'LD_PRELOAD= /usr/lib/x86_64-linux-gnu/libtcmalloc.so.4 '
+++ LD_PRELOAD=' /usr/lib/x86_64-linux-gnu/libtcmalloc.so.4 '
+++ export LD_LIBRARY_PATH=:/usr/share/uxp/lib
+++ LD_LIBRARY_PATH=:/usr/share/uxp/lib
++ CP=/usr/share/uxp/jlib/proxy.jar:/usr/share/uxp/jlib/signature-xades.jar
++ PROXY_PARAMS=' -Dlogback.configurationFile=/etc/uxp/conf.d/proxy-logback.xml -Duxp.proxy.clientHandlers=ee.cyber.uxp.proxy.clientproxy.MetadataHandler -Duxp.proxy.serverServiceHandlers=ee.cyber.uxp.proxy.serverproxy.MetadataServiceSOAPHandlerImpl,ee.cyber.uxp.proxy.serverproxy.MonitoringServiceHandlerImpl -Duxp.proxy.serverRestApiHandlers=ee.cyber.uxp.proxy.serverproxy.MetadataServiceRESTHandlerImpl'
++ PROXY_JVM_OPTS='-Xms150m -Xmx512m -XX:MaxMetaspaceSize=256m'
++ . /etc/uxp/services/local.conf
+ date -R
Mon, 02 Jun 2025 21:18:36 +0300
+ exec authbind /usr/lib/jvm/java-17-openjdk-amd64/bin/java -Xms150m -Xmx512m -XX:MaxMetaspaceSize=256m -Dlogback.configurationFile=/etc/uxp/conf.d/proxy-logback.xml -Duxp.proxy.clientHandlers=ee.cyber.uxp.proxy.clientproxy.MetadataHandler -Duxp.proxy.serverServiceHandlers=ee.cyber.uxp.proxy.serverproxy.MetadataServiceSOAPHandlerImpl,ee.cyber.uxp.proxy.serverproxy.MonitoringServiceHandlerImpl -Duxp.proxy.serverRestApiHandlers=ee.cyber.uxp.proxy.serverproxy.MetadataServiceRESTHandlerImpl -XX:+UseG1GC -Xshare:on -Dfile.encoding=UTF-8 -Djava.library.path=/usr/share/uxp/lib/ -cp '/usr/share/uxp/jlib/proxy.jar:/usr/share/uxp/jlib/signature-xades.jar:/usr/share/uxp/jlib/addon/certprofile-trembita.jar:/usr/share/uxp/jlib/addon/proxy/metaservice-1.22.7.jar:/usr/share/uxp/jlib/addon/proxy/monitoring-1.22.7.jar:/usr/share/uxp/jlib/addon/proxy/cipher-jce-provider-1.22.7.jar:/usr/share/uxp/jlib/addon/proxy/ciplus-jce/*' -Dorg.bytedeco.javacpp.noPointerGC=true -Dorg.bytedeco.javacpp.maxBytes=0 -Dorg.bytedeco.javacpp.maxPhysicalBytes=0 ee.cyber.uxp.proxy.ProxyMain
```

---

## Maintainers
Container built and maintained by Kirill Shypachov on behalf of eGA Kyiv.
Uses official UXP modules version 1.22.7.