# uxp-securityserver-rest-api Service

## Overview

`uxp-securityserver-rest-api` is a RESTful management service designed to control and interact with the entire `uxp-securityserver` instance. It exposes administrative and operational functionality over HTTP and is intended for integration with user interfaces, orchestration layers, or external management systems.

> 📘 **OpenAPI UI** is available at:  
> `/api/v1/openapi-ui`

## Ports

- **8085** — REST API HTTP interface

## Containerized Execution in Kubernetes

When deployed in Kubernetes, this service **does not use the default shell script** (`/usr/share/uxp/bin/securityserver-rest-api.sh`) because shell binaries like `/bin/sh` and `/bin/bash` are removed from the image for security hardening.

Instead, the container is launched using a full `java` command passed directly as the container's `command` (in the pod spec).


## Purpose

The service enables:
- Managing `uxp-securityserver` configuration and status
- Interacting with security components via REST
- Automating tasks otherwise performed manually in the security server UI

## Docker Image Details

This service is built using a **multi-stage Dockerfile** with the following stages:

### Stage 1: Builder (`ubuntu:24.04`)
- Creates `uxp` user and group
- Extracts contents from:
  - `uxp-securityserver-rest-api`
  - `uxp-addon-trembita-crypto`
  - `uxp-addon-trembita-profile`

### Stage 2: Runtime (`eclipse-temurin:17-jdk-jammy`)
- Installs required native libraries:
  - `libgomp`, `libcihsm.so` (ШИФР-HSM), and Gryada-301 PKCS#11 libraries
- Copies required `.jar` files and configuration into `/app`
- Disables shell access (`/bin/bash` and `/bin/sh` removed)
- Runs as a non-root user `uxp` (UID 102)

### Environment Configuration

The image sets:
- `LD_LIBRARY_PATH=:/usr/share/uxp/lib`

Optionally, HSM drivers and configuration (`osplm.ini`) must be available in `/usr/share/uxp/lib/`.

## Required Volume Mounts

- `/etc/uxp/` – For configuration files (including `configuration-anchor.xml`, `db.properties`, etc.)
- `/etc/uxp/signer/` – Shared directory with software tokens or keys .p12

## Startup Script

The service is started using:
```
/usr/share/uxp/bin/securityserver-rest-api.sh
```

This script:
- Loads the main environment file: `/etc/uxp/services/securityserver-rest-api.conf`
- Loads global environment config: `/etc/uxp/services/global.conf`
- Applies optional `local.conf`
- Dynamically sources add-on configurations:
  - `certprofile-trembita-springboot.conf`
  - `trembita-crypto-springboot.conf`
- Constructs the Java command and executes Spring Boot using:
  ```bash
  org.springframework.boot.loader.PropertiesLauncher
  ```

## Java Runtime Details

The service runs with:

```bash
java \
  -Xmx128m \
  -XX:MaxMetaspaceSize=256m \
  -XX:+UseG1GC \
  -Xshare:on \
  -Dserver.port=8085 \
  -Dfile.encoding=UTF-8 \
  -Dlogging.config=/app/securityserver-rest-api-logback.xml \
  -Djava.library.path=/usr/share/uxp/lib/ \
  -cp /app/securityserver-rest-api.jar \
  -Dloader.path=/app/jlib/signature-xades.jar,/app/jlib/addon/certprofile-trembita.jar,/app/jlib/addon/proxy/cipher-jce-provider-1.22.7.jar,/app/jlib/addon/proxy/ciplus-jce/* \
  -Dorg.bytedeco.javacpp.noPointerGC=true \
  -Dorg.bytedeco.javacpp.maxBytes=0 \
  -Dorg.bytedeco.javacpp.maxPhysicalBytes=0 \
  org.springframework.boot.loader.PropertiesLauncher
```

## Runtime Properties (examples)

| Property                                              | Description                                 |
|-------------------------------------------------------|---------------------------------------------|
| `server.port=8085`                                    | REST API listen port                        |
| `uxp.proxy.server-port=5500`                          | UXP backend port                            |
| `uxp.identity-provider.security-server-client-id`     | OAuth2 client ID                            |
| `uxp.identity-provider.security-server-client-secret` | OAuth2 secret                               |
| `uxp.proxy.database-properties`                       | Path to database config                     |
| `uxp.identity-provider.database-properties`           | Same as above, shared config                |
| `uxp.common.configuration-anchor-file`                | Path to configuration anchor XML            |

