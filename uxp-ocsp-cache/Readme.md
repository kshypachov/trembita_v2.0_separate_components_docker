# uxp-ocsp-cache Service

## Overview

`uxp-ocsp-cache` is a lightweight, Java-based service that retrieves and caches OCSP responses for certificates used by key UXP components, including `uxp-proxy`, `uxp-securityserver-rest-api`, and `uxp-verifier`. It is packaged as part of `uxp-proxy`.

The service is typically deployed as a standalone container and relies on pre-built `.deb` packages, which are unpacked and embedded into the image during the build phase.

## Ports

- **5577** — OCSP Responder HTTP interface
- **5766** — Admin API
  - `GET /execute` — manually triggers OCSP response fetching, bypassing the default scheduler

## Docker Image Details

This service is built using a **multi-stage Dockerfile**:

### Stage 1: Builder (`ubuntu:24.04`)
- Creates `uxp` user and group
- Unpacks `.deb` packages:
  - `uxp-proxy`
  - `uxp-addon-trembita-crypto`
- Extracts necessary `.jar` files and configuration

### Stage 2: Runtime (`eclipse-temurin:17-jdk-jammy`)
- Installs native dependencies:
  - `libtcmalloc`, `libunwind`, `libgomp`, `libgoogle-perftools`
- Copies Java libraries to `/app/jlib/`
- Loads log configuration (`ocsp-cache-logback.xml`)
- Disables shell access (`/bin/bash`, `/bin/sh` removed)
- Drops privileges: runs as `uxp` user (UID 102)

### Key Environment Settings
- `LD_PRELOAD=/usr/lib/x86_64-linux-gnu/libtcmalloc.so.4`
- Container entrypoint (commented in Dockerfile) launches the Java process with memory and logging options, and sets OCSP-specific JVM properties.

## Required Volume Mounts

- `/etc/uxp/` – For configuration files, anchor, and global setup
- `/usr/share/uxp/` – For Java native libraries and extended configuration

## Configuration Files

| Path                                                   | Purpose                                          |
|--------------------------------------------------------|--------------------------------------------------|
| `/etc/uxp/services/ocsp-cache.conf`                    | Main startup config                              |
| `/etc/uxp/services/global.conf`                        | Global environment variables                     |
| `/etc/uxp/services/local.conf`                         | Local environment overrides                      |
| `/etc/uxp/conf.d/ocsp-cache-logback.xml`               | Logback logging configuration                    |
| `/etc/uxp/conf.d/ocsp-cache.ini`                       | Application-level config                         |
| `/usr/share/uxp/jlib/addon/ocsp-cache/*.conf`          | Addon configuration, e.g., crypto providers      |
| `/usr/share/uxp/bin/ocsp-cache.sh`                     | Entrypoint script                                |
| `/var/cache/uxp`                                       | Directory for cached OCSP responses              |


## Java Runtime Parameters

Some notable runtime JVM/system properties:

- `uxp.ocsp-cache.ocsp-responder-listen-port=5577`
- `uxp.ocsp-cache.admin-port=5766`
- `uxp.ocsp-cache.update-interval=0 0 0/6 1/1 * ? *`
- `uxp.ocsp-cache.client-connect-timeout=10000`
- `uxp.ocsp-cache.client-read-timeout=5000`
- Logging config: `/etc/uxp/conf.d/ocsp-cache-logback.xml`
- Java version: OpenJDK 17

## Java Runtime

The service is executed with:

```bash
java \
  -Xmx128m \
  -XX:MaxMetaspaceSize=100m \
  -XX:+UseG1GC \
  -Xshare:on \
  -Dfile.encoding=UTF-8 \
  -Dlogback.configurationFile=/app/ocsp-cache-logback.xml \
  -Djava.library.path=/usr/share/uxp/lib/ \
  -cp "<resolved classpath>" \
  -Duxp.ocsp-cache.admin-port=5766 \
  -Duxp.ocsp-cache.ocsp-responder-listen-port=5577 \
  -Duxp.ocsp-cache.update-interval=0 0 0/6 1/1 * ? * \
  -Duxp.ocsp-cache.full-update-freshness-divisor=10 \
  -Duxp.ocsp-cache.client-connect-timeout=10000 \
  -Dorg.bytedeco.javacpp.noPointerGC=true \
  -Dorg.bytedeco.javacpp.maxBytes=0 \
  -Dorg.bytedeco.javacpp.maxPhysicalBytes=0 \
  ee.cyber.uxp.ocsp.OcspCacheMain
```

---

## ⚙️ Startup Script Output (set -x)

