# UXP Identity Provider REST API

This component provides OAuth2-based authentication services for the UXP Security Server. It is exposed via port `8087`, and in the original monolithic product, all URIs with the `/auth-api` prefix are redirected to this service.

The API specification is available through the Swagger UI integrated in the Trembita Security Server.

---

## 📁 Files and Configuration

The following configuration and script files are used during startup in the legacy deployment model:

- `/etc/uxp/services/identity-provider-rest-api.conf` – Main startup script configuration
- `/etc/uxp/services/global.conf` – Loaded via `identity-provider-rest-api.conf`
- `/etc/uxp/conf.d/identity-provider-rest-api-logback.xml` – Logging configuration
- `/etc/uxp/services/local.conf` – Additional startup parameters
- `/usr/share/uxp/bin/identity-provider-rest-api.sh` – Startup script
- `/etc/uxp/conf.d/identity-provider.ini` – Application-specific configuration

---

## 🚀 Startup Command

In legacy setups, the Java application is launched with:

```bash
exec /usr/lib/jvm/java-17-openjdk-amd64/bin/java \
  -Xmx128m \
  -XX:MaxMetaspaceSize=256m \
  -Dserver.port=8087 \
  -Dlogging.config=/etc/uxp/conf.d/identity-provider-rest-api-logback.xml \
  -XX:+UseG1GC \
  -Xshare:on \
  -Dfile.encoding=UTF-8 \
  -Djava.library.path=/usr/share/uxp/lib/ \
  -cp /usr/share/uxp/jlib/identity-provider-rest-api.jar \
  org.springframework.boot.loader.PropertiesLauncher
```

---
## ⚡ Runtime JVM Options (jcmd)

<details>
<summary>Click to expand output of <code>jcmd &lt;pid&gt; VM.command_line</code></summary>
VM Arguments:
jvm_args: -Xmx128m -XX:MaxMetaspaceSize=256m -Dserver.port=8087 -Dlogging.config=/etc/uxp/conf.d/identity-provider-rest-api-logback.xml -XX:+UseG1GC -Xshare:on -Dfile.encoding=UTF-8 -Djava.library.path=/usr/share/uxp/lib/ 
java_command: org.springframework.boot.loader.PropertiesLauncher
java_class_path (initial): /usr/share/uxp/jlib/identity-provider-rest-api.jar
Launcher Type: SUN_STANDARD
</details>

<details>
<summary>Click to expand output of <code>jcmd &lt;pid&gt; VM.system_properties</code></summary>