## Security

- **Shellless Image**: no `/bin/sh` or `/bin/bash`
- **Non-root Execution**: runs as `uxp` user
- **Secrets**: must be injected securely via Kubernetes secrets
- **HSM Support**:
  - `libcihsm.so` (ШИФР-HSM)
  - Gryada-301 PKCS#11 `.so` files and `osplm.ini` (must be preloaded)

## Startup Script Output
Below is an example of the full startup script output during container initialization, showing how the environment is assembled and parameters are passed:

```bash
/usr/share/uxp/bin/securityserver-rest-api.sh 
+ . /etc/uxp/services/securityserver-rest-api.conf
++ . /etc/uxp/services/global.conf
++ for addon in ${ADDON_PATH}/securityserver-rest-api/*.conf
++ '[' -e /usr/share/uxp/jlib/addon/securityserver-rest-api/certprofile-trembita-springboot.conf ']'
++ . /usr/share/uxp/jlib/addon/securityserver-rest-api/certprofile-trembita-springboot.conf
+++ ADDON_CP=,/usr/share/uxp/jlib/addon/certprofile-trembita.jar
++ for addon in ${ADDON_PATH}/securityserver-rest-api/*.conf
++ '[' -e /usr/share/uxp/jlib/addon/securityserver-rest-api/trembita-crypto-springboot.conf ']'
++ . /usr/share/uxp/jlib/addon/securityserver-rest-api/trembita-crypto-springboot.conf
+++ ADDON_CP=',/usr/share/uxp/jlib/addon/certprofile-trembita.jar,/usr/share/uxp/jlib/addon/proxy/cipher-jce-provider-1.22.7.jar,/usr/share/uxp/jlib/addon/proxy/ciplus-jce/*'
+++ ADDON_PARAMS=' -Dorg.bytedeco.javacpp.noPointerGC=true -Dorg.bytedeco.javacpp.maxBytes=0 -Dorg.bytedeco.javacpp.maxPhysicalBytes=0 '
+++ export LD_LIBRARY_PATH=:/usr/share/uxp/lib
+++ LD_LIBRARY_PATH=:/usr/share/uxp/lib
++ CP=/usr/share/uxp/jlib/securityserver-rest-api.jar
++ BOOT_CP=/usr/share/uxp/jlib/signature-xades.jar
++ SECURITYSERVER_REST_API_PARAMS=' -Dserver.port=8085 -Dlogging.config=/etc/uxp/conf.d/securityserver-rest-api-logback.xml'
++ SECURITYSERVER_REST_API_JVM_OPTS=' -Xmx128m -XX:MaxMetaspaceSize=256m'
++ . /etc/uxp/services/local.conf
+ date -R
Mon, 02 Jun 2025 12:38:49 +0300
+ exec /usr/lib/jvm/java-17-openjdk-amd64/bin/java -Xmx128m -XX:MaxMetaspaceSize=256m -Dserver.port=8085 -Dlogging.config=/etc/uxp/conf.d/securityserver-rest-api-logback.xml -XX:+UseG1GC -Xshare:on -Dfile.encoding=UTF-8 -Djava.library.path=/usr/share/uxp/lib/ -cp /usr/share/uxp/jlib/securityserver-rest-api.jar '-Dloader.path=/usr/share/uxp/jlib/signature-xades.jar,,/usr/share/uxp/jlib/addon/certprofile-trembita.jar,/usr/share/uxp/jlib/addon/proxy/cipher-jce-provider-1.22.7.jar,/usr/share/uxp/jlib/addon/proxy/ciplus-jce/*' -Dorg.bytedeco.javacpp.noPointerGC=true -Dorg.bytedeco.javacpp.maxBytes=0 -Dorg.bytedeco.javacpp.maxPhysicalBytes=0 org.springframework.boot.loader.PropertiesLauncher
```

## ⚡ Runtime JVM Options (jcmd)

<details>
<summary>Click to expand output of <code>jcmd &lt;pid&gt; VM.command_line</code></summary>