<pre>
/usr/share/uxp/bin/ocsp-cache.sh
+ . /etc/uxp/services/ocsp-cache.conf
++ . /etc/uxp/services/global.conf
++ '[' -z /usr/share/uxp/jlib/addon ']'
++ for addon in ${ADDON_PATH}/ocsp-cache/*.conf
++ '[' -e /usr/share/uxp/jlib/addon/ocsp-cache/trembita-crypto.conf ']'
++ . /usr/share/uxp/jlib/addon/ocsp-cache/trembita-crypto.conf
+++ ADDON_CP=':/usr/share/uxp/jlib/addon/proxy/cipher-jce-provider-1.22.7.jar:/usr/share/uxp/jlib/addon/proxy/ciplus-jce/*'
+++ ADDON_PARAMS=' -Dorg.bytedeco.javacpp.noPointerGC=true -Dorg.bytedeco.javacpp.maxBytes=0 -Dorg.bytedeco.javacpp.maxPhysicalBytes=0 '
+++ export 'LD_PRELOAD= /usr/lib/x86_64-linux-gnu/libtcmalloc.so.4 '
+++ LD_PRELOAD=' /usr/lib/x86_64-linux-gnu/libtcmalloc.so.4 '
+++ export LD_LIBRARY_PATH=:/usr/share/uxp/lib
+++ LD_LIBRARY_PATH=:/usr/share/uxp/lib
++ CP=/usr/share/uxp/jlib/ocsp-cache.jar:/usr/share/uxp/jlib/signature-xades.jar
++ OCSP_CACHE_PARAMS=' -Dlogback.configurationFile=/etc/uxp/conf.d/ocsp-cache-logback.xml'
++ OCSP_CACHE_JVM_OPTS=' -Xmx128m -XX:MaxMetaspaceSize=100m '
++ . /etc/uxp/services/local.conf
++ OCSP_CACHE_PARAMS=' -Dlogback.configurationFile=/etc/uxp/conf.d/ocsp-cache-logback.xml'
+ date -R
Sun, 01 Jun 2025 09:26:00 +0300
+ exec /usr/lib/jvm/java-17-openjdk-amd64/bin/java -Xmx128m -XX:MaxMetaspaceSize=100m -Dlogback.configurationFile=/etc/uxp/conf.d/ocsp-cache-logback.xml -XX:+UseG1GC -Xshare:on -Dfile.encoding=UTF-8 -Djava.library.path=/usr/share/uxp/lib/ -cp '/usr/share/uxp/jlib/ocsp-cache.jar:/usr/share/uxp/jlib/signature-xades.jar:/usr/share/uxp/jlib/addon/proxy/cipher-jce-provider-1.22.7.jar:/usr/share/uxp/jlib/addon/proxy/ciplus-jce/*' -Dorg.bytedeco.javacpp.noPointerGC=true -Dorg.bytedeco.javacpp.maxBytes=0 -Dorg.bytedeco.javacpp.maxPhysicalBytes=0 ee.cyber.uxp.ocsp.OcspCacheMain
</pre>

---

## ⚡️ Runtime JVM Options (jcmd)

<details>
<summary>Click to expand `jcmd &lt;pid&gt; VM.command_line` output</summary>

```text
VM Arguments:
jvm_args: -Xmx128m -XX:MaxMetaspaceSize=100m -Dlogback.configurationFile=/etc/uxp/conf.d/ocsp-cache-logback.xml -XX:+UseG1GC -Xshare:on -Dfile.encoding=UTF-8 -Djava.library.path=/usr/share/uxp/lib/ -Dorg.bytedeco.javacpp.noPointerGC=true -Dorg.bytedeco.javacpp.maxBytes=0 -Dorg.bytedeco.javacpp.maxPhysicalBytes=0 
java_command: ee.cyber.uxp.ocsp.OcspCacheMain
java_class_path (initial): /usr/share/uxp/jlib/ocsp-cache.jar:/usr/share/uxp/jlib/signature-xades.jar:/usr/share/uxp/jlib/addon/proxy/cipher-jce-provider-1.22.7.jar:/usr/share/uxp/jlib/addon/proxy/ciplus-jce/cipherplus-1.0.28-1.5.8-linux-x86_64.jar:/usr/share/uxp/jlib/addon/proxy/ciplus-jce/javacpp-1.5.8.jar:/usr/share/uxp/jlib/addon/proxy/ciplus-jce/pkcs11-wrapper-1.6.9-1.jar:/usr/share/uxp/jlib/addon/proxy/ciplus-jce/ciplus-jce-1.0.24.jar:/usr/share/uxp/jlib/addon/proxy/ciplus-jce/cipherplus-1.0.28-1.5.8.jar
Launcher Type: SUN_STANDARD
```
</details>

<details>
<summary>Click to expand `jcmd &lt;pid&gt; VM.flags` output</summary>

```text
-XX:CICompilerCount=3 -XX:CompressedClassSpaceSize=83886080 -XX:ConcGCThreads=1 -XX:G1ConcRefinementThreads=4 -XX:G1EagerReclaimRemSetThreshold=8 -XX:G1HeapRegionSize=1048576 -XX:GCDrainStackTargetSize=64 -XX:InitialHeapSize=65011712 -XX:MarkStackSize=4194304 -XX:MaxHeapSize=134217728 -XX:MaxMetaspaceSize=104857600 -XX:MaxNewSize=79691776 -XX:MinHeapDeltaBytes=1048576 -XX:MinHeapSize=8388608 -XX:NonNMethodCodeHeapSize=5832780 -XX:NonProfiledCodeHeapSize=122912730 -XX:ProfiledCodeHeapSize=122912730 -XX:+RequireSharedSpaces -XX:ReservedCodeCacheSize=251658240 -XX:+SegmentedCodeCache -XX:SoftMaxHeapSize=134217728 -XX:-THPStackMitigation -XX:+UseCompressedClassPointers -XX:+UseCompressedOops -XX:+UseFastUnorderedTimeStamps -XX:+UseG1GC -XX:+UseSharedSpaces
```

</details>

<details>
<summary>Click to expand `jcmd &lt;pid&gt; VM.system_properties` output</summary>

```text
#Sat May 31 22:50:52 EEST 2025
uxp.ocsp-cache.ocsp-responder-listen-port=5577
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
sun.java.command=ee.cyber.uxp.ocsp.OcspCacheMain
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
uxp.ocsp-cache.client-read-timeout=5000
uxp.proxy.log-monitoring-signatures=false
uxp.proxy-monitoring-agent.params-collecting-interval-seconds=15
uxp.proxy.round-robin-quarantine-time=300000
java.class.path=/usr/share/uxp/jlib/ocsp-cache.jar\:/usr/share/uxp/jlib/signature-xades.jar\:/usr/share/uxp/jlib/addon/proxy/cipher-jce-provider-1.22.7.jar\:/usr/share/uxp/jlib/addon/proxy/ciplus-jce/cipherplus-1.0.28-1.5.8-linux-x86_64.jar\:/usr/share/uxp/jlib/addon/proxy/ciplus-jce/javacpp-1.5.8.jar\:/usr/share/uxp/jlib/addon/proxy/ciplus-jce/pkcs11-wrapper-1.6.9-1.jar\:/usr/share/uxp/jlib/addon/proxy/ciplus-jce/ciplus-jce-1.0.24.jar\:/usr/share/uxp/jlib/addon/proxy/ciplus-jce/cipherplus-1.0.28-1.5.8.jar
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
uxp.ocsp-cache.client-connect-timeout=10000
uxp.message-log-s3.access-key=pC9hJZZJZdSdWAOqIgIT
uxp.message-log-s3.secret-key=3daHty2NiBbzwJvZWCZkgTt7SUrR7pfqHN7DNFFZ
uxp.identity-provider.security-server-client-id=pvoqbggvvzpon1r4v55b7z8cu0de18cj
uxp.status-service.listen-address=127.0.0.1
uxp.proxy-monitoring-agent.port=2080
uxp.proxy-monitoring-agent.zabbix-configurator-client-connect-timeout-seconds=30
uxp.ocsp-cache.full-update-freshness-divisor=10
uxp.ocsp-cache.ocsp-responder-listen-address=127.0.0.1
logback.configurationFile=/etc/uxp/conf.d/ocsp-cache-logback.xml
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
uxp.ocsp-cache.update-interval=0 0 0/6 1/1 * ? *
uxp.monitoring-server.opdata-stats-period-seconds=300
uxp.proxy-status-check.clientproxy-listening-switch-enabled=true
java.vendor.url.bug=https\://bugs.launchpad.net/ubuntu/+source/openjdk-17
jetty.git.hash=e77516598a07cca826d27fa8a4f7c70e953921a6
uxp.ocsp-cache.admin-port=5766
uxp.proxy.verify-signing-certificate-qualified=true
user.dir=/
os.arch=amd64
java.vm.info=mixed mode, sharing
java.vm.version=17.0.15+6-Ubuntu-0ubuntu122.04
java.class.version=61.0
uxp.proxy.max-retained-soap-attachment-size-bytes=5242880
uxp.op-monitor.keep-records-for-days=7
```

</details>

## Purpose

The main function of the service is to:
- Regularly fetch and cache OCSP responses based on the configured schedule.
- Serve OCSP responses to other components over the responder port.
- Allow manual triggering of updates through the admin API.

## Security and Performance

- **No shell** access in container (`/bin/bash`, `/bin/sh` removed)
- **Non-root user** (`uxp`) runs the process
- **Heap/memory limits** defined explicitly
- **Native optimizations** via `tcmalloc`, `libunwind`, `gomp`
- **Startup classpath** includes crypto provider support from `ciplus-jce` and `signature-xades.jar`

## Author

Maintained by [Kirill Shypachov](https://github.com/kshypachov)  
On behalf of eGA Kyiv