java.specification.version=17
uxp.identity-provider.security-server-client-secret=2DmVrz_VUQUhn3ePNgWm8Ur-TwMK0la_
uxp.common.temp-files-path=/var/tmp/uxp/
sun.jnu.encoding=UTF-8
uxp.identity-provider.database-properties=/etc/uxp/db.properties
java.class.path=/usr/share/uxp/jlib/identity-provider-rest-api.jar
uxp.common.conf-backup-digest-algorithm-id=SHA-512
java.vm.vendor=Ubuntu
sun.arch.data.model=64
uxp.common.digest-chunk-size=0
java.vendor.url=https\://ubuntu.com/
user.timezone=Europe/Kyiv
uxp.identity-provider.oauth2-issuer-location=
org.jboss.logging.provider=slf4j
uxp.identity-provider.bcrypt..log-rounds=10
java.vm.specification.version=17
os.name=Linux
sun.java.launcher=SUN_STANDARD
user.country=UA
sun.boot.library.path=/usr/lib/jvm/java-17-openjdk-amd64/lib
sun.java.command=org.springframework.boot.loader.PropertiesLauncher
jdk.debug=release
uxp.identity-provider.login-max-failed-attempts=5
sun.cpu.endian=little
user.home=/var/lib/uxp
user.language=uk
uxp.identity-provider.public-client-redirect-uris=
java.specification.vendor=Oracle Corporation
uxp.message-log-s3.access-key=pC9hJZZJZdSdWAOqIgIT
uxp.common.license-file=/etc/uxp/license.lic
uxp.identity-provider.public-client-id=uxp-ss-ui
uxp.message-log-s3.secret-key=3daHty2NiBbzwJvZWCZkgTt7SUrR7pfqHN7DNFFZ
java.version.date=2025-04-15
java.home=/usr/lib/jvm/java-17-openjdk-amd64
logging.config=/etc/uxp/conf.d/identity-provider-rest-api-logback.xml
file.separator=/
uxp.identity-provider.security-server-client-id=pvoqbggvvzpon1r4v55b7z8cu0de18cj
java.vm.compressedOopsMode=32-bit
line.separator=\n
uxp.message-log.archive-interval=0 0 0/1 1/1 * ? *
java.vm.specification.vendor=Oracle Corporation
java.specification.name=Java Platform API Specification
FILE_LOG_CHARSET=UTF-8
uxp.message-log-s3.address=https\://192.168.99.136\:9000
java.awt.headless=true
uxp.common.conf-path=/etc/uxp/
uxp.identity-provider.oauth2-introspect-uri=http\://localhost\:8087/auth-api/v1/oauth2/introspect
java.protocol.handler.pkgs=org.springframework.boot.loader
sun.management.compiler=HotSpot 64-Bit Tiered Compilers
uxp.common.rsa-allowed=true
java.runtime.version=17.0.15+6-Ubuntu-0ubuntu122.04
uxp.common.template-path=/usr/share/uxp/templates/
user.name=uxp
path.separator=\:
os.version=5.15.0-125-generic
uxp.common.global-conf-path=/etc/uxp/globalconf/
uxp.common.tls-conf-path=/etc/uxp/ssl/
uxp.identity-provider.login-lockout-time-duration=15
uxp.common.conf-backup-digest-files-checked=false
uxp.identity-provider.public-client-access-token-time-to-live=180
java.runtime.name=OpenJDK Runtime Environment
uxp.message-log.archive-storage-type=s3
file.encoding=UTF-8
uxp.message-log-s3.trusted-certificate=/etc/uxp/ssl/public.crt
uxp.common.pkcs12-provider-name=BC
server.port=8087
java.vm.name=OpenJDK 64-Bit Server VM
uxp.common.configuration-anchor-file=/etc/uxp/configuration-anchor.xml
java.vendor.url.bug=https\://bugs.launchpad.net/ubuntu/+source/openjdk-17
jetty.git.hash=816018a420329c1cacd4116799cda8c8c60a57cd
java.io.tmpdir=/tmp
java.version=17.0.15
user.dir=/
uxp.common.device-templates-path=/etc/uxp/device-templates/
os.arch=amd64
uxp.message-log-s3.bucket-name=uxp-messagelog1227
java.vm.specification.name=Java Virtual Machine Specification
PID=233147
uxp.identity-provider.hostname=
CONSOLE_LOG_CHARSET=UTF-8
native.encoding=UTF-8
java.library.path=/usr/share/uxp/lib/
java.vm.info=mixed mode, sharing
java.vendor=Ubuntu
java.vm.version=17.0.15+6-Ubuntu-0ubuntu122.04
java.specification.maintenance.version=1
uxp.common.expiration-warning-threshold-days=32
sun.io.unicode.encoding=UnicodeLittle
java.class.version=61.0
</details>

<details>
<summary>Click to expand output of <code>jcmd &lt;pid&gt; VM.flags</code></summary>
-XX:CICompilerCount=3 
-XX:CompressedClassSpaceSize=218103808 
-XX:ConcGCThreads=1 
-XX:G1ConcRefinementThreads=4 
-XX:G1EagerReclaimRemSetThreshold=8 
-XX:G1HeapRegionSize=1048576 
-XX:GCDrainStackTargetSize=64 
-XX:InitialHeapSize=65011712 
-XX:MarkStackSize=4194304 
-XX:MaxHeapSize=134217728 
-XX:MaxMetaspaceSize=268435456 
-XX:MaxNewSize=79691776 
-XX:MinHeapDeltaBytes=1048576 
-XX:MinHeapSize=8388608 
-XX:NonNMethodCodeHeapSize=5832780 
-XX:NonProfiledCodeHeapSize=122912730 
-XX:ProfiledCodeHeapSize=122912730 
-XX:+RequireSharedSpaces 
-XX:ReservedCodeCacheSize=251658240 
-XX:+SegmentedCodeCache 
-XX:SoftMaxHeapSize=134217728 
-XX:-THPStackMitigation 
-XX:+UseCompressedClassPointers 
-XX:+UseCompressedOops 
-XX:+UseFastUnorderedTimeStamps 
-XX:+UseG1GC 
-XX:+UseSharedSpaces 
</details>

---
## 📦 Kubernetes Container Deployment

The above configuration and behavior describe the legacy monolithic application setup.
For deployment in a Kubernetes environment, a dedicated Dockerfile is provided with the following characteristics:

- ✅ Multi-stage build: minimal runtime image
- ✅ Built on top of eclipse-temurin:17-jdk-jammy
- ✅ Runs as non-root user (uxp)
- ✅ Logging output is configured to stdout (standard Kubernetes practice)
- ✅ No ENTRYPOINT is specified — the entrypoint must be passed explicitly in your Kubernetes deployment or Helm chart.
- ✅ Removed /bin/bash , /bin/sh
- 
---
## 🧩 Runtime Notes (Kubernetes)
- The container works in a read-only filesystem context.
- Rtime state and configuration are isolated to mounted volume:
  - /etc/uxp/db_properties – DB credentials
- The container does not provide shall access.