```txt
VM Arguments:
jvm_args: -Xmx128m -XX:MaxMetaspaceSize=256m -Dserver.port=8085 -Dlogging.config=/etc/uxp/conf.d/securityserver-rest-api-logback.xml -XX:+UseG1GC -Xshare:on -Dfile.encoding=UTF-8 -Djava.library.path=/usr/share/uxp/lib/ -Dloader.path=/usr/share/uxp/jlib/signature-xades.jar,,/usr/share/uxp/jlib/addon/certprofile-trembita.jar,/usr/share/uxp/jlib/addon/proxy/cipher-jce-provider-1.22.7.jar,/usr/share/uxp/jlib/addon/proxy/ciplus-jce/* -Dorg.bytedeco.javacpp.noPointerGC=true -Dorg.bytedeco.javacpp.maxBytes=0 -Dorg.bytedeco.javacpp.maxPhysicalBytes=0 
java_command: org.springframework.boot.loader.PropertiesLauncher
java_class_path (initial): /usr/share/uxp/jlib/securityserver-rest-api.jar
Launcher Type: SUN_STANDARD
```
</details>

<details>
<summary>Click to expand output of <code>jcmd &lt;pid&gt; VM.system_properties</code></summary>

```txt
#Mon Jun 02 12:48:46 EEST 2025
uxp.proxy-monitoring-agent.ignored-network-interfaces=lo
uxp.proxy.max-retained-soap-message-size-bytes=5242880
java.specification.version=17
uxp.identity-provider.security-server-client-secret=2DmVrz_VUQUhn3ePNgWm8Ur-TwMK0la_
uxp.common.temp-files-path=/var/tmp/uxp/
uxp.proxy.client-httpclient-target-selection-strategy=round-robin
sun.jnu.encoding=UTF-8
uxp.identity-provider.database-properties=/etc/uxp/db.properties
uxp.configuration-client.update-interval=60
uxp.proxy.ocsp-responder-client-read-timeout=30000
uxp.proxy.openapi-download-read-timeout=5000
uxp.proxy.csr-signature-digest-algorithm-id=SHA-256
sun.arch.data.model=64
uxp.proxy.server-port=5500
org.bytedeco.javacpp.noPointerGC=true
java.vendor.url=https\://ubuntu.com/
uxp.op-monitor-buffer.size=20000
uxp.pkcs11.signing-session-pool-wait-time-seconds=10
uxp.op-monitor-buffer.httpclient-read-timeout=60000
uxp.identity-provider.oauth2-issuer-location=
uxp.proxy.software-token-batch-signatures=false
uxp.identity-provider.bcrypt..log-rounds=10
uxp.proxy.internal-cipher-suites=TLS_AES_256_GCM_SHA384,TLS_ECDHE_ECDSA_WITH_AES_256_GCM_SHA384
uxp.configuration-client.httpclient-connect-timeout=10000
sun.boot.library.path=/usr/lib/jvm/java-17-openjdk-amd64/lib
sun.java.command=org.springframework.boot.loader.PropertiesLauncher
uxp.proxy.log-signatures=true
jdk.debug=release
uxp.anti-dos.max-parallel-connections=5000
uxp.status-service.listen-port=2082
uxp.center.allowed-certificate-profiles=ee.cyber.uxp.common.certificateprofile.ua.UaCertificateProfileInfoProvider
uxp.proxy-status-check.interval-seconds=15
java.specification.vendor=Oracle Corporation
uxp.common.license-file=/etc/uxp/license.lic
uxp.identity-provider.public-client-id=uxp-ss-ui
java.version.date=2025-04-15
java.home=/usr/lib/jvm/java-17-openjdk-amd64
logging.config=/etc/uxp/conf.d/securityserver-rest-api-logback.xml
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
uxp.identity-provider.oauth2-introspect-uri=http\://localhost\:8087/auth-api/v1/oauth2/introspect
java.protocol.handler.pkgs=org.springframework.boot.loader
sun.management.compiler=HotSpot 64-Bit Tiered Compilers
uxp.op-monitor.clean-interval=0 0 0/12 1/1 * ? *
java.runtime.version=17.0.15+6-Ubuntu-0ubuntu122.04
uxp.common.template-path=/usr/share/uxp/templates/
user.name=uxp
uxp.proxy.server-listen-address=0.0.0.0
uxp.proxy-monitoring-agent.sending-interval-seconds=180
uxp.identity-provider.login-lockout-time-duration=15
uxp.common.conf-backup-digest-files-checked=true
uxp.identity-provider.public-client-access-token-time-to-live=180
uxp.configuration-client.port=5665
uxp.message-log.timestamp-immediately=true
uxp.monitoring-server.opdata-stats-collection-enabled=true
uxp.proxy.server-httpclient-socket-buffer-size=16384
uxp.op-monitor-buffer.httpclient-connect-timeout=30000
file.encoding=UTF-8
org.bytedeco.javacpp.maxPhysicalBytes=0
uxp.message-log-s3.trusted-certificate=/etc/uxp/ssl/public.crt
server.port=8085
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
PID=32389
uxp.proxy.software-token-key-dir=/etc/uxp/signer/
CONSOLE_LOG_CHARSET=UTF-8
org.bytedeco.javacpp.maxBytes=0
loader.path=/usr/share/uxp/jlib/signature-xades.jar,,/usr/share/uxp/jlib/addon/certprofile-trembita.jar,/usr/share/uxp/jlib/addon/proxy/cipher-jce-provider-1.22.7.jar,/usr/share/uxp/jlib/addon/proxy/ciplus-jce/*
native.encoding=UTF-8
uxp.proxy.server-listen-port=5500
java.library.path=/usr/share/uxp/lib/
uxp.proxy.cert-reg-signature-digest-algorithm-id=SHA-512
java.vendor=Ubuntu
java.specification.maintenance.version=1
uxp.common.expiration-warning-threshold-days=32
uxp.proxy.server-jetty-thread-pool-max-size=60
sun.io.unicode.encoding=UnicodeLittle
uxp.proxy.log-monitoring-signatures=false
uxp.proxy-monitoring-agent.params-collecting-interval-seconds=15
uxp.proxy.client-httpclient-connect-timeout=30000
uxp.proxy.server-httpclient-idle-connection-eviction-period=1
uxp.proxy.round-robin-quarantine-time=300000
java.class.path=/usr/share/uxp/jlib/securityserver-rest-api.jar
uxp.proxy.batch-signatures-enabled=false
uxp.op-monitor-buffer.sending-interval-seconds=5
uxp.common.conf-backup-digest-algorithm-id=SHA-512
java.vm.vendor=Ubuntu
uxp.op-monitor-buffer.max-records-in-message=500
uxp.common.digest-chunk-size=262144
user.timezone=Europe/Kyiv
uxp.proxy-monitoring-agent.net-stats-file=/proc/net/dev
org.jboss.logging.provider=slf4j
java.vm.specification.version=17
os.name=Linux
uxp.proxy.client-connector-socket-buffer-size=16384
uxp.monitoring-service.httpclient-connect-timeout=30000
uxp.proxy-monitoring-agent.zabbix-configurator-client-read-timeout-seconds=300
sun.java.launcher=SUN_STANDARD
user.country=UA
uxp.proxy.server-connection-accept-rate-limit=0
uxp.identity-provider.login-max-failed-attempts=5
uxp.proxy-status-check.serverproxy-listening-switch-enabled=true
sun.cpu.endian=little
user.home=/var/lib/uxp
user.language=uk
uxp.status-service.allowed-hosts=127.0.0.1
uxp.identity-provider.public-client-redirect-uris=
uxp.message-log-s3.access-key=pC9hJZZJZdSdWAOqIgIT
uxp.message-log-s3.secret-key=3daHty2NiBbzwJvZWCZkgTt7SUrR7pfqHN7DNFFZ
uxp.proxy.client-http-port=80
uxp.identity-provider.security-server-client-id=pvoqbggvvzpon1r4v55b7z8cu0de18cj
uxp.proxy.database-properties=/etc/uxp/db.properties
uxp.status-service.listen-address=127.0.0.1
uxp.proxy-monitoring-agent.port=2080
uxp.proxy-monitoring-agent.zabbix-configurator-client-connect-timeout-seconds=30
FILE_LOG_CHARSET=UTF-8
uxp.proxy.max-retained-rest-payload-size-bytes=5242880
java.awt.headless=true
uxp.proxy.serverconf-reload-interval-seconds=60
uxp.common.conf-path=/etc/uxp/
uxp.proxy.signature-timestamp-required=true
uxp.op-monitor.max-stats-records-in-payload=10000
uxp.proxy.client-httpclient-idle-connection-eviction-period=1
uxp.monitoring-server.opdata-stats-polling-interval-seconds=900
uxp.common.rsa-allowed=false
uxp.proxy.additional-forbidden-rest-http-headers=
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
uxp.proxy.ocsp-usage-safety-offset=2
uxp.configuration-client.httpclient-read-timeout=30000
uxp.identity-provider.hostname=
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


## Author

Maintained by [Kirill Shypachov](https://github.com/kshypachov)  
On behalf of eGA Kyiv